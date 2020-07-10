#ifndef _STATIC_COMPARE_HS_
#define _STATIC_COMPARE_HS_

namespace saitou{

template<auto N1, decltype(N1) N2>
struct larger_than{
	static constexpr bool value = N1 > N2;
	constexpr operator bool() const noexcept{
		return N1 > N2;
	}
};
template<auto N1, decltype(N1) N2>
struct less_than{
	static constexpr bool value = N1 < N2;
	constexpr operator bool() const noexcept{
		return N1 < N2;
	}
};
template<auto N1, decltype(N1) N2>
struct greater_equal{
	static constexpr bool value = N1 >= N2;
	constexpr operator bool() const noexcept{
		return N1 >= N2;
	}
};
template<auto N1, decltype(N1) N2>
struct less_equal{
	static constexpr bool value = N1 <= N2;
	constexpr operator bool() const noexcept{
		return N1 <= N2;
	}
};


}

#endif

