//
//
//
//
//

#include "Tools.hh"
#include "CommandLineOptions.hh"
#include <string>
#include <limits.h>
#include <unistd.h>

#ifdef USE_PSTREAMS
#   include <pstreams/pstream.h>
#endif

//============================================================================//

std::string get_exe_path()
{
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    return std::string( result, (count > 0) ? count : 0 );
}

//============================================================================//

std::string get_working_path()
{
    char* cwd;
    char buff[PATH_MAX + 1];

    cwd = getcwd( buff, PATH_MAX + 1 );
    if( cwd != NULL )
        return std::string(cwd);
    else
        return "";
}

//============================================================================//

std::string RemoveWeirdControlCharacters(const std::string& str)
{
    std::string retstr = "";
    for(uint32 i = 0; i < str.length(); ++i) {
        if(!isprint(str[i]) && (str[i] != '\n' || str[i] != ' ')) {
            if(str[i] == '\t') { retstr += "    "; }
            else { retstr += " "; }
        } else {
            retstr += str[i];
        }
    }
    return retstr;
}

//============================================================================//

bool RemoveWeirdControlCharactersFile(const std::string& filename)
{
    std::stringstream ss;
    ss << "rm_CTRL-M " << filename;
    return (std::system(ss.str().c_str()) == 0) ? false : true;
}

//============================================================================//

int UpdateWinHosts(std::string odir)
{
    std::stringstream cmd;
    cmd << "curl http://winhelp2002.mvps.org/hosts.zip > "
        << tools::MakeDirectoryString(odir) << "WINHOSTS2002.zip";
    return std::system(cmd.str().c_str());
}

//============================================================================//

int UnzipToFolder(const std::string& file, const std::string& directory)
{
    std::string command = "unzip -u -d " + directory + " " + file;
    return std::system(command.c_str());
}

//============================================================================//

class IPaddr
{
public:
    typedef std::string         IP_t;
    typedef IP_t&               IP_t_ref;
    typedef IP_t*               IP_t_ptr;
    typedef const IP_t          cIP_t;
    typedef const IP_t_ref      cIP_t_ref;
    typedef const IP_t_ptr      cIP_t_ptr;
    typedef std::set<IP_t>      ForbiddenList_t;

public:
    //------------------------------------------------------------------------//
    IPaddr(const std::string& str)
    : host(RemoveWeirdControlCharacters(str))
    {
        if(host.find("www.") == 0)
            host.erase(host.find("www."), IP_t("www.").length());
    }

    ~IPaddr() { }

public:
    //------------------------------------------------------------------------//
    bool valid()
    {
        return forbidden.find(host) == forbidden.end();
    }
    //------------------------------------------------------------------------//
    bool operator==(const IPaddr& rhs)
    {
      return this->host == rhs.host;
    }
    //------------------------------------------------------------------------//
    bool operator!=(const IPaddr& ip2)
    {
        return !(*this == ip2);
    }
    //------------------------------------------------------------------------//
    IPaddr& operator=(const IPaddr& rhs)
    {
        if(*this != rhs)
            host = rhs.host;
        return *this;
    }
    //------------------------------------------------------------------------//
    friend std::ostream& operator<<(std::ostream& out, const IPaddr& ip)
    {
        out << "127.0.0.1  " << ip.host << " ";
        if(ip.host.find("www.") != 0)
            out << "\n127.0.0.1  www." << ip.host << " ";
        return out;
    }
    //------------------------------------------------------------------------//
    bool operator>(const IPaddr& rhs) const
    {
        return this->host < rhs.host;
    }
    //------------------------------------------------------------------------//
    bool operator<(const IPaddr& rhs) const
    {
        return this->host < rhs.host;
    }

public:
    //------------------------------------------------------------------------//
    static ForbiddenList_t* GetForbiddenList() { return &forbidden; }
    //------------------------------------------------------------------------//

private:
    static ForbiddenList_t forbidden;
    IP_t host;

};

//============================================================================//

IPaddr::ForbiddenList_t IPaddr::forbidden;

//============================================================================//

void WriteBeginning(std::ostream& out, std::string file)
{
  std::ifstream in(file.c_str());
  std::stringstream ss;
  if(in)
  {
    std::string line;
    while(!in.eof())
    {
      getline(in, line);
      ss << line << std::endl;
    }
    in.close();
    out << ss.str();
  } else {
    throw std::runtime_error("Unable to open original hosts file");
    exit(EXIT_FAILURE);
  }

}

//============================================================================//

void ProcessOriginal(const std::string& file)
{
    std::ifstream in(file.c_str());
    std::stringstream ss;
    if(in)
    {
        std::vector<std::string> delimit;
        while(tools::GetLineAndDelimit(in," \t,","#",delimit))
        {
            for(unsigned i = 1; i < delimit.size(); ++i)
                IPaddr::GetForbiddenList()->insert(delimit.at(i));

        }
    } else {
      throw std::runtime_error("Unable to open original hosts file");
      exit(EXIT_FAILURE);
    }

}

//============================================================================//

int main(int argc, char** argv)
{
    bool updatewinhosts = false;
    std::string winhostfile = "HOSTS";
    std::vector<std::string> files;
    std::string output = "hosts.profile.0";
    std::string orig = "";
    std::string odir = ".";

    CommandLineOptions* clo = new CommandLineOptions();

    clo->AddOption('f',"files","Provide files to combine",-1,FORCE);
    clo->AddOption('u',"update-winhost","Update the WIN host filename");
    clo->AddOption('U',"update-dir","Working directory of update", 1);
    clo->AddOption('o',"output-file","Provide the output file", 1);
    clo->AddOption('d',"default","Original hosts file", 1,FORCE);

    clo->ProcessCommandLine(argc,argv);

    clo->GetOption('f', files);
    clo->GetOption('u', updatewinhosts);
    clo->GetOption('U', odir);
    clo->GetOption('o', output);
    clo->GetOption('d', orig);

    if(odir.find_last_of("/") == odir.length()-1)
        odir.erase(odir.length()-1);

    if(odir != ".")
    {
        bool ret = tools::AddDirectory(odir);
        if(!ret)
            std::runtime_error("Unable to make output directory " + odir);
    }

    odir = tools::MakeDirectoryString(odir);

#ifdef USE_PSTREAMS
    std::vector<std::string> files2;
    for(auto itr = files.begin(); itr != files.end(); ++itr)
    {
        std::stringstream ss;
        ss << "/bin/ls " << *itr;
        // run a process and create a streambuf that reads its stdout and stderr
        redi::ipstream proc(ss.str().c_str(), redi::pstreams::pstdout);
        std::string line, stdout = "", stderr = "";
        // read child's stdout
        while (std::getline(proc.out(), line))
            stdout += line + " ";
        // read child's stderr
        while (std::getline(proc.err(), line))
            stderr += line;
        if(!stderr.empty())
            throw std::runtime_error(stderr.c_str());

        auto flist = tools::delimit(stdout, " \n\t");

        if(flist.size() == 0 || (flist.size() == 1 && flist.front() == *itr))
            files2.push_back(*itr);
        else
            for(auto f : flist)
                files2.push_back(f);
    }

    files = files2;
#endif

    std::string win_host_folder = "";
    std::string win_host_zipped = "";
    if(updatewinhosts)
    {
        UpdateWinHosts(odir);
        std::string dir = odir + "WIN_HOSTS";
        std::string file = odir + "WINHOSTS2002.zip";
        int success = UnzipToFolder(file, dir);
        if(success != 0) {
            Exception("Update WinHosts2002",
                      "Error unzipping " + file + " to directory", dir, WARNING);
        }
        winhostfile = dir + "/HOSTS";
        files.push_back(winhostfile);
        win_host_folder = dir;
        win_host_zipped = file;
    }

    ProcessOriginal(orig);

    // make the scope local
    {
        std::string fname = odir + "Forbidden.txt";
        std::ofstream out(fname.c_str());
        if(out)
            for(auto itr : *IPaddr::GetForbiddenList())
                out << itr << std::endl;
    }

    std::set<IPaddr> ip_addresses;
    std::vector<std::string> discarded;

    for( auto file : files )
    {
        std::vector<std::string> delimit;
        std::ifstream in;
        bool cppremovectrl = RemoveWeirdControlCharactersFile(file);
        std::cout << "Opening file " << file << "..." << std::endl;
        in.open(file.c_str());
        if(!in)
        {
            Exception("IFSTREAM", "Could not open file", file, WARNING);
            continue;
        }

        while(tools::GetLineAndDelimit(in," \t,","#",delimit))
        {
            if(cppremovectrl)
            {
                std::string str = "";
                std::vector<std::string> fixeddelimit;
                for(auto ite = delimit.begin(); ite != delimit.end(); ++ite)
                {
                    str = RemoveWeirdControlCharacters(*ite);
                    if(!str.empty()) { fixeddelimit.push_back(str); }
                }
                delimit.clear();
                delimit = fixeddelimit;
            }

            //----------------------------------------------------------------//
            auto discard = [&discarded,&delimit] (IPaddr::cIP_t_ref s)
            {
                discarded.push_back(tools::undelimit({delimit.at(0), s}, "  "));
            };
            //----------------------------------------------------------------//

            for(unsigned i = 1; i < delimit.size(); ++i)
            {
                IPaddr ip(delimit.at(i));

                if(i > 1 && (delimit.at(0) == "127.0.0.1" ||
                             delimit.at(0) == "0.0.0.0"))
                {
                    std::stringstream ss;
                    ss << "Does " << delimit.at(i) << " from \"";
                    tools::print(delimit,"\t|\t", ss);
                    ss << "\" belong in the host file? (y/n)\n";
                    std::cout << ss.str();
                    std::string answer;
                    std::cin >> answer;
                    if(!tools::GetBoolFromString(answer))
                    {
                        discard(delimit.at(i));
                        continue;
                    }
                }

                if(ip.valid())
                    ip_addresses.insert(ip);
                else
                    discard(delimit.at(i));
            }
        }
        std::cout << "Done reading " << file << "." << std::endl;
        in.close();

    }

    output = odir + output;
    std::ofstream out(output.c_str());
    if(out)
    {
        WriteBeginning(out, orig);
        std::cout << "Printing IP hosts : " << output << " ..." << std::endl;
        for( auto ip : ip_addresses )
            out << ip << std::endl;
        out.close();
    } else {
        throw std::runtime_error("Unable to open output file: " + output);
    }

    std::cout << "Printing discarded..." << std::endl;
    std::string dname = odir + "Discarded.txt";
    std::ofstream discard(dname.c_str());
    if(discard)
        for( auto something : discarded )
            discard << something << std::endl;
    discard.close();

    if(!win_host_folder.empty())
    {
        std::stringstream ss;
        ss << "if [ -d " << win_host_folder << " ]; then rm -rf "
           << win_host_folder << "; fi";
        int ret = std::system(ss.str().c_str());
        if(ret > 0)
            std::runtime_error("Command failed: " + ss.str());
    }

    if(!win_host_zipped.empty())
    {
        std::stringstream ss;
        ss << "if [ -f " << win_host_zipped << " ]; then rm -rf "
           << win_host_zipped << "; fi";
        int ret = std::system(ss.str().c_str());
        if(ret > 0)
            std::runtime_error("Command failed: " + ss.str());
    }

    return 0;
}

//===================================================================================







