#ifndef PHYSICS_SHADER_CLASS
#define PHYSICS_SHADER_CLASS

#include "PhysicsMesh.hpp"
#include "ShaderBase.hpp"
#include <fstream>
#include <vector>

class PhysicsShader : public ShaderBase {
  public:
    PhysicsShader();
    PhysicsShader(const std::vector<PhysicsMesh*> meshesToAdd);

    void createDescritorSetLayout(VkDevice device);
    void createDescriptorPool(VkDevice device, int swapChainSize);
    void createDescriptorSet(std::vector<VkImage> swapChainImages,
                             VkDevice device);
    void addMesh(PhysicsMesh* mesheToAdd);

    const std::vector<std::shared_ptr<PhysicsMesh>>& getMeshes() {
        return meshes;
    }

  protected:
    std::vector<std::shared_ptr<PhysicsMesh>> meshes;
};

#endif // PHYSICS_SHADER_CLASS