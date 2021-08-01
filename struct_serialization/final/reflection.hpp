#pragma once

#include <array>
#include "json.hpp"
#include <string>
#include "type_traits.hpp"

using namespace std;
using namespace nlohmann;


/*-----------------------------------------------------------------------------*/
/// Main templates for reflection
template<typename T, T mPtr, unsigned Index>
struct MemberBinding {
	constexpr static T value = mPtr;
	constexpr static unsigned index = Index;
};

template<typename...>
struct Pack {};

template<typename>
struct Members{};

/*-----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------*/
/// SOME TAGS for template function overloading resolution
constexpr int NON_CLASS_TYPE_TAG 		= 0;
constexpr int CLASS_TYPE_TAG 			= 1;
constexpr int PRIMITIVE_TYPE_TAG		= 2;
constexpr int ARRAY_TYPE_TAG			= 3;
constexpr int PRIMITIVE_ARRAY_ITEM_TAG	= 4;
constexpr int CLASS_ARRAY_ITEM_TAG		= 5;
/*-----------------------------------------------------------------------------*/





/*-----------------------------------------------------------------------------*/
/// START of macro definition to write template code automatically
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

/*
/// These macros are not used for now, do not delete them
#define MAKE_ARRAY(NAME, ...) \
		MAKE_ARRAY_IMPL(NAME,GET_ARG_COUNT(__VA_ARGS__),__VA_ARGS__)
#define MAKE_ARRAY_IMPL(NAME, N, ...) \
constexpr std::array<const char*, N> arr_##NAME = {MACRO_CONCAT(CON_STR,N)(__VA_ARGS__)}
*/

#define MACRO_CONCAT(A,B) A##_##B
#define MACRO_EXPAND(x) x

#define MAKE_ARG_LIST(N, op, arg, ...) \
        MACRO_CONCAT(MAKE_ARG_LIST, N)(op, arg, __VA_ARGS__)

#define MAKE_MEMBER_BIND_LIST(N, arg, ...) \
		MACRO_CONCAT(MAKE_MEMBER_BIND_LIST,N)(0,arg,__VA_ARGS__)



/**
 * @brief This is the critical MACRO which creates specialized Members
 * of user requested structs
 * The Member struct holds all the meta-data to reflect the user defined
 * struct
 */
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

/**
 * @brief REFLECTION is the ONLY user API MACRO, that needs to be used
 * to enable reflection capability
 */
#define REFLECTION(STRUCT_NAME, ...) \
		MAKE_META_DATA(STRUCT_NAME, GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)


/*-----------------------------------------------------------------------------*/




/*-----------------------------------------------------------------------------*/
/// START of Reflective Print utility

template<typename MessageType,typename MT, MT MPTR, int Tag>
struct PrintItem;


template<typename A,typename MT, MT MPTR>
struct PrintItem<A,MT,MPTR,PRIMITIVE_TYPE_TAG> {
	static void Print(const A& v,const std::string& name,int offset) {
		std::cout << std::string(2*offset,' ') << name <<" : "<< v.*MPTR << std::endl;
	}
};

template<typename A,typename MT, MT MPTR>
struct PrintItem<A,MT,MPTR,PRIMITIVE_ARRAY_ITEM_TAG> {
	static void Print(const A& v,const std::string&,int,int idx) {
		std::cout << (v.*MPTR)[idx];
	}
};

template<typename A,typename MT, MT MPTR>
struct PrintItem<A,MT,MPTR,CLASS_ARRAY_ITEM_TAG> {
	static void Print(const A& v,const std::string&,int offset,int idx) {
		using M = Members<typename MT_traits<typename MT_traits<MT>::member_type>::member_type>;
		std::cout << std::endl;
		PrintPack((v.*MPTR)[idx],(typename M::type*)nullptr,offset + 2);
	}
};


template<typename A,typename MT, MT MPTR>
struct PrintItem<A,MT,MPTR,ARRAY_TYPE_TAG> {
	static void Print(const A& v,const std::string& name,int offset) {
		const int Sz = GetArrLen(v.*MPTR);
		using M = typename MT_traits<typename MT_traits<MT>::member_type>::member_type;
		std::cout <<std::string(2*offset,' ')<<name<<" : [";
		for(int i = 0;i<Sz;i++) {
			if(i && std::is_class<M>::value) std::cout <<std::string(2*offset,' ')<<"   ,";
			else if(i) std::cout <<",";
			PrintItem<A,MT,MPTR, PRIMITIVE_ARRAY_ITEM_TAG + std::is_class<M>::value >::Print(v,name,offset,i);
		}
		if(std::is_class<M>::value) std::cout <<std::string(2*offset,' ')<<"]"<< std::endl;
		else std::cout <<"]"<< std::endl;
	}
};



template<typename A,typename MT, MT MPTR>
struct PrintItem<A,MT,MPTR,NON_CLASS_TYPE_TAG> {
	static void Print(const A& v,const std::string& name,int offset) {
		PrintItem<A,MT,MPTR,
			PRIMITIVE_TYPE_TAG + std::is_array<typename MT_traits<MT>::member_type>::value>::Print(v,name,offset);
	}
};

template<typename A,typename MT, MT MPTR>
struct PrintItem<A,MT,MPTR,CLASS_TYPE_TAG> {
	static void Print(const A& v,const std::string&,int offset) {
		using M = Members<typename MT_traits<MT>::member_type>;
		PrintPack(v.*MPTR,(typename M::type*)nullptr,offset + 1);
	}
};


template<typename A, typename MT, MT MPTR, unsigned Ndx,typename... Rest>
void PrintPack(const A &v,Pack<MemberBinding<MT,MPTR,Ndx>, Rest...> *,int offset = 0) {
	using M = Members<A>;
	PrintItem<A,MT,MPTR, std::is_class<typename MT_traits<MT>::member_type>::value >::Print(v,M::names()[Ndx],offset);
	PrintPack(v,(Pack<Rest...> *)nullptr,offset);
}

/**
 * @brief Terminative specialization
 * 
 * @tparam A 
 */
template<typename A>
void PrintPack(const A&, Pack<> *,int) {}
/*-----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------*/
/// START of JSON TO STRUCT COPY functions

/**
 * @brief Primary template for CopyParam
 * 
 * @tparam MessageType -- type of the struct
 * @tparam MT -- Member type
 * @tparam MPTR -- Object of type MT, Basically of (ClassType::*) type
 * @tparam Tag -- overload resolution
 */
template<typename MessageType,typename MT, MT MPTR, int Tag>
struct CopyParam;

template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,PRIMITIVE_TYPE_TAG> {
	static void copy(const json& j,MessageType& m,const std::string& key) {
		m.*MPTR = j[key];
	}
};

template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,PRIMITIVE_ARRAY_ITEM_TAG> {
	static void copy(const json& j,MessageType& m,const std::string& key,int idx) {
		(m.*MPTR)[idx] = j[key][idx];
	}
};

template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,CLASS_ARRAY_ITEM_TAG> {
	static void copy(const json& j,MessageType& m,const std::string& key,int idx) {
		using M = Members<typename MT_traits<typename MT_traits<MT>::member_type>::member_type>;
		fillMessage(j[key][idx],(m.*MPTR)[idx],(typename M::type*)nullptr);
	}
};

template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,ARRAY_TYPE_TAG> {
	static void copy(const json& j,MessageType& m,const std::string& key) {
		const int Sz = min(j[key].size(),GetArrLen(m.*MPTR));
		using M = typename MT_traits<typename MT_traits<MT>::member_type>::member_type;
		for(int i = 0;i<Sz;i++) {
			CopyParam<MessageType,MT,MPTR, PRIMITIVE_ARRAY_ITEM_TAG + std::is_class<M>::value >::copy(j,m,key,i);
		}
	}
};


template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,NON_CLASS_TYPE_TAG> {
	static void copy(const json& j,MessageType& m,const std::string& key) {
		CopyParam<MessageType,MT,MPTR,
		PRIMITIVE_TYPE_TAG + std::is_array<typename MT_traits<MT>::member_type>::value>::copy(j,m,key);
	}
};

template<typename MessageType,typename MT, MT MPTR>
struct CopyParam<MessageType,MT,MPTR,CLASS_TYPE_TAG> {
	static void copy(const json& j,MessageType& m,const std::string& key) {
		using M = Members<typename MT_traits<MT>::member_type>;
		fillMessage(j[key],m.*MPTR,(typename M::type*)nullptr);
	}
};


template<typename MessageType, typename MT, MT MPTR, unsigned Ndx, typename... Rest>
void fillMessage(const json& j, MessageType& m, Pack<MemberBinding<MT,MPTR,Ndx>, Rest...> *) {
	using M = Members<MessageType>;
	CopyParam<MessageType,MT,MPTR, std::is_class<typename MT_traits<MT>::member_type>::value >::copy(j,m,M::names()[Ndx]);
	fillMessage(j,m,(Pack<Rest...> *)nullptr);
}


/**
 * @brief Terminating specialization
 * 
 * @tparam MessageType 
 */
template<typename MessageType>
void fillMessage(const json&, MessageType&, Pack<> *) {}
/*-----------------------------------------------------------------------------*/