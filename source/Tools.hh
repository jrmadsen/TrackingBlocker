//
//
//
//
//
//
//

#ifndef tools_hh_
#define tools_hh_ 1

#include "MyTools.hh"
#include "MyHelpers.hh"

#ifndef CXX11

#if __cplusplus > 199711L   // C++11
#define CXX11
#endif

#endif

//============================================================================//

namespace tools
{
    //using namespace MyTools;

    using MyTools::MakeLower;
    using MyTools::MakeUpper;
    using MyTools::lower;
    using MyTools::upper;
    using MyTools::checkpos;
    using MyTools::checkposic;
    using MyTools::s2i;
    using MyTools::s2d;
    using MyTools::s2n;
    using MyTools::n2s;
    using MyTools::RemoveElement;
    using MyTools::print;
    using MyTools::subvector;
    using MyTools::checkforID;
    using MyTools::removepos;
    using MyTools::Purge;
    using MyTools::RemoveDuplicates;
    using MyTools::count;
    using MyTools::IsDuplicate;
    using MyTools::Scale;
    using MyTools::mean;
    using MyTools::StdDev;
    using MyTools::RelativeError;
    using MyTools::ExitWithoutParams;
    using MyTools::OpenFile;
    using MyTools::RemoveComment;
    using MyTools::GetLine;
    using MyTools::GetLineAndDelimit;
    using MyTools::delimit;
    using MyTools::undelimit;
    using MyTools::Capitalize;
    using MyTools::Uncapitalize;
    using MyTools::RemoveWhitespace;
    using MyTools::RemoveLeadingWhitespace;
    using MyTools::RemoveTrailingWhitespace;
    using MyTools::StripWhitespace;
    using MyTools::strcmpic;
    using MyTools::xmlopen;
    using MyTools::xmlclose;
    using MyTools::tablevel;
    using MyTools::AddValue;
    using MyTools::GetMin;
    using MyTools::GetMax;
    using MyTools::Sum;
    using MyTools::GenerateXMLblock;
    using MyTools::AddLine;
    using MyTools::lineXML;
    using MyTools::GetBoolFromString;
    using MyTools::RequireSize;
    using MyTools::CheckSize;
    using MyTools::GetParam;
    using MyTools::CheckForParam;
    using MyTools::convert;
    using MyTools::convert2str;
    using MyTools::convertfromstring;
    using MyTools::SetDoublePrecision;
    using MyTools::TruncatePrecision;
    using MyTools::GetIteratorPosition;
    using MyTools::GetIteratorIndex;
    using MyTools::AddDirectory;
    using MyTools::MakeDirectory;
    using MyTools::mkdir;
    using MyTools::MakeDirectoryString;
    using MyTools::join;
    using MyTools::append_string;
    using MyTools::exists;
    using MyTools::interpolate;
    using MyTools::loginterpolate;
    using MyTools::ExplicitEuler;
    using MyTools::ImplicitEuler;
    using MyTools::factorial;
    using MyTools::FreeClear;
    using MyTools::CheckFile;
    using MyTools::modulus;
    using MyTools::AddVectorData2Map;
    using MyTools::GetC_Array;
    using MyTools::GetEnvironmentVariable;
    using MyTools::LevenshteinDistance;
    using MyTools::GetEnv;
    using MyTools::l1norm;
    using MyTools::l2norm;
    using MyTools::norm;

}

namespace tools
{
    inline void print_line_break(std::ostream&,uint32);
    inline void print_line_break(std::stringstream&,uint32);
    inline void printhere(uint32&, std::ostream& out = std::cout,
                          const std::string& descript = "here");
    template <typename T>
    inline void printhere(uint32&, const T&, std::ostream& out = std::cout,
                          const std::string& descript = "here");
    template <typename T>
    inline std::string getstr(const T& container,
                              const std::string delim = ", ");
}

//============================================================================//
inline void tools::print_line_break(std::ostream& out, uint32 ncount)
{
    for(uint32 i = 0; i < ncount; ++i) { out << std::endl; }
}
//============================================================================//
inline void tools::print_line_break(std::stringstream& out, uint32 ncount)
{
    for(uint32 i = 0; i < ncount; ++i) { out << std::endl; }
}
//============================================================================//
inline void tools::printhere(uint32& ncount, std::ostream& out,
                             const std::string& descript)
{
    out << descript << " " << ncount++ << std::endl;
}
//============================================================================//
template <typename T>
inline void tools::printhere(uint32& ncount, const T& message,
                             std::ostream& out, const std::string& descript)
{
    out << descript << " " << ncount++ << " : " << message << std::endl;
}
//============================================================================//
template <typename T>
inline std::string tools::getstr(const T& container, const std::string delim)
{
    typedef typename T::const_iterator citerator;
    std::stringstream ss;
    uint32 n = 0;
    for(citerator ite = container.begin(); ite != container.end(); ++ite, ++n)
    {
        ss << *ite;
        if(n+1 < container.size())
            ss << delim;
    }
    return ss.str();
}
//============================================================================//
#ifdef CXX11
//============================================================================//
inline std::string strbool(const bool& val)
{
    return (val) ? "ON" : "OFF";
}
//============================================================================//
inline std::string ynbool(const bool& val)
{
    return (val) ? "YES" : "NO";
}
//============================================================================//
#endif






//============================================================================//
namespace tools
{

    template <typename T> T summation(const std::vector<T> dataset)
    {
        T sum_;
        for(uint32 i = 0; i < dataset.size(); ++i) { sum_ += dataset.at(i); }
        return sum_;
    }

    //========================================================================//
    template <typename T>
    class sum_vector
    {
    public:
        sum_vector(const std::vector<T>& data_)
        :   data_set(data_),data_sum(tools::summation(data_))
        { }

#ifndef SWIG
        inline const T& operator[](const uint32& id_) const { return data_set[id_]; }
#endif
        inline const T& at(const uint32& id_) const { return data_set.at(id_); }
        inline void set(const uint32& id_, const T& val_)
        {
            data_sum += (val_ - data_set.at(id_));
            data_set[id_] = val_;
        }
        inline std::vector<T>* GetDataset() { return &data_set; }

    private:
        std::vector<T> data_set;
        T data_sum;
        T data_max;
    };
    //========================================================================//

}
//============================================================================//

#endif // tools_hh_

