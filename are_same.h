#ifndef _ARE_SAME_HS_
#define _ARE_SAME_HS_

#include <type_traits>

namespace saitou{

	template<typename T, typename ...>
	struct are_same: std::true_type{
	};

	template<typename T, typename U, typename ...Ts>
	struct are_same<T, U, Ts...>: std::bool_constant<std::is_same<T, U>{} && are_same<T, Ts...>{}>{
	};

}
#endif
