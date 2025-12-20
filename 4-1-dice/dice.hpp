#pragma once

#include <map>
#include <random>

struct DiceInfo
{
    int count = 1;
    int sides = 6;
    int mod = 0;

    bool operator==(const DiceInfo& other) const = default;
};

class RandomGenerator
{
public:
    static std::mt19937& getGenerator()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }

    static int getRandomDieRoll(int sides)
    {
        std::uniform_int_distribution<> dist_i(1, sides);
        return dist_i(getGenerator());
    }
    static std::vector<int> getRandomDiceRolls(int sides, int count)
    {
        std::vector<int> results;
        std::uniform_int_distribution<> dist_i(1, sides);
        results.reserve(count);
        for (int i = 0; i < count; i++) results.push_back(dist_i(getGenerator()));
        return results;
    }
    static int getRandomDiceRollSum(int sides, int count)
    {
        int acc = 0;
        std::uniform_int_distribution<> dist_i(1, sides);
        for (int i = 0; i < count; i++)
        {
            acc += dist_i(getGenerator());
        }
        return acc;
    }
};

namespace DiceRandom
{
    DiceInfo parseDiceGroupToken(std::string_view token);
    std::vector<DiceInfo> parseDiceString(std::string_view str);

    int rollDice(const DiceInfo& dice);

    std::vector<int> rollDiceGroups(const std::vector<DiceInfo>& diceGroups);
    std::vector<int> rollDiceGroups(std::string_view input);

    std::map<int,int> getDiceGroupRollCount(const std::vector<DiceInfo>& diceGroups, int times = 1);
    std::map<int,int> getDiceGroupRollCount(std::string_view input, int times = 1);
}

