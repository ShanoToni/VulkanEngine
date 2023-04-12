#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#define GLFW_INCLUDE_VULKAN
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string.h>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

// Validation layers
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescriptor() {
        VkVertexInputBindingDescription bindingDescriptor{};
        bindingDescriptor.binding = 0;
        bindingDescriptor.stride = sizeof(Vertex);
        bindingDescriptor.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptor;
    }
    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescription() {
        std::array<VkVertexInputAttributeDescription, 3>
            attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

#ifdef NDEBUG
const bool enableVailidationLayers = false;
#else
const bool enableVailidationLayers = true;
#endif

static VkResult CreateDebugUtilsMessengerExt(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerExt");
    if (func) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void
DestroyDebugUtilsMessengerExt(VkInstance instance,
                              const VkAllocationCallbacks* pAllocator,
                              VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerExt");
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

class Renderer {
  public:
    Renderer();
    void run();

    ~Renderer();

  private:
    void initVulkan();
    void mainLoop();
    void drawFrame();
    void cleanup();

    // Vulkan Device Physical
    void pickPhysicalDevice();
    int rateDevice(const VkPhysicalDevice& device);
    bool isDeviceSuitable(const VkPhysicalDevice& device);
    bool checkDeivceExtensionsSupport(const VkPhysicalDevice& device);

    VkPhysicalDevice physicalDevice;
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR choosePresentMode(
        const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilites);
    void createSwapChain();
    void recreateSwapChain();
    void cleanupSwapchain();

    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    // Graphics Pipeline
    void createGraphicsPipeline();
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    // Render Pass
    void createRenderPass();
    VkRenderPass renderPass;
    // UBO descriptor
    void createDescriptorSetLayout();

    // Framebuffers
    void createFrameBuffers();

    std::vector<VkFramebuffer> swapChainFrameBuffers;

    // Command Buffers
    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex);

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    // Vertex Buffers
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);
    void createVertexBuffer();
    void createIndexBuffer();

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    // Descriptor pool
    void createDescriptorPool();

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    // Image Loader
    void createTextureImage();
    void createImage(uint32_t W, uint32_t H, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkImage& image,
                     VkDeviceMemory& imageMemory);

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    void createImageViews();
    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags);
    void createTextureImageView();

    VkImageView textureImageView;

    void createTextureSampler();

    VkSampler textureSampler;

    // depth
    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    // Helpers
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void transitionImageLayout(VkImage image, VkFormat format,
                               VkImageLayout oldLayout,
                               VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                           uint32_t height);

    // Descriptor sets
    void createDescriptorSets();

    // Uniform buffers
    void createUniformBuffers();
    void updateUniformBuffer(size_t currentImage);

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    // Memory requirements
    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);

    // Synchronization
    void createSyncObjects();

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame;

  public:
    inline void setFrameBufferResized(bool var) { frameBufferResized = var; }

  private:
    bool frameBufferResized;

    // Validation layers
    bool checkValidationLayerSupport();
    std::vector<const char*> glfwGetRequiredExtensions();
    void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();

    VkDebugUtilsMessengerEXT debugMessenger;

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                  void* pUserData) {
        if (messageSeverity >=
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            std::cerr << "Validation layer: " << pCallbackData->pMessage
                      << std::endl;
        }

        return VK_FALSE;
    }

    // GLFW
    struct DestroyglfwWin {
        void operator()(GLFWwindow* ptr) { glfwDestroyWindow(ptr); }
    } windowDestroy;
    std::unique_ptr<GLFWwindow, DestroyglfwWin> window;
};

static void framebufferResizeCallback(GLFWwindow* window, int width,
                                      int height) {
    auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
    app->setFrameBufferResized(true);
}

#endif // RENDERER_CLASS
