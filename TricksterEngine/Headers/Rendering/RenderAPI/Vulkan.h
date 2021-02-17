#pragma once
#include "RenderAPI.h"

namespace Trickster
{
	//TODO:
	//CREATE A DESCRIPTOR LAYOUT / SET MANAGING SYSTEM THAT ALLOWS FOR ONE BIND CALL.
	//ADD A SWAPCHAIN SYSTEM -> should be more dynamic, where I can choose as a end-user what kind of swapchain I want.
	//CREATE A DRAW QUEUE -> is the command queue that we have (instructions that we send to the GPU)
	//MANAGE THE COMMAND QUEUE yeah idk what I imagined here, but lets manage it
	//DRAW THE BUFFERS THAT YOU GET PASSED IN
	
	
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
		TRICKSTER_API void SetupSubscriptions();
		TRICKSTER_API void RecreateSwapChain();
		TRICKSTER_API void CleanSwapChain();
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


		
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		//Information about the GPU and has a handle to the vulkan physical device
		TricksterPhysicalDevice m_PhysicalDevice;
		TricksterDevice m_Device;
		TricksterCommand m_Command;
		TricksterWindow m_Window;
		TricksterSwapChain m_SwapChain;
		TricksterPipeline m_Pipeline;
		std::vector<TricksterShader> m_Shaders;
		std::vector<const char*> validationLayers;
	};
}
