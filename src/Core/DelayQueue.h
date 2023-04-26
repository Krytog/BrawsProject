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

    template <typename Callable, typename... Args>
    void PushTime(const TimePoint& time_point, Callable&& callable, Args&&... args) {
        queue_time_.Push(time_point, std::forward<Callable>(callable), std::forward<Args>(args)...);
    }

    template <typename Callable, typename... Args>
    void PushTicks(const uint64_t ticks_count, Callable&& callable, Args&&... args) {
        queue_ticks_.Push(ticks_count, std::forward<Callable>(callable), std::forward<Args>(args)...);
    }

    void TryExecute(const TimePoint& time_point, const uint64_t ticks_count);

    bool Empty() const;

private:
    DelayQueue();

    class DelayQueueTime {
    public:
        class CompareFunctor {
        public:
            bool operator()(const TypeQueueTime& first, const TypeQueueTime& second);
        };

        template <
            typename Callable, typename... Args,
            std::enable_if_t<std::is_void_v<typename std::result_of<Callable(Args...)>::type>, bool> = true>
        void Push(const TimePoint& time_point, Callable&& cb, Args&&... args) {
            queue_.emplace(time_point, [cb, args...] { std::invoke(cb, args...); });
        }

        void TryExecute(const TimePoint& time_point);

        bool Empty() const;

    private:
        std::priority_queue<TypeQueueTime, std::vector<TypeQueueTime>, CompareFunctor> queue_;
    };

    class DelayQueueTicks {
    public:
        class CompareFunctor {
        public:
            bool operator()(const TypeQueueTicks& first, const TypeQueueTicks& second);
        };

        template <
            typename Callable, typename... Args,
            std::enable_if_t<std::is_void_v<typename std::result_of<Callable(Args...)>::type>, bool> = true>
        void Push(const uint64_t ticks_count, Callable&& cb, Args&&... args) {
            queue_.emplace(ticks_count, [cb, args...] { std::invoke(cb, args...); });
        }

        void TryExecute(const uint64_t time_point);

        bool Empty() const;

    private:
        std::priority_queue<TypeQueueTicks, std::vector<TypeQueueTicks>, CompareFunctor> queue_;
    };

    DelayQueueTime queue_time_;
    DelayQueueTicks queue_ticks_;
};