

#ifndef identifier_hh_
#define identifier_hh_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Identifier
{
public:
    Identifier(const char& char_, const std::string& str_, bool incomp_ = false)
    :   full_identifier("ls_"+std::string(&char_)+"_"+str_),
    short_identifier("s_"+std::string(&char_)), long_identifier("l_"+str_),
    has_short(true), has_long(true), incomplete(incomp_)
    { }

    Identifier(const std::string& str_, bool incomp_ = false)
    :   full_identifier("ls_0_"+str_), short_identifier("s_0"), long_identifier("l_"+str_),
    has_short(false), has_long(true), incomplete(incomp_)
    { }

    Identifier(const char& char_, bool incomp_ = false)
    :   full_identifier("ls_"+std::string(&char_)+"_0"),
    short_identifier("s_"+std::string(&char_)), long_identifier("l_0"),
    has_short(true), has_long(false), incomplete(incomp_)
    { }

    inline const std::string& GetFullID() const { return full_identifier; }
    inline const std::string& GetShortID() const { return short_identifier; }
    inline const std::string& GetLongID() const { return long_identifier; }
    inline bool GetHasShort() const { return has_short; }
    inline bool GetHasLong() const { return has_long; }
    inline bool GetIncomplete() const { return incomplete; }

    inline bool operator==(const Identifier& id_)
    {
        if((id_.incomplete && !this->incomplete) ||
           (!id_.incomplete && !this->incomplete)) {
            if(this->has_short && this->has_long) {
                return (id_.short_identifier == this->short_identifier ||
                        id_.long_identifier == this->long_identifier) ? true : false;
            } else {
                if(this->has_short) { return (id_.short_identifier == this->short_identifier) ? true : false; }
                else { return (id_.long_identifier == this->long_identifier) ? true : false; }
            }
        } else if(!id_.incomplete && this->incomplete) {
            if(id_.has_short && id_.has_long) {
                return (id_.short_identifier == this->short_identifier ||
                        id_.long_identifier == this->long_identifier) ? true : false;
            } else {
                if(id_.has_short) { return (id_.short_identifier == this->short_identifier) ? true : false; }
                else { return (id_.long_identifier == this->long_identifier) ? true : false; }
            }
        } else if(id_.incomplete && this->incomplete) {
            return (this->full_identifier == id_.full_identifier) ? true : false;
        }
        //Exception("Identifier","Bad logic in operator == ",FATAL);
        return false;
    }
    inline bool operator!=(const Identifier& id_)
    {
        return !(*this == id_);
    }

    bool operator<(const Identifier& id_)
    {
        return (this->full_identifier < id_.full_identifier) ? true : false;
    }

    bool operator<=(const Identifier& id_)
    {
        return (*this < id_) || (*this == id_);
    }

    bool operator>(const Identifier& id_)
    {
        return !(*this <= id_);
    }

    bool operator>=(const Identifier& id_)
    {
        return !(*this < id_);
    }

    friend bool operator<(const Identifier& id1_, const Identifier& id2_)
    {
        return id1_.full_identifier < id2_.full_identifier;
    }

    Identifier& operator=(const Identifier& rhs)
    {
        if(this != &rhs) {
            full_identifier = rhs.full_identifier;
            short_identifier = rhs.short_identifier;
            long_identifier = rhs.long_identifier;
            has_short = rhs.has_short;
            has_long = rhs.has_long;
            incomplete = rhs.incomplete;
        }
        return *this;
    }

protected:
    std::string full_identifier;
    std::string short_identifier;
    std::string long_identifier;
    bool has_short;
    bool has_long;
    bool incomplete;

private:

};




#endif
