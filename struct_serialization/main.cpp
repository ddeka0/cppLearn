#include <iostream>
#include <fstream>
#include <array>
#include "json.hpp"
#include <string>
#include "message_headers.hpp"

using namespace std;
using namespace nlohmann;


template<typename T, T mPtr, unsigned Index>
struct MemberBinding {
	constexpr static T value = mPtr;
	constexpr static unsigned index = Index;
};

template<typename...>
struct Pack {};

template<typename>
struct Members{};

template<typename>
struct PM_traits {}; 

template<class T, class U>
struct PM_traits<U T::*> {
	using member_type = U;
};






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

#define MAKE_ARG_LIST_1(op, arg, ...)   op(arg)
#define MAKE_ARG_LIST_2(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_1(op, __VA_ARGS__))
#define MAKE_ARG_LIST_3(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_2(op, __VA_ARGS__))
#define MAKE_ARG_LIST_4(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_3(op, __VA_ARGS__))
#define MAKE_ARG_LIST_5(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_4(op, __VA_ARGS__))
#define MAKE_ARG_LIST_6(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_5(op, __VA_ARGS__))
#define MAKE_ARG_LIST_7(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_6(op, __VA_ARGS__))
#define MAKE_ARG_LIST_8(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_7(op, __VA_ARGS__))
#define MAKE_ARG_LIST_9(op, arg, ...)   op(arg), MACRO_EXPAND(MAKE_ARG_LIST_8(op, __VA_ARGS__))
#define MAKE_ARG_LIST_10(op, arg, ...)  op(arg), MACRO_EXPAND(MAKE_ARG_LIST_9(op, __VA_ARGS__))

#define MEM_BIND(mptr,I) MemberBinding<decltype(mptr), mptr, I>

#define MAKE_MEMBER_BIND_LIST_1(indx, arg, ...)   MEM_BIND(arg,indx)
#define MAKE_MEMBER_BIND_LIST_2(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_1(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_3(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_2(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_4(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_3(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_5(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_4(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_6(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_5(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_7(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_6(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_8(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_7(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_9(indx, arg, ...)   MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_8(indx+1, __VA_ARGS__))
#define MAKE_MEMBER_BIND_LIST_10(indx, arg, ...)  MEM_BIND(arg,indx), MACRO_EXPAND(MAKE_MEMBER_BIND_LIST_9(indx+1, __VA_ARGS__))

#define FIELD(t) t

#define MAKE_ARRAY(NAME, ...) \
		MAKE_ARRAY_IMPL(NAME,GET_ARG_COUNT(__VA_ARGS__),__VA_ARGS__)

#define MAKE_ARRAY_IMPL(NAME, N, ...) \
constexpr std::array<const char*, N> arr_##NAME = {MACRO_CONCAT(CON_STR,N)(__VA_ARGS__)}
#define MACRO_CONCAT(A,B) A##_##B
#define MACRO_EXPAND(x) x

#define MAKE_ARG_LIST(N, op, arg, ...) \
        MACRO_CONCAT(MAKE_ARG_LIST, N)(op, arg, __VA_ARGS__)

#define MAKE_MEMBER_BIND_LIST(N, arg, ...) \
		MACRO_CONCAT(MAKE_MEMBER_BIND_LIST,N)(0,arg,__VA_ARGS__)


#define MAKE_META_DATA_IMPL(STRUCT_NAME, ...) \
		template<>	\
		struct Members<STRUCT_NAME>	{	\
			using type = Pack< \
				MAKE_MEMBER_BIND_LIST(GET_ARG_COUNT(__VA_ARGS__),__VA_ARGS__) \
			>;	\
			constexpr static const char * name = #STRUCT_NAME; \
			static const char * const * names() { \
				return arr_##STRUCT_NAME; \
			} \
		}; \


#define MAKE_META_DATA(STRUCT_NAME, N, ...) \
    	static const char* arr_##STRUCT_NAME[] = { MACRO_EXPAND(MACRO_CONCAT(CON_STR, N)(__VA_ARGS__)) };\
    	MAKE_META_DATA_IMPL(STRUCT_NAME, MAKE_ARG_LIST(N, &STRUCT_NAME::FIELD, __VA_ARGS__))


#define REFLECTION(STRUCT_NAME, ...) \
		MAKE_META_DATA(STRUCT_NAME, GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)


template<typename A, typename MT, MT MPTR, unsigned Ndx, typename... Rest>
void PrintPack(std::ostream & out, const A &v,
						Pack<MemberBinding<MT,MPTR,Ndx>, Rest...> * metadata) {
	using M = Members<A>;
	out <<" "<< Ndx <<" : "<<M::names()[Ndx] <<" : "<< v.*MPTR<<std::endl;
	PrintPack(out,v,(Pack<Rest...> *)nullptr);
}

template<typename A>
void PrintPack(std::ostream &out, const A &v, Pack<> * metadata) {}



void parseFile(std::string fileName,json& j) {
	std::ifstream _file(fileName);
	_file >> j;
}


// template<typename MessageType>
// void fillMessage(const json& j,MessageType& m) {

// 	#define Entry_L1(v1)  					\
// 	do {									\
// 		m.v1 = j[#v1];						\
// 	}while(0);								\

// 	#define Entry_L2(v1,v2)  				\
// 	do {									\
// 		m.v1.v2 = j[#v1][#v2];				\
// 	}while(0);								\

// 	#define Entry_L3(v1,v2,v3)  			\
// 	do {									\
// 		m.v1.v2.v3 = j[#v1][#v2][#v3];		\
// 	}while(0);								\


// 	#include "conversion_messageA.hpp"
// 	#undef Entry_L1
// 	#undef Entry_L2
// 	#undef Entry_L3
// }

template<typename MessageType,typename MT, MT MPTR, int isNested>
struct CopyParam;


template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,0> {
	static void copy(const json& j,MessageType& m,const std::string& key) {
		m.*MPTR = j[key];
	}
};

template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,1> {
	static void copy(const json& j,MessageType& m,const std::string& key) {
		// do nothing
	}
};


template<typename MessageType, typename MT, MT MPTR, unsigned Ndx, typename... Rest>
void fillMessage(const json& j, MessageType& m, Pack<MemberBinding<MT,MPTR,Ndx>, Rest...> * metadata) {
	using M = Members<MessageType>;
	CopyParam<MessageType,MT,MPTR, std::is_class<typename PM_traits<MT>::member_type>::value >::copy(j,m,M::names()[Ndx]);
	fillMessage(j,m,(Pack<Rest...> *)nullptr);
}

template<typename MessageType>
void fillMessage(const json& j, MessageType& m, Pack<> * metadata) {}


REFLECTION(messageA,a,b,c);
// REFLECTION(messageB,p,q,r);
// REFLECTION(messageC,x);

int main() {


	json j;
	parseFile("input_a.json",j);

	messageA ma{};

	using M = Members<messageA>;
	fillMessage(j,ma,(typename M::type*)nullptr);

	// std::cout << ma.a << std::endl;
	// std::cout << ma.b << std::endl;
	// std::cout << ma.c.p << std::endl;
	// std::cout << ma.c.q << std::endl;
	// std::cout << ma.c.r.x << std::endl;
}