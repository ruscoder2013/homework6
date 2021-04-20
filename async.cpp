#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <algorithm>
#include <thread>
#include <condition_variable>

#include "threadsafe_queue.h"
#include "async.h"

struct file_record {
    std::string file_name;
    std::string bulk;
};

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
    return oss.str();
}

std::string time_from_epoch()
{
    std::ostringstream oss;
    using namespace std::chrono;
    system_clock::time_point tp = system_clock::now();
    system_clock::duration dtn = tp.time_since_epoch();
    auto tt = dtn.count() * system_clock::period::num / system_clock::period::den;
    oss << tt;
    return oss.str();
}

std::string name_file() {
    static std::string prev_name = "";
    std::string time_str = time_from_epoch();
    std::string name = "bulk" + time_str;

    std::size_t found = prev_name.find(name);
    if (found!=std::string::npos) {
        std::string sub = prev_name.substr(found+name.size());
        if(sub.empty()) {
            name += "2";
        }
        else {
            int n = atoi(sub.c_str());
            n++;
            name += std::to_string(n);
        }
    }
    prev_name = name;
    return name+".log";
}

namespace async {
    struct Handle {
        std::thread *log_thread;
        std::thread *file1_thread;
        std::thread *file2_thread;
        std::condition_variable cv;
        std::mutex mut;

        std::condition_variable cv_file;
        std::mutex mut_file;

        threadsafe_queue<std::string> messages;
        threadsafe_queue<file_record> messages_2;
        std::vector<std::string> commands;
        std::chrono::milliseconds ms;
        std::string string_buffer;
        std::string cmd;
        std::string file_name; 
        int brace_count = 0;
        int handle = 1;
        bool finish = false;
        bool ready_flag = false;
        int N;
        std::ofstream out;
        ~Handle() {
            finish = true;
            cv.notify_one();
            cv_file.notify_all();
            log_thread->join();
            file1_thread->join();
            file2_thread->join();
            if(log_thread!=nullptr)
                delete log_thread;
            if(file1_thread!=nullptr)
                delete file1_thread;
            if(file2_thread!=nullptr)
                delete file2_thread;
        }
    };
    using handle_t = void*;
    std::vector<Handle*> handles;
    
    void write_to_cout2(Handle &handle) {
        std::unique_lock<std::mutex> lk(handle.mut);
        while(!handle.finish) {
            handle.cv.wait(lk, [&handle](){ 
                return !handle.messages.empty() || handle.finish; 
                });
            while(!handle.messages.empty())
            {
                std::string str;
                auto success = handle.messages.try_pop(str);
                std::cout << str;
            }
            if(handle.finish)
                break; 
        }
    }
    void write_to_file_1(Handle &handle, bool chet) {
        std::unique_lock<std::mutex> lk(handle.mut_file);
        while(!handle.finish) {
            handle.cv_file.wait(lk, [&handle](){ 
                return !handle.messages_2.empty() || handle.finish; 
            });
            while(!handle.messages_2.empty()) {
                file_record record;
                auto success = handle.messages_2.try_pop(record);
                if(success)
                {
                    std::ofstream out;
                    out.open(record.file_name);
                    out << record.bulk;
                    out.close();
                }
            }
            if(handle.finish)
                break; 
        }
    }
    handle_t connect(std::size_t bulk) {
        Handle* handle = new Handle();
        //handle->log_thread = new std::thread(write_to_cout, std::ref(handle->messages), std::ref(handle->finish));
        handle->N = bulk;
        handle->log_thread = new std::thread(async::write_to_cout2, std::ref(*handle));
        
        handle->file1_thread = new std::thread(write_to_file_1, std::ref(*handle), false);
        handle->file2_thread = new std::thread(write_to_file_1, std::ref(*handle), true);
        handles.push_back(handle);

        return reinterpret_cast<handle_t>(handle);
    }

    
    void write_to_file(Handle* handel, std::string bulk) {
        file_record record;
        record.file_name = handel->file_name;
        record.bulk = bulk;

        handel->messages_2.push(record);
        handel->cv_file.notify_one();
    }

    void try_to_show(Handle* handel) {
        if(handel->brace_count==0) {
            auto str = get_bulk_str(handel->commands);
            handel->messages.push(str);
            handel->cv.notify_one();
            write_to_file(handel, str);
            handel->commands.clear();
        }
        
    }

    void receive(handle_t handler_, const char* data, std::size_t size) {
        std::string s(data);
        Handle* handler = reinterpret_cast<Handle*>(handler_);
        if(handler->string_buffer.size()!=0)
        {
            s = handler->string_buffer + s;
            handler->string_buffer.clear();
        }
        std::stringstream ss(s);
        std::string cmd;
        std::vector<std::string> elems;
        while (std::getline(ss, cmd)) {
            if(ss.eof()) continue;
            //std::cout << cmd << std::endl;
            if(cmd.compare("{")==0) {
                try_to_show(handler);
                handler->brace_count++;
            } else if(cmd.compare("}")==0) {
                handler->brace_count--;
                try_to_show(handler);
                if(handler->brace_count<0) {
                    handler->brace_count++;
                    continue;
                }
            } else {
                if (handler->commands.size()==0) 
                    handler->file_name = name_file();
                handler->commands.push_back(cmd);
                if(handler->commands.size()>=handler->N)
                    try_to_show(handler);
            }
        }
        if(data[size-1]!='\n')
            handler->string_buffer = cmd;
        //try_to_show(handler);
    }
    void disconnect(handle_t handle_) {
        Handle* handle = reinterpret_cast<Handle*>(handle_);
        std::vector<Handle*>::iterator missing = std::find(handles.begin(), handles.end(), handle);
        if(missing!=handles.end())
        {
            try_to_show(handle);
            handles.erase(missing);
            
            delete handle;
        }
    }
}