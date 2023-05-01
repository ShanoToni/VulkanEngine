#ifndef SHADER_BASE_CLASS
#define SHADER_BASE_CLASS

#include "Mesh.hpp"

#include <fstream>
#include <vector>

class ShaderBase {
  public:
    ShaderBase();
    ShaderBase(const std::vector<Mesh*> meshesToAdd);

    void initShaderPipeline(float W, float H, VkExtent2D swapChainExtent,
                            VkRenderPass renderPass, VkDevice device,
                            VertexData dataType);
    void createDescritorSetLayout(VkDevice device);
    void createDescriptorPool(VkDevice device, int swapChainSize);
    void createDescriptorSet(std::vector<VkImage> swapChainImages,
                             VkDevice device);

    VkPipeline getPipeline() { return ShaderPipeline; }
    inline VkPipelineLayout getPipelineLayout() { return pipelineLayout; }

    inline VkDescriptorPool& getDescriptorPool() { return descriptorPool; }
    inline VkDescriptorSetLayout& getDescriptorSetLayout() {
        return descriptorSetLayout;
    }

    const std::vector<std::unique_ptr<Mesh>>& getMeshes() { return meshes; }
    // inline bool inUse() { return meshes.size() > 0 ? true : false; }

    // virtual ~ShaderBase() = 0;

    void cleanup(VkDevice device);

  private:
    void addMesh(Mesh* mesheToAdd);

    std::vector<std::unique_ptr<Mesh>> meshes;

  protected:
#ifdef __linux__
    std::string vertexShaderPath = "./bin/resources/shaders/vert.spv";
    std::string fragmentShaderPath = "./bin/resources/shaders/frag.spv";
#elif _WIN32
    std::string vertexShaderPath = "bin/Debug/resources/shaders/vert.spv";
    std::string fragmentShaderPath = "bin/Debug/resources/shaders/frag.spv";
#endif
    std::vector<char> readfile(std::string filepath);
    VkShaderModule createShaderModule(const std::vector<char>& code,
                                      VkDevice device);

    VkPipeline ShaderPipeline;
    VkPipelineLayout pipelineLayout;

    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;

    std::string vertPath;
    std::string fragPath;
};

#endif // SHADER_BASE_CLASS