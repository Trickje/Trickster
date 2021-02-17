#pragma once
#include "RenderAPI.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
namespace Trickster
{
	//TODO:
	//CREATE A DESCRIPTOR LAYOUT / SET MANAGING SYSTEM THAT ALLOWS FOR ONE BIND CALL.
	//ADD A SWAPCHAIN SYSTEM -> should be more dynamic, where I can choose as a end-user what kind of swapchain I want.
	//CREATE A DRAW QUEUE -> is the command queue that we have (instructions that we send to the GPU)
	//MANAGE THE COMMAND QUEUE yeah idk what I imagined here, but lets manage it
	//DRAW THE BUFFERS THAT YOU GET PASSED IN

	//TODO:
	//The previous chapter already mentioned that you should allocate multiple
	//resources like buffers from a single memory allocation,
	//but in fact you should go a step further. Driver developers
	//recommend that you also store multiple buffers,
	//like the vertex and index buffer, into a single VkBuffer
	//and use offsets in commands like vkCmdBindVertexBuffers. 

	
	//The vertex that will be used in all of my programs
	struct TricksterVertex
	{
		glm::vec3 position;
		glm::vec3 color; //Temp
		glm::vec2 texCoord;
		TRICKSTER_API static VkVertexInputBindingDescription  GetBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0; //this might change with multiple arrays
			bindingDescription.stride = sizeof(TricksterVertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //This will change for instanced rendering
			return bindingDescription;
		}
		TRICKSTER_API static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
			//Do this for every element in the vertex
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
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(TricksterVertex, texCoord);
			return attributeDescriptions;
		}
	};

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};
	
	class Vulkan : public RenderAPI
	{
	public:
		TRICKSTER_API Vulkan();
		TRICKSTER_API ~Vulkan();
		TRICKSTER_API void Initialize() override;
		TRICKSTER_API void DrawFrame() override;
		TRICKSTER_API void Resize(int width, int height) override;
	private:

		
		/*
		 * _______ _________ _______           _______ _________ _______ 
		 * (  ____ \\__   __/(  ____ )|\     /|(  ____ \\__   __/(  ____ \
		 * | (    \/   ) (   | (    )|| )   ( || (    \/   ) (   | (    \/
		 * | (_____    | |   | (____)|| |   | || |         | |   | (_____
		 * (_____  )   | |   |     __)| |   | || |         | |   (_____  )
		 *       ) |   | |   | (\ (   | |   | || |         | |         ) |
		 * /\____) |   | |   | ) \ \__| (___) || (____/\   | |   /\____) |
		 * \_______)   )_(   |/   \__/(_______)(_______/   )_(   \_______)
         *
		 */
		
		//Contains information about the users hardware
		struct TricksterPhysicalDevice
		{
			VkPhysicalDevice get;
			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			VkPhysicalDeviceMemoryProperties memory_properties;
			uint32_t queue_family_index;
			static const uint32_t extension_count = 1;
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
		};

		
		
		/*
		 *  _______           _        _______ __________________ _______  _        _______ 
		 *  (  ____ \|\     /|( (    /|(  ____ \\__   __/\__   __/(  ___  )( (    /|(  ____ \
		 *  | (    \/| )   ( ||  \  ( || (    \/   ) (      ) (   | (   ) ||  \  ( || (    \/
		 *  | (__    | |   | ||   \ | || |         | |      | |   | |   | ||   \ | || (_____
		 *  |  __)   | |   | || (\ \) || |         | |      | |   | |   | || (\ \) |(_____  )
		 *  | (      | |   | || | \   || |         | |      | |   | |   | || | \   |      ) |
		 *  | )      | (___) || )  \  || (____/\   | |   ___) (___| (___) || )  \  |/\____) |
		 *  |/       (_______)|/    )_)(_______/   )_(   \_______/(_______)|/    )_)\_______)
         *
		 */

		
		TRICKSTER_API void SetupPhysicalDevice();
		TRICKSTER_API void SetupApp(const char* a_ApplicationName);
		TRICKSTER_API void SetupDevice();
		TRICKSTER_API void SetupCommandPool();
		TRICKSTER_API void SetupCommandBuffers();
		TRICKSTER_API void SetupWindow();
		TRICKSTER_API void SetupSwapChain();
		//You are not able to change the pipeline after this, so make it count.
		TRICKSTER_API void SetupGraphicsPipeline();
		//Because it is Vulkan, this needs to explicitly done
		TRICKSTER_API void SetupFixedFunctions();
		TRICKSTER_API void SetupRenderPass();
		//This should be called after the SwapChain is created
		//And after the Pipeline has been created (uses render pass)
		TRICKSTER_API void SetupFrameBuffers();
		TRICKSTER_API void SetupSync();
		//working on currently TODO: stuff
		TRICKSTER_API void SetupDescriptorSetLayout();
		TRICKSTER_API void SetupDescriptorPool();
		TRICKSTER_API void SetupDescriptorSets();
		TRICKSTER_API void SetupSubscriptions();
		TRICKSTER_API void SetupVertexBuffer();
		TRICKSTER_API void SetupIndexBuffer();
		TRICKSTER_API void SetupUniformBuffers();
		TRICKSTER_API void UpdateUniformBuffer(uint32_t currentImage);
		TRICKSTER_API void SetupImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, TricksterImage& image);
		TRICKSTER_API void SetupBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		TRICKSTER_API void CopyBuffer(TricksterBuffer& srcBuffer, TricksterBuffer& dstBuffer, VkDeviceSize size);
		TRICKSTER_API void RecreateSwapChain();
		TRICKSTER_API void SetupTextureImage();
		TRICKSTER_API void CleanSwapChain();
		TRICKSTER_API void CleanBuffer(TricksterBuffer& buffer);
		//Combiner function to make code more readable
		//Returns the shader so that you can change things to it
		TRICKSTER_API Trickster::Vulkan::TricksterShader& AddShader(const std::string& filenameVertexShader, const std::string& filenameFragmentShader);
		TRICKSTER_API std::vector<char> ReadShaderFile(const std::string& filename);
		TRICKSTER_API VkShaderModule CreateShaderModule(const std::vector<char>& code);
		TRICKSTER_API Trickster::Vulkan::TricksterSwapChain QuerySwapChainInfo();
		/*
		 @Args
		 a_Data: the std::vector.data() that you want to flush
		 a_TypeSize: the size of each element
		 a_Count: the element count
		 */
		TRICKSTER_API void FlushBufferToGPU(void* a_Data, uint64_t a_TypeSize, uint64_t a_Count);
		//vkUnmapMemory(m_Device.get, buffer_memory);
		//vkFreeMemory(m_Device.get, buffer_memory, nullptr);
		
		TRICKSTER_API bool HasExtensions(
			const VkPhysicalDevice& a_PhysicalDevice, 
			const char** extensions, 
			uint32_t extension_count);
		
		TRICKSTER_API bool GetQueueFamily(
			const VkPhysicalDevice& a_PhysicalDevice, 
			VkQueueFlags a_Flags, 
			uint32_t& queue_family_index);

		TRICKSTER_API uint32_t FindMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);
		TRICKSTER_API VkCommandBuffer StartSingleUseCommand();
		TRICKSTER_API void EndSingleUseCommand(VkCommandBuffer commandBuffer);
		TRICKSTER_API void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		TRICKSTER_API void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		TRICKSTER_API void CleanImage(TricksterImage& image);
		TRICKSTER_API void SetupImageView(TricksterImage& image);
		TRICKSTER_API void SetupTextureSampler();

		/*
		 *  _______  _______  _______  ______   _______  _______  _______ 
		 *  (       )(  ____ \(       )(  ___ \ (  ____ \(  ____ )(  ____ \
		 *  | () () || (    \/| () () || (   ) )| (    \/| (    )|| (    \/
		 *  | || || || (__    | || || || (__/ / | (__    | (____)|| (_____
		 *  | |(_)| ||  __)   | |(_)| ||  __ (  |  __)   |     __)(_____  )
		 *  | |   | || (      | |   | || (  \ \ | (      | (\ (         ) |
		 *  | )   ( || (____/\| )   ( || )___) )| (____/\| ) \ \__/\____) |
		 *  |/     \|(_______/|/     \||/ \___/ (_______/|/   \__/\_______)
		 */


		const std::vector<TricksterVertex> vertices = {

	{{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
		};
		const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
		};
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkSampler m_TextureSampler;
		TricksterBuffer m_VertexBuffer;
		TricksterBuffer m_StagingBuffer;
		TricksterBuffer m_IndexBuffer;
		std::vector<TricksterBuffer> m_UniformBuffers;
		//Information about the GPU and has a handle to the vulkan physical device
		TricksterPhysicalDevice m_PhysicalDevice;
		TricksterDevice m_Device;
		TricksterCommand m_Command;
		TricksterWindow m_Window;
		TricksterSwapChain m_SwapChain;
		TricksterPipeline m_Pipeline;
		TricksterDescriptor m_Descriptor;
		TricksterImage m_Image;
		std::vector<TricksterShader> m_Shaders;
		std::vector<const char*> validationLayers;
	};
}
