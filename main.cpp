#include <exception>
#include <iostream>
#include "consoleHandling.hpp"

int main() {
    try {
        launch();
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    return 0;
}