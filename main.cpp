#include <iostream>
#include <string>
#include <vector>
#include <chrono>

void print_bulk(std::vector<std::string>& commands, int& count)
{
    if (commands.size()==0) return;
    std::cout << "bulk: ";
    for(int i = 0; i < commands.size(); i++)
    {
        if (i>0)
            std::cout << ", ";
        std::cout << commands[i];
    }
    std::cout << std::endl;
    commands.clear();
    count = 0;
}

int main() {
    int N;
    std::cout << "N = ";
    std::cin >> N;
    std::vector<std::string> commands;
    
    std::string cmd; 
    std::cin >> cmd;
    commands.push_back(cmd);
    int count = 1;
    std::chrono::milliseconds ms;
    while(cmd.compare("end")!=0) {
        if (count == N) {
            print_bulk(commands, count);
        }
        std::cin >> cmd;
        if(cmd.compare("{")==0)
        {
            print_bulk(commands, count); 
            int brace_count = 1;
            do {
                std::cin >> cmd;
                if(cmd.compare("{")==0)
                {
                    brace_count++;
                }    
                else if(cmd.compare("}")==0) {
                    brace_count--;
                } 
                else {
                    commands.push_back(cmd);
                }
            } while(brace_count!=0);
            print_bulk(commands, count);
        }
        else if (cmd.compare("}")==0){
            std::cout << "Error brace" << std::endl;
            return -1;
        } else {
            commands.push_back(cmd);
            /*std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            char buff [80]; 
            strftime(buff, 20, "%d/%m/%Y:%H:%M:%S", localtime(&t));
            string s(buff);
            std::tm now_tm = *std::localtime(&now_c);*/
            count++;
        }
    }
    return 0;
}