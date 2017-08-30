//
//
//
//
//
//
//  This is a utility class from processing command line options
//  It offers greater functionality than <getopt.h> (long_options)
//  Every command has two methods of being called (short = dash + character)
//  or (long = dash dash + string). Consecutive options (e.g. -a -b combined to -ab)
//  will throw an error even if neither option requires a parameter (to be fixed in future)
//  Example of implementation:


#ifndef commandlineoptions_hh_
#define commandlineoptions_hh_

#include "Option.hh"
#include "TypeConversion.hh"
#include "MyHelpers.hh"
#include "Tools.hh"

// Containers
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <queue>
#include <stack>
#include <list>
#include <deque>

// Numerical
#include <float.h>
#include <cfloat>
#include <functional>
#include <numeric>
#include <algorithm>
#include <limits>

// IOS Libraries
#include <ios>
#include <iomanip>

// Stream Libraries
#include <sstream>
#include <istream>
#include <fstream>
#include <ostream>
#include <iostream>

// C/C++ Libraries
#include <cstdlib>
#include <cstdio>
#include <cassert>

// RTTI
#include <typeinfo>

// Memory tools
#include <memory>

// Options
#include <cstdarg>



namespace OPTREQUIRE
{
    enum OptionRequirement { FORCE, OPTIONAL };

    inline std::string OptionRequirementString(OptionRequirement req)
    { return (req == FORCE) ? "FORCED" : "OPTIONAL"; }
}

using namespace OPTREQUIRE;
using OPTREQUIRE::FORCE;
using OPTREQUIRE::OPTIONAL;

//============================================================================//

class CommandLineOptions
{
public:
    typedef std::deque<std::string>                     StrContainer_t;
    typedef std::deque<Option*>                         OptContainer_t;
    typedef std::deque<uint32>                          IndexContainer_t;
    typedef std::pair<uint32, OptionRequirement>        ReqPair_t;
    typedef std::deque<char>                            CharContainer_t;
    typedef std::pair<std::string, ReqPair_t>           StrReqPair_t;
    typedef std::map<StrReqPair_t, CharContainer_t >    SelectionSet_t;

public:
    inline CommandLineOptions(std::string* = 0);
    inline ~CommandLineOptions();

    inline static CommandLineOptions* Instance();
    inline static void Destroy();

private:
    inline static CommandLineOptions* MyInstance(CommandLineOptions*);

public:
    // Main function called from main after options are added
    inline void ProcessCommandLine(int,char**);

private:
    // Internal function used by above (original commands are translated into strings
    inline void ProcessCommandLine();

private:
    // Print the options
    inline void PrintOptions() { PrintOptions(std::cout); }
    inline void PrintOptions(std::ostream&);

public:
    // Get the last parameter if not part of option, default is string declared in constructor
    inline std::string GetMacro() { return (macro) ? *macro : ""; }
    // Get the index of the last command, returns -1 if last command is part of an option
    inline int GetLastIndex() { return rIndex; }

public:
    inline bool GetOption(char);
    inline bool GetOption(const std::string&);
    // ==================================================================================
    // Functions for resetting variables declared in main to command line specifications
    template <typename T> inline void GetOption(char,T&);
    //============================================================================//
    template <typename T> inline void GetOption(char, std::vector<T>&);
    //============================================================================//
    template <typename T> inline void GetOption(const std::string&, T&);
    //============================================================================//
    template <typename T> inline void GetOption(const std::string&, std::vector<T>&);
    //============================================================================//
    template <typename T> inline void GetOption(char, T*&);
    //============================================================================//
    template <typename T> inline void GetOption(const std::string&, T*&);

    // ======================================================
    // Functions for adding the options
    // First parameter is long_option, second parameter is
    // description, third specifies if a following parameter
    // is needed, fourth is short_option, fifth (optional) is
    // how many parameters the option accepts (default is 1)
    // a fifth parameter < 0, specifies an unlimited number of
    // parameters the option can accept
    inline void AddOption(char,
                          const std::string&,
                          const std::string&,
                          int nargs = 0,
                          OptionRequirement req = OPTIONAL);


    // For debugging purposes
    inline void SetVerboseLevel(uint16 value) { VERBOSITY = value; }

    //inline void PrintOption(std::ostream&,char);
    inline void DeclareSelectionList(std::string, uint32,
                                     OptionRequirement req = OPTIONAL);
    inline void Add2SelectionList(std::string,char);

    inline bool IsEnabled(char);
    inline bool IsEnabled(const std::string&);

    inline void ProcessFile(const std::string&);

private:
    inline void CheckDuplicateOption(char);
    inline void CheckDuplicateOption(std::string);
    inline void RemoveLeadingDashes(std::string&);
    inline char GetIndex(std::string);
    inline std::string GetString(char c);
    inline bool IsPotentialCommand(std::string);
    inline bool IsCommand(std::string);
    inline bool IsShort(std::string);
    inline void PrintSelectSetOptions(std::ostream&);
    inline void PrintForcedOptions(std::ostream&);

    inline IndexContainer_t GetCommandIndexes(StrContainer_t&);
    inline void ProcessComboShortCommand(std::string);
    inline StrContainer_t delimit(std::string,std::string);
    inline StrContainer_t delimit(std::string,char);
    inline StrContainer_t delimit(const StrContainer_t&, std::string);

private:
    std::string* macro;
    int rIndex;

    StrContainer_t commands;
    OptContainer_t options;
    std::map<char, std::string> variables;
    std::map<char, StrContainer_t> multivariables;
    std::map<char, Option*> optMap;
    uint16 VERBOSITY;
    std::string info;
    bool hasBeenProcessed;
    SelectionSet_t selection_set;
    std::vector<char> forcedRequirements;
    std::map<uint32,bool> combinedFlag;
    StrContainer_t unused_parameters;
    std::set<char> long_specified;

private:
    // private copy constructors
    CommandLineOptions& operator=(const CommandLineOptions&)
    {
        // blank, shouldnt be copied
        return *this;
    }

    CommandLineOptions(const CommandLineOptions&)
    : macro(0), rIndex(0), commands(StrContainer_t()),
      options(OptContainer_t()), variables(std::map<char, std::string>()),
      multivariables(std::map<char, StrContainer_t>()),
      optMap(std::map<char, Option*>()),
      VERBOSITY(0), info(""), hasBeenProcessed(false),
      selection_set(SelectionSet_t()),
      forcedRequirements(std::vector<char>()),
      combinedFlag(std::map<uint32,bool>()),
      unused_parameters(StrContainer_t()), long_specified(std::set<char>())
    {
        // blank, shouldnt be copied
    }

};



//============================================================================//
bool CommandLineOptions::GetOption(char id)
{
    return (optMap.find(id)->second->active()) ? true : false;
}
//============================================================================//
bool CommandLineOptions::GetOption(const std::string& str)
{
    return GetOption(GetIndex(str));
}
//============================================================================//
//============================================================================//
//                              TEMPLATE FUNCTIONS
//============================================================================//
template <typename T>
void CommandLineOptions::GetOption(char id, T& var)
{
    if(1 > optMap.count(id))
    {
        Exception("CommandLineOptions","Request to get unknown option",
                  id,WARNING);
        return;
    }

    TypeConversion<T> converter;
    if(optMap.find(id)->second->active() && variables.count(id) > 0)
    {
        std::string optionarg = variables.find(id)->second;
        var = converter(optionarg);
    }
    else if(optMap.find(id)->second->active() &&
            !optMap.find(id)->second->argument())
    {
        var = converter.invert(var);
    }
}
//============================================================================//
template <typename T>
void CommandLineOptions::GetOption(char id, std::vector<T>& var)
{
    if(1 > optMap.count(id))
    {
        Exception("CommandLineOptions","Request to get unknown option",
                  id,WARNING);
        return;
    }

    TypeConversion<T> converter;
    if(optMap.find(id)->second->active() && variables.count(id) > 0)
    {
        StrContainer_t optionarg = multivariables.find(id)->second;
        var = converter[optionarg];
    }
    else if(optMap.find(id)->second->active() &&
            !optMap.find(id)->second->argument())
    {
        var = converter.invert(var);
    }
}
//============================================================================//
template <typename T>
void CommandLineOptions::GetOption(char id, T*& var)
{
    if(1 > optMap.count(id))
    {
        Exception("CommandLineOptions","Request to get unknown option",
                  id,WARNING);
        return;
    }

    PointerTypeConversion<T> converter;
    if(optMap.find(id)->second->active() && variables.count(id) > 0)
    {
        std::string optionarg = variables.find(id)->second;
        StrContainer_t optionargVector
                = multivariables.find(id)->second;
        //if(optionargVector.size() == 1) {
            var = converter(optionarg);
        //} else { var = converter(optionargVector); }
    }
    else if(optMap.find(id)->second->active() &&
              !optMap.find(id)->second->argument())
    {
        var = converter.create();
    }
}
//============================================================================//
template <typename T>
void CommandLineOptions::GetOption(const std::string& str, T& var)
{
    char id = GetIndex(str);
    GetOption(id,var);
}
//============================================================================//
template <typename T>
void CommandLineOptions::GetOption(const std::string& str, std::vector<T>& var)
{
    char id = GetIndex(str);
    GetOption(id,var);
}
//============================================================================//
template <typename T>
void CommandLineOptions::GetOption(const std::string& str, T*& var)
{
    char id = GetIndex(str);
    GetOption(id,var);
}
//============================================================================//




//============================================================================//
//
CommandLineOptions* CommandLineOptions::Instance() { return MyInstance(NULL); }
//============================================================================//

CommandLineOptions* CommandLineOptions::MyInstance(CommandLineOptions* clo)
{
    static CommandLineOptions* myclo = NULL;
    if(clo) { myclo = clo; }
    return myclo;
}
//============================================================================//
void CommandLineOptions::Destroy()
{
    CommandLineOptions* myclo = MyInstance(NULL);
    if(myclo) { delete myclo; }
}
//============================================================================//
//          IMPLEMENTATIONS OF NON-TEMPLATED FUNCTIONS
//============================================================================//
CommandLineOptions::CommandLineOptions(std::string* defaultMacro)
: macro(defaultMacro), rIndex(0), commands(StrContainer_t()),
  options(OptContainer_t()), variables(std::map<char, std::string>()),
  multivariables(std::map<char, StrContainer_t>()),
  optMap(std::map<char, Option*>()),
  VERBOSITY(0), info("AvailableOptions:"), hasBeenProcessed(false),
  selection_set(SelectionSet_t()),
  forcedRequirements(std::vector<char>()),
  combinedFlag(std::map<uint32,bool>()),
  unused_parameters(StrContainer_t()), long_specified(std::set<char>())
{
    if(!MyInstance(NULL)) { MyInstance(this); }
    else {
        Exception("CommandLineOptions","Singleton Instance already exists",
                  Fatal);
    }
    // Add the help option
    options.push_back(new Option());
    optMap['h'] = options.back();
}
//============================================================================//
namespace hidden
{
template <typename T>
inline void FreeClear(T& container)
{
    for(typename T::iterator ite = container.begin();
        ite != container.end(); ++ite)
        delete *ite;
    container.clear();
}
}
//============================================================================//
CommandLineOptions::~CommandLineOptions()
{
    optMap.clear();
    hidden::FreeClear(options);
}
//============================================================================//
//  This function reads the command line, converts arguments to strings,
//  filters out equal signs and commas
void CommandLineOptions::ProcessCommandLine(int argc, char** argv)
{
    if(VERBOSITY > 0)
        std::cout << "\n# Processing " << argc-1
                  << " command line options..." << std::endl;

    // Translate the program arguments into vector string
    // so it can be manipulated for syntax issues
    for(int i = 1; i < argc; ++i)
    {
        std::string cmd = std::string(argv[i]);
        // handles the user including '=' between option and variable
        if(cmd.find("=") != std::string::npos ||
           cmd.find(",") != std::string::npos)
        {
            StrContainer_t tmp = delimit(cmd,"=,");
            std::copy(tmp.begin(),tmp.end(),
                      std::inserter(commands, commands.end()));
            // otherwise, just push it back
        } else
        {
            commands.push_back(cmd);
        }
    }

    if(VERBOSITY > 0)
    {
        std::cout << "Commands: " << std::endl;
        for(unsigned i = 0; i < commands.size(); ++i)
            std::cout << "\t" << i << " --> " << commands.at(i) << std::endl;
    }
    // Do actual processing
    ProcessCommandLine();

    hasBeenProcessed = true;

}
//============================================================================//
void CommandLineOptions::ProcessFile(const std::string& fname)
{

    std::ifstream in(fname.c_str());
    if(in)
    {
        std::vector<std::string> delim;
        while(tools::GetLineAndDelimit(in, " ,=", "#", delim))
        {
            if(delim.size() < 1) { break; }
            std::string cmd = delim.at(0);
            RemoveLeadingDashes(cmd);
            bool found = false;
            for(uint32 i = 0; i < options.size(); ++i)
            {
                Option* opt = options.at(i);
                if(cmd.size() > 1 && opt->command() == cmd)
                {
                    commands.push_back("--" + cmd);
                    found = true;
                    break;
                }
            }
            if(!found)
                commands.push_back("-" + cmd);
            for(uint32 i = 1; i < delim.size(); ++i)
                commands.push_back(delim.at(i));
        }
    }

    ProcessCommandLine();


}
//============================================================================//
// This function checks if their are duplicate options during run-time
void CommandLineOptions::CheckDuplicateOption(char id)
{
    if(id == 'h')
    {
        std::cout << "\n\tError : Do not assign char id to 'h' -- "
                     "reserved for help"
        << std::endl;
        info = "Options correctly added before error:";
        PrintOptions();
    } else if(optMap.count(id) > 0)
    {
        std::cout << "\n\tError : Cannot assign duplicate char identifiers - "
        << id << std::endl;
        info = "Options correctly added before error:";
        PrintOptions();
    }
}
//============================================================================//
void CommandLineOptions::CheckDuplicateOption(std::string long_option)
{
    for(uint32 i = 0; i < options.size(); ++i)
    {
        if(long_option == options.at(i)->command())
        {
            std::cout << "\n\tError : Cannot assign duplicate long options - "
            << long_option << std::endl;
            info = "Options correctly added before error:";
            PrintOptions();
        }
    }
}
//============================================================================//
void CommandLineOptions::AddOption(char id,
                                   const std::string& cmd,
                                   const std::string& descript,
                                   int nparams,
                                   OptionRequirement req)
{
    CheckDuplicateOption(id);
    CheckDuplicateOption(cmd);

    options.push_back(new Option(cmd, descript, !(nparams == 0), id, nparams));
    optMap[id] = options.back();

    if(req == FORCE) { forcedRequirements.push_back(id); }
}
//============================================================================//
void CommandLineOptions::ProcessCommandLine()
{
    if(VERBOSITY > 1)
    {
        std::cout << "# Input Commands: \n#\t";
        std::ostream_iterator<std::string> output(std::cout,"  ");
        std::copy(commands.begin(),commands.end(),output);
        std::cout << std::endl;
    }

    StrContainer_t commands_copy = commands;
    IndexContainer_t commandIndex = GetCommandIndexes(commands);
    std::sort(commandIndex.begin(), commandIndex.end());
    // LI = limit index
    uint32 LI = commandIndex.size();
    uint32 ncommands = commands.size();
    IndexContainer_t commandIndexCopy = commandIndex;

    while(commands.size() > 0)
    {
        StrContainer_t::iterator itr = commands.begin();
        bool combined = false;
        if(!IsPotentialCommand(*itr) || !IsCommand(*itr))
        {
            unused_parameters.push_back(*itr);
            commands.pop_front();
            continue;
        }
        else if(IsShort(*itr) && itr->length() > 2)
            combined = true;

        char cID;
        int index = (commandIndex.size() > 0) ? commandIndex.front() : 0;
        if(combined)
        {
            cID = itr->at(itr->length()-1);
            ProcessComboShortCommand(*itr);
            commands.pop_front();
            commandIndex.pop_front();
            itr = commands.begin();
        }
        else
        {
            cID = GetIndex(*itr);
            commands.pop_front();
            commandIndex.pop_front();
            itr = commands.begin();
        }

        Option* opt = optMap.find(cID)->second;
        opt->SetActive(true);

        if(opt->ID() == 'h')
            PrintOptions();

        int nparameters = opt->nparameters();
        if(nparameters < 0)
        {
            if(commandIndex.size() > 0)
                nparameters = commandIndex.front() - index - 1;
            else
                nparameters = ncommands - index - 1;
        }

        for(int i = 0; i < nparameters; ++i)
        {
            std::string acmd = commands.front();
            if(IsCommand(acmd))
            {
                Exception("CommandLineOptions::ProcessCommandLine()",
                          "Not enough arguments for " + opt->as_string(),
                          i, opt->nparameters(), FATAL);
            } else
            {
                variables[cID] = acmd;
                multivariables[cID].push_back(acmd);
            }
            commands.pop_front();
        }
    }

    if(VERBOSITY > 3)
    {
        std::map<char, Option*>::iterator ite;
        std::cout << std::endl;
        for(ite = optMap.begin(); ite != optMap.end(); ++ite)
        {
            std::cout << "# Option setting of " << ite->first << std::endl;
            ite->second->print(std::cout);
        }
        std::cout << std::endl;
    }


    if(VERBOSITY > 2)
    {
        std::cout << "# Activated Commands are:\t" << std::flush;
        for(unsigned int i = 0; i < LI; ++i)
        {
            std::cout << commands_copy.at(commandIndexCopy.at(i)) << " ("
                      << commandIndexCopy.at(i) << ")";
            if(i+1 < LI)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    if(VERBOSITY > 1)
        std::cout << "# Unused parameters: "
                  << unused_parameters.size() << std::endl;
    // Print Unused parameters (if any) and set macro to last one
    for(unsigned int i = 0; i < unused_parameters.size(); ++i)
    {
        if(i+1 < unused_parameters.size())
            Exception("CommandLineOptions","Unused parameter - "
                      + (unused_parameters.at(i)),WARNING);

        if(i+1 == unused_parameters.size() && macro)
        {
            *macro = unused_parameters.at(i);
            rIndex = static_cast<int>(i);
        }
    }

    // Force requirments
    std::vector<char>::iterator forceit;
    for(forceit = forcedRequirements.begin();
        forceit != forcedRequirements.end(); ++forceit)
        if(!optMap.find(*forceit)->second->active())
            Exception("CommandLineOptions","Missing required option",
                      optMap.find(*forceit)->second->as_string(), Fatal);

    // Enforce pairing
    SelectionSet_t::iterator ite;
    for(ite = selection_set.begin(); ite != selection_set.end(); ++ite)
    {
        //std::string category = ite->first.first;
        unsigned int req_count = ite->first.second.first;
        OptionRequirement require = ite->first.second.second;
        unsigned int active_count = 0;
        for(unsigned int i = 0; i < ite->second.size(); ++i)
        {
            if(optMap.count(ite->second.at(i)) < 1)
            {
                std::stringstream ss;
                ss << ite->second.at(i);
                Exception("CommandLineOptions","Unknown option - " + ss.str(),Fatal);
            } else
            {
                if(optMap.find(ite->second.at(i))->second->active())
                    ++active_count;
            }
        }
        if((active_count > 0 && active_count != req_count) ||
           (active_count != req_count && require == FORCE))
        {
            std::cout << "\nINFO:" << std::endl;
            PrintSelectSetOptions(std::cout);
            Exception("CommandLineOptions",
                      "Mismatch in required settings (there is either a "
                      "mismatch in the required number of arguments or you must"
                      " specify a certain option",Fatal);
        }
    }

    if(VERBOSITY > 0 && macro)
        std::cout << "# Macro is " << *macro << "\n" << std::endl;

}
//============================================================================//
//  This function is called when there is an error in the command line input
void CommandLineOptions::PrintOptions(std::ostream& out)
{
    out << "\n\t" << info << "\n" << std::endl;
    for(unsigned int i = 0; i < options.size(); ++i) {
        std::string dash = "-";
        dash += (options.at(i)->command().length() > 1) ? "-":"";
        dash += options.at(i)->command();
        out << "\t\t-" << options.at(i)->ID() << "   " << std::setw(20)
            << std::left
        << dash << " " << std::setw(10) << " : " <<
        std::setw(40) << options.at(i)->description();
        if(options.at(i)->nparameters() < 0) {
            std::cout << " (accepts unlimited number of parameters)";
        } else if(options.at(i)->nparameters() != 1) {
            std::cout << " (requires " << options.at(i)->nparameters()
                      << " parameters)";
        }
        std::cout << std::endl;
    }
    out << "\n" << std::endl;
    exit(0);
}
//============================================================================//
//  This function finds the charID associated with the longID
char CommandLineOptions::GetIndex(std::string str)
{
    RemoveLeadingDashes(str);

    for(unsigned int i = 0; i < options.size(); ++i)
    {
        // if specified by longID
        if(str == options.at(i)->command())
        {
            return options.at(i)->ID();
        // if specified by charID
        } else if(str.length() == 1)
        {
            std::string tmp = GetString(str.at(0));
            return GetIndex(tmp);
        }
    }

    std::cout << "GetIndex::Invalid option : " << str << std::endl;
    PrintOptions();
    return 0;
}
//============================================================================//
//  This function finds the longID associated with the charID
std::string CommandLineOptions::GetString(char c)
{
    for(unsigned int i = 0; i < options.size(); ++i)
        if(c == options.at(i)->ID())
            return options.at(i)->command();

    std::cout << "GetString::Invalid option : " << c << std::endl;
    PrintOptions();
    return "";
}
//============================================================================//
//  Quick check for if string has leading dash
bool CommandLineOptions::IsPotentialCommand(std::string cmd)
{
    if(cmd.find("-") == 0) { return true; }
    return false;
}
//============================================================================//
//  Parses commands to see if cmd is valid
bool CommandLineOptions::IsCommand(std::string cmd)
{
    uint32 ndashes = 0;
    if(cmd[0] == '-') { ++ndashes; }
    if(cmd[1] == '-') { ++ndashes; }

    switch(ndashes)
    {
        case 0:
            return false;
            break;
        case 1:
            for(uint32 i = 0; i < options.size(); ++i)
                if(cmd[1] == options.at(i)->ID())
                    return true;
            break;
        case 2:
            if(cmd.length() == 2)
                return false;
            cmd.erase(0,2);
            for(uint32 i = 0; i < options.size(); ++i)
                if(cmd == options.at(i)->command())
                {
                    long_specified.insert(options.at(i)->ID());
                    return true;
                }
            break;
    }

    return false;
}
//============================================================================//
//  Should not be called directly with command line string
//  Should only be called with copy of string from command line
void CommandLineOptions::RemoveLeadingDashes(std::string& str)
{
    while(str[0] == '-')
        str.erase(0,1);
}
//============================================================================//
// for combined options (e.g. "-czvf" in tar -czvf file.tar.gz DIR)
void CommandLineOptions::ProcessComboShortCommand(std::string cmd)
{
    RemoveLeadingDashes(cmd);

    for(uint32 i = 0; i < options.size(); ++i)
        if(cmd.find(options.at(i)->command()) != std::string::npos)
        {
            Exception("CommandLineOptions","Option |"+cmd
                      +"| may be a long-option (needs two leading dashes)",
                      WARNING);
        }

    std::vector<char> activated_ids;

    for(uint32 i = 0; i < cmd.length(); ++i)
    {
        if(i == 0 && cmd[i] == '-')
            continue;
        bool found = false;
        // process each character individually
        for(uint32 j = 0; j < options.size(); ++j)
            if(cmd[i] == options.at(j)->ID())
            {
                found = true;
                break;
            }

        // Handle whether found or not
        if(found)
        {
            std::stringstream ss;
            ss << cmd[i];
            if(optMap.find(cmd[i])->second->argument())
            {
                Exception("CommandLineOptions","Option |"+ss.str()
                          +"| needs an argument (in context |"+cmd+"|)",Fatal);
            }
            activated_ids.push_back(cmd[i]);
        } else
        {
            std::stringstream ss;
            ss << cmd[i];
            Exception("CommandLineOptions","Option: "+ss.str()
                      +" is invalid in this context |"+cmd+"|",Warning);
            PrintOptions();
        }
    }

    for(uint32 i = 0; i < activated_ids.size(); ++i)
    {
        if(VERBOSITY > 1)
            std::cout << "Activating command " << activated_ids.at(i)
                      << "..." << std::endl;
        optMap.find(activated_ids.at(i))->second->SetActive(true);
    }
}
//============================================================================//
CommandLineOptions::IndexContainer_t
CommandLineOptions::GetCommandIndexes(StrContainer_t& cmdline)
{
    IndexContainer_t indexes;
    for(uint32 i = 0; i < cmdline.size(); ++i)
    {
        std::string cmd = cmdline.at(i);
        // Quick check for anything starting with dash
        if(IsPotentialCommand(cmd))
        {
            // Actually checks list of options
            if(IsCommand(cmd))
            {
                // add index identifying option location
                indexes.push_back(i);
                // for combined options
                // (e.g. "-czvf" in tar -czvf file.tar.gz DIR)
                if(IsShort(cmd) && cmd.length() > 2)
                    combinedFlag[i] = true;
                else
                    combinedFlag[i] = false;
            }
        }
    }

    return indexes;
}
//============================================================================//
bool CommandLineOptions::IsShort(std::string cmd)
{
    if(cmd.length() < 2)
        return false;
    else if(cmd[1] == '-')
        return false;
    else if(cmd[0] == '-')
        return true;
    return false;
}
//============================================================================//
void CommandLineOptions::DeclareSelectionList(std::string name_, uint32 req_num,
                                              OptionRequirement req)
{
    ReqPair_t mf2 = std::make_pair(req_num,req);
    StrReqPair_t mf1 = std::make_pair(name_,mf2);

    selection_set.insert(std::make_pair(mf1, CharContainer_t(0)));
}
//============================================================================//
void CommandLineOptions::Add2SelectionList(std::string category, char param)
{
    SelectionSet_t::iterator ite;
    for(ite = selection_set.begin(); ite != selection_set.end(); ++ite)
    {
        if(ite->first.first == category)
        {
            ite->second.push_back(param);
            return;
        }
    }
    Exception("CommandLineOptions","Add2SelectionSet - category " + category
              + " does not exist",Fatal);

}
//============================================================================//
void CommandLineOptions::PrintSelectSetOptions(std::ostream& out)
{
    SelectionSet_t::iterator ite;

    for(ite = selection_set.begin(); ite != selection_set.end(); ++ite)
    {
        out << "\tThe selection set labeled -- " << ite->first.first
            << " -- requires you to select "
            << ite->first.second.first << " option(s) ("
            << OptionRequirementString(ite->first.second.second)
            << ") from:\n" << std::endl;
        for(uint32 i = 0; i < ite->second.size(); ++i)
        {
            out << "\t\t"
                << optMap.find(ite->second.at(i))->second->as_string()
                << std::endl;
        }
        out << std::endl;
    }
    out << std::endl;

}
//============================================================================//
void CommandLineOptions::PrintForcedOptions(std::ostream& out)
{
    if(forcedRequirements.size() == 0)
        return;
    out << "\n\tThe following options are always required:\n" << std::endl;
    for(uint32 i = 0; i < forcedRequirements.size(); ++i)
        std::cout << "\t\t"
                  << optMap.find(forcedRequirements.at(i))->second->as_string()
                  << std::endl;

    SelectionSet_t::iterator ite;

    for(ite = selection_set.begin(); ite != selection_set.end(); ++ite)
        if(ite->first.second.second == FORCE)
        {
            std::cout << "\n\t\tThe selection set categorically labeled -- "
                      << ite->first.first << " -- requires you to select "
            << ite->first.second.first << " option(s) ("
            << OptionRequirementString(ite->first.second.second) << ") from:"
            << std::endl;
            for(uint32 i = 0; i < ite->second.size(); ++i)
                std::cout << "\t\t\t"
                          << optMap.find(ite->second.at(i))->second->as_string()
                          << std::endl;
        }
    std::cout << std::endl;
}
// ============================================================================
//                                                      Delimit Line
inline CommandLineOptions::StrContainer_t
CommandLineOptions::delimit(std::string line, std::string delimiter)
{
    StrContainer_t newdelim;
    size_t d1 = 0;
    size_t d2 = 0;
    while(true)
    {
        d1 = line.find_first_not_of(delimiter, d2);
        if(d1 == std::string::npos)
            break;
        d2 = line.find_first_of(delimiter, d1);
        if(d2 == std::string::npos)
            d2 = line.length();
        newdelim.push_back(line.substr(d1,d2-d1));
    }

    return newdelim;

}
// ============================================================================
//                                                      Delimit Line
inline CommandLineOptions::StrContainer_t
CommandLineOptions::delimit(std::string line, char delimiter)
{
    StrContainer_t delimitedLine;
    std::string token;
    std::istringstream iss(line);
    while( getline(iss,token,delimiter) )
        if( !token.empty() )
            delimitedLine.push_back(token);

    return delimitedLine;
}
// ============================================================================
//                                                      Delimit Line
inline CommandLineOptions::StrContainer_t
CommandLineOptions::delimit(const StrContainer_t& line,
                            std::string delimiter)
{
    StrContainer_t newDelimited;
    for(uint32 i = 0; i < line.size(); ++i)
    {
        StrContainer_t newline = delimit(line[i],delimiter);
        for(uint32 j = 0; j < newline.size(); ++j)
            newDelimited.push_back(newline[j]);
    }
    return newDelimited;
}
// ============================================================================
//                                                      Is Enabled (char)
inline bool CommandLineOptions::IsEnabled(char ID)
{
    if(0 < optMap.count(ID))
        return optMap.find(ID)->second->active();
    else
        Exception("CommandLineOptions","Invalid ID - could not find command",
                  ID,WARNING);
    return false;
}
// ============================================================================
//                                                      Is Enabled (string)
inline bool CommandLineOptions::IsEnabled(const std::string& str)
{
    return IsEnabled(GetIndex(str));
}
//============================================================================//




#endif


