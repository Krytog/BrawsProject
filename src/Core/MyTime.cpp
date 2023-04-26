#include "MyTime.h"

MyTime::MyTime() {
    start_clock_ = std::chrono::steady_clock::now();
}

double MyTime::EvaluateTime() const {
    std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - start_clock_;
    return time_interval.count();
}

void MyTime::ResetTime() {
    start_clock_ = std::chrono::steady_clock::now();
}

MyTime& MyTime::operator=(const MyTime& other) {
    start_clock_ = other.start_clock_;
    return *this;
}