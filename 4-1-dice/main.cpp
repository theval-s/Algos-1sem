#include "dice.hpp"
#include <iostream>
#include <fstream>
#include <ranges>

#include <gtest/gtest.h>

TEST(StringParsing, BasicTest)
{
    std::string input = "2d8+4,5d6,1d4+1";
    std::vector<DiceInfo> expected {
        {2, 8, 4},
        {5, 6, 0},
        {1, 4, 1}
    };
    EXPECT_EQ(DiceRandom::parseDiceString(input), expected);
}

TEST(StringParsing, EmptyCount)
{
    const std::string input = " d20 , d8+4, d3-1";
    std::vector<DiceInfo> expected {
        {1,20,0},
        {1, 8, 4},
        {1,3,-1}
    };
    EXPECT_EQ(DiceRandom::parseDiceString(input), expected);
}

TEST(StringParsing, EmptyString)
{
    const std::string input = "";
    std::vector<DiceInfo> expected {
    };
    EXPECT_EQ(DiceRandom::parseDiceString(input), expected);
}

void outputDiceProbabilityToCsv(std::string_view input, int rollTimes = 1000)
{
    std::map<int, int> diceRollsMap = DiceRandom::getDiceGroupRollCount(input, rollTimes);
    int count_sum = std::accumulate(std::views::values(diceRollsMap).begin(),
                                    std::views::values(diceRollsMap).end(),
                                    0);

    std::ofstream outputFile{std::string(input) + ".csv"};
    outputFile << "Value,Probability\n";
    for (auto &[value, count] : diceRollsMap)
    {
        outputFile << value << "," << static_cast<float>(count)/static_cast<float>(count_sum) << "\n";
    }
    outputFile.close();
}

int main()
{
    outputDiceProbabilityToCsv("1d6");
    outputDiceProbabilityToCsv("2d6");
    outputDiceProbabilityToCsv("3d6");
    outputDiceProbabilityToCsv("1d10");
    outputDiceProbabilityToCsv("2d10");
    outputDiceProbabilityToCsv("3d10");

    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
