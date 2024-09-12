#include <cstddef>
#include <iostream> 
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>

struct Map;
typedef std::tuple<std::vector<unsigned int>, std::vector<Map>> Input;

struct Range {
    
    Range(unsigned int src_start,unsigned int dest_start,unsigned int range) 
        : source_start{src_start}, destination_start{dest_start}, range{range}
        {}

    bool is_in_range(unsigned int seed) const {
        if (source_start <= seed and source_start + (range) > seed) 
            return true;
        return false; 
    }

    unsigned int get_mapping(unsigned int seed) const {
        return destination_start + (seed - source_start);
    }

private: 
    unsigned int source_start; 
    unsigned int destination_start; 
    unsigned int range; 
};

struct Map {

    Map(std::vector<Range> range_collection) : range_collection{range_collection} 
    {};

    void add_range(Range range) {
        range_collection.push_back(range);
    }

    [[nodiscard]] unsigned int map(unsigned int seed) const {
        for (const auto& range : range_collection) {
            if (range.is_in_range(seed)) return range.get_mapping(seed);
        }
        return seed;
    }

private: 
    std::vector<Range> range_collection;
};

std::vector<unsigned int> split_to_numbers(const std::string& line)  {
    std::vector<unsigned int> tokens;
    std::stringstream ss{line};
    std::string token;

    char nubmer_delimeter{' '};
    while (std::getline(ss, token, nubmer_delimeter)) {

        const auto token_to_ul{std::stoul(token)};
        unsigned int number{static_cast<unsigned int>(std::stoul(token))};
        if (token_to_ul > std::numeric_limits<unsigned int>::max()) 
            throw std::out_of_range("A number is to big for uint");

        tokens.push_back(number);
    }
    return tokens;
}

unsigned int solve(const std::vector<Map>& maps, unsigned int seed) {
    unsigned int current_value{seed};
    std::cout << "starting seed: " << current_value << std::endl;
    for (const auto& map : maps) {
       current_value = map.map(current_value);
       std::cout << "next mapping: " << current_value << "\n";
    }
    return current_value;
}

unsigned int part1(const Input& input) {
    unsigned int min_so_far{std::numeric_limits<unsigned int>::max()};
    const auto seeds{std::get<0>(input)};
    const auto maps{std::get<1>(input)};

    for (const auto seed : seeds) {
        if (solve(maps,seed) < min_so_far)  min_so_far = solve(maps,seed);
        // std::cout << "min: " <<  min_so_far  << "\n";
    }
    return min_so_far;
}

Input parse() {

    std::ifstream file{"input"};

    if (!file.is_open())
    {
        file.close();
        throw std::runtime_error{"Error oppening file\n"};
    }

    std::string seed_deliminter{":"};

    std::string line{};
    std::vector<Map> maps;
    // Get the first line, as they are the seeds we need to be working with 
    std::getline(file,line);
    std::vector<unsigned int> seeds{
        split_to_numbers(line.substr(line.find(":") + 2, line.length()))};

    // Throw away next blank line
    std::getline(file,line);

    std::vector<Range> ranges;
    while (std::getline(file, line))
    {
        if (line.find(":") != std::string::npos) {
            continue;
        }

        // std::cout << "line len: " << line.length() << " ";
        if (line.empty()) {
            // reset ranges and save map 
            maps.push_back(Map{ranges});
            ranges.clear();
            continue;
        }

        std::cout << line << std::endl;
        // Expensive?
        auto numbers{split_to_numbers(line)};
        // std::cout << "numbers: " << numbers[0] << " " << numbers[1]  << " " << numbers[2] << "\n";
        ranges.push_back(Range{
            numbers.at(1),
            numbers.at(0),
            numbers.at(2), 
        });


    }

    // Last line is not followed by an empty line
    maps.push_back(Map{ranges});
    ranges.clear();

    file.close();
    // I think this copys the whole thing, maybe change this later 
    return std::make_tuple(seeds, maps);
}

int main()
{
    auto input{parse()};
    Range test_range{50, 52, 48};

    //std::cout << test_range.is_in_range(55) << std::endl;
    //std::cout << test_range.get_mapping(55) << std::endl;

    std::cout << "Part 1: " << part1(input);
/* 
    auto begin1{std::chrono::high_resolution_clock::now()};
    int result_part1{part1(input)};
    auto end1{std::chrono::high_resolution_clock::now()};
    auto duration1{
        std::chrono::duration<double, std::milli>(end1 - begin1).count()};

    auto begin2{std::chrono::high_resolution_clock::now()};
    int result_part2{part2(input)};
    auto end2{std::chrono::high_resolution_clock::now()};
    auto duration2{
        std::chrono::duration<double, std::milli>(end2 - begin2).count()};

    std::cout << "Result Part 1: " << result_part1 << "\n";
    std::cout << "Part 1 completed in: " << duration1 << "ms\n";

    std::cout << "Result Part 2: " << result_part2 << "\n";
    std::cout << "Part 2 completed in: " << duration2 << "ms\n"; */
    return 0;
}