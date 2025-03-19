# Multithreaded Guess the Number Game

This project is a **C++ implementation** of a **multithreaded number-guessing game**, where players try to guess a randomly generated target number. The game is controlled by a host thread, while each player has their own thread to make guesses. 

## Features
- Uses **threads** for players and the host.
- Implements **mutex synchronization** to prevent race conditions.
- Players take turns guessing the target number.
- The game consists of multiple **rounds**, with a new random number generated each round.
- Supports **dynamic number of players and rounds**.
- Ensures that all players start guessing **simultaneously** by synchronizing the start time.
