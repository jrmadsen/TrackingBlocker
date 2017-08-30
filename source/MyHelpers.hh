//
//
//
//
//
//


#ifndef myhelpers_hh_
#define myhelpers_hh_

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

#if defined(USE_MPI)
#   include <mpi.h>
#endif

// C/C++ Libraries
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cctype>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <stdexcept>
#include <string>

#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <limits>
#include <fstream>

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

#ifdef CXX11
#   include <mutex>
#else
#   if defined(USE_BOOST) || defined(USE_BOOST_THREAD)
#       include <boost/thread/mutex.hpp>
#       include <boost/thread/lock_guard.hpp>
#   endif
#endif

namespace
{
    //========================================================================//
    enum MyExceptionSeverity
    {
        FATAL,
        WARNING,
        Fatal,
        Warning,
        Abort,
        ABORT
    };
    //========================================================================//
    namespace details
    {
#ifdef CXX11
        inline std::mutex& exception_mutex()
        {
            static std::mutex* mutex = new std::mutex;
            return *mutex;
        }
#else
#   if defined(USE_BOOST) || defined(USE_BOOST_THREAD)
        inline boost::mutex& exception_mutex()
        {
            static boost::mutex* mutex = new boost::mutex;
            return *mutex;
        }
#   endif
#endif
        namespace {
            static void ExitFunction()
            {
                int ec = 2;
#if defined(USE_MPI)
                MPI_Abort(MPI_COMM_WORLD, ec);
#else
                throw std::runtime_error("A fatal runtime error occurred -- See exception message");
                exit(ec);
#endif
            }

            static void EvaluateExceptionSeverity(const MyExceptionSeverity& severity)
            {
                switch(severity) {
                    case Fatal:
                    case FATAL:
                        std::cerr << " - FATAL ERROR\n" << std::endl;
                        ExitFunction();
                        break;
                    case Warning:
                    case WARNING:
                        std::cerr << " - WARNING" << std::endl;
                        break;
                    case Abort:
                    case ABORT:
                        std::cerr << " - FATAL_ERROR\n" << std::endl;
                        abort();
                        break;
                }
            }
        }
    }

#ifdef CXX11
#   define cxx11_lock_guard std::lock_guard<std::mutex> lock(details::exception_mutex());
#else
#   if defined(USE_BOOST) || defined(USE_BOOST_THREAD)
#       define cxx11_lock_guard boost::lock_guard<boost::mutex> lock(details::exception_mutex());
#   else
#       define cxx11_lock_guard
#   endif
#endif

    //========================================================================//
    //========================================================================//
    //          EXCEPTIONS
    //========================================================================//
    inline void Exception(const std::string& ss, MyExceptionSeverity severity)
    {
        cxx11_lock_guard
        std::cerr << "Exception: " << ss;
        details::EvaluateExceptionSeverity(severity);
    }

    inline void Exception(const std::string& ss1,
                          const std::string& ss2,
                          MyExceptionSeverity severity)
    {
        cxx11_lock_guard
        std::cerr << "Exception: " << ss2 << " **[ Issued by "
                  << ss1 << " ]**";
        details::EvaluateExceptionSeverity(severity);
    }

    template <typename T>
    inline void Exception(const std::string& ss1,
                          T ss2,
                          MyExceptionSeverity severity)
    {
        cxx11_lock_guard
        std::cerr << "Exception: " << ss1 << " :: " << ss2;
        details::EvaluateExceptionSeverity(severity);
    }

    template <typename T>
    inline void Exception(const std::string& ss1,
                          const std::string& ss2,
                          T ss3,
                          MyExceptionSeverity severity)
    {
        cxx11_lock_guard
        std::cerr << "Exception: " << ss2 << " :: " << ss3
                  << " **[ Issued by " << ss1 << " ]**";
        details::EvaluateExceptionSeverity(severity);
    }

    template <typename T, typename U>
    inline void Exception(const std::string& ss1,
                          const std::string& ss2,
                          const T& ss3,
                          const U& ss4,
                          MyExceptionSeverity severity)
    {
        cxx11_lock_guard
        std::cerr << "Exception: " << ss2 << " :: " << ss3 << " vs. "
                  << ss4 << " **[ Issued by " << ss1 << " ]**";
        details::EvaluateExceptionSeverity(severity);
    }

    template <typename T>
    inline void Exception(const std::string& ss1,
                          const std::string& ss2,
                          const std::vector<T>& ss3,
                          MyExceptionSeverity severity)
    {
        cxx11_lock_guard
        std::stringstream ss3V;
        ss3V << "{ ";
        for(unsigned int i = 0; i < ss3.size(); ++i)
        { ss3V << ss3.at(i); if(i+1 < ss3.size()) { ss3V << ", "; } }
        std::cerr << "Exception: " << ss2 << " :: " << ss3V.str()
                  << " **[ Issued by " << ss1 << " ]**";
        details::EvaluateExceptionSeverity(severity);
    }

    //========================================================================//
    //========================================================================//
    //          MESSAGES
    //========================================================================//
    template <typename T>
    inline void Message(const std::string& theclass,
                        const std::string& thefunction,
                        const T& message)
    {
        cxx11_lock_guard
        std::cout << "Message : [" << theclass << "] :: ["
        << thefunction << "] : " << message << std::endl;
    }

    template <typename T>
    inline void Message(const std::string& theclass,
                        const std::string& thefunction,
                        const std::string& message,
                        const T& variable,
                        const std::string& endtag = "")
    {
        cxx11_lock_guard
        std::cout << "Message : [" << theclass << "] :: ["
        << thefunction << "] : " << message << " \t-->\t"
        << variable << " " << endtag << std::endl;
    }

    template <typename T, typename U>
    inline void Message(const std::string& theclass,
                        const std::string& thefunction,
                        const std::string& message,
                        const T& variable1,
                        const U& variable2,
                        const std::string& endtag = "")
    {
        cxx11_lock_guard
        std::cout << "Message : [" << theclass << "] :: ["
        << thefunction << "] : " << message << " \t-->\t"
        << variable1 << " vs. "
        << variable2 << " " << endtag << std::endl;
    }

    template <typename T>
    inline void SimpleMessage(const std::string& thefunction,
                              const T& message)
    {
        cxx11_lock_guard
        std::cout << "Message :: [" << thefunction << "] : "
        << message << std::endl;
    }

    template <typename T>
    inline void SimpleMessage(const std::string& thefunction,
                              const std::string& message,
                              const T& variable,
                              const std::string& endtag = "")
    {
        cxx11_lock_guard
        std::cout << "Message :: [" << thefunction << "] : "
        << message << " \t-->\t" << variable << " " << endtag
        << std::endl;
    }

    template <typename T, typename U>
    inline void SimpleMessage(const std::string& thefunction,
                              const std::string& message,
                              const T& variable1,
                              const U& variable2,
                              const std::string& endtag = "")
    {
        cxx11_lock_guard
        std::cout << "Message :: [" << thefunction << "] : "
        << message << " \t-->\t" << variable1 << " vs. "
        << variable2 << " " << endtag << std::endl;
    }

}

#undef cxx11_lock_guard

//============================================================================//
//  Checks for alpha characters in string that should be numeric
class CheckAlpha
{
public:
    bool operator()(char c)
    {
        if(isalpha(c) && c != 'e' && c != 'E')
        {
            std::string ss(&c);
            Exception("Invalid conversion of alpha character ("+ss+") to number",
                      WARNING);
        }
        return isalpha(c);
    }

    bool check(std::string str)
    {
        for(uint32 i = 0; i < str.length(); ++i)
            if(isalpha(str.at(i)) && str.at(i) != 'e' && str.at(i) != 'E')
                return true;
        return false;
    }
};

//============================================================================//
// Verifies string is fully numeric
class CheckNumeric
{
public:
    bool operator()(int c) { return isdigit(c); }

    bool check(const std::string& str)
    {
        return (str.find_first_not_of("0123456789.e+-*/") == std::string::npos)
                ? true : false;
    }

    bool check(const std::vector<std::string>& vstr)
    {
        for(uint32 i = 0; i < vstr.size(); ++i) {
            if(!check(vstr.at(i))) { return false; }
        }
        return true;
    }
};

//============================================================================//
// General Deletion
template <typename T>
class deleter
{
public:
    void operator()(const T* ite) const
    {
        delete ite;
    }

};

//============================================================================//

class PercentageStatus
{
public:
    PercentageStatus(uint32 ntot_, uint32 noutputs_)
    : ntot(ntot_), noutputs(noutputs_), last_perc(-1), ncurrent(0)
    { }


    inline bool print(uint32 n)
    {
        uint32 perc = percent(n);
        if(perc%(static_cast<uint32>(100./noutputs)) == 0 &&
           static_cast<int>(perc) != last_perc)
        {
            last_perc = perc;
            return true;
        }
        else
        {
            return false;
        }
    }

    friend std::ostream& operator << (std::ostream& out,
                                      const PercentageStatus& _ps)
    {
        out << _ps.percent(_ps.ncurrent) << "%" << std::flush;
        return out;
    }

    inline void Reset()
    {
        last_perc = -1;
        ncurrent = 0;
    }
private:

    inline uint32 percent(const uint32& n) const
    {
        return static_cast<uint32>(static_cast<double>
                                   (const_cast<uint32&>(ncurrent) = n)
                                   /static_cast<double>(ntot)*100.);
    }

private:
    uint32 ntot;
    uint32 noutputs;
    int last_perc;
    uint32 ncurrent;
};

//============================================================================//

class SimpleTimer
{
public:
    SimpleTimer()
    : tstart(time(NULL)), tend(time(NULL)), telapsed(0.),
      cstart(clock()), cend(clock()), celapsed(0.)
    {

    }

public:
    inline void Start()
    {
        telapsed += tend - tstart;
        celapsed += cend - cstart;
        tstart = time(NULL);
        cstart = clock();
    }

    inline void Stop()
    {
        tend = time(NULL);
        cend = clock();
    }

    inline long int GetUserElapsedTime() const { return telapsed; }
    inline long int GetSysElapsedTime() const { return celapsed; }

    friend std::ostream& operator << (std::ostream& out, const SimpleTimer& st)
    {
        out << st.GetUserElapsedTime() << " s, " << st.GetSysElapsedTime()
            << " ms" << std::flush;
        return out;
    }

private:
    time_t tstart;
    time_t tend;
    time_t telapsed;

    clock_t cstart;
    clock_t cend;
    clock_t celapsed;
};

//============================================================================//
// This is a class for reading and writing binary files
// The constructor automatically calls SetBigEndian(IsBigEndian())
// to determine whether or not the computer is BigEndian or LittleEndian
// when reading or writing a file that is of known endian-ness
// the endian-ness of the file should be set before reading/writing the file
/*
#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 5096
#endif

class BinaryReaderWriter
{
public:
    //========================================================================//
    BinaryReaderWriter(const std::string& mode_)
    : file(0), fname(""), is_valid(false), BUFFER_SIZE(DEFAULT_BUFFER_SIZE),
      buffer(new char[DEFAULT_BUFFER_SIZE]),
      file_end(0), bytes_read(0), big_endian(IsBigEndian()), mode(mode_)
    { }
    //========================================================================//
    BinaryReaderWriter(const std::string& mode_, const std::string& fname_)
    : file(0),fname(fname_), is_valid(false),
      BUFFER_SIZE(DEFAULT_BUFFER_SIZE),
      buffer(new char[DEFAULT_BUFFER_SIZE]),
      file_end(get_file_num_bytes(fname_)), bytes_read(0),
      big_endian(IsBigEndian()), mode(mode_)
    {
        open();
    }
    //========================================================================//
    ~BinaryReaderWriter()
    {
        if(is_open())
            close();
    }
    //========================================================================//

public:
    //========================================================================//
    //template <typename T> inline void operator()(const T& val) { write(val); }
    //========================================================================//
    inline void SetFilename(const std::string& val)
    {
        fname = val;
        if(mode.find("w") == std::string::npos &&
           mode.find("r") != std::string::npos)
            file_end = get_file_num_bytes(val);
    }
    //========================================================================//
    inline void open(const std::string& mode_, const std::string& val)
    {
        fname = val;
        mode = mode_;
        try
        {
            file = std::fopen(fname.c_str(), mode_.c_str());
        } catch (const std::exception& e)
        {
            std::cerr << "Exception -- " << e.what()
                      << " **[ issued by BinaryReadWrite -- open() ]**"
                      << std::endl;
        }

        if(!file)
        {
            Exception("BinaryReaderWriter","Unsuccessful open of file",
                      fname, WARNING);
            file = 0;
        } else
        {
            if(mode.find("w") == std::string::npos &&
               mode.find("r") != std::string::npos)
                file_end = get_file_num_bytes(fname);
        }
    }
    //========================================================================//
    inline void open()
    {
        if(fname.length() == 0)
        {
            Exception("BinaryReadWrite -- open()",
                      "Cannot open file without setting name of file",
                      FATAL);
        }
        try
        {
            file = std::fopen(fname.c_str(), mode.c_str());
        } catch (const std::exception& e) {
            std::cerr << "Exception -- " << e.what()
                      << " **[ issued by BinaryReadWrite -- open() ]**"
                      << std::endl;
        }

        if(!file)
        {
            Exception("BinaryReaderWriter","Unsuccessful open of file",
                      fname,WARNING);
            file = 0;
        } else
        {
            if(mode.find("w") == std::string::npos &&
               mode.find("r") != std::string::npos)
                file_end = get_file_num_bytes(fname);
            std::cout << "BRW OPEN :: " << *this << std::endl;
        }
    }
    //========================================================================//
    inline void close()
    {
        int success_close;
        //int flush_close = std::fflush(file);

        try {
            success_close = std::fclose(file);
        } catch (const std::exception& e) {
            std::cerr << "Exception -- " << e.what()
                      << " **[ issued by BinaryReadWrite - close() ]**"
                      << std::endl;
        }

        if(!(success_close == 0))
        {
            Exception("BinaryReaderWriter -- close()",
                      "Unsuccessful close of "+fname,
                      success_close, FATAL);
        } else
        {
            file = 0;
        }

        std::cout << "BRW CLOSE :: " << *this << std::endl;

    }
    //========================================================================//
    inline bool is_open()
    {
        //std::cout << "BRW IS_OPEN :: " << *this << std::endl;
        return is_valid = (file != 0) ? true : false;
    }
    //========================================================================//
    inline bool eof()
    {
        //std::cout << "BRW EOF :: " << *this << std::endl;
        return (bytes_read >= file_end || !is_valid || feof(file)) ? true : false;
    }
    //========================================================================//
    inline void change_buffer_size(int size_)
    {
        if(size_ != BUFFER_SIZE)
        {
            const_cast<int&>(BUFFER_SIZE) = size_;
            if(buffer)
            {
                delete [] buffer;
                buffer = new char[BUFFER_SIZE];
            }
        }
    }
    //========================================================================//
    template <typename T>
    inline void write(const T& val, size_t nrepeat = 1)
    {
        if(!is_open())
            Exception("BinaryWriter",
                      "BinaryReaderWriter has not opened a file",FATAL);

        size_t successful_write = std::fwrite(&val, sizeof(T), nrepeat, file);

        if(!(successful_write == nrepeat))
        {
            std::stringstream ss;
            ss << val;
            Exception("BinaryReaderWriter",
                      "Unsuccessful write of "+ss.str(),
                      successful_write,nrepeat,FATAL);
        } else
        {
            file_end += sizeof(T);
        }

        //std::cout << "BRW WRITE :: " << *this << std::endl;

    }
    //========================================================================//
    template <typename T>
    inline bool read(T& val_, size_t nrepeat = 1)
    {
        //std::cout << "\nBRW READ :: " << *this << std::endl;

        //if(buffer) { delete [] buffer; }
        //buffer = new char[DEFAULT_BUFFER_SIZE];

        if(!is_open())
            Exception("BinaryWriter",
                      "BinaryReaderWriter has not opened a file",
                      FATAL);
        else if((bytes_read + sizeof(T)) > file_end)
        {
            Exception("BinaryReaderWriter",
                      "Bytes read + requested number of bytes exceeds "
                      "the file size",WARNING);
            bytes_read += sizeof(T);
            std::cout << "\nBRW READ :: " << *this << std::endl;
            close();
            return false;
        }
        size_t successful_read = std::fread(buffer, sizeof(T), nrepeat, file);
        if(successful_read == nrepeat)
        {
            BinaryReaderWriter::GetValue(buffer, val_);
            bytes_read += sizeof(val_);
        } else
        {
            Exception("BinaryReaderWriter",
                      "Unsuccessful read of value",
                      successful_read,nrepeat,WARNING);
        }

        //std::cout << "BRW READ :: " << *this << std::endl;

        return (successful_read == nrepeat);
    }
    //========================================================================//
    template <typename T>
    inline T read(size_t nrepeat = 1)
    {
        //std::cout << "\nBRW READ :: " << *this << std::endl;
        //if(buffer) { delete [] buffer; }
        //buffer = new char[DEFAULT_BUFFER_SIZE];

        T val_;
        if(!is_open())
            Exception("BinaryWriter",
                      "BinaryReaderWriter has not opened a file",FATAL);
        else if((bytes_read + sizeof(T)) > file_end)
        {
            Exception("BinaryReaderWriter",
                      "Bytes read + requested number of bytes exceeds "
                      "the file size",WARNING);
            bytes_read += sizeof(T);
            std::cout << "\nBRW READ :: " << *this << std::endl;
            close();
            return val_;
        }

        size_t successful_read = std::fread(buffer, sizeof(T), nrepeat, file);
        if(successful_read == nrepeat)
        {
            BinaryReaderWriter::GetValue(buffer, val_);
            bytes_read += sizeof(val_);
        } else
        {
            Exception("BinaryReaderWriter",
                      "Unsuccessful read of value",successful_read,
                      nrepeat,WARNING);
        }

        //std::cout << "BRW READ :: " << *this << std::endl;

        return val_;
    }
    //========================================================================//
    friend std::ostream& operator<<(std::ostream& out, const BinaryReaderWriter& brw)
    {
        out << "ADDR : " << brw.file << ", FILE : " << brw.fname
            << ", BYTE_READ : "
            << brw.bytes_read << ", TOT_BYTE : " << brw.file_end
            << ", MODE : " << brw.mode
            << ", ENDIAN : " << ((brw.big_endian) ? "BIG" : "LITTLE");
        return out;
    }
    //========================================================================//

private:
    FILE* file;
    std::string fname;
    bool is_valid;
    const int BUFFER_SIZE;
    char* buffer;
    unsigned long file_end;
    unsigned long bytes_read;
    bool big_endian;
    std::string mode;

public:
    //========================================================================//
    // For if reading a big_endian file on little_endian machine or vice versa
    inline void SetBigEndian(bool is_big_endian)
    {
        big_endian = is_big_endian;
    }
    //========================================================================//
    template <typename T>
    inline void GetValue(char* val_, T& rval_)
    {

#if BYTE_ORDER == BIG_ENDIAN
        if(!big_endian) // Little endian
#else
        if(big_endian)  // Big endian
#endif
        {
            const int SIZE = sizeof(rval_);
            char ctemp;
            for(int i = 0; i < SIZE/2; ++i) {
                ctemp = val_[i];
                val_[i] = val_[SIZE - 1 - i];
                val_[SIZE - 1 - i] = ctemp;
            }
        }
        rval_ = *(T*)val_;
    }
    //========================================================================//
    template <typename T>
    inline T GetValue(char* val_)
    {
        T rval_;
#if BYTE_ORDER == BIG_ENDIAN
        if(!big_endian) // Little endian
#else
        if(big_endian)  // Big endian
#endif
        {
            const int SIZE = sizeof(rval_);
            char ctemp;
            for(int i = 0; i < SIZE/2; ++i) {
                ctemp = val_[i];
                val_[i] = val_[SIZE - 1 - i];
                val_[SIZE - 1 - i] = ctemp;
            }
        }
        return *(T*)val_;
    }
    //========================================================================//

public:
    //========================================================================//
    // STATIC Functions and variables
    //========================================================================//
    // Determine if machine is big_endian or little_endian
    static inline bool IsBigEndian()
    {
        unsigned int i = 1;
        char* c = (char*)&i;
        // Little endian
        if (*c)
            return false;
        return true;    // Big endian
    }
    //========================================================================//
    static inline long get_file_num_bytes(const std::string& fname_)
    {
        std::ifstream myfile(fname_.c_str(), std::ifstream::binary);
        if(myfile)
        {
            myfile.seekg (0, std::ios::end);
            return myfile.tellg();
        }
        return 0;
    }
    //========================================================================//

};*/



//============================================================================//


#endif
