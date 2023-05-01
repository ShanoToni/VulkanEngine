#ifndef MESH_CLASS
#define MESH_CLASS

#include <array>
#include <vector>

#include "Camera.hpp"
#include "Texture.hpp"
#include "utils.hpp"

#include <fstream>
#include <vector>

enum class VertexData { PosCol = 0, PosColTex, PosColTexNorm };

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

    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptionsPosCol() {
        VkVertexInputAttributeDescription attributeDescriptionsPos;
        attributeDescriptionsPos.binding = 0;
        attributeDescriptionsPos.location = 0;
        attributeDescriptionsPos.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptionsPos.offset = offsetof(Vertex, pos);

        VkVertexInputAttributeDescription attributeDescriptionsCol;
        attributeDescriptionsCol.binding = 0;
        attributeDescriptionsCol.location = 1;
        attributeDescriptionsCol.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptionsCol.offset = offsetof(Vertex, color);

        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{
            attributeDescriptionsPos, attributeDescriptionsCol};
        return attributeDescriptions;
    }

    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptionsPosColTex() {

        VkVertexInputAttributeDescription attributeDescriptionsPos;
        attributeDescriptionsPos.binding = 0;
        attributeDescriptionsPos.location = 0;
        attributeDescriptionsPos.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptionsPos.offset = offsetof(Vertex, pos);

        VkVertexInputAttributeDescription attributeDescriptionsCol;
        attributeDescriptionsCol.binding = 0;
        attributeDescriptionsCol.location = 1;
        attributeDescriptionsCol.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptionsCol.offset = offsetof(Vertex, color);

        VkVertexInputAttributeDescription attributeDescriptionsTex;
        attributeDescriptionsTex.binding = 0;
        attributeDescriptionsTex.location = 2;
        attributeDescriptionsTex.format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptionsTex.offset = offsetof(Vertex, texCoord);

        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{
            attributeDescriptionsPos, attributeDescriptionsCol,
            attributeDescriptionsTex};
        return attributeDescriptions;
    }

    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptionsPosColTexNormal() {

        VkVertexInputAttributeDescription attributeDescriptionsPos;
        attributeDescriptionsPos.binding = 0;
        attributeDescriptionsPos.location = 0;
        attributeDescriptionsPos.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptionsPos.offset = offsetof(Vertex, pos);

        VkVertexInputAttributeDescription attributeDescriptionsCol;
        attributeDescriptionsCol.binding = 0;
        attributeDescriptionsCol.location = 1;
        attributeDescriptionsCol.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptionsCol.offset = offsetof(Vertex, color);

        VkVertexInputAttributeDescription attributeDescriptionsTex;
        attributeDescriptionsTex.binding = 0;
        attributeDescriptionsTex.location = 2;
        attributeDescriptionsTex.format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptionsTex.offset = offsetof(Vertex, texCoord);

        VkVertexInputAttributeDescription attributeDescriptionsNorm;
        attributeDescriptionsNorm.binding = 0;
        attributeDescriptionsNorm.location = 3;
        attributeDescriptionsNorm.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptionsNorm.offset = offsetof(Vertex, normal);

        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{
            attributeDescriptionsPos, attributeDescriptionsCol,
            attributeDescriptionsTex, attributeDescriptionsNorm};
        return attributeDescriptions;
    }
};

class Mesh {
  public:
    Mesh();
    Mesh(const Mesh& other);
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
    VkDescriptorSet& getDescriptorSet() { return descriptorSet; }
    std::vector<VkBuffer>& getUniformBuffers() { return uniformBuffers; }
    std::vector<VkDeviceMemory>& getUniformBufferMemory() {
        return uniformBuffersMemory;
    }

    VkBuffer& getVertexBuffer() { return vertexBuffer; }
    VkDeviceMemory& getVertexBufferMemory() { return vertexBufferMemory; }
    VkBuffer& getIndexBuffer() { return indexBuffer; }
    VkDeviceMemory& getIndexBufferMemory() { return indexBufferMemory; }

    void setTexture(Texture* tex) { texture.reset(tex); }
    const inline std::unique_ptr<Texture>& getTexture() { return texture; }

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

    std::unique_ptr<Texture> texture;
};

#endif // MESH_CLASS