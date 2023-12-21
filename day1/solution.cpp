#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<cctype>
#include<cmath>
#include<unordered_map>
#include<chrono>

const std::unordered_map<std::string, int> digis_map{
    {"eight", 8,},
    {"five", 5},
    {"four", 4},
    {"nine", 9},
    {"one", 1},
    {"seven", 7},
    {"six", 6},
    {"three", 3},
    {"two", 2}
};

const std::unordered_map<char, std::vector<std::string>> first_chars_map{
    {'o', std::vector<std::string>{"one"}},
    {'t', std::vector<std::string>{"two", "three"}},
    {'f', std::vector<std::string>{"four", "five"}},
    {'s', std::vector<std::string>{"six", "seven"}},
    {'e', std::vector<std::string>{"eight"}},
    {'n', std::vector<std::string>{"nine"}},
};

int find_digit(const std::string& search_string, const size_t begin_index) {
    std::string first_chars{"otfsen"};
    if (first_chars.find(search_string.at(begin_index)) == std::string::npos)
        return -1;

    std::vector<std::string> candidates{
        first_chars_map.at(search_string.at(begin_index))};

    for (const auto& candidate : candidates) {
        if (search_string.substr(begin_index, candidate.size()) == candidate)
            return digis_map.at(candidate);
    }
    return -1;
}


int part1(const std::vector<std::string>& input_vector) {
    int sum{0};
    for (const auto line : input_vector) {
        std::vector<int> digits{};
        for (const auto symbol : line) 
            if (std::isdigit(symbol)) digits.push_back(symbol - '0');

        sum += digits.front() * 10 + digits.back();
    }
    return sum;
}

int part2(const std::vector<std::string>& input_vector) {
    int sum{0};
    for (const auto line : input_vector) {

        std::vector<int> digits{};
        for (size_t index{0}; index < line.size(); index++) {
            auto symbol{line.at(index)};
            if (std::isdigit(symbol)) 
                digits.push_back(symbol - '0');

            if (find_digit(line,index) != -1) 
                digits.push_back(find_digit(line,index));
        }
        sum += digits.front() * 10 + digits.back();
    }
    return sum;
}

std::vector<std::string> parse() {
    std::ifstream file{"input"};

    if (!file.is_open()) {
        file.close();
        throw std::runtime_error{"Error oppening file\n"};
    }

    std::string line{};
    std::vector<std::string> input_vector{};

    while(std::getline(file,line)) 
        input_vector.push_back(line); 

    file.close();
    return input_vector;
}


int main() {
    std::vector<std::string> input{parse()};

    auto begin1{std::chrono::high_resolution_clock::now()};
    int result_part1{part1(input)};
    auto end1{std::chrono::high_resolution_clock::now()};
    auto duration1{std::chrono::duration<double, std::milli>(end1 - begin1).count()};
    
    auto begin2{std::chrono::high_resolution_clock::now()};
    int result_part2{part2(input)};
    auto end2{std::chrono::high_resolution_clock::now()};
    auto duration2{std::chrono::duration<double, std::milli>(end2 - begin2).count()};


    std::cout << "Result Part 1: " << result_part1 << "\n";
    std::cout << "Part 1 completed in: " << duration1 << "ms\n";

    std::cout << "Result Part 2: " << result_part2 << "\n";
    std::cout << "Part 2 completed in: " << duration2 << "ms\n";
    return 0;

}