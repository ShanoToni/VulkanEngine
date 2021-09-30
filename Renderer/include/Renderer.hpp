#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#define GLFW_INCLUDE_VULKAN
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string.h>

#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableVailidationLayers = false;
#else
    const bool enableVailidationLayers = true;
#endif

static VkResult CreateDebugUtilsMessengerExt(VkInstance instance,
     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
     const VkAllocationCallbacks* pAllocator,
     VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) 
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerExt");
        if(func) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
}

static void DestroyDebugUtilsMessengerExt(VkInstance instance,
     const VkAllocationCallbacks* pAllocator,
     VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerExt");
}


class Renderer{
public:
    Renderer();
    void run();

    ~Renderer();

private:
    void initVulkan();
    void mainLoop();
    void cleanup();

// Vulkan setup 
    void createInstance();
// Vulkan variables
    VkInstance instance;


// Validation layers
    bool checkValidationLayerSupport();
    std::vector<const char*> glfwGetRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

    VkDebugUtilsMessengerEXT debugMessenger;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) { 
            if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            {
                std::cerr<< "Validation layer: " << pCallbackData->pMessage <<std::endl;
            }

            return VK_FALSE;
        }

// GLFW
    struct DestroyglfwWin{
        void operator()(GLFWwindow* ptr){
            glfwDestroyWindow(ptr);
        }
    } windowDestroy;
    std::unique_ptr<GLFWwindow, DestroyglfwWin> window;
};

#endif //RENDERER_CLASS
