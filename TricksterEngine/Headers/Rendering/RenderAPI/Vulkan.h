#pragma once
#include "RenderAPI.h"

namespace Trickster
{
	//TODO:
	//CREATE A DESCRIPTOR LAYOUT / SET MANAGING SYSTEM THAT ALLOWS FOR ONE BIND CALL.
	//ADD A SWAPCHAIN SYSTEM
	//CREATE A DRAW QUEUE
	//MANAGE THE COMMAND QUEUE
	//DRAW THE BUFFERS THAT YOU GET PASSED IN
	
	
	class Vulkan : public RenderAPI
	{
	public:
		TRICKSTER_API Vulkan();
		TRICKSTER_API ~Vulkan();
		TRICKSTER_API void Initialize() override;
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

		struct TricksterCommandPool
		{
			VkCommandPool get;
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

		//TODO: maybe a way of changing present mode on runtime
		struct TricksterSwapChain
		{
			VkSwapchainKHR get;
			std::vector<VkImage> images;
			std::vector<VkImageView> image_views;
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
		};

		struct TricksterShader
		{
			VkShaderModule vertex;
			VkShaderModule fragment;
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
		TRICKSTER_API void SetupWindow();
		TRICKSTER_API void SetupSwapChain();
		//You are not able to change the pipeline after this, so make it count.
		TRICKSTER_API void SetupGraphicsPipeline();
		//Combiner function to make code more readable
		//Returns the shader so that you can change things to it
		TRICKSTER_API TricksterShader& AddShader(const std::string& filenameVertexShader, const std::string& filenameFragmentShader);
		TRICKSTER_API std::vector<char> ReadShaderFile(const std::string& filename);
		TRICKSTER_API VkShaderModule CreateShaderModule(const std::vector<char>& code);
		TRICKSTER_API TricksterSwapChain QuerySwapChainInfo();
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
		//Information about the GPU and has a handle to the vulkan physical device
		TricksterPhysicalDevice m_PhysicalDevice;
		TricksterDevice m_Device;
		TricksterCommandPool m_CommandPool;
		TricksterWindow m_Window;
		TricksterSwapChain m_SwapChain;
		std::vector<TricksterShader> m_Shaders;
		std::vector<const char*> validationLayers;
	};
}
