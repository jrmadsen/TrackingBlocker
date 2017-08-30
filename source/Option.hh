//
//
//
//
//
//


#ifndef option_hh_
#define option_hh_

#include "Identifier.hh"

//============================================================================//
class Option
{
public:
    Option(std::string cmd = "help",
           std::string descript = "print available options",
           bool needArg = false,
           char cId = 'h',
           int nparams = 1)
    :   command_(cmd),description_(descript),needArgument_(needArg),id_(cId),
        active_(false),nparameters_((needArg) ? nparams : 0), identifier_(Identifier(cId,cmd))
    { }

    virtual ~Option() { }

    void SetCommand(const std::string& str) { command_ = str; }
    void SetDescription(const std::string& str) { description_ = str; }
    void SetArgument(bool val) { needArgument_ = val; }
    void SetID(char val) { id_ = val; }
    void SetActive(bool val) { active_ = val; }
    void SetNumParameters(int val) { nparameters_ = val; }

    virtual std::string command() const { return command_; }
    virtual char ID() const { return id_; }

    std::string description() const { return description_; }
    bool argument() const { return needArgument_; }
    bool active() const { return active_; }
    int nparameters() const { return nparameters_; }

    inline const Identifier& GetIdentifier() const { return identifier_; }

    virtual std::string as_string() {
        std::stringstream ss;
        ss << "[## --" << command_ << " (-" << id_ << ") taking ";
        (nparameters_ >= 0) ? (ss << nparameters_) : (ss << "unlimitied");
        ss << " parameters ##]";
        return ss.str();
    }

    //virtual create_identifiers
    virtual void print(std::ostream& out)
    {
        out << std::boolalpha;
        out << "\tCommand \t - " << command_ <<
        "\n\tDescription  \t - " << description_ <<
        "\n\tNeed argument\t - " << needArgument_ <<
        "\n\tID           \t - " << id_ <<
        "\n\tActive       \t - " << active_ <<
        "\n\tN Parameters \t - " << nparameters_
        << std::endl;
        out.unsetf(std::ios_base::boolalpha);
    }

    bool operator==(const char& short_id) { return (short_id == id_) ? true : false; }
    bool operator==(const std::string& long_id) { return (long_id == command_
                                                          || long_id == std::string("--"+command_)
                                                          || long_id == std::string(&id_)
                                                          || long_id == std::string("-"+std::string(&id_))) ? true : false; }

protected:
    std::string command_;
    std::string description_;
    bool needArgument_;
    char id_;
    bool active_;
    int nparameters_;
    Identifier identifier_;

public:
    /*bool operator==(const Option& rhs)
    {
        return (this->key == rhs.key) ? true : false;
    }

    bool operator!=(const Option& rhs)
    {
        return !(*this == rhs);
    }*/

};

//============================================================================//

#endif
