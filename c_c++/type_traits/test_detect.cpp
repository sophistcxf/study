
template<typename _Default, typename _AlwaysVoid,
template<typename...> class _Op, typename... _Args>
struct __detector
{
    using type = _Default;
    using __is_detected = std::false_type;
};

template<typename _Default, template<typename...> class _Op,
    typename... _Args>
struct __detector<_Default, __void_t<_Op<_Args...>>, _Op, _Args...>
{
    using type = _Op<_Args...>;
    using __is_detected = std::true_type;
};

template<typename _Default, template<typename...> class _Op,
    typename... _Args>
using __detected_or_t
    = typename __detected_or<_Default, _Op, _Args...>::type;

int main() {
    return 0;
}