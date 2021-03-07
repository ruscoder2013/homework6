#include <iostream>
#include <string>
#include <vector>

int main() {
    int N;
    std::cout << "N = ";
    std::cin >> N;
    std::vector<std::string> commands;
    
    std::string cmd; 
    std::cin >> cmd;
    commands.push_back(cmd);
    int count = 1;
    while(cmd.compare("end")!=0) {
        if (count == N) {
            std::cout << "bulk: ";
            for(int i = 0; i < N; i++)
            {
                if (i>0)
                    std::cout << ", ";
                std::cout << commands[i];
            }
            std::cout << std::endl;
            commands.clear();
            count = 0;
        }
        std::cin >> cmd;
        if(cmd.compare("{")==0)
        {
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
            std::cin >> cmd;
            while(cmd.compare("}")!=0) {
                commands.push_back(cmd);
                std::cin >> cmd;
            }
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
        commands.push_back(cmd);
        count++;
    }
    return 0;
}