#include "Profiler.h"

#include "../Serializer.h"
#include <iostream>

Profiler &Profiler::GetInstance() {
    static Profiler profiler;
    return profiler;
}

void Profiler::AddTimeMark(std::string *dist) {
    std::chrono::time_point<std::chrono::steady_clock> time_point = std::chrono::steady_clock::now();
    Serializer::Serialize(time_point, dist, dist->size());
}

std::chrono::time_point<std::chrono::steady_clock> Profiler::ExtractTimeMark(std::string *from) {
    if (from->size() < sizeof(std::chrono::time_point<std::chrono::steady_clock>)) {
        return std::chrono::steady_clock::now() + std::chrono::hours(1);
    }
    size_t start = from->size() - sizeof(std::chrono::time_point<std::chrono::steady_clock>);
    std::string time_point_bytes = from->substr(start);
    for (size_t i = 0; i < sizeof(std::chrono::time_point<std::chrono::steady_clock>); ++i) {
        from->pop_back();
    }
    std::chrono::time_point<std::chrono::steady_clock> result;
    Serializer::Deserialize(result, time_point_bytes);
    return result;
}

void Profiler::StartEngineFrame() {
    start_engine_time_ = std::chrono::steady_clock::now();
}

void Profiler::StartSwarmSystemFrame() {
    start_swarmsystem_time_ = std::chrono::steady_clock::now();
}

void Profiler::FinishEngineFrame() {
    std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - start_engine_time_;
    double frame_time = time_interval.count();
    engine_times_.push_back(frame_time);
}

void Profiler::FinishSwarmSystemFrame() {
    std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - start_swarmsystem_time_;
    double frame_time = time_interval.count();
    swarmsystem_times_.push_back(frame_time);
}

void Profiler::HandlePing(std::chrono::time_point<std::chrono::steady_clock> &time_point) {
    std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - time_point;
    double frame_time = time_interval.count();
    pings_.push_back(frame_time);
}

void Profiler::OnPackageLost() {
    ++package_lost_;
}

void Profiler::OnPackageSuccess() {
    ++package_success_;
}

#define ACCEPTABLE_PING 1

void Profiler::PrintResults() {
    double ping_sum = 0;
    size_t accepted = 0;
    double worst_ping = 0;
    for (auto ping : pings_) {
        if (ping > ACCEPTABLE_PING) {
            continue;
        }
        ++accepted;
        ping_sum += ping;
        worst_ping = std::max(worst_ping, ping);
    }

    double engine_time_sum = 0;
    double worst_engine_time = 0;
    for (auto time : engine_times_) {
        engine_time_sum += time;
        worst_engine_time = std::max(worst_engine_time, time);
    }

    double swarmsystem_time_sum = 0;
    double worst_swarmsystem_time = 0;
    for (auto time : engine_times_) {
        swarmsystem_time_sum += time;
        worst_swarmsystem_time = std::max(worst_swarmsystem_time, time);
    }

    size_t total_packages = package_success_ + package_lost_;
    std::cout << "PROFILING RESULTS:\n";
    std::cout << "PACKAGE RECEIVED: " << total_packages << "\n";
    std::cout << "AMONG THEM LOST: " << package_lost_ << "\n";
    std::cout << "PACKAGE LOSS: " << 100 * static_cast<double>(package_lost_) / total_packages << "%\n";
    std::cout << "AVG PING: " << 1000 * ping_sum / accepted << "ms\n";
    std::cout << "THE WORST PING: " << 1000 * worst_ping << "ms\n";
    std::cout << "AVG ENGINE TIME: " << 1000 * engine_time_sum / engine_times_.size() << "ms\n";
    std::cout << "THE WORST ENGINE TIME: " << 1000 * worst_engine_time << "ms\n";
    std::cout << "AVG SWARMSYSTEM TIME: " << 1000 * swarmsystem_time_sum / engine_times_.size() << "ms\n";
    std::cout << "THE WORST SWARMSYSTEM TIME: " << 1000 * worst_swarmsystem_time << "ms\n";
}

Profiler::~Profiler() {
    PrintResults();
}

Profiler::Profiler() = default;