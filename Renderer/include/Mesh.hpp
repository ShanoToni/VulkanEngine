#ifndef MESH_CLASS
#define MESH_CLASS

#include <array>
#include <vector>

#include "Camera.hpp"
#include "utils.hpp"

#include <fstream>
#include <vector>

struct BasicUBO {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2>
    getAttributeDescriptionsPosCol() {
        std::array<VkVertexInputAttributeDescription, 2>
            attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }

    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptionsPosColTex() {
        std::array<VkVertexInputAttributeDescription, 3>
            attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    static std::array<VkVertexInputAttributeDescription, 4>
    getAttributeDescriptionsPosColTexNormal() {
        std::array<VkVertexInputAttributeDescription, 4>
            attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, normal);

        return attributeDescriptions;
    }
};

class Mesh {
  public:
    Mesh();
    Mesh(std::vector<Vertex> verts);
    Mesh(std::string filePath);

    void translate(glm::vec3 translateVector);
    void rotate(glm::vec3 rotateVector);
    void scale(glm::vec3 scaleVector);

    void createVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice,
                            VkCommandPool commandPool, VkQueue graphicsQueue);
    void createIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice,
                           VkCommandPool commandPool, VkQueue graphicsQueue);
    void createDescriptorSets(std::vector<VkImage> swapChainImages,
                              VkDescriptorSetLayout descriptorSetLayout,
                              VkDescriptorPool descriptorPool, VkDevice device);
    void createUniformBuffers(std::vector<VkImage> swapChainImages,
                              VkDevice device, VkPhysicalDevice physicalDevice);
    void updateUniformBuffer(uint32_t currentImage, Camera& cam,
                             VkExtent2D swapChainExtent, VkDevice device);

    std::vector<Vertex> getVertices() { return vertices; }
    std::vector<uint32_t> getIndices() { return indices; }
    void setIndices(std::vector<uint32_t> indicesToSet);

    glm::mat4 getModel() { return model; }
    VkDescriptorSet getDescriptorSet() { return descriptorSet; }
    std::vector<VkBuffer>& getUniformBuffers() { return uniformBuffers; }
    std::vector<VkDeviceMemory>& getUniformBufferMemory() {
        return uniformBuffersMemory;
    }

    VkBuffer& getVertexBuffer() { return vertexBuffer; }
    VkDeviceMemory& getVertexBufferMemory() { return vertexBufferMemory; }
    VkBuffer& getIndexBuffer() { return indexBuffer; }
    VkDeviceMemory& getIndexBufferMemory() { return indexBufferMemory; }

  protected:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    VkDescriptorSet descriptorSet;
    BasicUBO ubo;
    glm::mat4 model;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
};

#endif // MESH_CLASS