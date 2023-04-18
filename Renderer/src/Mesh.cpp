#include "Mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Mesh::Mesh() { model = glm::mat4(1.0f); }

Mesh::Mesh(const Mesh& other) {
    vertices = other.vertices;
    indices = other.indices;
    descriptorSet = other.descriptorSet;
    ubo = other.ubo;
    model = other.model;

    vertexBuffer = other.vertexBuffer;
    vertexBufferMemory = other.vertexBufferMemory;
    indexBuffer = other.indexBuffer;
    indexBufferMemory = other.indexBufferMemory;

    uniformBuffers = other.uniformBuffers;
    uniformBuffersMemory = other.uniformBuffersMemory;

    texture.reset(other.texture.get());
}

Mesh::Mesh(std::vector<Vertex> verts) {
    for (auto Vertex : verts) {
        vertices.push_back(Vertex);
    }
    model = glm::mat4(1.0f);
}

Mesh::Mesh(std::string filePath) {
    tinyobj::attrib_t attribute;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warn, &err,
                          filePath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {attribute.vertices[3 * index.vertex_index + 0],
                          attribute.vertices[3 * index.vertex_index + 1],
                          attribute.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {
                attribute.texcoords[2 * index.texcoord_index + 0],
                1.0f - attribute.texcoords[2 * index.texcoord_index + 1]};
            vertex.normal = {attribute.normals[3 * index.normal_index + 0],
                             attribute.normals[3 * index.normal_index + 1],
                             attribute.normals[3 * index.normal_index + 2]};
            vertex.color = glm::vec3(1.0f);

            vertices.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }
    model = glm::mat4(1.0f);
}

void Mesh::translate(glm::vec3 translateVector) {
    model = glm::translate(model, translateVector);
    ubo.model = model;
}

void Mesh::rotate(glm::vec3 rotateVector) {
    model = glm::rotate(model, glm::radians(90.f), rotateVector);
    ubo.model = model;
}

void Mesh::scale(glm::vec3 scaleVector) {
    model = glm::scale(model, scaleVector);
    ubo.model = model;
}

void Mesh::createVertexBuffer(VkDevice& device,
                              VkPhysicalDevice& physicalDevice,
                              VkCommandPool commandPool,
                              VkQueue graphicsQueue) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer,
                 vertexBufferMemory, device, physicalDevice);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize, commandPool, device,
               graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Mesh::createIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice,
                             VkCommandPool commandPool, VkQueue graphicsQueue) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                     VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer,
                 indexBufferMemory, device, physicalDevice);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize, commandPool, device,
               graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Mesh::createDescriptorSets(std::vector<VkImage> swapChainImages,
                                VkDescriptorSetLayout descriptorSetLayout,
                                VkDescriptorPool descriptorPool,
                                VkDevice device) {
    std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(),
                                               descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount =
        static_cast<uint32_t>(swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(BasicUBO);

        // No Images for mesh for now

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture->getTextureImageView();
        imageInfo.sampler = texture->getTextureSampler();

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }
}

void Mesh::createUniformBuffers(std::vector<VkImage> swapChainImages,
                                VkDevice device,
                                VkPhysicalDevice physicalDevice) {
    VkDeviceSize bufferSize = sizeof(ubo);

    uniformBuffers.resize(swapChainImages.size());
    uniformBuffersMemory.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     uniformBuffers[i], uniformBuffersMemory[i], device,
                     physicalDevice);
    }
}

void Mesh::updateUniformBuffer(uint32_t currentImage, Camera& cam,
                               VkExtent2D swapChainExtent, VkDevice device) {
    ubo.model = model;

    ubo.view = cam.getView();
    ubo.proj = glm::perspective(
        glm::radians(45.0f),
        swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 999999.f);
    ubo.proj[1][1] *= -1;
    ubo.model = model;

    void* data;
    vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0,
                &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
}

void Mesh::setIndices(std::vector<uint32_t> indicesToSet) {
    for (auto index : indicesToSet) {
        indices.push_back(index);
    }
}
