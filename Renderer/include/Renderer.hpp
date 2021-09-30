#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#define GLFW_INCLUDE_VULKAN
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string.h>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <algorithm>

#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// Validation layers 
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

// Queue family 
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

// Swap Chain
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilites;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


class Renderer{
public:
    Renderer();
    void run();

    ~Renderer();

private:
    void initVulkan();
    void mainLoop();
    void cleanup();

// Vulkan Device Physical
    void pickPhysicalDevice();
    int rateDevice(const VkPhysicalDevice& device);
    bool isDeviceSuitable(const VkPhysicalDevice& device);
    bool checkDeivceExtensionsSupport(const VkPhysicalDevice& device);

    VkPhysicalDevice physicalDevice;
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

// Vulkan Device 
    void createLogicalDevice();

    VkDevice device;

// Queue Families
    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

    VkQueue graphicsQueue;
    VkQueue presentQueue;

// Vulkan instace 
    void createInstance();

    VkInstance instance;

// Window Surface
    void createSurface();

    VkSurfaceKHR surface;

// SwapChain
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilites);
    void createSwapChain();
    void createImageViews();

    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;


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
