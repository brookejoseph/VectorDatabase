#include "vectore-store.cc"
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <queue>
#include <functional>

class BattleVector : public Vector
{
public:
    float strength;
    float agility;

    BattleVector(int id, std::vector<float> values, float strength, float agility)
        : Vector(id, values), strength(strength), agility(agility) {}

    float calculatePower() const
    {
        return strength * 0.6f + agility * 0.4f;
    }
};

class VectorBattleArena
{
private:
    std::vector<BattleVector> warriors;
    std::mutex arenaMutex;
    std::random_device rd;
    std::mt19937 gen;

    float calculateSimilarity(const BattleVector &v1, const BattleVector &v2)
    {
        float dotProduct = 0.0f;
        float mag1 = 0.0f, mag2 = 0.0f;

        for (size_t i = 0; i < v1.values.size(); ++i)
        {
            dotProduct += v1.values[i] * v2.values[i];
            mag1 += v1.values[i] * v1.values[i];
            mag2 += v2.values[i] * v2.values[i];
        }

        return dotProduct / (std::sqrt(mag1) * std::sqrt(mag2));
    }

    BattleVector *conductBattle(BattleVector &v1, BattleVector &v2)
    {
        float similarity = calculateSimilarity(v1, v2);
        float v1Score = v1.calculatePower() * (1 + similarity);
        float v2Score = v2.calculatePower() * (1 + similarity);

        std::uniform_real_distribution<> dis(0, 1);
        float randomFactor = dis(gen);

        if (v1Score * randomFactor > v2Score * (1 - randomFactor))
        {
            return &v1;
        }
        else
        {
            return &v2;
        }
    }

    void evolutionStep(BattleVector &loser)
    {
        std::normal_distribution<> d(0, 0.1);
        for (float &value : loser.values)
        {
            value += d(gen);
        }
        loser.strength += d(gen);
        loser.agility += d(gen);
    }

public:
    VectorBattleArena() : gen(rd()) {}

    void addWarrior(const BattleVector &warrior)
    {
        std::lock_guard<std::mutex> lock(arenaMutex);
        warriors.push_back(warrior);
    }

    void runTournament(int numRounds)
    {
        std::vector<std::thread> battleThreads;

        for (int round = 0; round < numRounds; ++round)
        {
            std::cout << "Round " << round + 1 << " of the tournament!" << std::endl;

            std::vector<BattleVector *> roundWinners;

            for (size_t i = 0; i < warriors.size(); i += 2)
            {
                if (i + 1 < warriors.size())
                {
                    battleThreads.emplace_back([this, i, &roundWinners]()
                                               {
                        BattleVector* winner = conductBattle(warriors[i], warriors[i+1]);
                        BattleVector* loser = (winner == &warriors[i]) ? &warriors[i+1] : &warriors[i];
                        
                        std::lock_guard<std::mutex> lock(arenaMutex);
                        roundWinners.push_back(winner);
                        evolutionStep(*loser); });
                }
                else
                {
                    roundWinners.push_back(&warriors[i]);
                }
            }

            for (auto &thread : battleThreads)
            {
                thread.join();
            }
            battleThreads.clear();

            std::lock_guard<std::mutex> lock(arenaMutex);
            warriors.clear();
            for (auto *winner : roundWinners)
            {
                warriors.push_back(*winner);
            }

            std::cout << warriors.size() << " warriors remain!" << std::endl;
        }
    }

    void displayWarriors()
    {
        std::lock_guard<std::mutex> lock(arenaMutex);
        for (const auto &warrior : warriors)
        {
            std::cout << "Warrior ID: " << warrior.id << ", Strength: " << warrior.strength
                      << ", Agility: " << warrior.agility << ", Power: " << warrior.calculatePower() << std::endl;
            std::cout << "Values: ";
            for (float value : warrior.values)
            {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }
};

void runBattleArena()
{
    VectorBattleArena arena;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 10);

    for (int i = 0; i < 16; ++i)
    {
        std::vector<float> values = {static_cast<float>(dis(gen)), static_cast<float>(dis(gen)), static_cast<float>(dis(gen))};
        arena.addWarrior(BattleVector(i, values, static_cast<float>(dis(gen)), static_cast<float>(dis(gen))));
    }

    std::cout << "Initial warriors:" << std::endl;
    arena.displayWarriors();

    arena.runTournament(4);

    std::cout << "\nFinal warriors:" << std::endl;
    arena.displayWarriors();
}

int main()
{
    std::cout << "Welcome to the Vector Battle Arena!" << std::endl;
    std::cout << "1. Run Battle Arena" << std::endl;
    std::cout << "2. Exit" << std::endl;

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        runBattleArena();
        break;
    case 2:
        std::cout << "Exiting..." << std::endl;
        break;
    default:
        std::cout << "Invalid choice. Exiting..." << std::endl;
    }

    return 0;
}