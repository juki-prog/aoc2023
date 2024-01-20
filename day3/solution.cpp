#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

int check_for_number(const std::string &line, size_t search_index)
{
    if (!isdigit(line.at(search_index)))
        return 0;

    // Search for beginning
    size_t start_index{search_index};
    while (start_index > 0 && isdigit(line.at(start_index)))
        start_index--;

    // Correct overstepping
    if (!isdigit(line.at(start_index)))
        start_index++;

    std::string number{""};
    while (start_index < line.size() && isdigit(line.at(start_index)))
    {
        number += line.at(start_index);
        start_index++;
    }
    return std::stoi(number);
}

std::vector<int> check_adjacent_symbols(const std::vector<std::string> &input_vector,
                                        size_t symbol_index, size_t line_index)
{

    const auto &line{input_vector[line_index]};
    std::vector<int> values;
    const bool line_above_exists{line_index > 0};
    const bool line_below_exists{line_index < input_vector.size()};
    const bool left_symbol_exists{symbol_index > 0};
    const bool right_symbol_exists{symbol_index + 1 < line.size()};

    if (left_symbol_exists && isdigit(line.at(symbol_index - 1)))
    {
        values.push_back(check_for_number(line, symbol_index - 1));
    }

    if (right_symbol_exists && isdigit(line.at(symbol_index + 1)))
    {
        values.push_back(check_for_number(line, symbol_index + 1));
    }

    // Check for number above and below symbol
    bool no_number_above{true};
    bool no_number_below{true};
    if (line_above_exists && isdigit((input_vector.at(line_index - 1)).at(symbol_index)))
    {
        const auto &line_above{input_vector.at(line_index - 1)};
        values.push_back(check_for_number(line_above, symbol_index));
        no_number_above = false;
    }

    if (line_below_exists && isdigit((input_vector.at(line_index + 1).at(symbol_index))))
    {
        const auto &line_below{input_vector.at(line_index + 1)};
        values.push_back(check_for_number(line_below, symbol_index));
        no_number_below = false;
    }

    // Check diagonally left above the symbol
    if (no_number_above &&
        line_above_exists &&
        left_symbol_exists &&
        isdigit((input_vector.at(line_index - 1)).at(symbol_index - 1)))
    {
        const auto &line_above{input_vector.at(line_index - 1)};
        values.push_back(check_for_number(line_above, symbol_index - 1));
    }

    // Check diagonally left below the symbol
    if (no_number_below &&
        line_below_exists &&
        left_symbol_exists &&
        isdigit((input_vector.at(line_index + 1)).at(symbol_index - 1)))
    {
        const auto &line_below{input_vector.at(line_index + 1)};
        values.push_back(check_for_number(line_below, symbol_index - 1));
    }

    // Check diagonally right above
    if (no_number_above &&
        line_above_exists &&
        right_symbol_exists &&
        isdigit((input_vector.at(line_index - 1)).at(symbol_index + 1)))
    {
        const auto &line_above{input_vector.at(line_index - 1)};
        values.push_back(check_for_number(line_above, symbol_index + 1));
    }

    // Check diagonally right below
    if (no_number_below &&
        line_below_exists &&
        right_symbol_exists &&
        isdigit((input_vector.at(line_index + 1)).at(symbol_index + 1)))
    {
        const auto &line_below{input_vector.at(line_index + 1)};
        values.push_back(check_for_number(line_below, symbol_index + 1));
    }

    return values;
}

int part1(const std::vector<std::string> &input_vector)
{
    int sum{0};
    for (size_t line_index{0}; line_index < input_vector.size(); line_index++)
    {
        const auto &line{input_vector.at(line_index)};
        for (size_t symbol_index{0}; symbol_index < line.size();
             symbol_index++)
        {
            // Only continue if we find a symbol other than digit or .
            const auto &symbol{line.at(symbol_index)};
            if (symbol == '.' || isdigit(symbol))
                continue;

            const auto &values{check_adjacent_symbols(
                input_vector, symbol_index, line_index)};
            sum += std::accumulate(values.begin(), values.end(), 0);
        }
    }
    return sum;
}

int part2(const std::vector<std::string> &input_vector)
{
    int sum{0};
    for (size_t line_index{0}; line_index < input_vector.size(); line_index++)
    {
        const auto &line{input_vector[line_index]};
        for (size_t symbol_index{0}; symbol_index < line.size(); symbol_index++)
        {
            if (line[symbol_index] != '*')
                continue;

            const auto &values{check_adjacent_symbols(input_vector, symbol_index, line_index)};
            if (values.size() == 2)
                sum += values[0] * values[1];
        }
    }
    return sum;
}

std::vector<std::string>
parse()
{
    std::ifstream file{"input"};

    if (!file.is_open())
    {
        file.close();
        throw std::runtime_error{"Error oppening file\n"};
    }

    std::string line{};
    std::vector<std::string> input_vector;

    while (std::getline(file, line))
    {
        input_vector.push_back(line);
    }
    file.close();
    return input_vector;
}

int main()
{
    auto input{parse()};

    auto begin1{std::chrono::high_resolution_clock::now()};
    int result_part1{part1(input)};
    auto end1{std::chrono::high_resolution_clock::now()};
    auto duration1{
        std::chrono::duration<double, std::milli>(end1 - begin1).count()};

    auto begin2{std::chrono::high_resolution_clock::now()};
    int result_part2{part2(input)};
    auto end2{std::chrono::high_resolution_clock::now()};
    auto duration2{std::chrono::duration<double, std::milli>(end2 -
                                                             begin2)
                       .count()};

    std::cout << "Result Part 1: " << result_part1 << "\n";
    std::cout << "Part 1 completed in: " << duration1 << "ms\n";

    std::cout << "Result Part 2: " << result_part2 << "\n";
    std::cout << "Part 2 completed in: " << duration2 << "ms\n";
    return 0;
}