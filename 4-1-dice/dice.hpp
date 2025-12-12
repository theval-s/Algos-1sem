
#include <random>

struct DiceInfo
{
    int count = 1;
    int sides = 6;
    int mod = 0;
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

    static float getRandomDieRoll(int sides)
    {
        std::uniform_int_distribution<> dist_i(1, sides);
        return dist_i(getGenerator());
    }
    static float getRandomDiceRollSum(int sides, int count)
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

int rollDice(const DiceInfo& dice)
{
    return RandomGenerator::getRandomDiceRollSum(dice.sides, dice.count) + dice.mod;
}