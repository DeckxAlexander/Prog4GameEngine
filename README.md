# Bomberman Engine
 
A replication of the original Bomberman game made with **minigin** for the Programming 4 course at DAE.

[![Build Status](https://github.com/DeckxAlexander/Prog4GameEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/DeckxAlexander/cmake/actions)
[![Build Status](https://github.com/DeckxAlexander/Prog4GameEngine/actions/workflows/emscripten.yml/badge.svg)](https://github.com/DeckxAlexander/emscripten/actions)

# Github

[Github Repository](https://github.com/DeckxAlexander/Prog4GameEngine)


# Engine

To create the engine of the game we started from the Minigin start project. 
Almost every week we learned a new Design Pattern and implemented it into the engine.

## Design Patterns

This project uses the following Game Patterns:

- Game Loop
- Update
- Components
- Dirty flag
- Commands
- Subject / Observers
- Service locator
- Singleton
- States


# Design choices

During development I made the following design choices:

### 60 FPS
FPS is capped at 60 to ensure a consistent frame rate across different hardware.

### Time Manager
I used a Time Manager Singleton to provide global access to delta time, ensuring it can be accessed from anywhere in the engine without needing to pass it through function parameters.

### States 
I used the State pattern for enemy behavior, since enemies can switch between different behaviors such as wandering or chasing the player. 
Enemies can also have different levels of intelligence, with their specific behavior encapsulated within each state.

### Threading
I used multithreading for the soundsystem to prevent it from blocking or slowing down the gameloop

### Service locator
The soundsystem uses a service locator to make sure its always accesible.
In addition, there is also a service for the grid component, since alot of components need to access it.

### Singleton
I used Singletons for the GameSceneLoader and GameManager, as these classes need to be globally accessible and consistent across all scenes throughout the game.
I also used a Singleton for the ColliderManager to keep track of all colliders in the game, used for movement etc.

### Scores
The scoreboard is saved to a CSV file, allowing high scores and a list of all players to exist across multiple sessions.

### Levels
Levels are loaded from a text file, allowing for easy customization of level data without changing the source code.

### Game Manager
The GameManager keeps track of the game's score and overall state. It is also responsible for transferring player data between levels. 
This prevents player data from being lost during scene transitions.

### Scene transistions
The SceneManager maintains a collection of all available scenes and keeps track of the currently active scene through an index. 
When a new scene is loaded, the previous scene is removed from memory to safe up memory. 
However the Main Menu scene remains loaded because it is accessed frequently throughout the game.


# Game

The game is a replication of the first bomberman game.
It features 4 enemies:
- Balloom
- Oneal
- Doll
- Minvo

It features 3 powerups:
- Flames
- Extra Bomb
- Detonator

And it features 3 different gamemodes:
- Solo
- Coop
- Versus

### Game rules
- Each Bomberman starts with **3 extra lives**.
- Bomberman loses a life when colliding with an enemy or when caught in the blast radius of a bomb.
- A level is completed when all enemies have been killed and the exit has been found.

# Controls

## Keyboard

### Game:
Moving - WASD

Place Bomb - X

Detonate - C

### Menu:
Moving Selected - WASD

Executing Selected - SPACE

## GamePad

### Game:
Moving - DPAD

Place Bomb - A

Detonate - X

### Menu:
Moving Selected - DPAD

Executing Selected - X


  


