#include "PhysicsMesh.hpp"

PhysicsMesh::PhysicsMesh(const PhysicsMesh& other) {
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

    directionalLightData.ambientIntensity =
        other.directionalLightData.ambientIntensity;
    directionalLightData.diffuseIntensity =
        other.directionalLightData.diffuseIntensity;
    directionalLightData.color = other.directionalLightData.color;
    directionalLightData.direction = other.directionalLightData.direction;
}

void PhysicsMesh::createDescriptorSets(
    std::vector<VkImage> swapChainImages,
    VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool,
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
        // UBO
        VkDescriptorBufferInfo uboBufferInfo{};
        uboBufferInfo.buffer = uniformBuffers[i];
        uboBufferInfo.offset = 0;
        uboBufferInfo.range = sizeof(BasicUBO);
        // Directinal Light
        VkDescriptorBufferInfo dirBufferInfo{};
        dirBufferInfo.buffer = directionalLightBuffers[i];
        dirBufferInfo.offset = 0;
        dirBufferInfo.range = sizeof(DirectionalLightStruct);
        // No Images for mesh for now

        // VkDescriptorImageInfo imageInfo{};
        // imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        // imageInfo.imageView = texture->getTextureImageView();
        // imageInfo.sampler = texture->getTextureSampler();

        VkWriteDescriptorSet uboDescriptorWrite{};
        uboDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        uboDescriptorWrite.dstSet = descriptorSet;
        uboDescriptorWrite.dstBinding = 0;
        uboDescriptorWrite.dstArrayElement = 0;
        uboDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboDescriptorWrite.descriptorCount = 1;
        uboDescriptorWrite.pBufferInfo = &uboBufferInfo;

        VkWriteDescriptorSet dirDescriptorWrite{};
        dirDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        dirDescriptorWrite.dstSet = descriptorSet;
        dirDescriptorWrite.dstBinding = 1;
        dirDescriptorWrite.dstArrayElement = 0;
        dirDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        dirDescriptorWrite.descriptorCount = 1;
        dirDescriptorWrite.pBufferInfo = &dirBufferInfo;

        std::vector<VkWriteDescriptorSet> descritorWrites{uboDescriptorWrite,
                                                          dirDescriptorWrite};

        vkUpdateDescriptorSets(device,
                               static_cast<uint32_t>(descritorWrites.size()),
                               descritorWrites.data(), 0, nullptr);
    }
}

void PhysicsMesh::createLightingUBOBuffers(std::vector<VkImage> swapChainImages,
                                           VkDevice device,
                                           VkPhysicalDevice physicalDevice) {
    // Create direction buffer
    VkDeviceSize directionalBufferSize = sizeof(directionalLightData);

    directionalLightBuffers.resize(swapChainImages.size());
    directionalLightBuffersMemory.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        createBuffer(directionalBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     directionalLightBuffers[i],
                     directionalLightBuffersMemory[i], device, physicalDevice);
    }
}

void PhysicsMesh::setLightingUBOBuffers(uint32_t currentImage, VkDevice device,
                                        DirectionalLight* dirLight) {
    directionalLightData.color = glm::vec4(dirLight->getColor(), 1.0f);
    directionalLightData.direction = glm::vec4(dirLight->getDirection(), 1.0f);
    directionalLightData.ambientIntensity = dirLight->getAmbientIntensity();
    directionalLightData.diffuseIntensity = dirLight->getDiffuseIntensity();
    // send directional light data
    VkDeviceSize directionalBufferSize = sizeof(directionalLightData);

    void* directionalData;
    vkMapMemory(device, directionalLightBuffersMemory[currentImage], 0,
                directionalBufferSize, 0, &directionalData);
    memcpy(directionalData, &directionalLightData, directionalBufferSize);
    vkUnmapMemory(device, directionalLightBuffersMemory[currentImage]);
}
