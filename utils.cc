#ifndef UTILS_H
#define UTILS_H

#include<vector>
#include<algorithm>
#include<regex>

void split(std::string inputs,  std::vector<std::string> &res, std::string sep = " "){

    size_t cur = 0, prev = 0;
    cur = inputs.find_first_of(sep);
    while(cur != std::string::npos){
        res.push_back(inputs.substr(prev, cur-prev));
        prev = cur+1;
        cur = inputs.find_first_of(sep, prev);
    }

    res.push_back(inputs.substr(prev, cur-prev));
}

void split(std::string inputs,  std::vector<std::string> &res, std::regex sep = std::regex("[\\s+]")){

    std::sregex_token_iterator iter(inputs.begin(), inputs.end(), sep, -1), end;
    while(iter != end){
        res.push_back(*iter++);
    }
    return ;
}
std::string to_upper(const std::string &word){
    std::string upper_case(word);
    std::transform(upper_case.begin(), upper_case.end(), upper_case.begin(),
    [](unsigned char c){return std::toupper(c);});
    return upper_case;
}

#endif
