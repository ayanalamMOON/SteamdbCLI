#pragma once
#include <stdexcept>
#include <iostream>
#include <exception>

// Custom exception class for parsing errors
class ParsingError : public std::runtime_error {
public:
    explicit ParsingError(const std::string& message) : std::runtime_error(message) {}
};

// Custom exception class for file I/O errors
class FileIOError : public std::runtime_error {
public:
    explicit FileIOError(const std::string& message) : std::runtime_error(message) {}
};

// Declare the global error handler function as an external function
extern void globalErrorHandler();
