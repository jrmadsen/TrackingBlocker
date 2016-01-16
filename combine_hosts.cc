//
//
//
//
//

#include "Tools.hh"
#include "CommandLineOptions.hh"

//===================================================================================

std::string RemoveWeirdControlCharacters(const std::string& str)
{
    //std::cout << "Removing weird characters from " << str << "..." << std::flush;
    std::string retstr = "";
    for(uint32 i = 0; i < str.length(); ++i) {
        if(!isprint(str[i]) && (str[i] != '\n' || str[i] != ' ')) {
            if(str[i] == '\t') { retstr += "    "; }
            else { retstr += " "; }
        } else {
            retstr += str[i];
        }
    }
    //std::cout << "DONE (" << retstr << ")" << std::endl;
    return retstr;
}

//===================================================================================

bool RemoveWeirdControlCharactersFile(const std::string& filename)
{
    std::stringstream ss;
    ss << "rm_CTRL-M.sh " << filename;
    return (std::system(ss.str().c_str()) == 0) ? false : true;
}

//===================================================================================

int UpdateWinHosts()
{
    return std::system("curl http://winhelp2002.mvps.org/hosts.zip > WINHOSTS2002.zip");
}

//===================================================================================

int UnzipToFolder(const std::string& file, const std::string& directory)
{
    std::string command = "unzip -u -d " + directory + " " + file;
    return std::system(command.c_str());
}

//===================================================================================

class IP_ADDRESS
{
public:
    IP_ADDRESS(const std::string& str1, const std::string& str2)
    :   ip_id(RemoveWeirdControlCharacters(str1)), host(RemoveWeirdControlCharacters(str2))
    { }

    ~IP_ADDRESS() { }

    bool IsLocal() { return (tools::lower(host) == "localhost") ? true : false; }

    bool valid() const
    {
        return ((ip_id.length() > 0 && host.length() > 0) &&
                (ip_id.find("0.0.0.0") == std::string::npos ||
                 ip_id.find("127.0.0.1") == std::string::npos)) ? true : false;
    }

    bool operator==(const IP_ADDRESS& rhs)
    {
        return (host.length() == rhs.host.length() &&
                host == rhs.host) ? true : false;
    }

    bool operator!=(const IP_ADDRESS& ip2)
    {
        return !(*this == ip2);
    }

    IP_ADDRESS& operator=(const IP_ADDRESS& rhs)
    {
        if(*this != rhs) {
            ip_id = rhs.ip_id;
            host = rhs.host;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const IP_ADDRESS& ip)
    {
        out << ip.ip_id << "  " << ip.host << " ";
        return out;
    }

    bool operator>(const IP_ADDRESS& ip2) const
    {
        return (host > ip2.host) ? true : false;
    }

    bool operator<(const IP_ADDRESS& ip2) const
    {
        return (host < ip2.host) ? true : false;
    }

private:
    std::string ip_id;
    std::string host;

};

//===================================================================================

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

//===================================================================================

int main(int argc, char** argv)
{
    bool updatewinhosts = false;
    std::string winhostfile = "HOSTS";
    std::vector<std::string> files;
    std::string output = "hosts.ultrasafe";
    std::string orig = "";

    CommandLineOptions* clo = new CommandLineOptions();

    clo->AddOption('f',"files","Provide files to combine",true,-1,FORCE);
    clo->AddOption('u',"update-winhost","Update the WIN host filename",false,0);
    clo->AddOption('o',"output-file","Provide the output file",true, 1);
    clo->AddOption('d',"default","Original hosts file",true, 1,FORCE);

    clo->ProcessCommandLine(argc,argv);

    clo->GetOption('f', files);
    clo->GetOption('u', updatewinhosts);
    clo->GetOption('o', output);
    clo->GetOption('d', orig);

    if(updatewinhosts) {
        UpdateWinHosts();
        std::string dir = "WIN_HOSTS";
        std::string file = "WINHOSTS2002.zip";
        int success = UnzipToFolder(file, dir);
        if(success != 0) {
            Exception("Update WinHosts2002","Error unzipping " + file + " to directory",dir, WARNING);
        }
        winhostfile = dir + "/HOSTS";
	files.push_back(winhostfile);
    }

    std::set<IP_ADDRESS> IPS;
    std::vector<std::string> discarded;

    for( auto file : files ) {
        std::vector<std::string> delimit;
        std::ifstream in;
        bool cppremovectrl = RemoveWeirdControlCharactersFile(file);
        std::cout << "Opening file " << file << "..." << std::endl;
        in.open(file.c_str());
        if(!in) {
            Exception("IFSTREAM", "Could not open file", file, WARNING);
            continue;
        }

        if(cppremovectrl) { std::cout << file << " : C++ Remove CTRL characters..." << std::endl; }

        bool start = false;
        while(tools::GetLineAndDelimit(in," \t,","#",delimit)) {

            if(cppremovectrl) {
                std::string str = "";
                std::vector<std::string> fixeddelimit;
                for(auto ite = delimit.begin(); ite != delimit.end(); ++ite) {
                    //if(RemoveWeirdControlCharacters(*ite).empty()) { delimit.erase(ite); }
                    str = RemoveWeirdControlCharacters(*ite);
                    if(!str.empty()) { fixeddelimit.push_back(str); }
                }
                delimit.clear();
                delimit = fixeddelimit;
            }

            if(delimit.size() == 2) {

                IP_ADDRESS IP(IP_ADDRESS(delimit.at(0),delimit.at(1)));
                if(!start && IP.IsLocal()) {
                    start = true;
                } else if(start && IP.IsLocal()) {
                    continue;
                } else if(!IP.IsLocal()) {
                    IPS.insert(IP);
                } else {
                    discarded.push_back(tools::undelimit(delimit," "));
                }

            } else if(delimit.size() > 2) {
                tools::print_line_break(std::cout,1);
                tools::print(delimit,"\t|\t",std::cout);
                tools::print_line_break(std::cout,2);
                std::cout << "Does this belong in the combined? [size = " << delimit.size() << "]" << std::endl;
                std::string answer;
                std::cin >> answer;
                if(tools::GetBoolFromString(answer)) {
                    IP_ADDRESS IP(IP_ADDRESS(IP_ADDRESS(delimit.at(0),delimit.at(1))));
                    std::cout << "Adding " << IP << "..." << std::endl;
                    IPS.insert(IP);
                } else {
                    discarded.push_back(tools::undelimit(delimit," "));
                }
            } else {
                discarded.push_back(tools::undelimit(delimit,", "));
            }
        }
        std::cout << "Done reading " << file << std::endl;
        in.close();

    }

    std::ofstream out(output.c_str());
    WriteBeginning(out, orig);
    std::cout << "Printing IP hosts..." << std::endl;
    for( auto ip : IPS ) {
        out << ip << std::endl;
    }

    out.close();

    std::cout << "Printing discarded..." << std::endl;
    std::ofstream discard("Discarded.txt");

    for( auto something : discarded ) {
        discard << something << std::endl;
    }

    discard.close();

    return 0;
}

//===================================================================================







