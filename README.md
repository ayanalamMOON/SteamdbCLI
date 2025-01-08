# Steamdb CLI

## Overview

Steamdb CLI is a command-line interface (CLI) tool that allows users to search for information about games available on Steam. The tool fetches data from the Steam database and provides details such as the game's name, app ID, current price, lowest price, Metacritic score, release date, tags, and description.

## Purpose

The purpose of this project is to provide a simple and efficient way for users to access game information from the Steam database using a command-line interface. This tool can be useful for gamers, developers, and researchers who want to quickly retrieve game data without using a web browser.

## How to Set Up the Development Environment

1. Install dependencies:
   - CMake
   - A C++ compiler (e.g., g++, clang++)
   - libcurl

2. Build the project:
   ```
   mkdir -p build && cd build
   cmake ..
   make
   ```

3. Run the tests:
   ```
   make test
   ```

## How to Use

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/steamdb_cli.git
   cd steamdb_cli
   ```

2. Build the project:
   ```
   mkdir -p build && cd build
   cmake ..
   make
   ```

3. Run the CLI tool:
   ```
   ./steamdb_cli
   ```

4. Enter the name of the game you want to search for when prompted.

## Examples

### Example 1: Searching for a Game

```
Welcome to Steamdb CLI!
Type the name of a game to search for its information.

Enter game name (or type 'exit' to quit): Half-Life 2
Fetched Data: Half-Life 2
```

### Example 2: Exiting the CLI

```
Welcome to Steamdb CLI!
Type the name of a game to search for its information.

Enter game name (or type 'exit' to quit): exit
```

## Expected Output

When you search for a game, the CLI tool will fetch and display the game's information, including its name, app ID, current price, lowest price, Metacritic score, release date, tags, and description. If the game is found in the cache, the cached data will be displayed. If the game is not found in the cache, the tool will fetch the data from the Steam database and add it to the cache.

## Error Handling

If there is a network error or any other issue while fetching the game data, the CLI tool will display an appropriate error message and log the error in the log file.

## Logging

The CLI tool logs information, errors, and debug messages to a log file named `steamdb_cli.log`. The log file is created in the same directory as the executable.

## Contributing

Please refer to the `CONTRIBUTING.md` file for guidelines on how to contribute to this project.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more information.
