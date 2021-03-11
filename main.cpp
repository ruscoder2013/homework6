#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>

std::string get_bulk_str(std::vector<std::string>& commands) {
    std::ostringstream oss;
    if (commands.size()==0) return "";
    oss << "bulk: ";
    for(int i = 0; i < commands.size(); i++)
    {
        if (i>0)
            oss << ", ";
        oss << commands[i];
    }
    oss << std::endl;
    commands.clear();
    return oss.str();
}

std::string time_from_epoch()
{
    std::ostringstream oss;
    using namespace std::chrono;
    system_clock::time_point tp = system_clock::now();
    system_clock::duration dtn = tp.time_since_epoch();
    auto tt = dtn.count() * system_clock::period::num / system_clock::period::den;
    oss << "bulk" << tt << ".log";
    return oss.str();
}

void write_to_file(std::string file_name, std::string input) {
    if (input.empty()) return;
    std::ofstream out(file_name);
    out << input;
    out.close();
}

int main(int argc, char *argv[]) {
    int N;
    if (argc > 1)
        N = atoi(argv[1]);
    else 
        return -1;
    
    std::vector<std::string> commands;
    std::chrono::milliseconds ms;
    std::string cmd;
    std::string file_name; 
    int brace_count = 0;
    
    std::cin >> cmd;
    while(!std::cin.eof()) {
        if(cmd.compare("{")==0) {
            if(brace_count==0) {
                auto str = get_bulk_str(commands);
                std::cout << str;
                write_to_file(file_name, str);
            }
            brace_count++;
        } else if(cmd.compare("}")==0) {
            brace_count--;
            if(brace_count==0) {
                auto str = get_bulk_str(commands);
                std::cout << str;
                write_to_file(file_name, str);
            }
            if(brace_count<0) {
                std::cout << "Error brace" << std::endl;
                return -1;
            }
        } else {
            commands.push_back(cmd);
            file_name = time_from_epoch();
        }
        if((commands.size()>=N)&&(brace_count==0)) {
            auto str = get_bulk_str(commands);
            std::cout << str;
            write_to_file(file_name, str);
        }
        std::cin >> cmd;
    }
    if(brace_count==0) {
        auto str = get_bulk_str(commands);
        std::cout << str;
        write_to_file(file_name, str);
    }
    return 0;
}