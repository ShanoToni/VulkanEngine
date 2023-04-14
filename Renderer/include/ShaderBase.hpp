#ifndef SHADER_BASE_CLASS
#define SHADER_BASE_CLASS

#include "Mesh.hpp"

#include <fstream>
#include <vector>

class ShaderBase {
  public:
    ShaderBase(std::string vertexShaderPath, std::string fragmentShaderPath);
    ShaderBase(std::string vertexShaderPath, std::string fragmentShaderPath,
               std::vector<MeshBase*> meshesToAdd);

    void initShaderPipeline(float W, float H, VkExtent2D swapChainExtent,
                            VkRenderPass renderPass, VkDevice device);
    void createDescritorSetLayout(VkDevice device);
    void createDescriptorPool(VkDevice device, int swapChainSize);
    void createDescriptorSet(std::vector<VkImage> swapChainImages,
                             VkDevice device);

    void addMesh(MeshBase* mesheToAdd);

    // std::vector<MeshBase*> getMeshes();
    VkPipeline getPipeline() { return ShaderPipeline; }
    inline VkPipelineLayout getPipelineLayout() { return pipelineLayout; }

    inline VkDescriptorPool& getDescriptorPool() { return descriptorPool; }
    inline VkDescriptorSetLayout& getDescriptorSetLayout() {
        return descriptorSetLayout;
    }

    // inline bool inUse() { return meshes.size() > 0 ? true : false; }

    virtual ~ShaderBase() = 0;

  private:
    std::vector<char> readfile(std::string filepath);
    VkShaderModule createShaderModule(const std::vector<char>& code,
                                      VkDevice device);

    VkPipeline ShaderPipeline;
    VkPipelineLayout pipelineLayout;

    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;

    std::string vertPath;
    std::string fragPath;
    // std::vector<<MeshBase*> meshes;
};

#endif // SHADER_BASE_CLASS