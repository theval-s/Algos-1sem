#include "dice.hpp"

#include <string_view>
#include <ranges>
#include <algorithm>
#include <iostream>

namespace DiceRandom
{
    DiceInfo parseDiceGroupToken(std::string_view token)
    {
        DiceInfo diceGroup;

        auto dPos = token.find('d');
        if (dPos == std::string_view::npos)
        {
            throw std::invalid_argument("expected \'d\' in dice token");
        }

        //temp strings for stoi but it looks cleaner
        try
        {
            diceGroup.count = std::stoi(std::string(token.substr(0, dPos)));
        }
        catch (std::invalid_argument &e)
        {
            diceGroup.count = 1;
            //empty values are treated as 1 for "d6+x",
            // std::cerr << "count <= 0. error in token?" << std::endl;
            // return std::nullopt;
        }

        auto signPos = token.find_first_of("-+");
        if (signPos == std::string_view::npos)
        {
            //no mod - thats ok and possible
            diceGroup.sides = std::stoi(std::string(token.substr(dPos + 1)));
        }

        else if (signPos < dPos)
        {
            throw std::invalid_argument("mod sign before d, wrong format");
        }

        else
        {
            diceGroup.sides = std::stoi(std::string(token.substr(dPos + 1, signPos - dPos)));
            diceGroup.mod = std::stoi(std::string(token.substr(signPos)));
        }

        if (diceGroup.sides <= 0)
        {
            std::cerr << "sides <= 0. error in token? assuming 6" << std::endl;
            diceGroup.sides = 6;
        }
        return diceGroup;
    }

    std::vector<DiceInfo> parseDiceString(std::string_view str)
    {
        try
        {
            return str
                | std::views::filter([](auto ch) { return ch != ' '; })
                | std::views::split(',')
                | std::views::transform([](auto&& r)
                {
                    return std::string_view(&*r.begin(), std::ranges::distance(r));
                })
                |std::views::transform(parseDiceGroupToken)
                |std::ranges::to<std::vector<DiceInfo>>();

        } catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return {};
        }
    }

    int rollDice(const DiceInfo& dice)
    {
        return RandomGenerator::getRandomDiceRollSum(dice.sides, dice.count) + dice.mod;
    }

    std::vector<int> rollDiceGroups(const std::vector<DiceInfo>& diceGroups)
    {
        std::vector<int> result;
        result.reserve(diceGroups.size());
        for (auto g : diceGroups)
        {
            result.push_back(rollDice(g));
        }
        return result;
    }

    std::vector<int> rollDiceGroups(std::string_view input)
    {
        return rollDiceGroups(parseDiceString(input));
    }

    std::map<int, int> getDiceGroupRollCount(const std::vector<DiceInfo>& diceGroups, int times)
    {
        std::map<int, int> result{};
        for (int i = 0; i < times; i++)
        {
            for (auto g : diceGroups)
            {
                result[rollDice(g)] += 1;
            }
        }
        return result;
    }

    std::map<int, int> getDiceGroupRollCount(std::string_view input, int times)
    {
        return getDiceGroupRollCount(parseDiceString(input), times);
    }
} //namespace