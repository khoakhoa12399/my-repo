#ifndef _ARE_ALL_CONVERTIBLE_HS_
#define _ARE_ALL_CONVERTIBLE_HS_

#include <type_traits>
namespace saitou{

template<typename T, typename ...Ts>
struct are_all_convertible: std::true_type{
};
template<typename T, typename U, typename ...Ts>
struct are_all_convertible<T, U, Ts...>: std::bool_constant<std::is_convertible<U, T>::value && are_all_convertible<T, Ts...>::value>{
};

}
#endif
