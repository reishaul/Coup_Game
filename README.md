# Coup Game Project (C++ with SFML)

## Overview
This project is a C++ implementation of a strategic multiplayer game inspired by **Coup**, extended with unique roles such as **Governor**, **Spy**, **Baron**, **General**, **Judge**, and **Merchant**. The game features a graphical user interface using **SFML** and includes game logic, player management, and role-specific behavior.

## Features
- Turn-based strategy gameplay with 6 different roles
- Role-based actions (e.g., gather, tax, coup, arrest, bribe, sanction)
- Role abilities affect the game state (e.g., Merchant earns bonuses)
- Object-oriented design with clear separation of logic and GUI
- Unit testing support and memory checking with Valgrind

---

## Project Structure

### Directories and Files

├── include/

│ ├── Game.hpp # Main game engine, manages players and turn logic

│ ├── Player.hpp # Base class for all players with shared logic

│ ├── Governor.hpp # Governor role - inherits from Player

│ ├── Judge.hpp # Judge role - inherits from Player

│ ├── Spy.hpp # Spy role - inherits from Player

│ ├── Baron.hpp # Baron role - inherits from Player

│ ├── General.hpp # General role - inherits from Player

│ └── Merchant.hpp # Merchant role - inherits from Player

│
├── src/

│ ├── Game.cpp # Implementation of Game.hpp

│ ├── Player.cpp # Implementation of Player.hpp

│ ├── Governor.cpp # Implementation of Governor.hpp

│ ├── Judge.cpp # Implementation of Judge.hpp

│ ├── Spy.cpp # Implementation of Spy.hpp

│ ├── Baron.cpp # Implementation of Baron.hpp

│ ├── General.cpp # Implementation of General.hpp

│ ├── Merchant.cpp # Implementation of Merchant.hpp

│ ├── main.cpp # Main GUI-based gameplay using SFML

│ └── Demo.cpp # Optional demo to showcase gameplay in terminal

│
├── game_test.cpp # Unit tests for game logic

├── doctest.h # Header-only testing framework (used in game_test.cpp)

├── Makefile # Build system

└── README.md # This file


---

## Class Structure

### `Game`
- Manages the game state: players, turns, last arrest, etc.
- Adds/removes players and handles round progression.

### `Player` (Abstract)
- Base class for all roles.
- Implements common actions like `gather`, `tax`, `coup`, `arrest`, `bribe`.
- Contains access control for actions and coin management.

### Derived Roles (inherit from `Player`)
- **Governor**: Special influence on political dynamics.
- **Judge**: Can use justice-related actions.
- **Spy**: Manipulates turn order or inspects others.
- **Baron**: Economic/political hybrid.
- **General**: Gains/returns coins during arrests.
- **Merchant**: Gains bonus coins during specific actions.

Each role overrides or extends some of the base `Player` methods.

---
## Graphical User Interface

The graphical user interface (GUI) is built using SFML, providing a simple and interactive environment for players to engage with the game.

The interface includes the following main screens:

Start Screen-

A visually designed welcome screen with a large Start button.
Once clicked, the game transitions to the player setup screen.

Add Players Screen-

The user is prompted to add between 2 to 6 players by entering their names.
Each new player is automatically assigned a random role from the available ones (e.g., Governor, Spy, etc.).

Main Game Screen-

Displays all the players, their roles, and their available actions.
Action buttons allow players to perform their operations (such as gather, tax, coup, etc.).
A console feedback log (in the terminal) provides real-time textual output for every action taken, enhancing traceability and debugging.

---

## Building the Project

To compile the project:

```bash
make
```

This creates:

coup_game: the main executable.

demo: a demo executable (optional).

test_runner: executable for unit testing.

To run the game:
```bash
make Main
```

To run the demo:
```bash
./demo
```

Running Tests
To build and run tests:
```bash
make test
```

To run memory checks with Valgrind:
```bash
make valgrind
```

Dependencies
C++17

SFML (libsfml-graphics, libsfml-window, libsfml-system)

Valgrind for memory checking

Cleaning Build Files
To clean up all object files and executables:
```bash
make clean
```

Notes
The game is designed for extensibility. To add a new role, simply inherit from Player and override relevant methods.

The game logic can run independently of the GUI, allowing easy testing or command-line versions.

Authors
Developed by REI SHAUL

E-mail: reishaul1@gmail.com
GitHub: [[link](https://github.com/reishaul)]


