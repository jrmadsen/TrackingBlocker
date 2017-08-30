//
//
//
//  A toolkit of functions for global use
//
//

#ifndef mytools_hh_
#define mytools_hh_

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

// Clang specific
#if __clang__ && __clang_major__ >= 5 // Clang 5.0 got rid of tr1 folder
#   define CLANG_V5
#endif


//=================================//
// Containers
#include <string>
#include <vector>
#include <map>

#ifdef CXX11   // C++11
#   include <unordered_map>
#   include <unordered_set>
#else
#   if __clang__ && __clang_major__ >= 5 // Clang 5.0 got rid of tr1 folder
#       include <unordered_map>
#       include <unordered_set>
#   else
#       include <tr1/unordered_map>
#       include <tr1/unordered_set>
#   endif
#endif

#include <set>
#include <iterator>
#include <queue>
#include <stack>
#include <list>
#include <deque>
//=================================//
// Numerical
#include <cfloat>
//#include <cmath>
#include <complex>

#ifdef CXX11 // C++11
#   include <random>
#endif

#include <functional>
#include <numeric>
#include <algorithm>
#include <limits>
//=================================//
// IOS Libraries
#include <ios>
#include <iomanip>
//=================================//
// Stream Libraries
#include <sstream>
#include <istream>
#include <fstream>
#include <ostream>
#include <iostream>
//=================================//
// C/C++ Libraries
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cctype>
#include <locale>
//=================================//
// RTTI
#include <typeinfo>
//=================================//
// Memory tools
#include <memory>
//=================================//
// Exceptions
#include <stdexcept>
//=================================//
// Options
#include <cstdarg>
//=================================//
// Threads
#ifdef CXX11 // C++11
#   include <thread>
#   include <mutex>
#   include <future>
#endif
//=================================//

//=================================//
// System
#include <sys/stat.h>
#include <unistd.h>
//=================================//


#include "MyHelpers.hh"

const double Avogad = 6.022E+23;

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


namespace MyTools
{
    typedef std::vector<std::string>             StrVector_t;

    inline void MakeLower(std::string&);
    inline void MakeUpper(std::string&);
    inline char lower(char);
    inline char upper(char);
    inline std::string lower(std::string);
    inline std::string upper(std::string);

    inline bool checkpos(std::string, std::string);
    inline bool checkposic(std::string, std::string);

    inline int StringToInt(std::string);
    inline double StringToDouble(std::string);
    inline int s2i(std::string);
    inline double s2d(std::string);
    template <typename T>
    inline void StringToNumber(std::string,T&);
    template <typename T>
    inline void s2n(std::string,T&);
    template <typename T>
    inline T StringToNumber(std::string);
    template <typename T>
    inline T s2n(std::string);
    template <typename T>
    inline std::vector<T> s2n(StrVector_t);
    inline std::string IntToString(int);
    inline std::string DoubleToString(double);
    template <typename T>
    inline std::string NumberToString(T);
    template <typename T>
    inline std::string n2s(T);
    template <typename T>
    inline StrVector_t NumberToString(const std::vector<T>&);
    template <typename T>
    inline StrVector_t n2s(const std::vector<T>&);

    inline std::string Capitalize(std::string);
    inline std::string Uncapitalize(std::string);
    inline void RemoveWhitespace(std::string&);
    inline void RemoveLeadingWhitespace(std::string&);
    inline void RemoveTrailingWhitespace(std::string&);
    inline void StripWhitespace(std::string&);
    inline bool strcmpic(std::string,std::string);

    inline StrVector_t delimit(const std::string&, char);
    inline StrVector_t delimit(const std::string&,
                               const std::string&);
    inline StrVector_t delimit(const std::string&,
                               const StrVector_t&);
    inline StrVector_t delimit(const std::string&,
                               const std::string&,
                               const StrVector_t&);
    inline StrVector_t delimit(const StrVector_t&,
                               const std::string&);

    inline std::string undelimit(StrVector_t,std::string);
    template <typename T>
    inline std::string undelimit(std::vector<T>,std::string);
    inline void undelimit(StrVector_t,char,std::string&);
    inline std::string undelimit(StrVector_t, std::string&,
                                 StrVector_t::iterator,
                                 StrVector_t::iterator);
    template <typename T>
    inline void RemoveElement(int,std::vector<T>&);
    template <typename T>
    inline void RemoveElement(std::vector<T>&,T t);

    template <typename T>
    inline void print(const std::vector<T>&,std::string,
                      std::ostream& out = std::cout);

    template <typename T>
    inline void print(const std::vector<T>&,int,
                      std::ostream& out = std::cout);

    template <typename T>
    inline void print(const std::vector<T>&,int,int,
                      std::ostream& out = std::cout);

    template <typename T, typename U>
    inline void print(const std::map<T,U>&,std::string c1 = " \t",
                      std::string c2 = "\n",std::ostream& out = std::cout);

    template <typename T, typename U>
    inline void print(const std::map<T,U>&,int,std::ostream& out = std::cout);

    template <typename T, typename U>
    inline void print(const std::map<T,U>&,int,int,
                      std::ostream& out = std::cout);

    template <typename T>
    inline std::vector<T> subvector(const std::vector<T>&, uint32, uint32);

    inline bool checkforID(std::string,std::string);
    inline bool checkforID(std::string,StrVector_t);

    inline std::string removepos(std::string,std::string);

    inline void Purge(std::string&,std::string);
    inline void Purge(std::string&,std::string,std::string);

    template <typename T>
    inline void RemoveDuplicates(std::vector<T>& vec);

    inline bool count(std::string,StrVector_t);

    template <typename T>
    inline bool IsDuplicate(T,std::vector<T>);

    template <typename T>
    inline void Scale(T&,T,T,T,T);

    template <typename T>
    inline void Scale(std::vector<std::vector<T> >&,T,T,T,T);

    template <typename T>
    inline T mean(const std::vector<T>&);

    template <typename T>
    inline T StdDev(const std::vector<T>&);
    template <typename T, typename U>
    inline T StdDev(const T&, const T&, const U&);

    template <typename T>
    inline T Variance(const std::vector<T>&);
    template <typename T, typename U>
    inline T Variance(const T&, const T&, const U&);

    template <typename T>
    inline T RelativeError(const std::vector<T>&);
    template <typename T, typename U, typename V>
    inline std::map<T, U> RelativeError(const std::map<T,U>&,
                                        const std::map<T,U>&,
                                        const std::map<T, V>&);

    template <typename T, typename U>
    inline void ExitWithoutParams(T,U,U);

    template <typename T, typename U>
    inline void ExitWithoutParams(T,U,U,std::string);

    inline bool OpenFile(std::ifstream&,std::string,bool);
    inline void OpenFile(std::ifstream&,std::string);

    inline bool OpenFile(std::ofstream&,std::string,bool);
    inline void OpenFile(std::ofstream&,std::string);

    inline void RemoveComment(std::string&, const std::string&);

    inline std::string GetLine(std::istream&, const std::string& str = "\n");
    inline bool GetLine(std::istream&,std::string&,
                        const std::string& str = "\n");
    inline StrVector_t GetLineAndDelimit(std::istream&, const std::string&);
    inline StrVector_t GetLineAndDelimit(std::istream&, const std::string&,
                                         const std::string&);
    inline bool GetLineAndDelimit(std::istream&, const std::string&,
                                  StrVector_t&);
    inline bool GetLineAndDelimit(std::istream&, const std::string&,
                                  const std::string&, StrVector_t&);
    inline bool GetLineAndDelimit(std::istream&, const std::string&,
                                  const StrVector_t&, const std::string&,
                                  StrVector_t&);

    inline std::string xmlopen(std::string);
    inline std::string xmlopen(std::string,std::string);

    inline std::string xmlclose(std::string);
    inline std::string xmlclose(std::string,std::string);

    inline std::string tablevel(int);
    inline int DetermineClose(int,std::vector<int>);

    template <typename T>
    inline void AddValue(std::vector<T>&,T);

    template <typename T>
    inline T GetMin(const std::vector<T>&);

    template <typename T>
    inline T GetMax(const std::vector<T>&);

    template <typename T>
    inline T Sum(const std::vector<T>&);

    template <typename T,typename U>
    inline void GenerateXMLblock(std::ostream&,const std::vector<T>&,
                                 std::vector<int>,
                                 const std::vector<int>&,
                                 const std::vector<U>&);
    inline void AddLine(std::ifstream&, std::string&, std::string);

    template <typename T>
    inline void lineXML(std::ostream&, std::string, T, std::string);

    template <typename T>
    inline void lineXML(std::ostream&, std::string, T, std::string, int);

    inline bool GetBool(const std::string&);
    inline std::vector<bool> GetBool(const StrVector_t&);
    inline bool GetBoolFromString(const std::string& val) { return GetBool(val); }
    inline std::vector<bool> GetBoolFromString(const StrVector_t& val)
    { return GetBool(val); }

    template <typename T>
    inline void RequireSize(const std::vector<T>&,int,std::string);

    template <typename T>
    inline bool CheckSize(const std::vector<T>&,int,std::string);

    inline std::string GetParam(const StrVector_t&, std::string);

    template <typename T>
    inline std::pair<bool,uint32> CheckForParam(const std::vector<T>&, T);
    inline std::pair<bool,uint32> CheckForParam(const StrVector_t&,
                                                std::string);

    template<typename T, typename U>
    inline std::vector<T> convert(const std::vector<U>&);

    template<typename U>
    inline StrVector_t convert2str(const std::vector<U>&);

    template <typename T>
    inline std::vector<T> convertfromstring(const StrVector_t&);

    inline void SetDoublePrecision(double&,int,double);

    template <typename T, typename U>
    inline T TruncatePrecision(const T&, const int&, const U&);

    template <typename T>
    inline typename std::vector<T>::iterator
    GetIteratorPosition(std::vector<T>,T);

    template <typename T>
    inline uint32 GetIteratorIndex(std::vector<T>,T);

    typedef std::string     str_t;
    typedef const str_t&    cr_str_t;

    inline str_t join(cr_str_t dir, cr_str_t fname,  cr_str_t delim = "/");
    inline str_t append_string(cr_str_t str, cr_str_t f);

    inline bool exists(cr_str_t);

    inline bool AddDirectory(cr_str_t);
    inline bool MakeDirectory(cr_str_t);
    inline bool mkdir(cr_str_t str) { return AddDirectory(str); }
    inline str_t MakeDirectoryString(cr_str_t str) { return append_string(str, "/"); }

    template <typename T>
    inline T interpolate(T,T,T,T,T);
    template <typename T>
    inline T loginterpolate(T,T,T,T,T);

    inline double ExplicitEuler(double,double,double);
    inline double ImplicitEuler(double,double,double);

    template <typename T> inline T factorial(uint32);
    template <typename T> inline void FreeClear(T&);

    inline bool CheckFile(const std::string&);
    inline double modulus(const double&, const double&);

    template <typename T, typename U>
    void AddVectorData2Map(const T&, std::multimap<T,U>&,
                           const std::vector<U>&);
    template <typename T, typename U>
    void AddVectorData2Map(const std::vector<T>&, std::map<T,U>&,
                           const std::vector<U>&);
    template <typename T, typename U>
    void AddVectorData2Map(const T&,
#if defined(CXX11) || defined(CLANG_V5)
                           std::unordered_multimap<T,U>&,
#else
                           std::tr1::unordered_multimap<T,U>&,
#endif
                           const std::vector<U>&);
    template <typename T, typename U>
    void AddVectorData2Map(const std::vector<T>&,
#if defined(CXX11) || defined(CLANG_V5)
                           std::unordered_map<T,U>&,
#else
                           std::tr1::unordered_map<T,U>&,
#endif
                           const std::vector<U>&);

    template <typename T>
    inline typename T::value_type* GetC_Array(const T&);

    template <typename T, typename U>
    inline std::pair<T*,U*> GetC_Array(const std::vector<std::pair<T,U> >&);

    template <typename T, typename U>
    inline bool RealIntegerCompare(const T&, const U&);

    template <typename _Tp>
    _Tp GetEnvironmentVariable(const std::string& ID, _Tp _default = _Tp());

    unsigned LevenshteinDistance(const std::string&,
                                 const std::string&);
#ifndef SWIG
    namespace details
    {
        unsigned LevenshteinDistance(std::string, std::string);
    }
#endif

    template <typename _Tp>
    _Tp GetEnv(const std::string&, _Tp = _Tp());

    template <typename value_type, typename _InputIterator>
    value_type
    l1norm(_InputIterator _begin, _InputIterator _end);

    template <typename value_type, typename _InputIterator>
    value_type
    l2norm(_InputIterator _begin, _InputIterator _end);

    template <typename value_type, typename _InputIterator>
    value_type
    norm(_InputIterator _begin, _InputIterator _end, uint16 nid);

#ifndef SWIG
    template <typename _InputIterator>
    typename _InputIterator::value_type
    l1norm(_InputIterator _begin, _InputIterator _end);

    template <typename _InputIterator>
    typename _InputIterator::value_type
    l2norm(_InputIterator _begin, _InputIterator _end);

    template <typename _InputIterator>
    typename _InputIterator::value_type
    norm(_InputIterator _begin, _InputIterator _end, uint16 nid);
#endif

    template <typename _Tp>
    typename _Tp::value_type l1norm(const _Tp& array);

    template <typename _Tp>
    typename _Tp::value_type l2norm(const _Tp& array);

    template <typename _Tp>
    typename _Tp::value_type norm(const _Tp& array, uint16 nid);

}

//============================================================================//
//                                                      Make String Lower
inline void MyTools::MakeLower(std::string& str)
{
    for(std::string::size_type i = 0; i < str.size(); ++i) {
        str[i] = tolower(str[i]);
    }
}
//============================================================================//
//                                                      Make String Upper
inline void MyTools::MakeUpper(std::string& str)
{
    for(std::string::size_type i = 0; i < str.size(); ++i) {
        str[i] = toupper(str[i]);
    }
}
//============================================================================//
//                                                      Make String Lower
inline char MyTools::lower(char c) { return tolower(c); }
//============================================================================//
//                                                      Make String Lower
inline std::string MyTools::lower(std::string str)
{
    for(std::string::size_type i = 0; i < str.size(); ++i) {
        str[i] = tolower(str[i]);
    }
    return str;
}
//============================================================================//
//                                                      Make String Upper
inline char MyTools::upper(char c) { return toupper(c); }
//============================================================================//
//                                                      Make String Upper
inline std::string MyTools::upper(std::string str)
{
    for(std::string::size_type i = 0; i < str.size(); ++i) {
        str[i] = toupper(str[i]);
    }
    return str;
}
//============================================================================//
//                                                      Check for Position
inline bool MyTools::checkpos(std::string str, std::string param)
{
    if(str.find(param) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}
//============================================================================//
//                                              Check for Position - Ignore case
inline bool MyTools::checkposic(std::string str, std::string param)
{
    MakeLower(str);
    MakeLower(param);
    if(str.find(param) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}
//============================================================================//
//                                                      String to Int
inline int MyTools::StringToInt(std::string str)
{
    CheckAlpha checkalpha;
    std::find_if(str.begin(),str.end(),checkalpha);

    std::istringstream converter(str);
    int result;
    converter >> result;
    return result;
}
//============================================================================//
//                                                      String to Double
inline double MyTools::StringToDouble(std::string str)
{
    CheckAlpha checkalpha;
    std::find_if(str.begin(),str.end(),checkalpha);

    std::istringstream converter(str);
    double result;
    converter >> result;
    return result;
}
//============================================================================//
inline int MyTools::s2i(std::string str) { return StringToInt(str); }
//============================================================================//
inline double MyTools::s2d(std::string str) { return StringToDouble(str); }
//============================================================================//
//                                                      String to Number
template <typename T>
inline void MyTools::StringToNumber(std::string str, T& num)
{
    CheckAlpha checkalpha;
    std::find_if(str.begin(),str.end(),checkalpha);
    if(str.find("\\") == 0) { str.erase(0,1); }

    std::istringstream converter(str);
    converter >> num;
}
//============================================================================//
template <typename T>
inline void MyTools::s2n(std::string _s, T& n) { StringToNumber(_s,n); }
//============================================================================//
//                                                      String to Number
template <typename T>
inline T MyTools::StringToNumber(std::string str)
{
    T value;
    CheckAlpha checkalpha;
    std::find_if(str.begin(),str.end(),checkalpha);
    if(str.find("\\") == 0) { str.erase(0,1); }

    std::istringstream converter(str);
    converter >> value;
    return value;
}
//============================================================================//
//                                                      String to Number
template <typename T>
inline T MyTools::s2n(std::string str)
{
    return StringToNumber<T>(str);
}
//============================================================================//
//                                                      String to Number
template <typename T>
inline std::vector<T> MyTools::s2n(StrVector_t v)
{
    std::vector<T> nv(v.size(),0);
    for(uint32 i = 0; i < v.size(); ++i) {
        nv.at(i) = s2n<T>(v.at(i));
    }
    return nv;
}
//============================================================================//
//                                                      Int to String
inline std::string MyTools::IntToString(int num)
{
    std::stringstream out;
    out << num;
    return out.str();
}
//============================================================================//
//                                                      Double to String
inline std::string MyTools::DoubleToString(double num)
{
    std::stringstream out;
    out << num;
    return out.str();
}
//============================================================================//
//                                                      Number to String
template <typename T>
inline std::string MyTools::NumberToString(T num)
{
    std::stringstream out;
    out << num;
    return out.str();
}
//============================================================================//
template <typename T>
inline std::string MyTools::n2s(T num)
{
    return NumberToString<T>(num);
}
//============================================================================//
template <typename T>
inline MyTools::StrVector_t MyTools::n2s(const std::vector<T>& num)
{
    return NumberToString<T>(num);
}
//============================================================================//
template <typename T>
inline MyTools::StrVector_t MyTools::NumberToString(const std::vector<T>& num)
{
    StrVector_t v;
    for(uint32 i = 0; i < num.size(); ++i) {
        v.push_back(NumberToString(num[i]));
    }
    return v;
}
//============================================================================//
//                                                      Capitalize String
inline std::string MyTools::Capitalize(std::string str)
{
    if(str.size() > 0) { str[0] = toupper(str[0]); }
    return str;
}
//============================================================================//
//                                                      Uncapitalize String
inline std::string MyTools::Uncapitalize(std::string str)
{
    if(str.size() > 0) { str[0] = tolower(str[0]); }
    return str;
}
//============================================================================//
//                                                      Remove String whitespace
inline void MyTools::RemoveWhitespace(std::string& str)
{
    for(uint32 i = 0; i < str.length(); ++i) {
        if(str[i] == ' ') { str.erase(i,1); }
    }

    if(str.length() == 0) {
        std::cout << "Warning tools::RemoveWhitespace() "
        << "was passed an empty string" << std::endl;
    }
}
//============================================================================//
//                                              Remove Leading String whitespace
inline void MyTools::RemoveLeadingWhitespace(std::string& str)
{
    while(str.find_first_of(" \t") == 0)
    {
        str.erase(0, 1);
    }
}
//============================================================================//
//                                             Remove Trailing String whitespace
inline void MyTools::RemoveTrailingWhitespace(std::string& str)
{
    if(str.length() == 0)
        return;
    while(str.find_last_of(" \t") == str.length()-1)
    {
        str.erase(str.find_last_of(" \t"));
    }
}
//============================================================================//
//                                                      Strip String whitespace
inline void MyTools::StripWhitespace(std::string& str)
{
    RemoveLeadingWhitespace(str);
    RemoveTrailingWhitespace(str);
}
//============================================================================//
//                                                  String Compare - Ignore Case
inline bool MyTools::strcmpic(std::string str1, std::string str2)
{
    MakeLower(str1);
    MakeLower(str2);
    StripWhitespace(str1);
    StripWhitespace(str2);
    if(str1 == str2) { return true; }
    else { return false; }
}
//============================================================================//
//                                                      Delimit Line
inline
MyTools::StrVector_t MyTools::delimit(const std::string& line, char delimiter)
{
    StrVector_t delimitedLine;
    std::string token;
    std::istringstream iss(line);
    while( getline(iss,token,delimiter) ) {
        if( !token.empty() ) { delimitedLine.push_back(token); }
    }
    return delimitedLine;
}
//============================================================================//
//                                                      Delimit Line
inline
MyTools::StrVector_t MyTools::delimit(const std::string& line,
                                      const std::string& delimiter)
{
    StrVector_t newdelim;
    size_t d1 = 0;
    size_t d2 = 0;
    while(true) {
        d1 = line.find_first_not_of(delimiter, d2);
        if(d1 == std::string::npos) { break; }
        d2 = line.find_first_of(delimiter, d1);
        //if(d2 == std::string::npos) { d2 = std::string::npos; }
        std::string delim = "";
        try {
            delim = line.substr(d1,d2-d1);
        } catch (std::exception& e) {
            std::cout << "Standard Exception : " << e.what() << std::endl;
        }
        if(!delim.empty()) { newdelim.push_back(delim); }

    }

    return newdelim;
}
//============================================================================//
//                                                      Delimit Line
inline MyTools::StrVector_t MyTools::delimit(const std::string& line,
                                             const StrVector_t& delimiter)
{
    StrVector_t newdelim = MyTools::delimit(line, delimiter.front());

    for(uint32 i = 1; i < delimiter.size(); ++i) {
        newdelim = delimit(newdelim, delimiter.at(i));
    }

    return newdelim;
}
//============================================================================//
//                                                      Delimit Line
inline
MyTools::StrVector_t MyTools::delimit(const std::string& line,
                                      const std::string& delimiter,
                                      const StrVector_t& wholedelimiter)
{
    StrVector_t newdelim = MyTools::delimit(line, delimiter);

    for(uint32 i = 0; i < wholedelimiter.size(); ++i)
        for(StrVector_t::iterator ite = newdelim.begin(); ite != newdelim.end();
            ++ite)
        {
            if(ite->find(wholedelimiter.at(i)) != std::string::npos)
                newdelim.erase(ite);
        }

    return newdelim;
}
//============================================================================//
//                                                      Delimit Line
inline MyTools::StrVector_t MyTools::delimit(const StrVector_t& line,
                                             const std::string& delimiter)
{
    StrVector_t newdelim;

    for(uint32 i = 0; i < line.size(); ++i) {
        StrVector_t newline = delimit(line[i],delimiter);
        for(uint32 j = 0; j < newline.size(); ++j) {
            newdelim.push_back(newline[j]);
        }
    }
    return newdelim;
}
//============================================================================//
//                                                      Undelimit Line
template <typename T>
inline std::string MyTools::undelimit(std::vector<T> delimV,
                                      std::string delimiter)
{
    StrVector_t delimStr = NumberToString(delimV);
    std::string str = "";
    for(uint32 i = 0; i < delimStr.size(); ++i) {
        str += delimStr[i];
        if(i < delimStr.size()-1) { str += delimiter; }
    }
    return str;
}
//============================================================================//
//                                                      Undelimit Line
inline std::string MyTools::undelimit(StrVector_t delimStr,
                                      std::string delimiter)
{
    std::string str = "";
    for(uint32 i = 0; i < delimStr.size(); ++i) {
        str += delimStr[i];
        if(i < delimStr.size()-1) { str += delimiter; }
    }
    return str;
}
//============================================================================//
//                                                      Undelimit Line
inline void MyTools::undelimit(StrVector_t dl ,char delim, std::string& _s)
{
    _s="";
    if(dl.size() == 0)
        return;
    for(uint32 i = 0; i < dl.size(); ++i)
    {
        _s += dl[i];
        if(i < dl.size()-1)
            _s += delim;
    }
}
//============================================================================//
//                                                      Undelimit Line
inline std::string MyTools::undelimit(StrVector_t dl, std::string& _s,
                                      StrVector_t::iterator start,
                                      StrVector_t::iterator stop)
{
    std::string str = "";
    for(uint32 i = std::distance(dl.begin(),start);
        i < std::distance(dl.begin(),stop)+1; ++i)
    {
        str += dl.at(i);
        if(i < dl.size()-1)
            str += _s;
    }
    return str;
}
//============================================================================//
//                                                      Remove Vector Element
template <typename T>
void MyTools::RemoveElement(int pos, std::vector<T>& vec)
{
    vec.erase(vec.begin()+pos);
}

//============================================================================//
//                                                      Remove Vector Element
template <typename T>
void MyTools::RemoveElement(std::vector<T>& vec, T t)
{
    typename std::vector<T>::iterator ite;
    for(ite = vec.begin(); ite != vec.end(); ++ite)
        if( *ite == t ) {
            vec.erase(ite);
            break;
        }
}
//============================================================================//
//                                                      Print Vector
template <typename T>
inline void MyTools::print(const std::vector<T>& vec, std::string c,
                           std::ostream& out)
{
    for(uint32 i = 0; i < vec.size(); ++i)
    {
        out << vec.at(i);
        if(i+1 != vec.size())
            out << c;
    }

}
//============================================================================//
//                                                      Print Vector
template <typename T>
inline void MyTools::print(const std::vector<T>& vec, int width,
                           std::ostream& out)
{
    out.width(width);
    for(uint32 i = 0; i < vec.size(); ++i)
        out << std::setw(width) << vec.at(i);
}
//============================================================================//
//                                                      Print Vector
template <typename T>
inline void MyTools::print(const std::vector<T>& vec, int width, int width2,
                           std::ostream& out)
{
    out.width(width);
    out.precision(width2);
    for(uint32 i = 0; i < vec.size(); ++i)
        out << std::setw(width) << std::setprecision(width2) << vec.at(i);
}
//============================================================================//
//                                                      Print Map
template <typename T, typename U>
inline void MyTools::print(const std::map<T, U>& vec,
                           std::string c1,
                           std::string c2, std::ostream& out)
{

    typename std::map<T,U>::const_iterator ite;
    for(ite = vec.begin(); ite != vec.end(); ++ite)
    {
        out << c1 << ite->first << c1 << ite->second << c2;
        if(std::distance(vec.begin(),ite)+1 >= vec.size())
            out << c2;
    }

}
//============================================================================//
//                                                      Print Map
template <typename T, typename U>
inline void MyTools::print(const std::map<T,U>& vec, int width,
                           std::ostream& out)
{
    typename std::map<T,U>::const_iterator ite;
    for(ite = vec.begin(); ite != vec.end(); ++ite)
        out << std::setw(width) << ite->first << " "
            << std::setw(width) << ite->second;
}
//============================================================================//
//                                                      Print Map
template <typename T, typename U>
inline void MyTools::print(const std::map<T,U>& vec,
                           int width, int width2,
                           std::ostream& out)
{
    typename std::map<T,U>::const_iterator ite;
    for(ite = vec.begin(); ite != vec.end(); ++ite)
        out << std::setw(width) << std::setprecision(width2) << ite->first
            << " " << std::setw(width) << std::setprecision(width2)
            << ite->second;
}
//============================================================================//
//                                                      Get portion of vector
template <typename T>
inline std::vector<T> MyTools::subvector(const std::vector<T>& v,
                                         uint32 first, uint32 last)
{
    std::vector<T> nv;
    for(uint32 i = first; i < last; ++i) {
        if(i >= v.size()) { break; }
        nv.push_back(v.at(i));
    }
    return nv;
}
//============================================================================//
//                                                      Check for ID
inline bool MyTools::checkforID(std::string str, std::string param)
{
    for(std::string::size_type i = 0; i < param.size(); ++i)
        if(str.find(param[i]) == std::string::npos)
            return false;
    return true;
}
//============================================================================//
//                                                      Check for ID
inline bool MyTools::checkforID(std::string str, StrVector_t param)
{
    for(uint32 j = 0; j < param.size(); ++j)
        for(std::string::size_type i = 0; i < param[j].size(); ++i)
            if(str.find(param[i]) != std::string::npos)
                return true;

    return false;
}
//============================================================================//
//                                                      Remove Position
inline std::string MyTools::removepos(std::string orig, std::string param)
{
    return (orig.find(param) != std::string::npos)
            ? orig.erase(orig.find(param),param.size())
            : orig;
}
//============================================================================//
//                                                      Purge Line
inline void MyTools::Purge(std::string& str,std::string delimiter)
{
    bool fixed = false;
    while (!fixed) {
        fixed = true;
        if(checkposic(str,delimiter)) {
            str.replace(str.find(delimiter),delimiter.size()," ");
            fixed = false;
        }
    }
}
//============================================================================//
//                                                      Purge Line
inline void MyTools::Purge(std::string& str,std::string delimiter,
                           std::string replace)
{
    bool fixed = false;
    while (!fixed) {
        fixed = true;
        if(checkposic(str,delimiter)) {
            str.replace(str.find(delimiter),delimiter.size(),replace);
            fixed = false;
        }
    }
}
//============================================================================//
//                                                      Remove Duplicates
template<typename T>
inline void MyTools::RemoveDuplicates(std::vector<T>& vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}
//============================================================================//
//                                                      Count
inline bool MyTools::count(std::string str,StrVector_t allstrs)
{
    for(uint32 i = 0; i < allstrs.size(); ++i) {
        if((lower(allstrs[i])).find(lower(str)) != std::string::npos) {
            return true;
        }
    }
    return false;
}
//============================================================================//
//                                                      Is Duplicate
template <typename T>
inline bool MyTools::IsDuplicate(T val, std::vector<T> v)
{
    for(uint32 i = 0; i < v.size(); ++i) {
        if(v.at(i) == val) { return true; }
    }
    return false;
}
//============================================================================//
//                                                      Scale number
template <typename T>
inline void MyTools::Scale(T& val, T min, T max, T a, T b)
{
    // scale val from [min,max] to [a,b]
    val = (b-a)*(val-min)/(max-min)+a;
}
//============================================================================//
//                                                      Scale range
template <typename T>
inline void MyTools::Scale(std::vector<std::vector<T> >& v,
                           T min, T range, T scaleMin, T scaleMax)
{
    T scaleRange = scaleMax - scaleMin;

    double avg = 0;
    assert(range != 0);
    for(uint32 j = 0; j < v.size(); ++j) {
        for(uint32 i = 0; i < v[j].size(); ++i) {
            v[j][i] = ((scaleRange*(v[j][i]-min))/range) + scaleMin;
            double inverse = fabs(scaleMax-v[j][i])/(v[j][i]);
            if(j > 4 && fabs(inverse-avg) > 10000.) { inverse = avg; }
            else { avg = (avg+inverse)/2.; }
            v[j][i] *= pow(inverse,2);
            if(fabs(v[j][i] - avg) > 1.e6) { v[j][i] *= 1.e6; }
        }
    }
}
//============================================================================//
//                                                      Mean
template <typename T>
inline T MyTools::mean(const std::vector<T>& v)
{
    T accum = std::accumulate(v.begin(),v.end(),accum);
    return accum/static_cast<T>(v.size());
}
//============================================================================//
//                                                      Standard Deviation
template <typename T>
inline T MyTools::StdDev(const std::vector<T>& v)
{
    T mean_ = mean(v);
    T var = 0;
    for(uint32 i = 0; i < v.size(); ++i) {
        var += pow(v.at(i)-mean_,2);
    }
    var /= (static_cast<T>(v.size())-static_cast<T>(1));
    return sqrt(var);
}
//============================================================================//
//                                                      Standard Deviation
template <typename T, typename U>
inline T MyTools::StdDev(const T& sum, const T& sum_sq, const U& N)
{
    return sqrt(Variance(sum,sum_sq,N));
}
//============================================================================//
//                                                      Variance
template <typename T>
inline T MyTools::Variance(const std::vector<T>& v)
{
    T mean_ = mean(v);
    T var = 0;
    for(uint32 i = 0; i < v.size(); ++i) { var += pow(v.at(i)-mean_,2); }
    return var / (static_cast<T>(v.size())-static_cast<T>(1));
}
//============================================================================//
//                                                      Variance
template <typename T, typename U>
inline T MyTools::Variance(const T& sum, const T& sum_sq, const U& N)
{
    return (sum_sq/static_cast<T>(N) - pow(sum/static_cast<T>(N),2))
            /static_cast<double>(N - 1);
}
//============================================================================//
//                                                      Relative Error
template <typename T>
inline T MyTools::RelativeError(const std::vector<T>& v)
{
    T sum = 0; T sum2 = 0;
    for(uint32 i = 0; i < v.size(); ++i) {
        sum += v.at(i);
        sum2 += v.at(i)*v.at(i);
    }
    return sqrt(sum2/(sum*sum)) - (1./static_cast<T>(v.size()));
}
//============================================================================//
//                                                      Relative Error
template <typename T, typename U, typename V>
inline std::map<T, U> MyTools::RelativeError(const std::map<T,U>& sums,
                                             const std::map<T,U>& sqrs,
                                             const std::map<T, V>& ncounts)
{
    std::map<T, U> result;
    typename std::map<T,U>::const_iterator ite_sum = sums.begin();
    typename std::map<T,U>::const_iterator ite_sqr = sqrs.begin();
    typename std::map<T,V>::const_iterator ite_cts = ncounts.begin();
    for(; ite_sum != sums.end(); ++ite_sum, ++ite_sqr, ++ite_cts)
        result[ite_sum->first]
                = sqrt(ite_sqr->second/(ite_sum->second*ite_sum->second))
                  - (1./static_cast<U>(ite_cts->second));
    return result;
}
//============================================================================//
//                                                      Exit w/o params
template <typename T, typename U>
inline void MyTools::ExitWithoutParams(T arg, U args, U nParams)
{
    if(args < nParams)
    {
        std::cout << arg << " needs " << nParams << " to continue"
        << ". Exitting...\n" << std::endl;
        exit(-1);
    }
}
//============================================================================//
//                                                      Exit w/o params
template <typename T, typename U>
inline void MyTools::ExitWithoutParams(T arg, U args, U nParams,
                                       std::string message)
{
    if(args < nParams) {

        std::cout << arg << " needs " << nParams << " to continue"
        << ". Please specify " << message << " in the command line.\n"
        << std::endl;
        exit(-1);
    }
}
//============================================================================//
//                                                      Open File
inline bool MyTools::OpenFile(std::ifstream& inf, std::string fname,
                              bool doExit)
{
    inf.open(fname.c_str());
    if(!inf)
    {
        if(doExit)
            Exception("Tools","OpenFile - unable to open file",fname,FATAL);
        else
        {
            Exception("Tools","OpenFile - unable to open file",fname,WARNING);
            return false;
        }
    }
    return true;
}
//============================================================================//
//                                                      Open File
inline void MyTools::OpenFile(std::ifstream& inf, std::string fname)
{
    inf.open(fname.c_str());
    if(!inf)
        Exception("Tools","OpenFile - unable to open file",fname,FATAL);
}
//============================================================================//
//                                                      Open File
inline bool MyTools::OpenFile(std::ofstream& inf, std::string fname, bool doExit)
{
    inf.open(fname.c_str());
    if(!inf)
    {
        if(doExit)
            Exception("Tools","OpenFile - unable to open file",fname,FATAL);
        else
        {
            Exception("Tools","OpenFile - unable to open file",fname,WARNING);
            return false;
        }
    }
    return true;
}
//============================================================================//
//                                                      Open File
inline void MyTools::OpenFile(std::ofstream& inf, std::string fname)
{
    inf.open(fname.c_str());
    if(!inf)
        Exception("Tools","OpenFile - unable to open file",fname,FATAL);
}
//============================================================================//
//                                                      Remove Comment
inline void MyTools::RemoveComment(std::string& line,
                                   const std::string& commentTag)
{
    if(checkposic(line,commentTag))
        line.erase(lower(line).find(commentTag));
}
//============================================================================//
//                                                      Get Line
inline std::string MyTools::GetLine(std::istream& inf, const std::string& str)
{
    std::string line;
    getline(inf,line,'\n');
    uint32 ending_delimiter = line.length();
    for(uint32 i = 0; i < str.length(); ++i)
        if(line.find(str.at(i)) != std::string::npos)
            ending_delimiter = line.find(str.at(i));
    return (ending_delimiter == line.length())
            ? line
            : line.substr(0,ending_delimiter);
}
//============================================================================//
//                                                      Get Line
inline bool MyTools::GetLine(std::istream& inf, std::string& line,
                             const std::string& str)
{
    getline(inf,line,'\n');
    uint32 ending_delimiter = line.length();
    for(uint32 i = 0; i < str.length(); ++i)
        if(line.find(str.at(i)) != std::string::npos)
            ending_delimiter = line.find(str.at(i));
    line = (ending_delimiter == line.length())
           ? line
           : line.substr(0,ending_delimiter);
    return (line.length() > 0) ? true : false;
}
//============================================================================//
//                                                      Get Line and Delimit
inline
MyTools::StrVector_t MyTools::GetLineAndDelimit(std::istream& inf,
                                                const std::string& str)
{
    while(!inf.eof())
    {
        std::string line;
        getline(inf,line,'\n');
        StrVector_t delimited = delimit(line,str);
        if(!delimited.empty())
            return delimited;
    }
    StrVector_t d;
    return d;
}
//============================================================================//
//                                                      Get Line and Delimit
inline
MyTools::StrVector_t MyTools::GetLineAndDelimit(std::istream& inf,
                                                const std::string& str,
                                                const std::string& commentTag)
{
    while(!inf.eof())
    {
        std::string line;
        getline(inf,line,'\n');
        RemoveComment(line,commentTag);
        StrVector_t delimited = delimit(line,str);
        if(!delimited.empty())
            return delimited;
    }
    StrVector_t d;
    std::cout << "WARNING! Function GetLineAndDelimit reached end of file and ";
    std::cout << "is returning empty vector" << std::endl;
    return d;
}
//============================================================================//
//                                                      Get Line and Delimit
inline bool MyTools::GetLineAndDelimit(std::istream& inf,
                                       const std::string& str,
                                       const std::string& commentTag,
                                       StrVector_t& v)
{
    v.clear();
    // Loop just in case of blank line
    while(!inf.eof())
    {
        std::string line;
        getline(inf,line,'\n');
        RemoveComment(line,commentTag);
        v = delimit(line,str);
        if(0 < v.size())
            return true;
    }
    return false;
}
//============================================================================//
//                                                      Get Line and Delimit
inline bool MyTools::GetLineAndDelimit(std::istream& inf,
                                       const std::string& str,
                                       const StrVector_t& wholestr,
                                       const std::string& commentTag,
                                       StrVector_t& v)
{
    v.clear();
    // Loop just in case of blank line
    while(!inf.eof()) {
        std::string line;
        getline(inf,line,'\n');
        RemoveComment(line,commentTag);
        v = delimit(line, str, wholestr);
        if(!v.empty()) { return true; }
    }
    return false;
}
//============================================================================//
//                                                      Get Line and Delimit
inline bool MyTools::GetLineAndDelimit(std::istream& inf,
                                       const std::string& str,
                                       StrVector_t& v)
{
    v.clear();
    // Loop just in case of blank line
    while(!inf.eof()) {
        std::string line;
        getline(inf,line,'\n');
        v = delimit(line,str);
        if(!v.empty()) {
            return true;
        }
    }
    return false;
}
//============================================================================//
//                                                      XML Open
inline std::string MyTools::xmlopen(std::string str,std::string space)
{
    return space + "<" + str + ">";
}
//============================================================================//
//                                                      XML Open
inline std::string MyTools::xmlopen(std::string str)
{
    return "<" + str + ">";
}
//============================================================================//
//                                                      XML Close
inline std::string MyTools::xmlclose(std::string str,std::string space)
{
    return space + "</" + str + ">";
}
//============================================================================//
//                                                      XML Close
inline std::string MyTools::xmlclose(std::string str)
{
    return "</" + str + ">";
}
//============================================================================//
//                                                      Tab level
inline std::string MyTools::tablevel(int level)
{
    std::string str = "";
    if(level < 0) { level = 0; }
    for(int i = 0; i < level; ++i) {
        str += "\t";
    }
    return str;
}
//============================================================================//
//                                                      Determine Close
inline int MyTools::DetermineClose(int pos,std::vector<int> vec)
{
    std::vector<int>::iterator ite = std::find(vec.begin(),vec.end(),pos);
    if(ite == vec.end()) { return -1; }
    else { return std::distance(vec.begin(),ite); }
}
//============================================================================//
//                                                      Add value
template <typename T>
inline void MyTools::AddValue(std::vector<T>& v, T val)
{
    for(uint32 i = 0; i < v.size(); ++i) {
        v[i] += val;
    }
}
//============================================================================//
//                                                      Get Min
template <typename T>
T MyTools::GetMin(const std::vector<T>& v) {
    return *std::min_element(v.begin(),v.end());
}
//============================================================================//
//                                                      Get Max
template <typename T>
T MyTools::GetMax(const std::vector<T>& v) {
    return *std::max_element(v.begin(),v.end());
}
//============================================================================//
//                                                      Get Sum
template <typename T>
T MyTools::Sum(const std::vector<T>& v) {
    T value;
    return std::accumulate(v.begin(),v.end(),value);
}
//============================================================================//
//                                                      Generate XML block
template <typename T, typename U>
inline void MyTools::GenerateXMLblock(std::ostream& out,
                                      const std::vector<T>& tags,
                                      std::vector<int> levels,
                                      const std::vector<int>& subsets,
                                      const std::vector<U>& values)
{
    if(tags.size() != values.size() ||
       tags.size() != levels.size() ||
       tags.size() != subsets.size() ) {
        Exception("GenerateXMLblock","incompatible sizes",FATAL); }

    std::vector<T> closeTags;
    std::vector<int> closeTagsLoc;

    std::vector<int>::iterator upper;
    upper = std::upper_bound(levels.begin(),levels.end(),0);

    if(upper == levels.end()) {
        Exception("GenerateXMLblock","no master XML tag",FATAL);
    } else {
        closeTags.push_back(tags[std::distance(levels.begin(),upper)]);
        closeTagsLoc.push_back(tags.size()+1);
    }

    for(uint32 i = 0; i < tags.size(); ++i) {
        if(DetermineClose(i,closeTagsLoc) > 0) {
            int index = DetermineClose(i,closeTagsLoc);
            out << tablevel(levels[index]) << xmlclose(closeTags[index])
            << std::endl << std::endl;
            closeTags.pop_back();
            closeTagsLoc.pop_back();
        }
        if(subsets[i] == 0) {
            out << tablevel(levels[i]) << xmlopen(tags[i])
            << values[i] << xmlclose(tags[i]) << std::endl;
        } else if( i != 0 && levels[i] != 1 ) {
            closeTags.push_back(tags[i]);
            int newLoc = i+subsets[i]+1;
            if(newLoc == closeTagsLoc[i-1]) { newLoc -= 1; }
            closeTagsLoc.push_back(newLoc);
            out << tablevel(levels[i]) << xmlopen(tags[i]) << std::endl;
        }
        if ( i == 0 ) {
            out << tablevel(levels[i]) << xmlopen(tags[i]) << std::endl;
        }
    }

    if(closeTags.size() > 0) {
        for(uint32 i = tags.size(); i < 2*tags.size(); ++i) {
            if(closeTags.size() == 0) { break; }
            int index = DetermineClose(i,closeTagsLoc);
            if(index < 0) { continue; }
            out << tablevel(levels[index]) << xmlclose(closeTags[index])
            << std::endl << std::endl;
            closeTags.pop_back();
            closeTagsLoc.pop_back();
        }
    }

}
//============================================================================//
//                                                      Add Line
inline void MyTools::AddLine(std::ifstream& in,
                             std::string& prev,
                             std::string tag)
{
    if(prev.find(tag) == std::string::npos) { return; }
    else {
        while(true) {
            prev.erase(prev.find(tag));
            std::string line;
            getline(in,line,'\n');
            prev += line;
            if(lower(prev).find(tag) == std::string::npos) { break; }
        }
    }
}
//============================================================================//
//                                                      Line XML
template <typename T>
inline void MyTools::lineXML(std::ostream& out, std::string tag,
                             T value, std::string delim)
{
    out << delim << xmlopen(tag) << value << xmlclose(tag) << std::endl;
}
//============================================================================//
//                                                      Line XML
template <typename T>
inline void MyTools::lineXML(std::ostream& out, std::string tag,
                             T value, std::string delim, int prec)
{
    out << delim << xmlopen(tag) << std::setprecision(prec) << value
        << xmlclose(tag) << std::endl;
}
//============================================================================//
//                                                      Get Bool From String
inline bool MyTools::GetBool(const std::string& val)
{
    std::string str = lower(val);

    return
    (str.length() == 1)
    ? ((str == "t" || str == "y") ? true : false)
    : ((str.find("true") != std::string::npos ||
        str.find("yes") != std::string::npos ||
        str == "on")
       ? true : false);

}
//============================================================================//
//                                                      Get Bool From String
inline std::vector<bool> MyTools::GetBool(const StrVector_t& str)
{
    std::vector<bool> boolstr(str.size(),false);
    for(uint32 i = 0; i < str.size(); ++i) {
        boolstr.at(i) = GetBoolFromString(str.at(i));
    }
    return boolstr;
}
//============================================================================//
//                                                      Require size
template <typename T>
inline void MyTools::RequireSize(const std::vector<T>& v, int i,
                                 std::string message)
{
    if(i == v.size()) { return; }
    else {
        int diff = i - v.size();
        std::cout << "Vector ";
        for(uint32 ii = 0; ii < v.size(); ++ii) {
            std::cout << v[ii] << " ";
        }
        std::cout << std::endl;
        std::string cond = "too ";
        cond += (diff > 0) ? "small " : "large ";
        cond += "by " + NumberToString(abs(diff));
        Exception("Tools::RequireSize", "bad number of parameters - "+message,
                  cond,Fatal);
    }
}
//============================================================================//
//                                                      Check Size
template <typename T>
inline bool MyTools::CheckSize(const std::vector<T>& v, int i,
                               std::string message)
{
    if(int(v.size()) >= i) { return true; }
    else {
        int diff = i - v.size();
        std::cout << "Vector ";
        for(uint32 ii = 0; ii < v.size(); ++ii) {
            std::cout << v[ii] << " ";
        }
        std::cout << std::endl;
        std::string cond = "too small by " + NumberToString(diff);
        Exception("Tools::CheckSize", "bad number of parameters - "+message,
                  cond,Fatal);
    }
    return false;
}
//============================================================================//
//                                                      Get Param
inline std::string MyTools::GetParam(const StrVector_t& v, std::string param)
{
    for(uint32 i = 0; i < v.size(); ++i)
        if(strcmpic(v[i],param))
        {
            CheckSize(v,i+1,"looking for "+param);
            return v[i+1];
        }

    Exception("GetParameter::could not find requested parameter",
              param,undelimit(v," "),Fatal);
    return "";      // gets rid of warnings
}
//============================================================================//
//                                                      Check For Param
template <typename T>
inline std::pair<bool,uint32> MyTools::CheckForParam(const std::vector<T>& v,
                                                     T param)
{
    for(uint32 i = 0; i < v.size(); ++i)
        if(v[i] == param)
            return std::pair<bool,uint32>(true,i);
    return std::pair<bool,uint32>(false,0);
}
//============================================================================//
//                                                      Check For Param
inline std::pair<bool,uint32> MyTools::CheckForParam(const StrVector_t& v,
                                                     std::string param)
{
    for(uint32 i = 0; i < v.size(); ++i) {
        if(strcmpic(v[i],param)) {
            return std::pair<bool,uint32>(true,i);
        }
    }
    return std::pair<bool,uint32>(false,0);
}
//============================================================================//
//                                                      Convert Vector
//============================================================================//
template<typename T, typename U>
inline std::vector<T> MyTools::convert(const std::vector<U>& vec)
{
#ifdef CXX11
    auto CONV = [] (T& newval, const U& oldval)
    {
        std::istringstream iss(oldval);
        iss >> newval;
    };
#endif

    std::vector<T> nvec;
    for(uint32 i = 0; i < vec.size(); ++i)
    {
        T nval;
#ifdef CXX11
        CONV(nval, vec.at(i));
#else
        std::istringstream iss(vec.at(i));
        iss >> nval;
#endif
        nvec.push_back(nval);
    }
    return nvec;
}
//============================================================================//

//============================================================================//
template<typename U>
inline MyTools::StrVector_t MyTools::convert2str(const std::vector<U>& vec)
{
    StrVector_t nvec;
    for(uint32 i = 0; i < vec.size(); ++i) {
        std::stringstream ss;
        ss << vec.at(i);
        nvec.push_back(ss.str());
    }
    return nvec;
}
//============================================================================//


//============================================================================//
//                                          Convert String Vector to Numbers
//============================================================================//
template<typename T>
inline std::vector<T> MyTools::convertfromstring(const StrVector_t& v)
{
    std::vector<T> tv;
    for(uint32 i = 0; i < v.size(); ++i)
        tv.push_back(s2n<T>(v.at(i)));
    return tv;
}
//============================================================================//
//                                      Truncate after certain decimal place
//============================================================================//
inline void MyTools::SetDoublePrecision(double& value, int precision,
                                        double units)
{
    double fac = (value < 0.0) ? -1. : 1.;
    value = fabs(value);
    uint64 tmp = static_cast<uint64>((value/units) * pow(10.,precision));
    value = (static_cast<double>(tmp) * pow(10.,-precision))*units;
    value *= fac;
}
//============================================================================//
//                                      Truncate after certain decimal place
//============================================================================//
template <typename T, typename U>
inline T MyTools::TruncatePrecision(const T& value, const int& precision,
                                    const U& units)
{
    T sign = (value < 0.0) ? -1. : 1.;
    T tmpvalue = fabs(value);
    uint64 tmp = static_cast<uint64>((tmpvalue/units) * pow(10.,precision));
    return sign*((static_cast<T>(tmp) * pow(10.,-precision))*units);
}
//============================================================================//
//                                                  Get Iterator at Position
//============================================================================//
template <typename T>
inline typename std::vector<T>::iterator
MyTools::GetIteratorPosition(std::vector<T> v, T criteria)
{
    typename std::vector<T>::iterator it;
    for(it = v.begin(); it != v.end(); ++it) {
        if( *it == criteria ) { return it; }
    }
    Exception("GetIteratorPosition - could not find component matching critera",
               criteria,Fatal);
    return v.end();
}
//============================================================================//
//                                                  Get Iterator Index
//============================================================================//
template <typename T>
inline uint32 MyTools::GetIteratorIndex(std::vector<T> v, T criteria)
{
    typename std::vector<T>::iterator it;
    for(it = v.begin(); it != v.end(); ++it) {
        if( *it == criteria ) { return std::distance(v.begin(),it); }
    }
    Exception("GetIteratorPosition - could not find component matching critera",
              criteria,Fatal);
    return v.size();
}
//============================================================================//
//                                                  Make directory
//============================================================================//
inline bool MyTools::MakeDirectory(const std::string& str)
{
    int r = 0;
    std::string cmd = "if [ ! -d " + str + " ]; then mkdir -p " + str + "; fi";
    if(system(NULL))
    {
        r = system(cmd.c_str());
        if(r == 0)
            return true;
        else
            Exception("MyTools::MakeDirectory", "Unable to create directory",
                      str, WARNING);
    } else
    {
        Exception("MyTools::MakeDirectory", "Unable to create directory because "
                                           "system is not available",
                  str, WARNING);
    }
    return false;
}
//============================================================================//
//                                                  Add directory
//============================================================================//
inline bool MyTools::AddDirectory(const std::string& str)
{
    return MakeDirectory(str);
}
//============================================================================//
//                                                  Make directory string
//============================================================================//
inline std::string MyTools::append_string(const std::string& str,
                                          const std::string& field)
{
    if(str.find(field, str.length()-1) != str.length()-1)
        return str + field;
    else
        return str;
}
//============================================================================//
//                                                  Join path and filename
//============================================================================//
inline std::string MyTools::join(const std::string& prefix,
                                 const std::string& postfix,
                                 const std::string& delim)
{
    if(postfix.length() == 0)
        return prefix;

    std::string _prefix = (prefix.length() > 0)
                          ? MyTools::append_string(prefix, delim)
                          : prefix;
    std::string _postfix = postfix;

    while(_postfix.find(delim) == 0)
        _postfix = _postfix.substr(1, _postfix.length());

    return _prefix + _postfix;
}
//============================================================================//
//                                                  Join path and filename
//============================================================================//
inline bool MyTools::exists(const std::string& filename)
{
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}
//============================================================================//
//                                                  Interpolate for new value
//============================================================================//
template <typename T>
inline T MyTools::interpolate (T x_new, T x1, T x2, T y1, T y2)
{
    if(x1 == x_new)
        return y1;
    else if(x2 == x_new)
        return y2;
    else if(!(x_new > x1 && x_new < x2 ))
        Exception("Interpolate","Interpolating for value not between bounds : "
                  +n2s(x_new),x1,x2,Warning);
    return y2 - ((y2-y1)/(x2-x1))*(x2 - x_new);
}
//============================================================================//
//                                                  Interpolate for new value
//============================================================================//
template <typename T>
inline T MyTools::loginterpolate (T x_new, T x1, T x2, T y1, T y2)
{
    if(x1 == x_new) { return y1; }
    else if(x2 == x_new) { return y2; }
    else if(!(x_new > x1 && x_new < x2 )) {
        Exception("LogInterpolate",
                  "Interpolating for value not between bounds : "+n2s(x_new),
                  x1,x2,Warning);
    }
    return log(y2) - ((log(y2)-log(y1))/(log(x2)-log(x1)))
            *(log(x2) - log(x_new));
}
//============================================================================//
//                                                  Explicit Euler Integration
//============================================================================//
inline double MyTools::ExplicitEuler(double x_, double dydx_, double h_)
{
    return x_ + h_*dydx_;
}
//============================================================================//
//                                                  Implicit Euler Integration
//============================================================================//
inline double MyTools::ImplicitEuler(double x_, double dydx_, double h_)
{
    return x_ + h_*(dydx_ + 0.5*h_*dydx_);
}
//============================================================================//
//                                                  Factorial
//============================================================================//
template <typename T>
inline T MyTools::factorial(uint32 number)
{

    T temp;

    if(number <= 1) return 1;

    temp = number * MyTools::factorial<T>(number - 1);
    return temp;

}
//============================================================================//
//                                                  Clear Vector, List, Deque
//============================================================================//
template <typename T>
inline void MyTools::FreeClear(T& container)
{
    for(typename T::iterator ite = container.begin();
        ite != container.end(); ++ite)
        delete *ite;
    container.clear();
}
//============================================================================//
inline bool MyTools::CheckFile(const std::string& str)
{
    std::ifstream in;
    in.open(str.c_str());

    return (in.fail()) ? false : true;
}
//============================================================================//
inline double MyTools::modulus(const double& val, const double& param)
{
    return fabs(val/param) - static_cast<uint32>(fabs(val/param));
    //return fabs(val/static_cast<double>(static_cast<int>(fabs(val/param))+1));

}
//============================================================================//
template <typename T, typename U>
void MyTools::AddVectorData2Map(const T& type, std::multimap<T,U>& themap,
                                const std::vector<U>& data)
{
    for(uint32 i = 0; i < data.size(); ++i) {
        themap.insert(std::make_pair(type,data.at(i)));
    }
}
//============================================================================//
template <typename T, typename U>
void MyTools::AddVectorData2Map(const std::vector<T>& type,
                                std::map<T,U>& themap,
                                const std::vector<U>& data)
{
    if(type.size() != data.size()) {
        Exception("MyTools::AddVectoData2Map",
                  "Discrepancy in key size vs. data size",
                  type.size(),data.size(),
                  ((type.size() > data.size()) ? FATAL : WARNING));
    }
    for(uint32 i = 0; i < type.size(); ++i) {
        themap.insert(std::make_pair(type.at(i),data.at(i)));
    }
}
//============================================================================//
template <typename T, typename U>
void MyTools::AddVectorData2Map(const T& type,
#if defined(CXX11) ||  defined(CLANG_V5)
                       std::unordered_multimap<T,U>& themap,
#else
                       std::tr1::unordered_multimap<T,U>& themap,
#endif
                       const std::vector<U>& data)
{
    for(uint32 i = 0; i < data.size(); ++i) {
        themap.insert(std::make_pair(type,data.at(i)));
    }
}
//============================================================================//
template <typename T, typename U>
void MyTools::AddVectorData2Map(const std::vector<T>& type,
#if defined(CXX11) ||  defined(CLANG_V5)
                       std::unordered_map<T,U>& themap,
#else
                       std::tr1::unordered_map<T,U>& themap,
#endif
                       const std::vector<U>& data)
{
    if(type.size() != data.size()) {
        Exception("MyTools::AddVectoData2Map",
                  "Discrepancy in key size vs. data size",
                  type.size(),data.size(),
                  ((type.size() > data.size()) ? FATAL : WARNING));
    }
    for(uint32 i = 0; i < type.size(); ++i) {
        themap.insert(std::make_pair(type.at(i),data.at(i)));
    }

}
//============================================================================//
template <typename T>
inline typename T::value_type* MyTools::GetC_Array(const T& vec)
{
    typedef typename T::value_type value_type;
    typedef typename T::const_iterator const_iterator;
    value_type* array = new value_type[vec.size()];
    for(const_iterator itr = vec.begin(); itr != vec.end(); ++itr)
    {
        array[std::distance(vec.begin(), itr)] = *itr;
    }
    return array;
}

//============================================================================//
template <typename T, typename U>
inline
std::pair<T*,U*> MyTools::GetC_Array(const std::vector<std::pair<T,U> >& vec)
{
    T* arrayT = new T[vec.size()];
    U* arrayU = new U[vec.size()];

    for(uint32 i = 0; i < vec.size(); ++i) {
        arrayT[i] = vec.at(i).first;
        arrayU[i] = vec.at(i).second;
    }
    return std::pair<T*,U*>(arrayT,arrayU);
}
//============================================================================//
template <typename T, typename U>
inline bool MyTools::RealIntegerCompare(const T& lhs, const U& rhs)
{
    return (static_cast<long>(lhs/1.e-12) == static_cast<long>(rhs/1.e-12))
            ? true : false;
}
//============================================================================//
template <typename _Tp>
_Tp MyTools::GetEnvironmentVariable(const std::string& ID, _Tp _default)
{
  char* env_var = std::getenv(ID.c_str());
  if(env_var)
  {
    std::string str_var = std::string(env_var);
    std::istringstream iss(str_var);
    _Tp var = _Tp();
    iss >> var;
    return var;
  }
  // return default if not specified in environment
  return _default;
}
//============================================================================//
// THIS SHIT DOES NOT WORK!!!!!!!!!
inline unsigned MyTools::LevenshteinDistance(const std::string& s1,
                                             const std::string& s2)
{
#ifdef CXX11
    // To change the type this function manipulates and returns, change
    // the return type and the types of the two variables below.
    int s1len = s1.size();
    int s2len = s2.size();

    auto column_start = (decltype(s1len))1;

    auto column = new decltype(s1len)[s1len + 1];
    std::iota(column + column_start, column + s1len + 1, column_start);

    for (auto x = column_start; x <= s2len; x++) {
        column[0] = x;
        auto last_diagonal = x - column_start;
        for (auto y = column_start; y <= s1len; y++) {
            auto old_diagonal = column[y];
            auto possibilities = {
                column[y] + 1,
                column[y - 1] + 1,
                last_diagonal + (s1[y - 1] == s2[x - 1]? 0 : 1)
            };
            column[y] = std::min(possibilities);
            last_diagonal = old_diagonal;
        }
    }
    auto result = column[s1len];
    delete[] column;
    return result;
#else
    // THIS SHIT DOES NOT WORK!!!!!!!!!
    return std::max(details::LevenshteinDistance(s1, s2),
                    details::LevenshteinDistance(s2, s1));
#endif
}
#ifndef SWIG
//============================================================================//
// THIS SHIT DOES NOT WORK!!!!!!!!!
inline unsigned MyTools::details::LevenshteinDistance(std::string _s,
                                                      std::string _t)
{
    throw std::runtime_error("LevenshteinDistance - THIS SHIT DOES NOT WORK");
    /*static StrVector_t removals;
    if(removals.size() == 0)
    {
        removals.push_back("'");
        removals.push_back("-");
    }

    for(unsigned i = 0; i < removals.size(); ++i)
    {
        while(s.find(removals.at(i)) != std::string::npos)
            s.erase(s.begin() + s.find(removals.at(i)));
        while(t.find(removals.at(i)) != std::string::npos)
            t.erase(t.begin() + t.find(removals.at(i)));
    }*/
    //std::cout << "s = " << s << ", t = " << t << std::endl;

    // degenerate cases
    if (_s == _t) return 0;
    if (_s.length() == 0) return _t.length();
    if (_t.length() == 0) return _s.length();

    // create two work vectors of integer distances
    unsigned* v0 = new unsigned[_t.length() + 1];
    unsigned* v1 = new unsigned[_t.length() + 1];

    // initialize v0 (the previous row of distances)
    // this row is A[0][i]: edit distance for an empty s
    // the distance is just the number of characters to delete from t
    for (unsigned i = 0; i < _t.length(); i++)
        v0[i] = i;

    for (unsigned i = 0; i < _s.length(); i++)
    {
        // calculate v1 (current row distances) from the previous row v0

        // first element of v1 is A[i+1][0]
        //   edit distance is delete (i+1) chars from s to match empty t
        v1[0] = i + 1;

        // use formula to fill in the rest of the row
        for (unsigned j = 0; j < _t.length(); j++)
        {
            unsigned cost = (_s[i] == _t[j]) ? 0 : 1;
            v1[j + 1] = std::min(std::min(v1[j] + 1, v0[j + 1] + 1),
                                 v0[j] + cost);
        }

        // copy v1 (current row) to v0 (previous row) for next iteration
        for (unsigned j = 0; j < _t.length(); j++)
            v0[j] = v1[j];
    }

    delete [] v0;
    delete [] v1;

    return v1[_t.length()];
}
#endif
//============================================================================//
template <typename _Tp>
_Tp MyTools::GetEnv(const std::string& env, _Tp _default)
{
    char* env_var = getenv(env.c_str());
    if(env_var)
    {
        std::string str_var = std::string(env_var);
        std::istringstream iss(str_var);
        iss >> _default;
    }

    return _default;
}
//============================================================================//
template <typename value_type, typename _InputIterator>
value_type
MyTools::l1norm(_InputIterator _begin, _InputIterator _end)
{
    value_type sum = value_type(0.0);
    for(_InputIterator itr = _begin; itr != _end; ++itr)
        sum += std::abs(*itr);
    return sum;
}
//============================================================================//
template <typename value_type, typename _InputIterator>
value_type
MyTools::l2norm(_InputIterator _begin, _InputIterator _end)
{
    value_type sum = value_type(0.0);
    for(_InputIterator itr = _begin; itr != _end; ++itr)
        sum += (*itr)*(*itr);
    return std::sqrt(sum);
}
//============================================================================//
template <typename value_type, typename _InputIterator>
value_type
MyTools::norm(_InputIterator _begin, _InputIterator _end, uint16 nid)
{
    value_type sum = value_type(0.0);
    for(_InputIterator itr = _begin; itr != _end; ++itr)
        sum += std::pow(std::abs(*itr), static_cast<value_type>(nid));
    return std::pow(sum, 1.0/static_cast<value_type>(nid));
}
//============================================================================//
#ifndef SWIG
template <typename _InputIterator>
typename _InputIterator::value_type
MyTools::l1norm(_InputIterator _begin, _InputIterator _end)
{
    typedef typename _InputIterator::value_type value_type;
    value_type sum = value_type(0.0);
    for(_InputIterator itr = _begin; itr != _end; ++itr)
        sum += std::abs(*itr);
    return sum;
}
//============================================================================//
template <typename _InputIterator>
typename _InputIterator::value_type
MyTools::l2norm(_InputIterator _begin, _InputIterator _end)
{
    typedef typename _InputIterator::value_type value_type;
    value_type sum = value_type(0.0);
    for(_InputIterator itr = _begin; itr != _end; ++itr)
        sum += (*itr)*(*itr);
    return std::sqrt(sum);
}
//============================================================================//
template <typename _InputIterator>
typename _InputIterator::value_type
MyTools::norm(_InputIterator _begin, _InputIterator _end, uint16 nid)
{
    typedef typename _InputIterator::value_type value_type;
    value_type sum = value_type(0.0);
    for(_InputIterator itr = _begin; itr != _end; ++itr)
        sum += std::pow(std::abs(*itr), static_cast<value_type>(nid));
    return std::pow(sum, 1.0/static_cast<value_type>(nid));
}
#endif
//============================================================================//
template <typename _Tp>
typename _Tp::value_type MyTools::l1norm(const _Tp& array)
{
    typedef typename _Tp::const_iterator iterator;
    typedef typename _Tp::value_type value_type;
    value_type sum = value_type(0.0);
    for(iterator itr = array.begin(); itr != array.end(); ++itr)
        sum += std::abs(*itr);
    return sum;
}
//============================================================================//
template <typename _Tp>
typename _Tp::value_type MyTools::l2norm(const _Tp& array)
{
    typedef typename _Tp::const_iterator iterator;
    typedef typename _Tp::value_type value_type;
    value_type sum = value_type(0.0);
    for(iterator itr = array.begin(); itr != array.end(); ++itr)
        sum += (*itr)*(*itr);
    return std::sqrt(sum);
}
//============================================================================//
template <typename _Tp>
typename _Tp::value_type MyTools::norm(const _Tp& array, uint16 nid)
{
    typedef typename _Tp::const_iterator iterator;
    typedef typename _Tp::value_type value_type;
    value_type sum = value_type(0.0);
    for(iterator itr = array.begin(); itr != array.end(); ++itr)
        sum += std::pow(std::abs(*itr), static_cast<value_type>(nid));
    return std::pow(sum, 1.0/static_cast<value_type>(nid));
}
//============================================================================//

namespace
{
    //========================================================================//
    //                                  Clear Vector, List, Deque, Set, Multiset
    //========================================================================//
    template <typename T>
    inline void FreeClear(T& container)
    {
        for(typename T::iterator ite = container.begin();
            ite != container.end(); ++ite)
            delete *ite;
        container.clear();
    }
    //========================================================================//
    //                                                              Clear Map
    //========================================================================//
    template <typename T, typename U>
    inline void FreeClear(std::map<T,U>& container)
    {
        for(typename std::map<T,U>::iterator ite = container.begin();
            ite != container.end(); ++ite)
            delete ite->second;
        container.clear();
    }
    //========================================================================//
    //                                                          Clear MultiMap
    //========================================================================//
    template <typename T, typename U>
    inline void FreeClear(std::multimap<T,U>& container)
    {
        for(typename std::multimap<T,U>::iterator ite = container.begin();
            ite != container.end(); ++ite)
            delete ite->second;
        container.clear();
    }

}


#endif







