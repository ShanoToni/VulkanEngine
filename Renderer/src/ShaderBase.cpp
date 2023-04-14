#include "ShaderBase.hpp"

ShaderBase::ShaderBase(std::string vertexShaderPath,
                       std::string fragmentShaderPath) {}

ShaderBase::ShaderBase(std::string vertexShaderPath,
                       std::string fragmentShaderPath,
                       std::vector<MeshBase*> meshesToAdd) {}

void ShaderBase::initShaderPipeline(float W, float H,
                                    VkExtent2D swapChainExtent,
                                    VkRenderPass renderPass, VkDevice device) {}

void ShaderBase::createDescritorSetLayout(VkDevice device) {}

void ShaderBase::createDescriptorPool(VkDevice device, int swapChainSize) {}

void ShaderBase::createDescriptorSet(std::vector<VkImage> swapChainImages,
                                     VkDevice device) {}

void ShaderBase::addMesh(MeshBase* mesheToAdd) {}

std::vector<char> ShaderBase::readfile(std::string filepath) {
    return std::vector<char>();
}

VkShaderModule ShaderBase::createShaderModule(const std::vector<char>& code,
                                              VkDevice device) {
    return VkShaderModule();
}
