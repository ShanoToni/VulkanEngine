#ifndef PHYSICS_MESH_CLASS
#define PHYSICS_MESH_CLASS

#include <array>
#include <vector>

#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "utils.hpp"

#include <fstream>
#include <vector>

struct DirectionalLightStruct {
    float ambientIntensity;
    float diffuseIntensity;
    glm::vec4 color;
    glm::vec4 direction;
};

class PhysicsMesh : public Mesh {
  public:
    PhysicsMesh() : Mesh() {
        directionalLightData.ambientIntensity = 0.f;
        directionalLightData.diffuseIntensity = 0.f;
        directionalLightData.color = glm::vec4(0.0f);
        directionalLightData.direction = glm::vec4(0.0f);
    };
    PhysicsMesh(const PhysicsMesh& other);
    PhysicsMesh(std::vector<Vertex> verts) : Mesh(verts) {
        directionalLightData.ambientIntensity = 0.f;
        directionalLightData.diffuseIntensity = 0.f;
        directionalLightData.color = glm::vec4(0.0f);
        directionalLightData.direction = glm::vec4(0.0f);
    };
    PhysicsMesh(std::string filePath) : Mesh(filePath) {
        directionalLightData.ambientIntensity = 0.f;
        directionalLightData.diffuseIntensity = 0.f;
        directionalLightData.color = glm::vec4(0.0f);
        directionalLightData.direction = glm::vec4(0.0f);
    };

    // Overrides
    void createDescriptorSets(std::vector<VkImage> swapChainImages,
                              VkDescriptorSetLayout descriptorSetLayout,
                              VkDescriptorPool descriptorPool, VkDevice device);
    // Additions
    void createLightingUBOBuffers(std::vector<VkImage> swapChainImages,
                                  VkDevice device,
                                  VkPhysicalDevice physicalDevice);
    void setLightingUBOBuffers(uint32_t currentImage, VkDevice device,
                               DirectionalLight* dirLight);

  protected:
    DirectionalLightStruct directionalLightData;
    std::vector<VkBuffer> directionalLightBuffers;
    std::vector<VkDeviceMemory> directionalLightBuffersMemory;
};

#endif // PHYSICS_MESH_CLASS