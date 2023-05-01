#ifndef DIRECTIONAL_LIGHT_CLASS
#define DIRECTIONAL_LIGHT_CLASS

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm.hpp>

class DirectionalLight {
  public:
    DirectionalLight();
    DirectionalLight(glm::vec3 col, float aIntensity, float dIntensity,
                     glm::vec3 dir);

    inline glm::vec3 getDirection() { return direction; }
    inline glm::vec3 getColor() { return color; }
    inline float getAmbientIntensity() { return ambientIntensity; }
    inline float getDiffuseIntensity() { return diffuseIntensity; }

  private:
    glm::vec3 direction;
    glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};
#endif // DIRECTIONAL_LIGHT_CLASS