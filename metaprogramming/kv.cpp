#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <type_traits>

// C++11 works fine in this file


// C++ language standard detection

#if (defined(__cplusplus) && __cplusplus >= 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    #define JSON_HAS_CPP_20
    #define JSON_HAS_CPP_17
    #define JSON_HAS_CPP_14
#elif (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_HAS_CXX17) && _HAS_CXX17 == 1) // fix for issue #464
    #define JSON_HAS_CPP_17
    #define JSON_HAS_CPP_14
#elif (defined(__cplusplus) && __cplusplus >= 201402L) || (defined(_HAS_CXX14) && _HAS_CXX14 == 1)
    #define JSON_HAS_CPP_14
#endif


enum class value_t : std::uint8_t
{
    null,
    object,
    array,
    string,
    boolean,   
    number_integer,
    discarded
};

/*Some templates to detect types*/
template<typename T>
struct is_Value_t : std::false_type {};

template<>
struct is_Value_t<value_t> : std::true_type {};

template<typename T>
using cleanType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
/*End of templates*/

#ifdef DEBUG_TRACE
#define PrintFunctionName std::cout << __LINE__ <<" : "<<__PRETTY_FUNCTION__ << std::endl; /* replace with __FUNC__ in case of error */
#else
#define PrintFunctionName (0);
#endif

#ifdef DEBUG_TRACE2
#define PrintFunctionName2 std::cout << __LINE__ <<" : "<<__PRETTY_FUNCTION__ << std::endl; /* replace with __FUNC__ in case of error */
#else
#define PrintFunctionName2 (0);
#endif


class json {

public:
    using reference = json&;
    using object_t = std::map<std::string,json>; /*container customization TODO later*/
    using array_t = std::vector<json>; /*container customization TODO later*/
    using string_t = std::string; /*string customization TODO later*/
    using boolean_t = bool;
    using number_integer_t = std::int32_t;

    template<typename T, typename... Args>
    static T* create(Args&& ... args)
    {
        PrintFunctionName
        /*Memory allocation customization can be done by using custom allocator
        TODO later */

        T* obj = new T(std::forward<Args>(args)...);
        return obj;
    }

    /*hold one value at a time */
    /* we will use an enum to construct the correct type*/
    union json_value
    {
        object_t* object;
        array_t* array;
        string_t* string;
        boolean_t boolean;
        number_integer_t number_integer;

        json_value() = default;
        json_value(boolean_t v) : boolean(v) {
            PrintFunctionName
        }
        json_value(number_integer_t v) : number_integer(v) {
            PrintFunctionName
        }

        /* for creating empty object, array, strings */
        json_value(value_t t)
        {
            PrintFunctionName
            switch (t)
            {
                case value_t::object:
                {
                    object = create<object_t>();
                    break;
                }

                case value_t::array:
                {
                    array = create<array_t>();
                    break;
                }

                case value_t::string:
                {
                    string = create<string_t>("");
                    break;
                }

                case value_t::boolean:
                {
                    boolean = boolean_t(false);
                    break;
                }

                case value_t::number_integer:
                {
                    number_integer = number_integer_t(0);
                    break;
                }

                case value_t::null:
                {
                    object = nullptr;  // silence warning, see #821
                    break;
                }

                default:
                {
                    std::cout <<"Value Type is not correct!" << std::endl;
                    break;
                }
            }
        }

        /// constructor for strings
        json_value(const string_t& value)
        {
            PrintFunctionName2
            string = create<string_t>(value);
        }

        /// constructor for rvalue strings
        json_value(string_t&& value)
        {
            PrintFunctionName2
            string = create<string_t>(std::move(value));
        }

        /// constructor for objects
        json_value(const object_t& value)
        {
            PrintFunctionName2
            object = create<object_t>(value);
        }

        /// constructor for rvalue objects
        json_value(object_t&& value)
        {
            PrintFunctionName2
            object = create<object_t>(std::move(value));
        }

        /// constructor for arrays
        json_value(const array_t& value)
        {
            PrintFunctionName
            array = create<array_t>(value);
        }

        /// constructor for rvalue arrays
        json_value(array_t&& value)
        {
            PrintFunctionName
            array = create<array_t>(std::move(value));
        }
    };

    /*Data Members*/
    value_t m_type = value_t::null;
    json_value m_value = {};






    // a utility function
    // TOOD add all types of type qeury/checkers 
    bool is_null()
    {
        PrintFunctionName
        return m_type == value_t::null;
    }




    // only two constructors for taking value_t 
    // will not be called by user, thee constuctors are internal to library
    json(const value_t v)
        : m_type(v), m_value(v)
    {
        PrintFunctionName2
    }

    json(std::nullptr_t = nullptr)
        : json(value_t::null) /*init place in most cases */
    {
        PrintFunctionName2
    }





    // access operator[]
    // TODO read only operator[]
    // TODO at function

    reference operator[](const typename object_t::key_type& key)
    {
        PrintFunctionName2
        // silently converts null to a valid jsob object
        // because user is using [] operator on it to access some value
        if (is_null())
        {
            m_type = value_t::object;
            m_value.object = create<object_t>();
        }

        return m_value.object->operator[](key); // calls std::map [] operator
    }
    



    // No value get method implemented of now
    
    /*separating out construction parts for json serializer class
    todo later */
    void construct(json& obj, number_integer_t val)
    {
        PrintFunctionName
        obj.m_type = value_t::number_integer;
        obj.m_value = val;
    }

    void construct(json& obj, string_t& s)
    {
        PrintFunctionName2
        obj.m_type = value_t::string;
        obj.m_value = s;
    }

    void construct(json& obj, string_t&& s)
    {
        PrintFunctionName2
        obj.m_type = value_t::string;
        obj.m_value = s;
    }




    // Catch all constructor for all possible json compatible types
    // TODO actually check types and disable this constructor 
    template <typename T,
                /* Check other Compatibility check for this CompatibleType */
                /* I want to disable this constructor if CompatibleType = value_t 
                This is just a short term check...  Compatibility check will do the work*/
                #if !defined(JSON_HAS_CPP_14)
                typename = typename std::enable_if<!is_Value_t<cleanType<T> >::value>::type
                #else
                typename = std::enable_if_t<!is_Value_t<cleanType<T> >::value>
                #endif
                /*Add more constraints if required later */
            >
    json(T&& val) {
        /*https://stackoverflow.com/questions/3582001/what-are-the-main-purposes-of-using-stdforward-and-which-problems-it-solves*/
        PrintFunctionName2
        construct(*this,std::forward<T>(val));
    }

    // ~json() {
    //     if(!is_null()) {
    //         // deallocate
    //         auto it = m_value.object->begin();
    //         while(it != nullptr) {
    //             if(it->second.m_type == value_t::object) {
    //                 // again deallocate
    //             }
    //             else {

    //             }
    //         }
    //     }
    // }

};

int main() {

    #ifdef DEBUG1
    json a;


    // Only number and strings are supported in this prototype
    // lot of other assignments (primitive or hybrid) are not supprted now
    // for example a["xyz"] = {"a":200};
    // for example a["pqr"] = ["item1","item2"]
    // for example a["x1"] = { 
    //                            "a" : 123,
    //                            ["deka","ssir"],
    //                            {
    //                                "k1":"ocean",
    //                                "k2":"water"
    //                            }
    //                       }
    
    a["k1"] = 12; // a becomes a object 
    a["k2"]["k3"] = "samsung";
    a["k4"]["k7"] = "oracle";
    a["k4"]["k8"] = 123;
    a["k4"]["k9"] = "google";

    std::cout << std:: endl <<"========= Type of a : "<<static_cast<int>(a.m_type) <<" =========" << std::endl << std::endl;
    
    /*We can hide (for library maintainer) the implementation details of .m_value.object etc etc ..*/
    /*Todo later */

    /*we also not have a good print facility now*/
    std::cout <<"Number of keys in a "<<a.m_value.object->size() << std::endl;
    std::cout <<"Number of keys in a[\"k2\"] "<<a["k2"].m_value.object->size() << std::endl;
    std::cout <<"Number of keys in a[\"k4\"] "<<a["k4"].m_value.object->size() << std::endl;

    /*Try to check the values also, not correct way to access values,
    TODO good apis for prnit, debug and iteration */

    std::cout <<"Value of a[\"k4\"][\"k7\"] "<<*(a["k4"]["k7"].m_value.string) << std::endl;
    std::cout <<"Value of a[\"k4\"][\"k8\"] "<<a["k4"]["k8"].m_value.number_integer << std::endl;
    #endif
    



    
    #ifdef DEBUG2
    json b;

    b = 12; // b remains as a number 
    // enum class value_t : std::uint8_t
    // {
    //     null,
    //     object,
    //     array,
    //     string,
    //     boolean,   
    //     number_integer,
    //     discarded
    // };
    /*check the type, TODO better way to check this */
    std::cout <<"Type of b : "<< static_cast<int>(b.m_type) <<" " << std::endl;

    #endif

    #ifdef DEBUG3
    json c;
    c = "rain"; // c remains as a string
    // enum class value_t : std::uint8_t
    // {
    //     null,
    //     object,
    //     array,
    //     string,
    //     boolean,   
    //     number_integer,
    //     discarded
    // };
    /*check the type, TODO better way to check this */
    std::cout <<"Type of c : "<< static_cast<int>(c.m_type) <<" " << std::endl;

    #endif

    #ifdef DEBUG4
    json d = "rain"; // c remains as a strin
    // enum class value_t : std::uint8_t
    // {
    //     null,
    //     object,
    //     array,
    //     string,
    //     boolean,   
    //     number_integer,
    //     discarded
    // };
    /*check the type, TODO better way to check this */
    std::cout <<"Type of d : "<< static_cast<int>(d.m_type) <<" " << std::endl;

    delete d.m_value.string;
    
    #endif
    

    /*TODO Serialization ....*/
    /*TODO parsing ....*/

}
