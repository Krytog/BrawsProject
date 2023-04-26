#pragma once

#include <chrono>

class MyTime {
public:
    MyTime();

    MyTime& operator=(const MyTime& other);

    void ResetTime();
    double EvaluateTime() const;

private:
    std::chrono::time_point<std::chrono::steady_clock> start_clock_;
};
