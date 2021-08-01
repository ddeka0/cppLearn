#pragma once
#include <cstdlib>

template<typename>
struct MT_traits {}; 



template<class T, class U>
struct MT_traits<U T::*> {
	using member_type = U;
};


// template<class T, class U,std::size_t Size>
// struct PM_traits<U (T::*)[Size]> {
// 	using member_type = U;
// 	constexpr static unsigned len = Size;
// };


template<class T, std::size_t Size>
struct MT_traits<T[Size]> {
	using member_type = T;
	constexpr static unsigned len = Size;
};

template<class T, std::size_t Size>
struct MT_traits<T(&)[Size]> {
	using member_type = T;
	constexpr static unsigned len = Size;
};


template<typename T, std::size_t Size>
std::size_t GetArrLen(T(&)[Size]) { return Size; }