#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string>
#include <sys/stat.h>
#include <sys/un.h>
#include <unordered_map>

using namespace std;

struct lis_input
{
    int starting_block;
    int number_of_blocks;
    int request_number;
};

int NUMBER_OF_PAGES = 0;
int PAGE_SIZE = 512; // This is in Bytes

inline std::string &rtrim(std::string &s)
{
    const char *t = " \t\n\r\f\v";
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string &ltrim(std::string &s)
{
    const char *t = " \t\n\r\f\v";
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string &trim(std::string &s)
{
    const char *t = " \t\n\r\f\v";
    return ltrim(rtrim(s));
}

vector<string> split_string(string line, char delimiter)
{
    cout << "REACHED HERE :) "<< line.length() << endl;
    istringstream tokenStream(trim(line));
    string intermediate;
    vector<string> tokens;
    int counter = 0;
    while (getline(tokenStream, intermediate, delimiter))
    {
        tokens.push_back(intermediate);
        counter+=intermediate.length();
        cout << counter++ << endl;
    }
    return tokens;
}

vector<lis_input> retrive_cache_info(string file_path)
{
    vector<lis_input> cache_blocks; // For Reduce file creation.
    ifstream file;
    string line;
    file.open(file_path.c_str());
    if (file.is_open())
    {
        while (getline(file, line))
        {
            cout << line << endl;
            vector<string> param_splits = split_string(line, ' ');
            lis_input cache_params;
            // cout << "REACHED HERE :)" << endl;
            // istringstream check1((line));
            // string intermediate;
            // cout << "REACHED HERE :) 2" << endl;
            // while (!check1.eof())
            // {
            //     cout << (check1.eof()) << endl;
            //     getline(check1, intermediate, ' ');
            //     cout << "REACHED HERE :) 3 "<< intermediate << endl;
            //     param_splits.push_back(intermediate);
            // }
            cache_params.starting_block = stoi(param_splits[0]);
            cache_params.number_of_blocks = stoi(param_splits[1]);
            cache_params.request_number = stoi(param_splits[4]);
            cache_blocks.push_back(cache_params);
        }
    }
    else
    {
        cout << "File Didn't Open In Map" << endl;
    }

    return cache_blocks;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Arguements Not Supported" << endl;
        return 0;
    }

    NUMBER_OF_PAGES = stoi(argv[1]);

    string trace_file_name = argv[2];
    trace_file_name += ".lis";
    vector<lis_input> cache_blocks = retrive_cache_info(trace_file_name);
    cout << cache_blocks.at(1).request_number << endl;
}