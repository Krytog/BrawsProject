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

void EventHandler::TryExecuteAll() {
    std::list<Event>::iterator it = events_.begin();
    while (it != events_.end()) {
        if (it->TryExecute() && it->GetStatus() == Disposable) {
            it = events_.erase(it);
            continue;
        }
        ++it;
    }
}

void EventHandler::Clear() {
    events_.clear();
}

bool EventHandler::Empty() const {
    return events_.empty();
}

EventHandler::EventHandler() = default;

EventHandler& EventHandler::GetInstance() {
    static EventHandler event_handler;
    return event_handler;
}
