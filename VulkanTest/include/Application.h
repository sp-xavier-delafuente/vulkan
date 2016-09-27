#ifndef APPLICATION_H
#define APPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "VDeleter.h"

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Application
{
public:
	Application();

	void run();

private:
	GLFWwindow* window;
	VDeleter<VkInstance> instance;
	VDeleter<VkDebugReportCallbackEXT> debugReportCallback;
	VkPhysicalDevice physicalDevice;
	VDeleter<VkDevice> device;
	VDeleter<VkSurfaceKHR> surface;
	VDeleter<VkSwapchainKHR> swapChain;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	const std::vector<const char*> validationLayers;
	const std::vector<const char*> deviceExtensions;
	std::vector<VkImage> swapChainImages;
	const bool enableValidationLayers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VDeleter<VkImageView>> swapChainImageViews;
	VDeleter<VkDescriptorSetLayout> descriptorSetLayout;
	VDeleter<VkPipelineLayout> pipelineLayout;
	VDeleter<VkRenderPass> renderPass;
	VDeleter<VkPipeline> graphicsPipeline;
	std::vector<VDeleter<VkFramebuffer>> swapChainFramebuffers;
	VDeleter<VkCommandPool> commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VDeleter<VkSemaphore> imageAvailableSemaphore;
	VDeleter<VkSemaphore> renderFinishedSemaphore;
	VDeleter<VkImage> textureImage;
	VDeleter<VkDeviceMemory> textureImageMemory;
	VDeleter<VkBuffer> vertexBuffer;
	VDeleter<VkDeviceMemory> vertexBufferMemory;
	VDeleter<VkBuffer> indexBuffer;
	VDeleter<VkDeviceMemory> indexBufferMemory;
	VDeleter<VkBuffer> uniformStagingBuffer;
	VDeleter<VkDeviceMemory> uniformStagingBufferMemory;
	VDeleter<VkBuffer> uniformBuffer;
	VDeleter<VkDeviceMemory> uniformBufferMemory;
	VDeleter<VkDescriptorPool> descriptorPool;
	VkDescriptorSet descriptorSet;
	VDeleter<VkImageView> textureImageView;
	VDeleter<VkSampler> textureSampler;

	void initWindow();

	void initVulkan();

	void createInstance();

	void setupDebugCallback();

	void createSurface();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSwapChain();

	void recreateSwapChain();

	void createImageViews();

	void createRenderPass();

	void createDescriptorSetLayout();

	void createGraphicsPipeline();

	void createFramebuffers();

	void createCommandPool();

	void createTextureImage();

	void createTextureImageView();

	void createTextureSampler();

	void createVertexBuffer();

	void createIndexBuffer();

	void createUniformBuffer();

	void createCommandBuffers();

	void createSemaphores();

	void updateUniformBuffer();

	void createDescriptorPool();

	void createDescriptorSet();

	void drawFrame();

	std::vector<const char*> getRequiredExtensions();

	bool checkValidationLayerSupport();

	bool isDeviceSuitable(VkPhysicalDevice device);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);

	void mainLoop();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory);

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkImage>& image, VDeleter<VkDeviceMemory>& imageMemory);

	void transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

	void createImageView(VkImage image, VkFormat format, VDeleter<VkImageView>& imageView);

	void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	static void onWindowResized(GLFWwindow* window, int width, int height);
};

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

#endif