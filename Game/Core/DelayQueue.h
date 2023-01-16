#pragma once

#include <chrono>
#include <queue>
#include <functional>

class DelayQueue {
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
    using TypeQueueTime = std::pair<TimePoint, std::function<void()>>;
    using TypeQueueTicks = std::pair<uint64_t, std::function<void()>>;
    static DelayQueue& GetInstance();

    template<typename... Args>
    void PushTime(const TimePoint& time_point, void(*func)(Args...), Args... args) {
        queue_time_.Push(time_point, func, args...);
    }

    template<typename F, typename... Args>
    void PushTime(const TimePoint& time_point, F* pointer, void(F::*func)(Args...), Args... args) {
        queue_time_.Push(time_point, pointer, func, args...);
    }

    template<typename F, typename... Args>
    void PushTime(const TimePoint& time_point, const F* pointer, void(F::*func)(Args...) const, Args... args) {
        queue_time_.Push(time_point, pointer, func, args...);
    }

    template<typename... Args>
    void PushTicks(const uint64_t ticks_count, void(*func)(Args...), Args... args) {
        queue_ticks_.Push(ticks_count, func, args...);
    }

    template<typename F, typename... Args>
    void PushTicks(const uint64_t ticks_count, F* pointer, void(F::*func)(Args...), Args... args) {
        queue_ticks_.Push(ticks_count, pointer, func, args...);
    }

    template<typename F, typename... Args>
    void PushTicks(const uint64_t ticks_count, const F* pointer, void(F::*func)(Args...) const, Args... args) {
        queue_ticks_.Push(ticks_count, pointer, func, args...);
    }

    void TryExecute(const TimePoint& time_point, const uint64_t ticks_count);

private:
    DelayQueue();

    class DelayQueueTime {
    public:
        class CompareFunctor {
        public:
            bool operator()(const TypeQueueTime& first, const TypeQueueTime& second);
        };

        template<typename... Args>
        void Push(const TimePoint& time_point, void(*func)(Args...), Args... args) {
            queue_.emplace(time_point, [func, args...](){func(args...);});
        }

        template<typename F, typename... Args>
        void Push(const TimePoint& time_point, F* pointer, void(F::*func)(Args...), Args... args) {
            queue_.emplace(time_point, [func, pointer, args...](){(pointer->*func)(args...);});
        }

        template<typename F, typename... Args>
        void Push(const TimePoint& time_point, const F* pointer, void(F::*func)(Args...) const, Args... args) {
            queue_.emplace(time_point, [func, pointer, args...](){(pointer->*func)(args...);});
        }

        void TryExecute(const TimePoint& time_point);
    private:
        std::priority_queue<TypeQueueTime, std::vector<TypeQueueTime>, CompareFunctor> queue_;
    };

    class DelayQueueTicks {
    public:
        class CompareFunctor {
        public:
            bool operator()(const TypeQueueTicks& first, const TypeQueueTicks& second);
        };

        template<typename... Args>
        void Push(const uint64_t ticks_count, void(*func)(Args...), Args... args) {
            queue_.emplace(ticks_count, [func, args...](){func(args...);});
        }

        template<typename F, typename... Args>
        void Push(const uint64_t ticks_count, F* pointer, void(F::*func)(Args...), Args... args) {
            queue_.emplace(ticks_count, [func, pointer, args...](){(pointer->*func)(args...);});
        }

        template<typename F, typename... Args>
        void Push(const uint64_t ticks_count, const F* pointer, void(F::*func)(Args...) const, Args... args) {
            queue_.emplace(ticks_count, [func, pointer, args...](){(pointer->*func)(args...);});
        }

        void TryExecute(const uint64_t time_point);
    private:
        std::priority_queue<TypeQueueTicks, std::vector<TypeQueueTicks>, CompareFunctor> queue_;
    };

    DelayQueueTime queue_time_;
    DelayQueueTicks queue_ticks_;
};