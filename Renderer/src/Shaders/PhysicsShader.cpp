#include "PhysicsShader.hpp"

PhysicsShader::PhysicsShader() : ShaderBase() {
#ifdef __linux__
    vertexShaderPath = "./bin/resources/shaders/physics_vert.spv";
    fragmentShaderPath = "./bin/resources/shaders/physics_frag.spv";
#elif _WIN32
    vertexShaderPath = "bin/Debug/resources/shaders/physics_vert.spv";
    fragmentShaderPath = "bin/Debug/resources/shaders/physics_frag.spv";
#endif
}

PhysicsShader::PhysicsShader(const std::vector<PhysicsMesh*> meshesToAdd)
    : ShaderBase() {
    for (auto m : meshesToAdd) {
        meshes.push_back(std::make_shared<PhysicsMesh>(*m));
    }
#ifdef __linux__
    vertexShaderPath = "./bin/resources/shaders/physics_vert.spv";
    fragmentShaderPath = "./bin/resources/shaders/physics_frag.spv";
#elif _WIN32
    vertexShaderPath = "bin/Debug/resources/shaders/physics_vert.spv";
    fragmentShaderPath = "bin/Debug/resources/shaders/physics_frag.spv";
#endif
}

void PhysicsShader::createDescritorSetLayout(VkDevice device) {
    // UBO
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    // Directinal Light
    VkDescriptorSetLayoutBinding dirLightLayoutBinding{};
    dirLightLayoutBinding.binding = 1;
    dirLightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dirLightLayoutBinding.descriptorCount = 1;
    dirLightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    dirLightLayoutBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        uboLayoutBinding, dirLightLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr,
                                    &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void PhysicsShader::createDescriptorPool(VkDevice device, int swapChainSize) {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    // UBO
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount =
        static_cast<uint32_t>((swapChainSize)*meshes.size());
    // Directinal Light
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[1].descriptorCount =
        static_cast<uint32_t>((swapChainSize)*meshes.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    poolInfo.maxSets = static_cast<uint32_t>(swapChainSize * meshes.size());

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void PhysicsShader::createDescriptorSet(std::vector<VkImage> swapChainImages,
                                        VkDevice device) {
    for (auto& mesh : getMeshes()) {
        mesh->createDescriptorSets(swapChainImages, descriptorSetLayout,
                                   descriptorPool, device);
    }
}

void PhysicsShader::addMesh(PhysicsMesh* mesheToAdd) {
    meshes.push_back(std::make_shared<PhysicsMesh>(*mesheToAdd));
}