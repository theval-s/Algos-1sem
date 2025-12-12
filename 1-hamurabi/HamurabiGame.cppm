//
// Created by Volkov Sergey on 14/11/2025.
//

module;

#include <algorithm>
#include <random>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <print>


//initially i tried to make it a single file module to test out how cpp modules work
//but Apple Clang does not support modules at all and I found it troublesome
//but technically this should work as a module
export module HamurabiGame;
export class HamurabiGame
{
public:
    HamurabiGame(const std::filesystem::path& fileName = "hamurabi.sav")
        : m_saveFilePath(fileName)
    {

    }

    void StartGame();
    void ProgressRound();

private:
    struct RoundInputValues
    {
        int acresDelta = 0;
        int bushelsFed = 0;
        int acresPlanted = 0;
    };

    struct RoundProgressValues
    {
        int peopleStarved = 0;
        int peopleArrived = 0;
        int peopleDiedFromPlague = 0;
        int wheatHarvested = 0;
        int wheatEatenByRats = 0;
        int wheatPerAcre = 0;
    };

    void ReadSaveGame();
    void SaveGame();
    void HandleRoundInput();
    void PrintRoundOutput(const RoundProgressValues&) const;
    void GameOver(int peopleStarved);
    void Win();
    int InputBuyAcres();
    int InputSellAcres();
    int InputFeed();
    int InputPlant();

    int m_population = 100;
    int m_wheat = 2800;
    int m_territory = 1000;

    RoundInputValues m_currentRoundInput{};

    int m_roundNumber = 0;
    int m_currentAcrePrice = 26;
    int m_cumulativeStarvedPercent = 0;

    std::filesystem::path m_saveFilePath;
};

#pragma region RNG
class RandomGenerator
{
public:
    static std::mt19937& getGenerator()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }

    static float getRandomValue()
    {
        static std::uniform_real_distribution<float> dist_f(0, 1);
        return dist_f(getGenerator());
    }

    static int getRandomAcrePrice()
    {
        static std::uniform_int_distribution dist_i(17, 26);
        return dist_i(getGenerator());
    }

    static int getRandomEatenWheat(int wheat)
    {
        std::uniform_int_distribution<> dist_f(0, static_cast<int>(wheat * 0.07f));
        return dist_f(getGenerator());
    }

    static bool getPlagueHappened()
    {
        static std::uniform_int_distribution dist_percent(1, 100); //1-20 and <3 for simplified ver
        return (dist_percent(getGenerator()) <= 15);
    }

    static int getWheatPerAcreValue()
    {
        static std::uniform_int_distribution dist_i(1, 6);
        return dist_i(getGenerator());
    }
};
#pragma endregion RNG


// HamurabiGame

void HamurabiGame::ReadSaveGame()
{
    if (!exists(m_saveFilePath))
    {
        std::cerr << std::format("Tried to read non-existent save file {}!", m_saveFilePath.string());
        return;
    }
    std::ifstream inFile(m_saveFilePath, std::ios::binary);
    std::cout << "Is file open? " << inFile.is_open() << std::endl;
    if (inFile.is_open())
    {
        inFile.read(reinterpret_cast<char*>(&m_roundNumber), sizeof(int))
                .read(reinterpret_cast<char*>(&m_wheat), sizeof(int))
                .read(reinterpret_cast<char*>(&m_population), sizeof(int))
                .read(reinterpret_cast<char*>(&m_territory), sizeof(int))
                .read(reinterpret_cast<char*>(&m_cumulativeStarvedPercent), sizeof(int))
                .read(reinterpret_cast<char*>(&m_currentRoundInput.bushelsFed), sizeof(int))
                .read(reinterpret_cast<char*>(&m_currentRoundInput.acresPlanted), sizeof(int));
    }
    // std::cerr << "Debug output after reading save. Read: "
    // << m_roundNumber << std::endl
    // << " Wheat: " << m_wheat << std::endl
    // << " Population: " << m_population << std::endl
    // << " Territory: " << m_territory << std::endl
    // << " Cumulative Starved: " << m_cumulativeStarvedPercent << std::endl
    // << " CurrentRoundInput: " << m_currentRoundInput.bushelsFed << std::endl
    // << " CurrentRoundInput: " << m_currentRoundInput.acresPlanted << std::endl;
}

void HamurabiGame::SaveGame()
{
    //What is needed to restore state:
    //round number, population, wheat, territory
    //cumulative starved people percent
    //feed and plant value from prev round input (for calcs)

    //if saved during the round itself current random generated values should be saved I guess
    if (exists(m_saveFilePath))
    {
        std::print("Save game file already exists. Rewrite \"{}\"?\ny/n:",
            m_saveFilePath.string());
        char ans;
        std::cin >> ans;
        if (ans != static_cast<int>('y'))
        {
            return;
        }
    }
    std::ofstream outFile(m_saveFilePath, std::ios::binary);
    if (outFile.is_open())
    {
        outFile.write(reinterpret_cast<char*>(&m_roundNumber), sizeof(int))
                .write(reinterpret_cast<char*>(&m_wheat), sizeof(int))
                .write(reinterpret_cast<char*>(&m_population), sizeof(int))
                .write(reinterpret_cast<char*>(&m_territory), sizeof(int))
                .write(reinterpret_cast<char*>(&m_cumulativeStarvedPercent), sizeof(int))
                .write(reinterpret_cast<char*>(&m_currentRoundInput.bushelsFed), sizeof(int))
                .write(reinterpret_cast<char*>(&m_currentRoundInput.acresPlanted), sizeof(int));
                // <<m_wheat
                // <<m_population
                // <<m_territory
                // <<m_cumulativeStarvedPercent
                // <<m_currentRoundInput.bushelsFed
                // <<m_currentRoundInput.acresPlanted;
    }

}

void HamurabiGame::HandleRoundInput()
{
    //Buy -> Eat -> Plant
    //We need to update values during input because input depends on values
    m_currentRoundInput.acresDelta = InputBuyAcres();
    if (m_currentRoundInput.acresDelta == 0)
    {
        m_currentRoundInput.acresDelta = -InputSellAcres();
    }
    m_wheat -= m_currentRoundInput.acresDelta * m_currentAcrePrice;
    m_territory += m_currentRoundInput.acresDelta;
    //Eat
    m_currentRoundInput.bushelsFed = InputFeed();
    m_wheat -= m_currentRoundInput.bushelsFed;

    //Plant
    m_currentRoundInput.acresPlanted = InputPlant();
    m_wheat -= (m_currentRoundInput.acresPlanted + 1) / 2;
}

void HamurabiGame::PrintRoundOutput(const RoundProgressValues& roundValues) const
{
    std::print("My lord! I beg to report to you,\n"
               "In year {} of your reign,\n{} people starved, {} came to the city.\n",
               m_roundNumber + 1, roundValues.peopleStarved, roundValues.peopleArrived);
    if (roundValues.peopleDiedFromPlague > 0) std::print("Plague killed half of our population.\n");
    std::print("Current population is {}, city has {} acres.\nWe harvested {} bushels of wheat\n"
               "Rats ate {} bushels\nYou now have {} bushels in store.\n",
               m_population, m_territory, roundValues.wheatHarvested, roundValues.wheatEatenByRats, m_wheat);
    std::print("Land is trading at {} bushels per acre.\n", m_currentAcrePrice);
    std::cout << std::endl;
}

void HamurabiGame::GameOver(int peopleStarved)
{
    std::print("You starved {} people in one year!\n"
               "Due to this extreme mismanagement you have not only been impeached\n"
               "and thrown out of office but you have also been declared national fink!!!!\n\n",
                peopleStarved);
    //Any other ways to lose?
}

void HamurabiGame::Win()
{
    int L = m_territory/m_population;
    if (m_cumulativeStarvedPercent/10>33 && L < 7)
    {
        std::cout << "Due to your incompetence, people have risen up and cast you out."
                     "Now you have to reign your pathetic existence in exile.\n";
    } else if (m_cumulativeStarvedPercent/10>10 && L < 9)
    {
        std::cout << "You reigned with an iron hand, like Neron or Ivan the Terrible."
                     "People sighed in relief, and nobody wants to see you as a ruler anymore.\n";
    } else if (m_cumulativeStarvedPercent/10>3 && L < 10)
    {
        std::cout << "You did pretty fine, of course you have some ill-wishers,"
                     "but most people would like to see you as a ruler again.\n";
    } else
    {
        std::cout << "Phenomenal! Charlemagne, Disraeli and Jefferson all together could not have done better!\n";
    }
}

int HamurabiGame::InputBuyAcres()
{
    while (true)
    {
        std::string buf;
        std::cout << "How many acres do you wish to buy? :";
        std::cin >> buf;
        if (std::cin.good() && !buf.empty())
        {
            try
            {
                int buyValue = std::stoi(buf);
                if (buyValue * m_currentAcrePrice > m_wheat)
                {
                    std::cout << "Lord, we only have " << m_wheat << " bushels in store!" << std::endl;
                }
                else
                {
                    return buyValue;
                }
            }
            catch (const std::exception& e)
            {
                std::cout << "I can't understand you, Lord! (" << e.what() << ")" << std::endl;
            }
        }
    }
}

int HamurabiGame::InputSellAcres()
{
    while (true)
    {
        std::string buf;
        std::cout << "How many acres do you wish to sell? :";
        std::cin >> buf;
        if (std::cin.good() && !buf.empty())
        {
            try
            {
                int buyValue = std::stoi(buf);
                if (buyValue > m_territory)
                {
                    std::cout << "Lord, we only have " << m_territory << " acres!" << std::endl;
                }
                else
                {
                    return buyValue;
                }
            }
            catch (const std::exception& e)
            {
                std::cout << "I can't understand you, Lord! (" << e.what() << ")" << std::endl;
            }
        }
    }
}

int HamurabiGame::InputFeed()
{
    while (true)
    {
        std::string buf;
        std::cout << "How many bushels do you wish to feed to your people? :";
        std::cin >> buf;
        if (std::cin.good() && !buf.empty())
        {
            try
            {
                int feedValue = std::stoi(buf);
                if (feedValue > m_wheat)
                {
                    std::cout << "Lord, we only have " << m_wheat << " bushels in store!" << std::endl;
                }
                else
                {
                    return feedValue;
                }
            }
            catch (const std::exception& e)
            {
                std::cout << "I can't understand you, Lord! (" << e.what() << ")" << std::endl;
            }
        }
    }
}

int HamurabiGame::InputPlant()
{
    while (true)
    {
        std::string buf;
        std::cout << "How many acres do you wish to plant with seed? :";
        std::cin >> buf;
        if (std::cin.good() && !buf.empty())
        {
            try
            {
                int plantValue = std::stoi(buf);
                if (plantValue > m_wheat * 2 || plantValue > m_population * 10)
                {
                    std::cout << "Lord, we only have " << m_wheat << " bushels in store and "
                        << m_population << " people!" << std::endl;
                }
                else
                {
                    return plantValue;
                }
            }
            catch (const std::exception& e)
            {
                std::cout << "I can't understand you, Lord! (" << e.what() << ")" << std::endl;
            }
        }
    }
}

void HamurabiGame::ProgressRound()
{
    RoundProgressValues vals;

    vals.wheatPerAcre = RandomGenerator::getWheatPerAcreValue();
    m_currentAcrePrice = RandomGenerator::getRandomAcrePrice();

    if (m_roundNumber == 0)
    {
        vals.peopleArrived = 5;
        vals.wheatHarvested = 400;
    }
    else
    {
        //Determine plague:
        if (RandomGenerator::getPlagueHappened()) vals.peopleDiedFromPlague = m_population / 2;

        //Starved:
        vals.peopleStarved = m_population - m_currentRoundInput.bushelsFed / 20;
        if (vals.peopleStarved > static_cast<int>(m_population * 0.45f))
        {
            GameOver(vals.peopleStarved);
            return;
        }
        if (vals.peopleStarved < 0) vals.peopleStarved = 0;
        vals.peopleArrived = vals.peopleStarved / 2 + (5 - vals.wheatPerAcre) * m_wheat / 600 + 1;
        vals.peopleArrived = std::clamp(vals.peopleArrived, 0, 50);
        if (vals.peopleStarved > 0)
            m_cumulativeStarvedPercent += std::ceil(vals.peopleStarved / (float)m_population  * 100);
        //std::cerr << "debug: starved cumulative:" << m_cumulativeStarvedPercent << std::endl;

        //Harvested:
        vals.wheatHarvested = vals.wheatPerAcre*m_currentRoundInput.acresPlanted;
        vals.wheatEatenByRats = RandomGenerator::getRandomEatenWheat(m_wheat+vals.wheatHarvested);


        m_population = m_population - vals.peopleStarved - vals.peopleDiedFromPlague + vals.peopleArrived;
        m_wheat = m_wheat - vals.wheatEatenByRats + vals.wheatHarvested;
    }

    PrintRoundOutput(vals);
    HandleRoundInput();
    std::cout << std::endl;
}

void HamurabiGame::StartGame()
{
    bool firstRound = true;
    if (exists(m_saveFilePath))
    {
        std::cout << "Save game detected. Open it?\ny/n:";
        if (std::getchar() == 'y')
        {
            ReadSaveGame();
            firstRound = false;
        }
    }

    while (m_roundNumber < 10)
    {
        if (firstRound)
        {
            std::cout << "My lord, would you like to take a break and return later?\ny/n:";
            char ans;
            std::cin >> ans;
            if (ans == static_cast<int>('y'))
            {
                std::cout << "Saving game\n";
                SaveGame();
                break;
            }
        }
        ProgressRound();
        m_roundNumber++;
    }
    if (m_roundNumber == 10) Win();
    std::cout << "So long for now." << std::endl;
}
