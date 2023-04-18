#include "Renderer.hpp"
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include <iostream>

int main() {
    Renderer app;

    try {
        app.run();
    } catch (const std::exception e) {
        std::cout << "RUNTIME ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "All good!" << std::endl;
    return EXIT_SUCCESS;
}