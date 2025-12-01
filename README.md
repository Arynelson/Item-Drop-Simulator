# Item Drop Simulator

A C++ command-line tool that simulates the monster drop system from Metin2. It reads drop tables from a config file, lets you fight different monsters, and shows what items you get based on realistic probability calculations.

## What it does

- Loads monster and item data from a text config file
- Simulates battles with different level monsters
- Calculates item drops using probability percentages
- Shows dropped items with colored rarity tiers (Common, Uncommon, Rare, Epic)
- Keeps a log of all your kills and drops in `drop_log.txt`
- Displays session statistics at the end

## How to compile

Make sure you have a C++ compiler installed (g++, clang, MSVC, etc.)

**Linux/Mac:**
```bash
g++ -o drop_simulator main.cpp -std=c++11
./drop_simulator
```

**Windows (MinGW):**
```bash
g++ -o drop_simulator.exe main.cpp -std=c++11
drop_simulator.exe
```

**Windows (MSVC):**
```bash
cl /EHsc main.cpp
main.exe
```

## Usage

1. Make sure `drops.txt` is in the same folder as the executable
2. Run the program
3. Choose a monster to fight by entering its number
4. The program will show you what items dropped (if any)
5. Keep fighting or press 0 to quit and see your stats

The program automatically creates a `drop_log.txt` file that records everything you got during the session.

## Config file format

The `drops.txt` file defines monsters and their drop tables. Here's the structure:

```
[MONSTER]
name=Monster Name
level=10
item=Item Name,Rarity,DropChance
```

- **name**: What the monster is called
- **level**: Monster level (just for display)
- **item**: Format is `ItemName,Rarity,Percentage`
  - Rarity can be: Common, Uncommon, Rare, Epic
  - Percentage is the drop chance (e.g., 5.0 = 5%)

You can add as many monsters and items as you want. Just follow the format.

## Example output

```
=== Available Monsters ===
[1] Wild Wolf (Lv. 5)
[2] Orc Warrior (Lv. 15)
[3] Dark Assassin (Lv. 25)
[0] Exit

Select monster to fight: 1

⚔ Fighting Wild Wolf...
Victory!

✦ Items Dropped:
  • Wolf Pelt [Common]
  • Iron Ore [Uncommon]
```

## Features

- **Realistic probability system**: Uses proper RNG with decimal precision for drop chances
- **Color-coded rarities**: Terminal colors make it easy to spot rare items at a glance
- **Persistent logging**: All drops are saved to a file with timestamps
- **Statistics tracking**: See how many of each item you got during your session
- **Easy to extend**: Adding new monsters or items is just editing the text file

## Technical notes

The simulator uses `<cstdlib>` random number generation seeded with the current time. Drop chances are calculated with 0.01% precision (rolls from 0.00 to 99.99).

ANSI escape codes are used for terminal colors. If you're on an older Windows terminal that doesn't support them, you might see weird characters instead of colors. Use Windows Terminal or a modern console app.

## Why I made this

This project demonstrates several key concepts needed for game server development:
- File parsing and configuration loading
- Probability systems and random number generation
- Data structure organization (structs and vectors)
- Logging and statistics tracking
- Clean, readable code structure

It's also directly relevant to Metin2 server work since drop tables are a core mechanic in the game.

## License

Free to use however you want. No restrictions.
