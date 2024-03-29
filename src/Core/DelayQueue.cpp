#include "DelayQueue.h"

DelayQueue& DelayQueue::GetInstance() {
    static DelayQueue instance;
    return instance;
}

DelayQueue::DelayQueue() = default;

void DelayQueue::DelayQueueTime::TryExecute(const TimePoint& time_point) {
    while (!queue_.empty() && time_point >= queue_.top().first) {
        queue_.top().second();
        queue_.pop();
    }
}

void DelayQueue::DelayQueueTime::Clear() {
    queue_ = {};
}

bool DelayQueue::DelayQueueTime::Empty() const {
    return queue_.empty();
}

void DelayQueue::DelayQueueTicks::TryExecute(const uint64_t ticks_count) {
    while (!queue_.empty() && ticks_count >= queue_.top().first) {
        queue_.top().second();
        queue_.pop();
    }
}

void DelayQueue::DelayQueueTicks::Clear() {
    queue_ = {};
}

bool DelayQueue::DelayQueueTicks::Empty() const {
    return queue_.empty();
}

void DelayQueue::TryExecute(const TimePoint& time_point, const uint64_t ticks_count) {
    queue_time_.TryExecute(time_point);
    queue_ticks_.TryExecute(ticks_count);
}

void DelayQueue::Clear() {
    queue_time_.Clear();
    queue_ticks_.Clear();
}

bool DelayQueue::Empty() const {
    return queue_ticks_.Empty() && queue_time_.Empty();
}

bool DelayQueue::DelayQueueTime::CompareFunctor::operator()(const TypeQueueTime& first,
                                                            const TypeQueueTime& second) {
    return first.first > second.first;
}

bool DelayQueue::DelayQueueTicks::CompareFunctor::operator()(const TypeQueueTicks& first,
                                                             const TypeQueueTicks& second) {
    return first.first > second.first;
}