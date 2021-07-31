#include <iostream>
#include <array>
#include <string>


#define RSEQ_N()    10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define GET_ARG_COUNT_INNER(...) MACRO_EXPAND(ARG_N(__VA_ARGS__))
#define GET_ARG_COUNT(...)   GET_ARG_COUNT_INNER(__VA_ARGS__, RSEQ_N())

#define CON_STR_1(element, ...) #element
#define CON_STR_2(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_1(__VA_ARGS__))
#define CON_STR_3(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_2(__VA_ARGS__))
#define CON_STR_4(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_3(__VA_ARGS__))
#define CON_STR_5(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_4(__VA_ARGS__))
#define CON_STR_6(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_5(__VA_ARGS__))
#define CON_STR_7(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_6(__VA_ARGS__))
#define CON_STR_8(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_7(__VA_ARGS__))
#define CON_STR_9(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_8(__VA_ARGS__))
#define CON_STR_10(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_9(__VA_ARGS__))
#define SEPERATOR ,


#define MAKE_ARRAY(NAME, ...) \
		MAKE_ARRAY_IMPL(NAME,GET_ARG_COUNT(__VA_ARGS__),__VA_ARGS__)

#define MAKE_ARRAY_IMPL(NAME, N, ...) \
constexpr std::array<const char*, N> arr_##NAME = {MACRO_CONCAT(CON_STR,N)(__VA_ARGS__)}
#define MACRO_CONCAT(A,B) A##_##B
#define MACRO_EXPAND(x) x

template<typename T, T mPtr, unsigned Index>
struct MemberBinding {
	constexpr static T value = mPtr;
	constexpr static unsigned index = Index;
};

template<typename...>
struct Pack {};

template<typename>
struct Members{};


struct Aggregate {
	int member1;
	std::string member2;
};

// specialize this template struct for aggregate
template<>
struct Members<Aggregate> {
	using type = Pack<
		MemberBinding<decltype(&Aggregate::member1), &Aggregate::member1, 0>,
		MemberBinding<decltype(&Aggregate::member2), &Aggregate::member2, 1>
	>;

	constexpr static const char * name = "Aggregate";
	static const char * const * names() {
		static const char* rv[] = {"member1","member2"};
		return rv;
	}
};


template<typename A, typename MT, MT MPTR, unsigned Ndx, typename... Rest>
void PrintPack(std::ostream & out, const A &v,
						Pack<MemberBinding<MT,MPTR,Ndx>, Rest...> * metadata) {
	using M = Members<A>;
	out <<" "<< Ndx <<" : "<<M::names()[Ndx] <<" : "<< v.*MPTR<<std::endl;
	PrintPack(out,v,(Pack<Rest...> *)nullptr);
}

template<typename A>
void PrintPack(std::ostream &out, const A &v, Pack<> * metadata) {}

auto func() {
	return 1;
}

int main() {
	std::cout << GET_ARG_COUNT(a,b,c,d) << std::endl;
	MAKE_ARRAY(t,a,b,c,e,d,f);

	for(auto e: arr_t) {
		std::cout << e << std::endl;
	}

	Aggregate val;
	val.member1 = 12;
	val.member2 = "Deka";

	using M = Members<Aggregate>;

	PrintPack(std::cout,val,(typename M::type *)nullptr);

}