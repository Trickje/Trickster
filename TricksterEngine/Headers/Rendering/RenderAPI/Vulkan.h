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
		//Something with a physical device
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
		//Information about the GPU and has a handle to the vulkan physical device
		TricksterPhysicalDevice m_PhysicalDevice;
		TricksterDevice m_Device;
		TricksterCommandPool m_CommandPool;
		std::vector<const char*> validationLayers;
	};
}
