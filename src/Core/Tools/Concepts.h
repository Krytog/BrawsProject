#define PARENS ()
/* Never to use! Use MACRO_CONCAT instead! */
#define MACRO_CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) MACRO_CONCAT_IMPL(x, y)
/* recursion depth == 31 for fast preprocessing */
#define EXPAND(...) EXPAND1(EXPAND1(__VA_ARGS__))
#define EXPAND1(...) EXPAND2(EXPAND2(__VA_ARGS__))
#define EXPAND2(...) EXPAND3(EXPAND3(__VA_ARGS__))
#define EXPAND3(...) EXPAND4(EXPAND4(__VA_ARGS__))
#define EXPAND4(...) __VA_ARGS__

#define DEFINE_HAS_METHOD_CONCEPT(method)                      \
    template <typename T>                                      \
    concept MACRO_CONCAT(HasMethod_, method) = requires(T t) { \
        t.method();                                            \
    };

#define DEFINE_HAS_MEMBER_CONCEPT(member)                      \
    template <typename T>                                      \
    concept MACRO_CONCAT(HasMember_, member) = requires(T t) { \
        t.member;                                              \
    };


template <typename ...Args>
struct AlwaysTrue {
    static constexpr bool value = true;  // NOLINT
};

/* Better use HasMembers(type, ...members) macro for multiple members */
#define HasMember(__type__, member_) std::enable_if_t<AlwaysTrue<decltype(std::declval<__type__>().member_)>::value, bool> = true
/* Better use HasMembers(type, ...methods) macro for multiple methods */
#define HasMethod(__type__, method) std::enable_if_t<static_cast<bool>(sizeof(&__type__::method)), bool> = true

#define __HAS_MEMBERS_IMPL_STEP() __HAS_MEMBERS_IMPL
#define __HAS_MEMBERS_IMPL(type, method, ...) HasMember(type, method) __VA_OPT__(, __HAS_MEMBERS_IMPL_STEP PARENS (type, __VA_ARGS__))
#define HasMembers(type, method, ...) EXPAND(__HAS_MEMBERS_IMPL(type, method, __VA_ARGS__))

#define __HAS_METHODS_IMPL_STEP() __HAS_METHODS_IMPL
#define __HAS_METHODS_IMPL(type, method, ...) HasMethod(type, method) __VA_OPT__(, __HAS_METHODS_IMPL_STEP PARENS (type, __VA_ARGS__))
#define HasMethods(type, method, ...) EXPAND(__HAS_METHODS_IMPL(type, method, __VA_ARGS__))