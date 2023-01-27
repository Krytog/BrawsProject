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
    template <typename PredicateReturn, typename... PredicateArgs, typename CallableReturn,
              typename... CallableArgs, typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>,
              std::enable_if_t<std::is_convertible_v<PredicateReturn, bool>, bool> = true,
              std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
    Event(PredicateReturn (*pr)(PredicateArgs...), PredicateArgsTuple&& pr_args,
          CallableReturn (*cb)(CallableArgs...), CallableArgsTuple&& cb_args, EventStatus status)
        : predicate_([pr, pr_args] { return static_cast<bool>(std::apply(pr, pr_args)); }),
          callable_([cb, cb_args] { std::apply(cb, cb_args); }),
          status_(status) {
    }

    template <typename PredicateReturn, typename... PredicateArgs, typename Invoker, typename CallableReturn,
              typename... CallableArgs, typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>,
              std::enable_if_t<std::is_convertible_v<PredicateReturn, bool>, bool> = true,
              std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
    Event(PredicateReturn (*pr)(PredicateArgs...), PredicateArgsTuple&& pr_args, Invoker* pointer,
          CallableReturn (Invoker::*cb)(CallableArgs...), CallableArgsTuple&& cb_args, EventStatus status)
        : predicate_([pr, pr_args] { return static_cast<bool>(std::apply(pr, pr_args)); }),
          callable_(
              [pointer, cb, cb_args] { std::apply(cb, std::tuple_cat(std::make_tuple(pointer), cb_args)); }),
          status_(status) {
    }

    template <typename PredicateReturn, typename... PredicateArgs, typename Invoker, typename CallableReturn,
              typename... CallableArgs, typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>,
              std::enable_if_t<std::is_convertible_v<PredicateReturn, bool>, bool> = true,
              std::enable_if_t<std::is_void_v<CallableReturn>, bool> = true>
    Event(PredicateReturn (*pr)(PredicateArgs...), PredicateArgsTuple&& pr_args, Invoker* pointer,
          CallableReturn (Invoker::*cb)(CallableArgs...) const, CallableArgsTuple&& cb_args,
          EventStatus status)
        : predicate_([pr, pr_args] { return static_cast<bool>(std::apply(pr, pr_args)); }),
          callable_(
              [pointer, cb, cb_args] { std::apply(cb, std::tuple_cat(std::make_tuple(pointer), cb_args)); }),
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

    /*  Syntax : &Predicate, std::make_tuple(PredicateArgs...), &Callable, std::make_tuple(CallableArgs...)
       Empty tuples can be replaced with {}
       'Reference' arguments for Predicate and Callable must be passed using std::ref
    */
    template <typename Predicate, typename... PredicateArgs, typename Callable, typename... CallableArgs,
              typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>>
    Event* CreateEvent(Predicate&& pr, PredicateArgsTuple&& pr_args, Callable&& cb,
                       CallableArgsTuple&& cb_args, EventStatus status) {
        events_.emplace_back(std::forward<Predicate>(pr), std::forward<PredicateArgsTuple>(pr_args),
                             std::forward<Callable>(cb), std::forward<CallableArgsTuple>(cb_args), status);
        return &events_.back();
    }

    /*  Syntax : &Predicate, std::make_tuple(PredicateArgs...), InvokerPtr, &Callable, std::make_tuple(CallableArgs...)
       Empty tuples can be replaced with {}
       'Reference' arguments for Predicate and Callable must be passed using std::ref
    */
    template <typename Predicate, typename... PredicateArgs, typename Invoker, typename Callable,
              typename... CallableArgs, typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>>
    Event* CreateEvent(Predicate&& pr, PredicateArgsTuple&& pr_args, Invoker* pointer, Callable&& cb,
                       CallableArgsTuple&& cb_args, EventStatus status) {
        events_.emplace_back(std::forward<Predicate>(pr), std::forward<PredicateArgsTuple>(pr_args), pointer,
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