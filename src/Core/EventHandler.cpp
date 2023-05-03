#include "EventHandler.h"

void Event::ForceExecute() {
    force_executed_ = true;
    callable_();
}

bool Event::TryExecute() const {
    if (force_executed_ && status_ == Disposable) { /* Lazy delete */
        return true;
    }
    if (predicate_()) {
        callable_();
        return true;
    }
    return false;
}

EventStatus Event::GetStatus() const {
    return status_;
}

void EventHandler::DestroyEvent(Event* event) {  /* Safety of erasure is guaranteed */
    events_.erase(cache_[event]);
    cache_.erase(event);
}

void EventHandler::TryExecuteAll() {
   std::erase_if(events_, [this](Event& event) {
       if (event.GetStatus() == Disposable && event.TryExecute()) {
           DestroyEvent(&event);
           return true;
       }
       return false;
    });
}

void EventHandler::Clear() {
    events_.clear();
    cache_.clear();
}

bool EventHandler::Empty() const {
    return events_.empty();
}

EventHandler::EventHandler() = default;

EventHandler& EventHandler::GetInstance() {
    static EventHandler event_handler;
    return event_handler;
}