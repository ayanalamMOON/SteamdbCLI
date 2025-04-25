#include "error_handling.h"
#include "logger.h"
#include <iostream>

// Global error handler function to catch and log unhandled exceptions
void globalErrorHandler() {
    try {
        throw; // Re-throw the current exception
    } catch (const NetworkError& e) {
        std::cerr << "Unhandled network exception: " << e.what() << std::endl;
        // If you have a global logger instance, you could log here
        // logger->error("Unhandled network exception: " + std::string(e.what()));
    } catch (const ParsingError& e) {
        std::cerr << "Unhandled parsing exception: " << e.what() << std::endl;
        // logger->error("Unhandled parsing exception: " + std::string(e.what()));
    } catch (const FileIOError& e) {
        std::cerr << "Unhandled file I/O exception: " << e.what() << std::endl;
        // logger->error("Unhandled file I/O exception: " + std::string(e.what()));
    } catch (const std::exception& e) {
        std::cerr << "Unhandled standard exception: " << e.what() << std::endl;
        // logger->error("Unhandled standard exception: " + std::string(e.what()));
    } catch (...) {
        std::cerr << "Unhandled unknown exception" << std::endl;
        // logger->error("Unhandled unknown exception");
    }
}