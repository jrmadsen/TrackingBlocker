//
//
//
//
//


#ifndef typeconversion_hh_
#define typeconversion_hh_

#include <vector>
#include <deque>

#include <sstream>
#include <istream>
#include <fstream>
#include <ostream>
#include <iostream>

// Define C++11
#ifndef CXX11
#   if __cplusplus > 199711L   // C++11
#       define CXX11
#   endif
#endif

// Define C++14
#ifndef CXX14
#   if __cplusplus > 201103L   // C++14
#       define CXX14
#   endif
#endif

#ifdef CXX14
typedef  uint8_t    uint8;
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef uint64_t    uint64;
#else
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long   uint64;
#endif
typedef unsigned char   uchar;

//============================================================================//

namespace CONVERSION
{
    //========================================================================//
    //                                                      Convert type
    template <typename T, typename U>
    inline void convert(T oldval, U& newval)
    {
        std::istringstream iss;
        iss << oldval;
        iss >> newval;
    }
    //========================================================================//
    //                                                      Convert type
    template <typename T, typename U>
    inline T convert(U oldval)
    {
        std::istringstream iss(oldval);
        T newval = T();
        iss >> newval;
        return newval;
    }
    //========================================================================//
    //                                                      Make String Lower
    inline std::string lower(std::string str)
    {
        for(std::string::size_type i = 0; i < str.size(); ++i)
            str[i] = tolower(str[i]);

        return str;
    }
    //========================================================================//
    //                                                      Make String Lower
    inline void MakeLower(std::string& str)
    {
        str = lower(str);
    }

}

//============================================================================//
//                                                              BASE - GENERIC
template <typename T>
class TypeConversion
{
public:
    template <typename U>
    T operator()(U val)
    {
        T result = CONVERSION::convert<T>(val);
        return result;
    }

    template <typename U>
    std::vector<T> operator[](const U& val)
    {
        std::vector<T> result;
        for(typename U::const_iterator itr = val.begin(); itr != val.end(); ++itr)
            result.push_back(CONVERSION::convert<T>(*itr));
        result.resize(val.size(), T());
        return result;
    }

    // no generic way to invert
    T invert(T val) { return val; }

    template <typename U>
    std::vector<typename U::value_type> invert(const U& val)
    {
        std::vector<typename U::value_type> result;
        for(typename U::const_iterator itr = val.begin(); itr != val.end(); ++itr)
            result.push_back(*itr);
        result.resize(val.size(), T());
        return result;
    }
};

//============================================================================//
//                                                              STRING
template <>
class TypeConversion<std::string>
{
public:
    std::string operator()(std::string val) { return val; }

    template <typename U>
    std::vector<typename U::value_type> operator[](const U& val)
    {
        std::vector<typename U::value_type> result;
        for(typename U::const_iterator itr = val.begin(); itr != val.end(); ++itr)
            result.push_back(*itr);
        return result;
    }

    std::string invert(std::string val) { return val; }
    template <typename _Tp>
    std::vector<std::string> invert(const _Tp& val)
    {
        return val;
    }
};

//============================================================================//
//                                                              BOOLEAN
template <>
class TypeConversion<bool>
{
public:
    bool operator()(std::string val) { return GetBoolFromString(val); }
    std::vector<bool> operator[](const std::vector<std::string>& val)
    { return GetBoolFromString(val); }

    bool invert(bool val) { return !val; }
    template <typename _Tp>
    std::vector<bool> invert(const _Tp& val)
    {
        std::vector<bool> returned;
        for(typename _Tp::iterator itr = val.begin(); itr != val.end(); ++itr)
            returned.push_back(!(*itr));
        return returned;
    }

private:
    //========================================================================//
    //                                                      Get Bool From String
    inline bool GetBoolFromString(std::string str)
    {
        if(str.length() == 0) { return true; }
        CONVERSION::MakeLower(str);
        if(str.length() == 1) {
            if(str == "t" || str == "y") { return true; }
        } else {
            if(str.find("true") != std::string::npos ||
               str.find("yes") != std::string::npos ||
               str == "on") { return true; }
        }
        return false;
    }
    //========================================================================//
    //                                                      Get Bool From String
    inline std::vector<bool> GetBoolFromString(const std::vector<std::string>& str)
    {
        std::vector<bool> boolstr(str.size(),false);
        for(uint32 i = 0; i < str.size(); ++i) {
            boolstr.at(i) = GetBoolFromString(str.at(i));
        }
        return boolstr;
    }
};

//============================================================================//
//                                                      BASE POINTER - GENERIC
template <typename T>
class PointerTypeConversion
{
public:
    template <typename U>
    T* operator()(U val) {
        return new T(val);
    }

    template <typename U>
    T* operator()(const std::vector<U>& val) {
        return new T(val);
    }

    T* create() { return new T; }

};

//============================================================================//


#endif
