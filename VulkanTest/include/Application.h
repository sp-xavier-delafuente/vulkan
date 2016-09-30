#ifndef APPLICATION_H
#define APPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "VertexData.h"
#include "VDeleter.h"
#include "Camera.h"

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
	VDeleter<VkPipeline> graphicsPipelineSolid;
	VDeleter<VkPipeline> graphicsPipelineWireframe;
	std::vector<VDeleter<VkFramebuffer>> swapChainFramebuffers;
	VDeleter<VkCommandPool> commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VDeleter<VkSemaphore> imageAvailableSemaphore;
	VDeleter<VkSemaphore> renderFinishedSemaphore;
	VDeleter<VkImage> textureImage;
	VDeleter<VkDeviceMemory> textureImageMemory;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
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
	VDeleter<VkImage> depthImage;
	VDeleter<VkDeviceMemory> depthImageMemory;
	VDeleter<VkImageView> depthImageView;

	Camera camera;
	glm::vec3 rotation = glm::vec3();
	glm::vec3 cameraPosition = glm::vec3();
	glm::vec2 mousePosition;
	float rotationSpeed = 1.0f;
	float zoomSpeed = 1.0f;
	bool wireframe;
	bool rotateCamera;
	bool panCamera;

	void initWindow();

	void initCamera();

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

	void createDepthResources();

	void createTextureImage();

	void createTextureImageView();

	void createTextureSampler();

	void loadModel();

	void createVertexBuffer();

	void createIndexBuffer();

	void createUniformBuffer();

	void createCommandBuffers();

	void createSemaphores();

	void updateUniformBuffer();

	void createDescriptorPool();

	void createDescriptorSet();

	bool drawFrame();

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

	void createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VDeleter<VkImageView>& imageView);

	void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkFormat findDepthFormat();

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	static void onWindowResized(GLFWwindow* window, int width, int height);

	static void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void onMousePressed(GLFWwindow* window, int button, int action, int mods);

	static void onCursorMoved(GLFWwindow* window, double xpos, double ypos);

	void handlePressedKey(int key, int scancode, int action, int mods);

	void handleMousePressed(int button, int action, int mods);

	void handleCursorMoved(double xpos, double ypos);
};

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

#endif