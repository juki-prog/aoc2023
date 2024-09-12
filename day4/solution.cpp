#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <numeric>

struct Card
{
    std::vector<int> winning_numbers;
    std::vector<int> owned_numbers;
    const u_int32_t card_number;

    Card(std::vector<int> in_numbers, std::vector<int> own_numbers, u_int32_t n)
        : winning_numbers{in_numbers}, owned_numbers{own_numbers}, card_number{n}
    {
        std::sort(winning_numbers.begin(), winning_numbers.end());
        std::sort(owned_numbers.begin(), owned_numbers.end());
    };

    int count_matching_numbers() const
    {
        int value{0};
        size_t owned_i{0};
        size_t winning_i{0};
        while (winning_i < winning_numbers.size() && owned_i < owned_numbers.size())
        {
            if (winning_numbers[winning_i] == owned_numbers[owned_i])
            {
                owned_i++;
                winning_i++;
                value++;
            }
            else if (winning_numbers[winning_i] > owned_numbers[owned_i])
            {
                owned_i++;
            }
            else if (winning_numbers[winning_i] < owned_numbers[owned_i])
            {
                winning_i++;
            }
        }
        return value;
    }
};

int pow2(int n)
{
    int result{1};
    for (int i{0}; i < n - 1; i++)
    {
        result *= 2;
    }
    return result;
}

int part1(const std::vector<Card> &input_vector)
{
    int sum{0};
    for (const auto &card : input_vector)
    {
        int matching_numbers{card.count_matching_numbers()};
        if (matching_numbers > 0)
        {
            sum += pow2(matching_numbers);
        }
    }
    return sum;
}

int part2(const std::vector<Card> &input_vector)
{
    std::vector<int> number_of_cards{};
    number_of_cards.resize(input_vector.size());
    std::fill(number_of_cards.begin(), number_of_cards.end(), 1);

    for (const auto &card : input_vector)
    {
        int matching_numbers{card.count_matching_numbers()};
        for (size_t card_index{card.card_number};
             card_index < card.card_number + matching_numbers && card_index < input_vector.size();
             card_index++)
        {
            number_of_cards[card_index] += number_of_cards[card.card_number - 1];
        }
    }

    return std::accumulate(number_of_cards.begin(), number_of_cards.end(), 0);
}

std::vector<Card>
parse()
{
    std::ifstream file{"input"};

    if (!file.is_open())
    {
        file.close();
        throw std::runtime_error{"Error oppening file\n"};
    }

    std::string line{};
    std::vector<Card> input_vector;
    unsigned int n{1};
    while (std::getline(file, line))
    {
        std::vector<int> winning_numbers;
        std::vector<int> owned_numbers;
        std::string temp_string{""};

        bool winning_part{true};
        for (size_t i{line.find(":") + 2}; i < line.size(); i++)
        {
            if (isdigit(line[i]))
            {
                temp_string += line[i];
            }
            else if (line[i] == '|')
            {
                if (i + 1 < line.size())
                    i += 1;
                winning_part = false;
            }
            else if (temp_string != "" && winning_part && line[i] == ' ')
            {
                winning_numbers.push_back(std::stoi(temp_string));
                temp_string = "";
            }
            else if (temp_string != "" && !winning_part && line[i] == ' ')
            {
                owned_numbers.push_back(std::stoi(temp_string));
                temp_string = "";
            }
        }
        if (temp_string != "")
            owned_numbers.push_back(std::stoi(temp_string));
        input_vector.push_back(Card{winning_numbers, owned_numbers, n});
        n++;
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