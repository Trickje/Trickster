#pragma once
#include "Rendering/Window.h"
#include <glm/gtx/hash.hpp>
namespace TE {
	class Vulkan;
	struct TricksterVertex
	{
		glm::vec3 position;
		glm::vec3 color; //for when there is no texture TODO:( in shader: color.a = 1 - texturesampler.a)
		glm::vec3 normal;
		glm::vec2 texCoord;
		TRICKSTER_API static VkVertexInputBindingDescription  GetBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0; //this might change with multiple arrays
			bindingDescription.stride = sizeof(TricksterVertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //This will change for instanced rendering
			return bindingDescription;
		}
		TRICKSTER_API static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
			//Do this for every element in the vertex
			attributeDescriptions.resize(4);
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(TricksterVertex, position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(TricksterVertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(TricksterVertex, normal);

			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[3].offset = offsetof(TricksterVertex, texCoord);
			return attributeDescriptions;
		}
		TRICKSTER_API bool operator==(const TricksterVertex& other) const {
			return position == other.position && color == other.color && texCoord == other.texCoord;
		}

	};
	//Contains information about the users hardware
	struct TricksterPhysicalDevice
	{
		VkPhysicalDevice get;
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
		VkPhysicalDeviceMemoryProperties memory_properties;
		uint32_t queue_family_index;
		static const uint32_t extension_count = 1;
		VkSampleCountFlagBits max_MSAA;
		const char* extenstions[extension_count] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};

	struct TricksterDevice
	{
		VkDevice get;
	};

	struct TricksterCommand
	{
		VkCommandPool pool;
		std::vector<VkCommandBuffer> buffers;
	};

	//Somewhere there should be a buffer pool
	//I'm too tired to find out where or what though
	struct TricksterBuffer
	{
		VkBuffer get;
		VkDeviceMemory memory;
	};

	struct TricksterWindow
	{
		std::shared_ptr<Window> get;
		GLFWwindow* glfw = nullptr;
	};
	struct TricksterSemaphores
	{
		VkSemaphore image_available;
		VkSemaphore render_finished;
	};
	//TODO: maybe a way of changing present mode on runtime
	struct TricksterSwapChain
	{
		VkSwapchainKHR get;
		std::vector<VkImage> images;
		std::vector<VkImageView> image_views;
		std::vector<TricksterSemaphores> semaphores;
		std::vector<VkFence> fences;
		std::vector<VkFence> imagesInFlight;
		VkSurfaceCapabilitiesKHR capabilities;
		VkSurfaceFormatKHR format;
		//The presentation mode will either be vertical sync
		//VK_PRESENT_MODE_FIFO_KHR
		//`
		//or triple buffering, aka mailbox (when the queue is full, instead of blocking it will replace the queued images with newer ones)
		//so no tearing and less latency than the standard vertical sync
		//VK_PRESENT_MODE_MAILBOX_KHR
		//`
		//VK_PRESENT_MODE_IMMEDIATE_KHR to disable vertical sync
		//Will result in tearing though
		VkPresentModeKHR present_mode;
		std::vector<VkFramebuffer> frame_buffers;
		//How many frames should be processed concurrently
		int MAX_FRAMES_IN_FLIGHT = 2;
		size_t currentFrame = 0;
		bool vertical_sync = true;

	};

	struct TricksterShader
	{
		VkShaderModule vertex;
		VkShaderModule fragment;

		//Maybe temporary, who knows
		VkPipelineShaderStageCreateInfo vertex_info;
		VkPipelineShaderStageCreateInfo fragment_info;
	};

	struct TricksterPipeline
	{
		VkPipeline get;
		VkRenderPass render_pass;
		VkPipelineLayout layout;
	};

	//This will tell the shader how our vertex buffer will look
	struct TricksterVertexLayout
	{
		VkVertexInputBindingDescription binding_description;
	};

	struct TricksterDescriptor
	{
		VkDescriptorSetLayout set_layout;
		VkDescriptorPool pool;
		std::vector<VkDescriptorSet> sets;
		void Initialize(VkDevice a_Device, VkBuffer a_UniformBuffer);
		//more stuff to be added here
		
	};
	struct TricksterImage
	{
		VkImage get;
		VkDeviceMemory memory;
		VkImageView view;
		VkFormat format;
		int width;
		int height;
		int channels;

		TRICKSTER_API VkResult Load(const std::string a_ImagePath, Vulkan* a_Vulkan);
		TricksterDescriptor descriptor;
		std::string path;
	};

	// Contains the instanced data
	struct InstanceBuffer {
		TricksterBuffer buffer = {};
		size_t size = 0;
		VkDescriptorBufferInfo descriptor;
	};

	
	struct TricksterModel
	{
		friend class Vulkan;

		//Model information
		std::vector<TricksterVertex> vertices;
		std::vector<uint32_t> indices;
		TricksterBuffer VertexBuffer;
		TricksterBuffer IndexBuffer;
		std::string texturePath;
		TricksterImage texture;

		TRICKSTER_API void Load(std::string a_ModelPath);
		TRICKSTER_API void SetupVertexBuffer();
		TRICKSTER_API void SetupIndexBuffer();
		TRICKSTER_API void SetupTextureImage(std::string a_TexturePath);
		TRICKSTER_API void Draw(const VkCommandBuffer& CommandBuffer, const VkDescriptorSet& DescriptorSet);
		
		static Vulkan* owner;
	};

	struct TricksterSprite
	{
		friend class Vulkan;
		static Vulkan* owner;

		TricksterImage texture;
		TricksterBuffer VertexBuffer;
		TricksterBuffer IndexBuffer;
		TRICKSTER_API void Setup(const std::string a_TexturePath);
		TRICKSTER_API void LoadTexture(const std::string a_TexturePath);
		TRICKSTER_API void SetupVertexBuffer();
		TRICKSTER_API void SetupIndexBuffer();
		TRICKSTER_API void Draw(const VkCommandBuffer& CommandBuffer, const VkDescriptorSet& DescriptorSet);
	};
}
