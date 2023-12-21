#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<chrono>
#include<fstream>
#include<utility>

struct Game {
    Game(int new_id) : id{new_id} {}

    void add_pair(std::pair<std::string,int> pair) {
        cubes.push_back(pair);
    }

    std::unordered_map<std::string, int> get_max_colours() const {
        std::unordered_map<std::string,int> max_cubes_map{
            {"blue", 0},
            {"red", 0},
            {"green",0},
        };

        for (const auto& pair : cubes) {
            if (max_cubes_map[std::get<std::string>(pair)] < std::get<int>(pair))
                max_cubes_map[std::get<std::string>(pair)] = std::get<int>(pair);
        }
        return max_cubes_map;
    }

    int get_id() const {
        return id;
    }

private:
    const int id{};
    std::vector<std::pair<std::string,int>> cubes{};
};

int part1(const std::vector<Game>& games_vector) {
    int sum{0};
    for (const auto& game : games_vector) {
        auto max_cubes{game.get_max_colours()};
        if (max_cubes["red"] > 12) continue;
        if (max_cubes["green"] > 13) continue;
        if (max_cubes["blue"] > 14) continue;

        // Game is possible 
        sum += game.get_id();
    }
    return sum;
}

int part2(const std::vector<Game>& games_vector) {
    int power{0};
    for (const auto& game : games_vector) {
        auto max_cubes{game.get_max_colours()};
        power += max_cubes["green"] * max_cubes["red"] * max_cubes["blue"];
    }
    return power;
};

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


std::vector<Game> parse() {
    std::ifstream file{"input"};

    if (!file.is_open()) {
        file.close();
        throw std::runtime_error{"Error oppening file\n"};
    }

    std::string line{}; 
    std::vector<Game> games{};
    while(std::getline(file,line)) { 
        auto split_line{split(line," ")};

        // Extract Game ID 
        std::string id{split_line.at(1)};
        id = id.substr(0,id.find(":"));

        Game new_game{std::stoi(id)};

        int cube_count{};
        std::string colour{};
        for (size_t index{2}; index < split_line.size(); index = index + 2) {
            cube_count = std::stoi(split_line.at(index));

            // Feels Hacky maybe change later 
            // removes the "," and ";" but doesnt remove anything if index + 1
            // is the end of the line 
            short remove_comma{1};
            if (index + 1 == split_line.size()-1) remove_comma = 0;
            colour = (split_line.at(index+1)).
                substr(0, split_line.at(index+1).size() - remove_comma);
            new_game.add_pair(std::make_pair(colour,cube_count));
        }
        games.push_back(new_game);
    }
    file.close();

    return games;
}


int main() {
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