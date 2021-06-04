#pragma once
#include "../RenderAPI.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include "Helpers.h"
namespace Trickster
{
	struct TricksterModel;
	//TODO:
	//CREATE A DESCRIPTOR LAYOUT / SET MANAGING SYSTEM THAT ALLOWS FOR ONE BIND CALL.
	//ADD A SWAPCHAIN SYSTEM -> should be more dynamic, where I can choose as a end-user what kind of swapchain I want.

	
	//The vertex that will be used in all of my programs

	//TODO: clean the mess up with the descriptors
	// and then pass the instances using a vertex buffer


	//Forget instances...
	//Just make descriptor sets per model, uniform buffers for each model
	//Maybe command buffer for each model? probably not
	
	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};
	
	class Vulkan : public RenderAPI
	{
		friend struct TricksterModel;
	public:
		TRICKSTER_API Vulkan();
		TRICKSTER_API ~Vulkan();
		TRICKSTER_API void Initialize() override;
		TRICKSTER_API void DrawFrame() override;
		TRICKSTER_API void Resize(int width, int height) override;
		TRICKSTER_API void LoadModel(const std::string& a_ModelName) override;
		TRICKSTER_API void DrawModel(const std::string& a_ModelName, const glm::mat4& a_ModelMatrix) override;
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
		TRICKSTER_API void DrawObjects(VkCommandBuffer cmd);
		TRICKSTER_API void UpdateRenderPass(int a_Frame);
		
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
		TRICKSTER_API void SetupDescriptorSetLayout();
		TRICKSTER_API void SetupDescriptorPool();
		TRICKSTER_API void SetupDescriptorSets();
		TRICKSTER_API void SetupSubscriptions();
		TRICKSTER_API void SetupUniformBuffers();
		TRICKSTER_API void UpdateUniformBuffer(uint32_t currentImage);
		TRICKSTER_API void SetupImage(
			uint32_t width, 
			uint32_t height, 
			VkFormat format, 
			VkImageTiling tiling, 
			VkImageUsageFlags usage, 
			VkMemoryPropertyFlags properties,
			uint32_t mipLevels,
			Trickster::TricksterImage& image, 
			VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT);
		TRICKSTER_API void SetupBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		TRICKSTER_API void CopyBuffer(Trickster::TricksterBuffer& srcBuffer, Trickster::TricksterBuffer& dstBuffer, VkDeviceSize size);
		TRICKSTER_API void RecreateSwapChain();
		TRICKSTER_API void CleanSwapChain();
		TRICKSTER_API void CleanBuffer(TricksterBuffer& buffer);
		//Combiner function to make code more readable
		//Returns the shader so that you can change things to it
		TRICKSTER_API Trickster::TricksterShader& AddShader(const std::string& filenameVertexShader, const std::string& filenameFragmentShader);
		TRICKSTER_API std::vector<char> ReadShaderFile(const std::string& filename);
		TRICKSTER_API VkShaderModule CreateShaderModule(const std::vector<char>& code);
		TRICKSTER_API Trickster::TricksterSwapChain QuerySwapChainInfo();
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
		TRICKSTER_API void TransitionImageLayout(
			VkImage image, 
			VkFormat format, 
			VkImageLayout oldLayout, 
			VkImageLayout newLayout,
			uint32_t mipLevels);
		TRICKSTER_API void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		TRICKSTER_API void CleanImage(Trickster::TricksterImage& image);
		TRICKSTER_API void SetupImageView(
			Trickster::TricksterImage& image, 
			VkImageAspectFlags aspectFlags,
			uint32_t mipLevels);
		TRICKSTER_API void SetupTextureSampler();
		TRICKSTER_API void SetupDepthResources();
		TRICKSTER_API VkFormat FindDepthFormat();
		TRICKSTER_API VkFormat FindSupportedImageFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		TRICKSTER_API bool HasStencilComponent(VkFormat format);
		TRICKSTER_API VkSampleCountFlagBits GetMaxUsableSampleCount();
		//Basically for multi sampling color
		TRICKSTER_API void SetupColorResources();
		TRICKSTER_API void GenerateMipmaps(TricksterImage& image, uint32_t mipLevels);

		
		
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

		uint32_t m_CurrentFrame;
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkSampler m_TextureSampler;
		std::vector<TricksterBuffer> m_UniformBuffers;
		//Information about the GPU and has a handle to the vulkan physical device
		TricksterPhysicalDevice m_PhysicalDevice;
		TricksterDevice m_Device;
		TricksterCommand m_Command;
		TricksterWindow m_Window;
		TricksterSwapChain m_SwapChain;
		TricksterPipeline m_Pipeline;
		TricksterDescriptor m_Descriptor;
		//TODO: implement the CameraDescriptor
		TricksterDescriptor m_CameraDescriptor;
		TricksterImage m_DepthImage;
		TricksterImage m_ColorImage;
		VkSampleCountFlagBits m_MSAASamples;
		uint32_t m_MipLevels;
		std::vector<VkRenderPassBeginInfo> m_RenderPassInfo;
		std::vector<TricksterShader> m_Shaders;
		std::string testModel = "planet_Terrestrial1.obj";
		std::vector<const char*> validationLayers;
		std::map<std::string, std::unique_ptr<TricksterModel>> m_Models;
		std::vector<std::pair<TricksterModel*, glm::mat4>> m_Matrices;
	};
}
