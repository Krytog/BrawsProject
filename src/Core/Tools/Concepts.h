#define MACRO_CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) MACRO_CONCAT_IMPL(x, y)

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