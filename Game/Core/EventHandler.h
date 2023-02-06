#pragma once

#include <list>
#include <tuple>
#include <functional>

enum EventStatus {
    Disposable,
    Reusable,
};

class Event {
public:
    template <typename Predicate, typename... PredicateArgs, typename Callable, typename... CallableArgs,
              typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>>
    Event(Predicate&& pr, PredicateArgsTuple&& pr_args, Callable&& cb, CallableArgsTuple&& cb_args,
          EventStatus status)
        : predicate_([pr, pr_args] { return static_cast<bool>(std::apply(pr, pr_args)); }),
          callable_([cb, cb_args] { std::apply(cb, cb_args); }),
          status_(status) {
    }

    void ForceExecute();

    bool TryExecute() const;

    EventStatus GetStatus() const;

private:
    std::function<bool()> predicate_;
    std::function<void()> callable_;
    EventStatus status_;
    bool force_executed_ = false;
};

class EventHandler {
public:
    static EventHandler& GetInstance();

    /*  Syntax : &Predicate, std::make_tuple(PredicateArgs...), InvokerPtr, &Callable,
       std::make_tuple(CallableArgs...) Empty tuples can be replaced with {} Reference arguments for
       Predicate and Callable must be passed using std::ref
    */
    template <typename Predicate, typename... PredicateArgs, typename Callable, typename... CallableArgs,
              typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>>
    Event* CreateEvent(Predicate&& pr, PredicateArgsTuple&& pr_args, Callable&& cb,
                       CallableArgsTuple&& cb_args, EventStatus status) {
        static_assert(std::is_convertible_v<decltype(std::apply(pr, pr_args)), bool>,
                      "Predicate result must be convertible to bool");
        static_assert(std::is_void_v<decltype(std::apply(cb, cb_args))>, "Callable result must be void");
        events_.emplace_back(std::forward<Predicate>(pr), std::forward<PredicateArgsTuple>(pr_args),
                             std::forward<Callable>(cb), std::forward<CallableArgsTuple>(cb_args), status);
        return &events_.back();
    }

    void TryExecuteAll();

    void Clear();

    /* Does not guarantee that all ForceExecuted Events are deleted before TryExecuteAll called */
    bool Empty() const;

private:
    EventHandler();

private:
    std::list<Event> events_;
};