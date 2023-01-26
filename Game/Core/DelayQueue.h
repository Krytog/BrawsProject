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

    template <typename Callable, typename... Params>
    void PushTime(const TimePoint& time_point, Callable&& callable, Params&&... params) {
        queue_time_.Push(time_point, std::forward<Callable>(callable), std::forward<Params>(params)...);
    }

    template <typename F, typename Callable, typename... Params>
    void PushTime(const TimePoint& time_point, F* pointer, Callable&& callable, Params&&... params) {
        queue_time_.Push(time_point, pointer, std::forward<Callable>(callable),
                         std::forward<Params>(params)...);
    }

    template <typename Callable, typename... Params>
    void PushTicks(const uint64_t ticks_count, Callable&& callable, Params&&... params) {
        queue_ticks_.Push(ticks_count, std::forward<Callable>(callable), std::forward<Params>(params)...);
    }

    template <typename F, typename Callable, typename... Params>
    void PushTicks(const uint64_t ticks_count, F* pointer, Callable&& callable, Params&&... params) {
        queue_ticks_.Push(ticks_count, pointer, std::forward<Callable>(callable),
                          std::forward<Params>(params)...);
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

        template <typename CallableReturn, typename... CallableArgs, typename... Params,
                  std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
        void Push(const TimePoint& time_point, CallableReturn (*func)(CallableArgs...), Params&&... args) {
            queue_.emplace(time_point, [func, args...] { func(args...); });
        }

        template <typename F, typename CallableReturn, typename... CallableArgs, typename... Params,
                  std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
        void Push(const TimePoint& time_point, F* pointer, CallableReturn (F::*func)(CallableArgs...),
                  Params&&... args) {
            queue_.emplace(time_point, [func, pointer, args...] { (pointer->*func)(args...); });
        }

        template <typename F, typename CallableReturn, typename... CallableArgs, typename... Params,
                  std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
        void Push(const TimePoint& time_point, const F* pointer,
                  CallableReturn (F::*func)(CallableArgs...) const, Params&&... args) {
            queue_.emplace(time_point, [func, pointer, args...] { (pointer->*func)(args...); });
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

        template <typename CallableReturn, typename... CallableArgs, typename... Params,
                  std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
        void Push(const uint64_t ticks_count, CallableReturn (*func)(CallableArgs...), Params&&... args) {
            queue_.emplace(ticks_count, [func, args...]() { func(args...); });
        }

        template <typename F, typename CallableReturn, typename... CallableArgs, typename... Params,
                  std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
        void Push(const uint64_t ticks_count, F* pointer, CallableReturn (F::*func)(CallableArgs...),
                  Params&&... args) {
            queue_.emplace(ticks_count, [func, pointer, args...]() { (pointer->*func)(args...); });
        }

        template <typename F, typename CallableReturn, typename... CallableArgs, typename... Params,
                  std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
        void Push(const uint64_t ticks_count, const F* pointer,
                  CallableReturn (F::*func)(CallableArgs...) const, Params&&... args) {
            queue_.emplace(ticks_count, [func, pointer, args...]() { (pointer->*func)(args...); });
        }

        void TryExecute(const uint64_t time_point);

        bool Empty() const;

    private:
        std::priority_queue<TypeQueueTicks, std::vector<TypeQueueTicks>, CompareFunctor> queue_;
    };

    DelayQueueTime queue_time_;
    DelayQueueTicks queue_ticks_;
};
