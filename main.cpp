#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <map>

// ANSI color codes for terminal output
#define RESET   "\033[0m"
#define GRAY    "\033[90m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

struct Item {
    std::string name;
    std::string rarity;
    double dropChance;

    std::string getColorCode() const {
        if (rarity == "Common") return GRAY;
        if (rarity == "Uncommon") return GREEN;
        if (rarity == "Rare") return BLUE;
        if (rarity == "Epic") return PURPLE;
        return RESET;
    }
};

struct Monster {
    std::string name;
    int level;
    std::vector<Item> dropTable;
};

class DropSimulator {
private:
    std::vector<Monster> monsters;
    std::ofstream logFile;
    int totalKills;
    std::map<std::string, int> itemsDropped;

public:
    DropSimulator() : totalKills(0) {
        srand(static_cast<unsigned>(time(0)));
        logFile.open("drop_log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << "\n=== New Session Started ===\n";
            logFile << "Time: " << getCurrentTime() << "\n\n";
        }
    }

    ~DropSimulator() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    std::string getCurrentTime() {
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return std::string(buf);
    }

    bool loadDropData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << RED << "Error: Cannot open " << filename << RESET << std::endl;
            return false;
        }

        std::string line;
        Monster currentMonster;
        bool readingMonster = false;

        while (getline(file, line)) {
            // skip empty lines and comments
            if (line.empty() || line[0] == '#') continue;

            if (line.find("[MONSTER]") == 0) {
                if (readingMonster && !currentMonster.name.empty()) {
                    monsters.push_back(currentMonster);
                }
                currentMonster = Monster();
                currentMonster.dropTable.clear();
                readingMonster = true;
            }
            else if (readingMonster) {
                if (line.find("name=") == 0) {
                    currentMonster.name = line.substr(5);
                }
                else if (line.find("level=") == 0) {
                    currentMonster.level = std::stoi(line.substr(6));
                }
                else if (line.find("item=") == 0) {
                    // Format: item=ItemName,Rarity,DropChance
                    size_t pos1 = line.find('=') + 1;
                    size_t pos2 = line.find(',', pos1);
                    size_t pos3 = line.find(',', pos2 + 1);

                    Item item;
                    item.name = line.substr(pos1, pos2 - pos1);
                    item.rarity = line.substr(pos2 + 1, pos3 - pos2 - 1);
                    item.dropChance = std::stod(line.substr(pos3 + 1));

                    currentMonster.dropTable.push_back(item);
                }
            }
        }

        // don't forget last monster
        if (readingMonster && !currentMonster.name.empty()) {
            monsters.push_back(currentMonster);
        }

        file.close();
        return !monsters.empty();
    }

    std::vector<Item> simulateDrop(const Monster& monster) {
        std::vector<Item> droppedItems;

        for (const auto& item : monster.dropTable) {
            double roll = (rand() % 10000) / 100.0;
            if (roll < item.dropChance) {
                droppedItems.push_back(item);
                itemsDropped[item.name]++;
            }
        }

        return droppedItems;
    }

    void displayMonsters() {
        std::cout << YELLOW << "\n=== Available Monsters ===" << RESET << std::endl;
        for (size_t i = 0; i < monsters.size(); i++) {
            std::cout << "[" << (i + 1) << "] " << monsters[i].name
                     << " (Lv. " << monsters[i].level << ")" << std::endl;
        }
        std::cout << "[0] Exit\n" << std::endl;
    }

    void simulateBattle(int monsterIndex) {
        if (monsterIndex < 0 || monsterIndex >= static_cast<int>(monsters.size())) {
            return;
        }

        Monster& monster = monsters[monsterIndex];
        totalKills++;

        std::cout << "\n" << RED << "⚔ Fighting " << monster.name << "..." << RESET << std::endl;
        std::cout << GREEN << "Victory!" << RESET << std::endl;

        std::vector<Item> drops = simulateDrop(monster);

        if (drops.empty()) {
            std::cout << GRAY << "No items dropped." << RESET << std::endl;
        } else {
            std::cout << YELLOW << "\n✦ Items Dropped:" << RESET << std::endl;
            for (const auto& item : drops) {
                std::cout << "  " << item.getColorCode() << "• " << item.name
                         << " [" << item.rarity << "]" << RESET << std::endl;
            }
        }

        // log to file
        if (logFile.is_open()) {
            logFile << "Kill #" << totalKills << " - " << monster.name << "\n";
            if (drops.empty()) {
                logFile << "  No drops\n";
            } else {
                for (const auto& item : drops) {
                    logFile << "  - " << item.name << " [" << item.rarity << "]\n";
                }
            }
            logFile << "\n";
        }
    }

    void showStatistics() {
        std::cout << "\n" << YELLOW << "=== Statistics ===" << RESET << std::endl;
        std::cout << "Total kills: " << totalKills << std::endl;

        if (!itemsDropped.empty()) {
            std::cout << "\nItems obtained:" << std::endl;
            for (const auto& pair : itemsDropped) {
                std::cout << "  " << pair.first << ": " << pair.second << std::endl;
            }
        }
    }

    void run() {
        if (monsters.empty()) {
            std::cerr << RED << "No monsters loaded!" << RESET << std::endl;
            return;
        }

        std::cout << PURPLE << "╔════════════════════════════════╗" << std::endl;
        std::cout << "║        Drop System Simulator ║" << std::endl;
        std::cout << "╚════════════════════════════════╝" << RESET << std::endl;

        int choice;
        while (true) {
            displayMonsters();
            std::cout << "Select monster to fight: ";
            std::cin >> choice;

            if (choice == 0) {
                break;
            }

            if (choice > 0 && choice <= static_cast<int>(monsters.size())) {
                simulateBattle(choice - 1);
            } else {
                std::cout << RED << "Invalid choice!" << RESET << std::endl;
            }
        }

        showStatistics();
        std::cout << "\n" << GREEN << "Thanks for playing!" << RESET << std::endl;
    }
};

int main() {
    DropSimulator simulator;

    if (!simulator.loadDropData("drops.txt")) {
        std::cerr << "Failed to load drop data. Make sure drops.txt exists." << std::endl;
        return 1;
    }

    simulator.run();

    return 0;
}
