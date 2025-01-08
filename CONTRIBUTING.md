# Contributing to Steamdb CLI

Thank you for considering contributing to Steamdb CLI! We welcome contributions from the community to help improve the project. Please take a moment to review these guidelines before getting started.

## How to Contribute

1. **Fork the repository**: Click the "Fork" button at the top right corner of the repository page to create a copy of the repository in your GitHub account.

2. **Clone the repository**: Clone the forked repository to your local machine using the following command:
   ```
   git clone https://github.com/yourusername/steamdb_cli.git
   cd steamdb_cli
   ```

3. **Create a new branch**: Create a new branch for your contribution using the following command:
   ```
   git checkout -b my-contribution
   ```

4. **Make your changes**: Make the necessary changes to the codebase. Ensure that your changes follow the coding style and guidelines of the project.

5. **Commit your changes**: Commit your changes with a descriptive commit message using the following command:
   ```
   git commit -m "Description of your changes"
   ```

6. **Push your changes**: Push your changes to your forked repository using the following command:
   ```
   git push origin my-contribution
   ```

7. **Create a pull request**: Go to the original repository on GitHub and create a pull request from your forked repository. Provide a clear and detailed description of your changes in the pull request.

## Reporting Issues

If you encounter any issues or bugs while using Steamdb CLI, please report them by creating an issue on the GitHub repository. Provide as much detail as possible, including steps to reproduce the issue, the expected behavior, and any relevant error messages.

## Requesting Features

If you have a feature request or an idea to improve Steamdb CLI, please open an issue on the GitHub repository. Describe the feature or improvement in detail and explain why it would be beneficial to the project.

## Setting Up the Development Environment

To set up the development environment for Steamdb CLI, follow these steps:

1. **Install dependencies**: Ensure that you have the necessary dependencies installed on your machine. This project requires the following:
   - CMake
   - A C++ compiler (e.g., g++, clang++)
   - libcurl

2. **Build the project**: Use the following commands to build the project:
   ```
   mkdir -p build && cd build
   cmake ..
   make
   ```

3. **Run the tests**: If the project includes tests, run them to ensure that your changes do not introduce any regressions. Use the following command to run the tests:
   ```
   make test
   ```

Thank you for your contributions! We appreciate your help in making Steamdb CLI better.
