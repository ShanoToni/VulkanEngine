#include "Renderer.hpp"
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include <iostream>

int main() {
    Renderer app;

    auto planeVertices = getPlaneVerts();
    auto planeIndices = getPlaneIndices();

    PhysicsMesh* plane = new PhysicsMesh(planeVertices);
    plane->scale(glm::vec3(10.f, 1.f, 10.f));
    plane->setIndices(planeIndices);

    auto cubeVerts = getCubeVerts();
    auto cubeIndices = getCubeIndices();
    PhysicsMesh* cube = new PhysicsMesh(cubeVerts);
    cube->setIndices(cubeIndices);
    cube->scale(glm::vec3(2.0f, 2.0f, 2.0f));
    cube->translate(glm::vec3(0.0f, 2.0f, 0.0f));

    DirectionalLight light(glm::vec3(1.0f), 0.6f, 0.9f,
                           glm::vec3(-.9f, -1.0f, 0.8f));

    app.addMesh({plane, cube});
    app.addDirectionalLight(light);

    try {
        app.run();
    } catch (const std::exception e) {
        std::cout << "RUNTIME ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "All good!" << std::endl;
    return EXIT_SUCCESS;
}