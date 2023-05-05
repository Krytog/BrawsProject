#pragma once

#include <string>
#include <chrono>
#include <vector>

class Profiler {
public:
    static Profiler& GetInstance();

    void AddTimeMark(std::string* dist);
    std::chrono::time_point<std::chrono::steady_clock> ExtractTimeMark(std::string* from);
    void HandlePing(std::chrono::time_point<std::chrono::steady_clock>& time_point);

    void StartEngineFrame();
    void FinishEngineFrame();

    void StartSwarmSystemFrame();
    void FinishSwarmSystemFrame();

    void OnPackageLost();
    void OnPackageSuccess();

    void PrintResults();

    ~Profiler();
private:
    Profiler();
    Profiler(const Profiler& other) = delete;
    Profiler& operator=(const Profiler& other) = delete;

    std::chrono::time_point<std::chrono::steady_clock> start_engine_time_;
    std::chrono::time_point<std::chrono::steady_clock> start_swarmsystem_time_;
    std::vector<double> engine_times_;
    std::vector<double> swarmsystem_times_;

    size_t package_lost_;
    size_t package_success_;

    std::vector<double> pings_;
};