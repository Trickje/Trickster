#include "pch.h"
#include "Rendering/RenderAPI/Vulkan/Vulkan.h"


#include "Core/Application.h"
#include "Core/Version.h"
#include "Events/EventManager.h"
#include "Rendering/MeshManager.h"
#include "Rendering/Camera.h"
#include <unordered_map>

#include "Core/FilePaths.h"

namespace Trickster {
    Trickster::Vulkan::Vulkan()
        : m_Pipeline()
    {
        validationLayers = {"VK_LAYER_LUNARG_monitor"
#ifdef DETAILED_CONSOLE
        	, "VK_LAYER_LUNARG_api_dump"
#endif
        };
    }

    Trickster::Vulkan::~Vulkan()
    {
        CleanSwapChain();
        vkDestroyDescriptorSetLayout(m_Device.get, m_Descriptor.set_layout, nullptr);
    	for(size_t i = 0; i < m_SwapChain.MAX_FRAMES_IN_FLIGHT; i++)
    	{
            vkDestroySemaphore(m_Device.get, m_SwapChain.semaphores[i].render_finished, nullptr);
            vkDestroySemaphore(m_Device.get, m_SwapChain.semaphores[i].image_available, nullptr);
            vkDestroyFence(m_Device.get, m_SwapChain.fences[i], nullptr);
    	}
        //CleanBuffer(m_VertexBuffer);
        //CleanBuffer(m_IndexBuffer);
        //CleanImage(m_Image);
        vkDestroySampler(m_Device.get, m_TextureSampler, nullptr);
        vkDestroyCommandPool(m_Device.get, m_Command.pool, nullptr);
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    	for(auto shader : m_Shaders)
    	{
            vkDestroyShaderModule(m_Device.get, shader.vertex, nullptr);
            vkDestroyShaderModule(m_Device.get, shader.fragment, nullptr);
    	}
        vkDestroyDevice(m_Device.get, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }
    RenderAPI* RenderAPI::Create()
    {
        return new Vulkan();
    }
    void Trickster::Vulkan::Initialize()
    {
        LOG("Rendering API: Vulkan");
        m_Window.get = nullptr;
        m_Window.get = Trickster::Application::Get()->GetWindow();
        m_Window.glfw = static_cast<GLFWwindow*>(m_Window.get->GetRaw());
    	if(m_Window.get == nullptr)
    	{
            LOG_ERROR("[Vulkan] Window is not made yet. You have to initialize Window before Vulkan.");
    	}
        TricksterModel::owner = this;
        SetupApp("Trickster Engine");
        //Getting the physical device
        SetupPhysicalDevice();
        LOG(std::string("[Vulkan] Using GPU ") + m_PhysicalDevice.properties.deviceName);
        SetupDevice();
        SetupWindow();
        SetupSwapChain();
        SetupDescriptorSetLayout();
        SetupRenderPass();
        SetupGraphicsPipeline();
        SetupCommandPool();
        SetupDepthResources();
        SetupColorResources();
        SetupFrameBuffers();
        SetupTextureSampler();
        SetupUniformBuffers();
    	SetupDescriptorPool();
        SetupDescriptorSets();
        SetupCommandBuffers();
        SetupSync();
        SetupSubscriptions();
        LOG("[Vulkan] Using " + std::to_string(m_MSAASamples) + " samples for multisampled rendering.");
        

    }

	/*****************************************
	 *****************************************
	 The drawing of the frame
	 
	 *****************************************
	 *****************************************/
    void Vulkan::DrawFrame()
    {
        
    	//This is for the v-sync
        vkWaitForFences(m_Device.get, 1, &m_SwapChain.fences[m_SwapChain.currentFrame], VK_TRUE, UINT64_MAX);
    	
        vkAcquireNextImageKHR(m_Device.get, m_SwapChain.get, UINT64_MAX, m_SwapChain.semaphores[m_SwapChain.currentFrame].image_available, VK_NULL_HANDLE, &m_CurrentFrame);

    	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
        if (m_SwapChain.imagesInFlight[m_CurrentFrame] != VK_NULL_HANDLE) {
            vkWaitForFences(m_Device.get, 1, &m_SwapChain.imagesInFlight[m_CurrentFrame], VK_TRUE, UINT64_MAX);
        }
        // Mark the image as now being in use by this frame
        m_SwapChain.imagesInFlight[m_CurrentFrame] = m_SwapChain.fences[m_SwapChain.currentFrame];


        m_RenderPassInfo[m_CurrentFrame].sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        m_RenderPassInfo[m_CurrentFrame].renderPass = m_Pipeline.render_pass;
        m_RenderPassInfo[m_CurrentFrame].framebuffer = m_SwapChain.frame_buffers[m_CurrentFrame];
        m_RenderPassInfo[m_CurrentFrame].renderArea.offset = { 0, 0 };
        m_RenderPassInfo[m_CurrentFrame].renderArea.extent = m_SwapChain.capabilities.currentExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[1].depthStencil = { 1000.f, 0 };

        m_RenderPassInfo[m_CurrentFrame].clearValueCount = static_cast<uint32_t>(clearValues.size());
        m_RenderPassInfo[m_CurrentFrame].pClearValues = clearValues.data();

        vkResetCommandBuffer(m_Command.buffers[m_CurrentFrame], 0);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional
        if (vkBeginCommandBuffer(m_Command.buffers[m_CurrentFrame], &beginInfo) != VK_SUCCESS) {
            LOG_ERROR("[Vulkan] Failed to begin recording CommandBuffer");
        }

        vkCmdBeginRenderPass(m_Command.buffers[m_CurrentFrame], &m_RenderPassInfo[m_CurrentFrame], VK_SUBPASS_CONTENTS_INLINE);


        vkCmdBindPipeline(m_Command.buffers[m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline.get);

        //Drawing the models
        for (auto& it : m_Matrices)
        {
            DrawModelImp(it.modelName, it.modelMatrix);
        }
        vkCmdEndRenderPass(m_Command.buffers[m_CurrentFrame]);

        if (vkEndCommandBuffer(m_Command.buffers[m_CurrentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
        m_Matrices.clear();
    	 VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_SwapChain.semaphores[m_SwapChain.currentFrame].image_available };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_Command.buffers[m_CurrentFrame];

        VkSemaphore signalSemaphores[] = { m_SwapChain.semaphores[m_SwapChain.currentFrame].render_finished };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        vkResetFences(m_Device.get, 1, &m_SwapChain.fences[m_SwapChain.currentFrame]);
        if (vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_SwapChain.fences[m_SwapChain.currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = { m_SwapChain.get };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &m_CurrentFrame;
        presentInfo.pResults = nullptr; // Optional
        VkResult result = vkQueuePresentKHR(m_PresentQueue, &presentInfo);
        vkQueueWaitIdle(m_PresentQueue);
        m_SwapChain.currentFrame = (m_SwapChain.currentFrame + 1) % m_SwapChain.MAX_FRAMES_IN_FLIGHT;

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            RecreateSwapChain();
        }
    }

    void Vulkan::Resize(int width, int height)
    {
        RecreateSwapChain();
    }

    void Vulkan::LoadModel(const std::string& a_ModelPath)
    {
    	if(m_Models.count(a_ModelPath) == 0)
    	{
            m_Models.insert({ a_ModelPath, std::make_unique<TricksterModel>() });
            auto& model = m_Models[a_ModelPath];
            model->Load(a_ModelPath);
            LOG("[Vulkan] Loaded model " + a_ModelPath);
    	}else
    	{
    		//Already loaded
            LOG_USELESS("[Vulkan] Already loaded this model: " + a_ModelPath);
    	}
    }

    void Vulkan::DrawModel(const std::string& a_ModelName, const glm::mat4& a_ModelMatrix)
    {
        m_Matrices.push_back({ a_ModelName, a_ModelMatrix });
    }

    void Vulkan::DrawModelImp(const std::string& a_ModelName, const glm::mat4& a_ModelMatrix)
    {
    	if(m_Models.count(a_ModelName) == 0)
    	{
            LoadModel(a_ModelName);
    	}
        auto& model = m_Models[a_ModelName];
       
    	
        //This is the draw function for a model
        VkBuffer vertexBuffers[] = { model->VertexBuffer.get };
        VkDeviceSize offsets[] = { 0 };
        UniformBufferObject ubo{};

        auto camera = MeshManager::GetInstance()->GetCamera();
        ubo.model = a_ModelMatrix;
        ubo.view = camera->GetView();// glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.projection = camera->GetProjection();//glm::perspective(glm::radians(45.0f), m_SwapChain.capabilities.currentExtent.width / (float)m_SwapChain.capabilities.currentExtent.height, 0.1f, 10.0f);
        
        vkCmdPushConstants(m_Command.buffers[m_CurrentFrame],
            m_Pipeline.layout,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(UniformBufferObject),
            &ubo);
    	
        // Binding point 0: Mesh vertex buffer
        vkCmdBindVertexBuffers(m_Command.buffers[m_CurrentFrame], 0, 1, vertexBuffers, offsets);
     
        

        vkCmdBindIndexBuffer(m_Command.buffers[m_CurrentFrame], model->IndexBuffer.get, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(m_Command.buffers[m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline.layout, 0, 1, &model->descriptor.sets[0], 0, nullptr);
        vkCmdDrawIndexed(m_Command.buffers[m_CurrentFrame], static_cast<uint32_t>(model->indices.size()), 1, 0, 0, 0);
        
    }

   

    void Vulkan::SetupPhysicalDevice()
    {
    	//Populate an array of devices
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance,
            &deviceCount,
            nullptr);
        VkPhysicalDevice* devices = new VkPhysicalDevice[deviceCount];
        vkEnumeratePhysicalDevices(m_Instance,
            &deviceCount,
            devices);

        //Check which one is compatible
    	//Want swapchain extension to be present on the graphics 

        m_PhysicalDevice.get = VK_NULL_HANDLE;
    	
    	//Loop through the physical devices and see which one has the required extensions
    	for(uint32_t i = 0; i < deviceCount; i++)
    	{
    		//Get the one that has the swapchain
    		if(HasExtensions(devices[i], m_PhysicalDevice.extenstions, m_PhysicalDevice.extension_count))
    		{
                uint32_t queue_family_index = 0;
    			//Check if the GPU has the QueueFamilies we want
    			if(GetQueueFamily(devices[i], VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT, queue_family_index))
    			{
                    VkPhysicalDeviceProperties properties;
                    VkPhysicalDeviceFeatures features;
                    VkPhysicalDeviceMemoryProperties memory_properties;
    				//Has the queue family Graphics (the one we need)
    				//Gets the properties and saves them to this Vulkan object (m_Properties)
                    vkGetPhysicalDeviceProperties(
                        devices[i],
                        &properties
                        );
                    vkGetPhysicalDeviceFeatures(
                        devices[i],
                        &features
                        );

                    vkGetPhysicalDeviceMemoryProperties(
                        devices[i],
                        &memory_properties
                        );
                    if(m_PhysicalDevice.get == VK_NULL_HANDLE || properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                    {
                        m_PhysicalDevice.get = devices[i];
                        m_PhysicalDevice.properties = properties;
                        features.samplerAnisotropy = VK_TRUE;
                        m_PhysicalDevice.features = features;
                        m_PhysicalDevice.memory_properties = memory_properties;
                        m_PhysicalDevice.queue_family_index = queue_family_index;
                    }

    			}
    			
    		}
    	}
        m_PhysicalDevice.max_MSAA = GetMaxUsableSampleCount();
        m_MSAASamples = m_PhysicalDevice.max_MSAA;
        m_PhysicalDevice.features.sampleRateShading = VK_TRUE;
    	
    }

    void Vulkan::SetupApp(const char* a_ApplicationName)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = a_ApplicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(TRICKSTER_VERSION_MAJOR, TRICKSTER_VERSION_MINOR, TRICKSTER_VERSION_PATCH);
        appInfo.pEngineName = "Trickster Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(TRICKSTER_VERSION_MAJOR, TRICKSTER_VERSION_MINOR, TRICKSTER_VERSION_PATCH);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("Failed to initialize Vulkan");
        }
    }

    void Vulkan::SetupDevice()
    {
        VkDevice device = VK_NULL_HANDLE;
        //TODO: for multi-threaded object loading onto the GPU, use https://youtu.be/QHKRKotwG0A?t=193 
        static const float queue_priority = 1.f;
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = m_PhysicalDevice.queue_family_index;
        //Maybe for multi-threading change this in the future, how many input channels to the GPU
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;

        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = &queue_create_info;
        create_info.queueCreateInfoCount = 1;
        create_info.pEnabledFeatures = &m_PhysicalDevice.features;
        create_info.enabledExtensionCount = m_PhysicalDevice.extension_count;
        create_info.ppEnabledExtensionNames = m_PhysicalDevice.extenstions;

        VkResult result = vkCreateDevice(
            m_PhysicalDevice.get,
            &create_info,
            nullptr,
            &device
            );
        m_Device.get = device;
        if(result != VK_SUCCESS)
        {
            LOG_ERROR("[Vulkan] Device failed to create.");
        }
        vkGetDeviceQueue(m_Device.get, m_PhysicalDevice.queue_family_index, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device.get, m_PhysicalDevice.queue_family_index, 0, &m_PresentQueue);
    	
    	
    }

    void Vulkan::SetupCommandPool()
    {
        VkCommandPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = m_PhysicalDevice.queue_family_index;

        VkCommandPool command_pool = VK_NULL_HANDLE;
        VkResult result = vkCreateCommandPool(
            m_Device.get, 
            &pool_info, 
            nullptr, 
            &command_pool);

    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Command Pool");
    	}
        m_Command.pool = command_pool;
    }

    void Vulkan::SetupCommandBuffers()
    {
        m_Command.buffers.resize(m_SwapChain.frame_buffers.size());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_Command.pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_Command.buffers.size();
        VkResult ec = vkAllocateCommandBuffers(m_Device.get, &allocInfo, m_Command.buffers.data());
    	if(ec != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to allocate CommandBuffers");
    	}
        m_RenderPassInfo.resize(m_Command.buffers.size());

        

    	
    }

    void Vulkan::SetupWindow()
    {
        VkResult ec = glfwCreateWindowSurface(m_Instance, m_Window.glfw, nullptr, &m_Surface);
    	if(ec != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Could not bind to the GLFW window.");
    	}
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice.get, m_PhysicalDevice.queue_family_index, m_Surface, &presentSupport);
        if(!presentSupport)
        {
            LOG_ERROR("[Vulkan] Graphics card does not support a graphics queue.");
        }
    }

    void Vulkan::SetupSwapChain()
    {
        m_SwapChain = QuerySwapChainInfo();
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_Surface;
        createInfo.minImageCount = m_SwapChain.capabilities.minImageCount + 1;
        createInfo.imageFormat = m_SwapChain.format.format;
        createInfo.imageColorSpace = m_SwapChain.format.colorSpace;
        createInfo.imageExtent = m_SwapChain.capabilities.currentExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
        createInfo.preTransform = m_SwapChain.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = m_SwapChain.present_mode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult ec = vkCreateSwapchainKHR(m_Device.get, &createInfo, nullptr, &m_SwapChain.get);
    	if(ec != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Swapchain.");
    	}
    	//Now just allocate the images in the struct
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(m_Device.get, m_SwapChain.get, &imageCount, nullptr);
        m_SwapChain.images.resize(imageCount);
        vkGetSwapchainImagesKHR(m_Device.get, m_SwapChain.get, &imageCount, m_SwapChain.images.data());

        //Create the image views
        m_SwapChain.image_views.resize(m_SwapChain.images.size());

    	//Loop over the swap chain images
    	for(uint32_t i = 0; i < m_SwapChain.images.size(); i++)
    	{
    		//Create a view for this image
            VkImageViewCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_SwapChain.images[i];
            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = m_SwapChain.format.format;
            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;
            VkResult viewError = vkCreateImageView(m_Device.get, &create_info, nullptr, &m_SwapChain.image_views[i]);
    		if(viewError != VK_SUCCESS)
    		{
                LOG_ERROR("[Vulkan] Failed to create Image View (Swap Chain).");
    		}
    	}

    }
    // https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Introduction 
    void Vulkan::SetupGraphicsPipeline()
    {
        auto shader = AddShader("3DLightvert.spv", "3DLightfrag.spv");
        VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
        vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module = shader.vertex;
        vertexShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
        fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageInfo.module = shader.fragment;
        fragmentShaderStageInfo.pName = "main";

        shader.vertex_info = vertexShaderStageInfo;
        shader.fragment_info = fragmentShaderStageInfo;
    	 VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

         
        VkVertexInputBindingDescription bindingDescription[1] = { TricksterVertex::GetBindingDescription()};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions = TricksterVertex::GetAttributeDescriptions();
    	 VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription[0]; // Optional
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional
        //Input assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;
        //Viewport
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)m_SwapChain.capabilities.currentExtent.width;
        viewport.height = (float)m_SwapChain.capabilities.currentExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        //Scissor
        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_SwapChain.capabilities.currentExtent;

        //Combine the scissor and viewport into a viewport state
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        //Rasterizer
        //Can also make the renderer a skeletal renderer by VK_POLYGON_MODE_LINE
        //But that will require enabling a GPU feature
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_NONE;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        //Multisampling
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_TRUE;
        multisampling.rasterizationSamples = m_MSAASamples;
        multisampling.minSampleShading = 0.2f; // min fraction for sample shading; closer to one is smooth
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        //Color blending
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        //idk re-read the 
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional


        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional
     
        //A limited amount of the state that we've specified in the
        //previous structs can actually be changed without recreating the pipeline. 
        VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates;

        VkPushConstantRange push_constant;
        push_constant.offset = 0;
        push_constant.size = sizeof(UniformBufferObject);
        push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    	
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1; // Optional
        pipelineLayoutInfo.pSetLayouts = &m_Descriptor.set_layout; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
        pipelineLayoutInfo.pPushConstantRanges = &push_constant; // Optional

        VkResult ec = vkCreatePipelineLayout(m_Device.get, &pipelineLayoutInfo, nullptr, &m_Pipeline.layout);
        if (ec != VK_SUCCESS) {
            LOG_ERROR("[Vulkan] Failed to create Pipeline Layout");
        }
    	

    	//Creating the graphics pipeline here
    	//https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Conclusion
    	
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
    	pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.layout = m_Pipeline.layout;
        pipelineInfo.renderPass = m_Pipeline.render_pass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        VkResult error_create_pipeline = vkCreateGraphicsPipelines(m_Device.get, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline.get);
    	if(error_create_pipeline != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Graphics Pipeline");
    	}
    }

    void Vulkan::SetupFixedFunctions()
    {
    	//Vertex input (how will the vertex input look)
    	//TODO: this is very temporary and will be replaced soon by an actual vertex input
      
    }

    TRICKSTER_API void Vulkan::SetupRenderPass()
    {
    	//TODO: Read more about this when creating the actual non-temporary system
        //https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_SwapChain.format.format;
        colorAttachment.samples = m_MSAASamples;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = FindDepthFormat();
        depthAttachment.samples = m_MSAASamples;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = m_SwapChain.format.format;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    	
        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    	
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 2;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    	
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcAccessMask = 0;
    	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VkResult ec = vkCreateRenderPass(m_Device.get, &renderPassInfo, nullptr, &m_Pipeline.render_pass);
    	if(ec != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Render Pass");
    	}

    	
    }

    void Vulkan::SetupFrameBuffers()
    {
        m_SwapChain.frame_buffers.resize(m_SwapChain.image_views.size());
        for (size_t i = 0; i < m_SwapChain.image_views.size(); i++) {
            std::array<VkImageView, 3> attachments = {
            m_ColorImage.view,
            m_DepthImage.view,
            m_SwapChain.image_views[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_Pipeline.render_pass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_SwapChain.capabilities.currentExtent.width;
            framebufferInfo.height = m_SwapChain.capabilities.currentExtent.height;
            framebufferInfo.layers = 1;

            VkResult ec = vkCreateFramebuffer(m_Device.get, &framebufferInfo, nullptr, &m_SwapChain.frame_buffers[i]);
             if(ec != VK_SUCCESS)
             {
                 LOG_ERROR("[Vulkan] Failed to create Framebuffer");
             }
        }
    }

    void Vulkan::SetupSync()
    {
    	//Semaphores and fences
        m_SwapChain.semaphores.resize(m_SwapChain.MAX_FRAMES_IN_FLIGHT);
        m_SwapChain.fences.resize(m_SwapChain.MAX_FRAMES_IN_FLIGHT);
        m_SwapChain.imagesInFlight.resize(m_SwapChain.images.size(), VK_NULL_HANDLE);
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (size_t i = 0; i < m_SwapChain.MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_Device.get, &semaphoreInfo, nullptr, &m_SwapChain.semaphores[i].image_available) != VK_SUCCESS ||
                vkCreateSemaphore(m_Device.get, &semaphoreInfo, nullptr, &m_SwapChain.semaphores[i].render_finished) != VK_SUCCESS ||
                vkCreateFence(m_Device.get, &fenceInfo, nullptr, &m_SwapChain.fences[i]) != VK_SUCCESS) {

                LOG_ERROR("[Vulkan] Failed to create Sync objects for a frame");
            }
        }
    	
    }

    void Vulkan::SetupDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    	// It is possible for the shader variable to represent an array of uniform buffer objects,
    	//and descriptorCount specifies the number of values in the array.
    	//This could be used to specify a transformation for each of the bones in
    	//a skeleton for skeletal animation, for example.
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
    	
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		
    	
    	
        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

    	VkDescriptorSetLayoutCreateInfo layoutInfo{};
    	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();
        VkResult result;
        result = vkCreateDescriptorSetLayout(m_Device.get, &layoutInfo, nullptr, &m_Descriptor.set_layout);
    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Descriptor Set Layout!");
    	}

    	
    }

    void Vulkan::SetupDescriptorPool()
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(m_SwapChain.images.size());
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(m_SwapChain.images.size());

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(m_SwapChain.images.size());


        VkResult result;
        result = vkCreateDescriptorPool(m_Device.get, &poolInfo, nullptr, &m_Descriptor.pool);
    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Descriptor Pool!");
    	}
    	
    }

    void Vulkan::SetupDescriptorSets()
    {
        std::vector<VkDescriptorSetLayout> layouts(m_SwapChain.images.size(), m_Descriptor.set_layout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_Descriptor.pool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(m_SwapChain.images.size());
        allocInfo.pSetLayouts = layouts.data();

        m_Descriptor.sets.resize(m_SwapChain.images.size());
        VkResult result;
        result = vkAllocateDescriptorSets(m_Device.get, &allocInfo, m_Descriptor.sets.data());
    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to allocate Descriptor Sets!");
    	}
        for (size_t i = 0; i < m_SwapChain.images.size(); i++) {
        	
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = m_UniformBuffers[i].get;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);
        	
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        	//Model image view
            int modelcount = 0;
        	for(auto& it : m_Models)
        	{
                imageInfo.imageView = it.second->texture.view;
                modelcount++;
        	}
            imageInfo.sampler = m_TextureSampler;


            std::vector<VkWriteDescriptorSet> descriptorWrites{};
            descriptorWrites.push_back({});
            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = m_Descriptor.sets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites.push_back({});
            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = m_Descriptor.sets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

        
          
            vkUpdateDescriptorSets(m_Device.get,
                static_cast<uint32_t>(descriptorWrites.size()), 
                descriptorWrites.data(), 
                0, 
                nullptr);
        	
           }
    }

    void Vulkan::SetupSubscriptions()
    {
        EventManager::GetInstance()->WindowEvents.OnWindowResize.AddListener(std::bind(&Trickster::Vulkan::Resize, this, std::placeholders::_1, std::placeholders::_2));
    }

    

  

    void Vulkan::SetupUniformBuffers()
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        m_UniformBuffers.resize(m_SwapChain.images.size());
    	for(size_t i = 0; i < m_SwapChain.images.size(); i++)
    	{
            SetupBuffer(bufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_UniformBuffers[i].get, m_UniformBuffers[i].memory);
    	}
    }

    void Vulkan::UpdateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

       // UniformBufferObject ubo{};
        auto camera = MeshManager::GetInstance()->GetCamera();
        glm::mat4 matrices[2];
       // ubo.model = glm::rotate(glm::translate(glm::identity<glm::mat4>(),{0.f,0.f,-4.f}), -1.57079633f, {1.f,0.f,0.f});
       matrices[0]=  camera->GetView();// glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        matrices[1]= camera->GetProjection();//glm::perspective(glm::radians(45.0f), m_SwapChain.capabilities.currentExtent.width / (float)m_SwapChain.capabilities.currentExtent.height, 0.1f, 10.0f);
        matrices[1][1] *= -1;
        void* data;
        vkMapMemory(m_Device.get, m_UniformBuffers[currentImage].memory, sizeof(glm::mat4), sizeof(glm::mat4) * 2, 0, &data);
        memcpy(data, &matrices, sizeof(matrices));
        vkUnmapMemory(m_Device.get, m_UniformBuffers[currentImage].memory);

        }

    void Vulkan::SetupImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
	    VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
        uint32_t mipLevels, Trickster::TricksterImage& image, VkSampleCountFlagBits numSamples)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = numSamples;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image.width = width;
        image.height = height;
        image.format = format;
        VkResult result;
        result = vkCreateImage(m_Device.get, &imageInfo, nullptr, &image.get);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("[Vulkan] Failed to create Image!");
        }
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_Device.get, image.get, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        result = vkAllocateMemory(m_Device.get, &allocInfo, nullptr, &image.memory);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("[Vulkan] Failed to allocate Image Memory!");
        }

        vkBindImageMemory(m_Device.get, image.get, image.memory, 0);
    }

    void Vulkan::SetupBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                             VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkResult result;
        result = vkCreateBuffer(m_Device.get, &bufferInfo, nullptr, &buffer);
    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Buffer!");
    	}
        VkMemoryRequirements memRequirements{};
        vkGetBufferMemoryRequirements(m_Device.get, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        result = vkAllocateMemory(m_Device.get, &allocInfo, nullptr, &bufferMemory);
    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to allocate Buffer Memory!");
    	}
        vkBindBufferMemory(m_Device.get, buffer, bufferMemory, 0);
    }

    void Vulkan::CopyBuffer(TricksterBuffer& srcBuffer, TricksterBuffer& dstBuffer, VkDeviceSize size)
    {
       VkCommandBuffer commandBuffer = StartSingleUseCommand();
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer.get, dstBuffer.get, 1, &copyRegion);
        EndSingleUseCommand(commandBuffer);
    }

    void Vulkan::RecreateSwapChain()
    {
        vkDeviceWaitIdle(m_Device.get);
        CleanSwapChain();

        SetupSwapChain();
        SetupRenderPass();
        SetupGraphicsPipeline();
        SetupColorResources();
        SetupDepthResources();
        SetupFrameBuffers();
        SetupUniformBuffers();
        SetupDescriptorPool();
        SetupDescriptorSets();
        SetupCommandBuffers();
        SetupSync();
    	
    }

   

   


    void Vulkan::CleanSwapChain()
    {
    	//https://vulkan-tutorial.com/Drawing_a_triangle/Swap_chain_recreation
        for (size_t i = 0; i < m_SwapChain.frame_buffers.size(); i++) {
            vkDestroyFramebuffer(m_Device.get, m_SwapChain.frame_buffers[i], nullptr);
        }

        vkFreeCommandBuffers(m_Device.get, m_Command.pool, static_cast<uint32_t>(m_Command.buffers.size()), m_Command.buffers.data());

        vkDestroyPipeline(m_Device.get, m_Pipeline.get, nullptr);
        vkDestroyPipelineLayout(m_Device.get, m_Pipeline.layout, nullptr);
        vkDestroyRenderPass(m_Device.get, m_Pipeline.render_pass, nullptr);

        vkDestroyImageView(m_Device.get, m_DepthImage.view, nullptr);
        vkDestroyImage(m_Device.get, m_DepthImage.get, nullptr);
        vkFreeMemory(m_Device.get, m_DepthImage.memory, nullptr);
        CleanImage(m_ColorImage);
        for (size_t i = 0; i < m_SwapChain.image_views.size(); i++) {
            vkDestroyImageView(m_Device.get, m_SwapChain.image_views[i], nullptr);
        }
        for (size_t i = 0; i < m_UniformBuffers.size(); i++) {
            CleanBuffer(m_UniformBuffers[i]);
        }
        vkDestroyDescriptorPool(m_Device.get, m_Descriptor.pool, nullptr);
        vkDestroySwapchainKHR(m_Device.get, m_SwapChain.get, nullptr);
    }

    void Trickster::Vulkan::CleanBuffer(Trickster::TricksterBuffer& buffer)
    {
        vkDestroyBuffer(m_Device.get, buffer.get, nullptr);
        vkFreeMemory(m_Device.get, buffer.memory, nullptr);
    }

    //The filenames are already in the shader directory of Application:: ShaderPath
    Trickster::TricksterShader& Vulkan::AddShader(const std::string& filenameVertexShader, const std::string& filenameFragmentShader)
    {
        m_Shaders.push_back(
            {
            CreateShaderModule(
                ReadShaderFile(
                    ShaderPath + filenameVertexShader))
            ,
            CreateShaderModule(
                ReadShaderFile(
                    ShaderPath + filenameFragmentShader))
            });
        return m_Shaders[m_Shaders.size() - 1];
    }

    std::vector<char> Vulkan::ReadShaderFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            LOG_ERROR("[Vulkan] Failed to open shader file " + filename);
            return {};
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    VkShaderModule Vulkan::CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shader_module;
        VkResult ec = vkCreateShaderModule(m_Device.get, &create_info, nullptr, &shader_module);
    	if(ec != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Shader Module.");
    	}
        return shader_module;
    }

    Trickster::TricksterSwapChain Vulkan::QuerySwapChainInfo()
    {
        TricksterSwapChain info;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice.get, m_Surface, &info.capabilities);
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice.get, m_Surface, &formatCount, nullptr);

        if (formatCount != 0) {
            formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice.get, m_Surface, &formatCount, formats.data());
        }
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice.get, m_Surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            present_modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice.get, m_Surface, &presentModeCount, present_modes.data());
        }

        if(formats.empty() && present_modes.empty())
        {
            LOG_ERROR("[Vulkan] Trying to bind not suitable SwapChain.");
        }
        info.format = formats[0]; //Just initializing it as failsafe
        //Get the right color formats
    	for(auto& format : formats)
    	{
    		if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    		{
                info.format = format;
    		}
    	}
    	
        

        //Set the present_mode to FIFO so that will be used when Mailbox is not available
        info.present_mode = VK_PRESENT_MODE_FIFO_KHR;
    	//Check if Mailbox is available, if not use FIFO
        for(auto& present_mode : present_modes)
        {
	        if(present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
	        {
                if (!m_SwapChain.vertical_sync) {
                    info.present_mode = present_mode;
                }
	        }
        }


        //Decide the resolution of swap images (probably the screen resolution)
    	//Can also be higher for better visual quality

        int width, height;
        glfwGetFramebufferSize(m_Window.glfw, &width, &height);

        VkExtent2D extent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        extent.width = std::max(info.capabilities.minImageExtent.width, std::min(info.capabilities.maxImageExtent.width, extent.width));
        extent.height = std::max(info.capabilities.minImageExtent.height, std::min(info.capabilities.maxImageExtent.height, extent.height));
        info.capabilities.currentExtent = extent;
        info.get = VK_NULL_HANDLE;
    	return info;
    }

    void Vulkan::FlushBufferToGPU(void* a_Data, uint64_t a_TypeSize, uint64_t a_Count)
    {
        //Creating the buffer
        

        const VkDeviceSize size = a_TypeSize * a_Count;
        VkMemoryPropertyFlags buffer_memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory buffer_memory = VK_NULL_HANDLE;

        VkBufferCreateInfo buffer_info = {};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size;
        buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkResult result = vkCreateBuffer(
            m_Device.get,
            &buffer_info,
            nullptr,
            &buffer
            );
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("[Vulkan] Failed to create Buffer.");
        }

        //Allocating the memory

        VkMemoryRequirements memory_requirements;
        vkGetBufferMemoryRequirements(
            m_Device.get,
            buffer,
            &memory_requirements);
        uint32_t memory_type = FindMemoryType(
            memory_requirements.memoryTypeBits,
            buffer_memory_properties);

        VkMemoryAllocateInfo memory_allocate_info = {};
        memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memory_allocate_info.allocationSize = size;
        memory_allocate_info.memoryTypeIndex = memory_type;

        VkResult memory_Result = vkAllocateMemory(
            m_Device.get,
            &memory_allocate_info,
            nullptr,
            &buffer_memory
            );
        if (memory_Result != VK_SUCCESS)
        {
            LOG_ERROR("[Vulkan] Failed to allocate memory.");
        }


        //Bind the buffer and the memory together
        VkResult result_BufferBind = vkBindBufferMemory(
            m_Device.get,
            buffer,
            buffer_memory,
            //The offset that the buffer is placed on from the memory you give this function access to
            0
            );
        if (result_BufferBind != VK_SUCCESS)
        {
            LOG_ERROR("[Vulkan] Failed to bind buffer to memory.");
        }

        //Map the memory

        void* mapped_memory = nullptr;
        VkResult map_Memory_Result = vkMapMemory(
            m_Device.get,
            buffer_memory,
            0,
            size,
            0,
            &mapped_memory
            );
        if (map_Memory_Result != VK_SUCCESS)
        {
            LOG_ERROR("[Vulkan] Failed to map memory.");
        }

        //Pass the memory over to the GPU
        memcpy(
            mapped_memory,
            a_Data,
            size
            );

    }

    bool Vulkan::HasExtensions(const VkPhysicalDevice& a_PhysicalDevice, const char** extensions,
                               uint32_t extension_count)
    {
        uint32_t device_extension_count = 0;
    	//Gets the amount of extensions, we need that to create an array to populate later on
        vkEnumerateDeviceExtensionProperties(
            a_PhysicalDevice,
            nullptr,
            &device_extension_count,
            nullptr
            );

    	//Making the array
        VkExtensionProperties* device_extensions = new VkExtensionProperties[device_extension_count];

        vkEnumerateDeviceExtensionProperties(
            a_PhysicalDevice,
            nullptr,
            &device_extension_count,
            device_extensions
            );
        //Array has been filled in

        //Loop through the extensions and check that the required ones are there
    	//For every extension we need
    	for(uint32_t i = 0; i < extension_count; i++)
    	{
            bool extension_found = false;
    		//For every extension the GPU has
    		for(uint32_t j = 0; j < device_extension_count; j++)
    		{
    			if(strcmp(extensions[i],device_extensions[j].extensionName) == 0)
    			{
                    extension_found = true;
                    break;
    			}
    		}
    		if(!extension_found)
    		{
                delete[] device_extensions;
    			//If a single extension is missing, just stop comparing.
                return false;
    		}
    	}
        delete[] device_extensions;
        return true;
    }

    bool Vulkan::GetQueueFamily(const VkPhysicalDevice& a_PhysicalDevice, VkQueueFlags a_Flags,
	    uint32_t& queue_family_index)
    {
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(
            a_PhysicalDevice,
            &queue_family_count,
            nullptr);
    	//Now we have the amount of queue families and we can make the array
        VkQueueFamilyProperties* queue_families = new VkQueueFamilyProperties[queue_family_count];


        vkGetPhysicalDeviceQueueFamilyProperties(
            a_PhysicalDevice,
            &queue_family_count,
            queue_families);
    	//Array is populated
        for(uint32_t i = 0; i < queue_family_count; i++)
        {
	        if(queue_families[i].queueCount > 0)
	        {
		        //Check the bit flags
	        	if((queue_families[i].queueFlags & a_Flags) == a_Flags)
	        	{
                    //Has the flags that we need
                    queue_family_index = i;
                    delete[] queue_families;
                    return true;
	        	}
	        }
        }
    	
        delete[] queue_families;
        return false;
    }

    uint32_t Vulkan::FindMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties)
    {
    	for(uint32_t i = 0; i < m_PhysicalDevice.memory_properties.memoryTypeCount; i++)
    	{
    		//Bitwise math to extract the types
    		if(type_filter & (1<<i) && (m_PhysicalDevice.memory_properties.memoryTypes[i].propertyFlags & properties) == properties)
    		{
    			//Return the valid index
                return i;
    		}
    	}
        LOG_ERROR("[Vulkan] Trying to find an invalid memory type.");
        return -1;
    }

    VkCommandBuffer Vulkan::StartSingleUseCommand()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_Command.pool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_Device.get, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void Vulkan::EndSingleUseCommand(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_GraphicsQueue);

        vkFreeCommandBuffers(m_Device.get, m_Command.pool, 1, &commandBuffer);
    }

    void Vulkan::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
	    VkImageLayout newLayout,uint32_t mipLevels)
    {
        VkCommandBuffer commandBuffer = StartSingleUseCommand();
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;
        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (HasStencilComponent(format)) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }else {
            LOG_ERROR("[Vulkan] Unsupported Layout Transition!");
        }
        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
            );
    	
        EndSingleUseCommand(commandBuffer);
    }

    void Vulkan::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        VkCommandBuffer commandBuffer = StartSingleUseCommand();
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };
        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
            );
    	
        EndSingleUseCommand(commandBuffer);
    }

    void Vulkan::CleanImage(Trickster::TricksterImage& image)
    {
        vkDestroyImageView(m_Device.get, image.view, nullptr);
        vkDestroyImage(m_Device.get, image.get, nullptr);
        vkFreeMemory(m_Device.get, image.memory, nullptr);
    }

    void Vulkan::SetupImageView(Trickster::TricksterImage& image, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image.get;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = image.format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        VkResult result;
        result = vkCreateImageView(m_Device.get, &viewInfo, nullptr, &imageView);
    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Image View!");
    	}
        image.view = imageView;
    }

    void Vulkan::SetupTextureSampler()
    {
    	
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
       // samplerInfo.anisotropyEnable = VK_TRUE;
        //samplerInfo.maxAnisotropy = m_PhysicalDevice.properties.limits.maxSamplerAnisotropy;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.f; //increase for lower level of detail
        samplerInfo.maxLod = static_cast<float>(m_MipLevels);
        samplerInfo.mipLodBias = 0.0f; // Optional
        VkResult result;
        result = vkCreateSampler(m_Device.get, &samplerInfo, nullptr, &m_TextureSampler);
    	if(result != VK_SUCCESS)
    	{
            LOG_ERROR("[Vulkan] Failed to create Sampler!");
    	}
    }

    void Vulkan::SetupDepthResources()
    {
        VkFormat depthFormat = FindDepthFormat();
        SetupImage(m_SwapChain.capabilities.currentExtent.width,
            m_SwapChain.capabilities.currentExtent.height,
            depthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,1,
            m_DepthImage, m_MSAASamples
            );
        SetupImageView(m_DepthImage, VK_IMAGE_ASPECT_DEPTH_BIT,1);
       
    }

    VkFormat Vulkan::FindDepthFormat()
    {
        return FindSupportedImageFormat(
            {
                VK_FORMAT_D32_SFLOAT,
                VK_FORMAT_D32_SFLOAT_S8_UINT,
            	VK_FORMAT_D24_UNORM_S8_UINT
            },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );
    }

    VkFormat Vulkan::FindSupportedImageFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                              VkFormatFeatureFlags features)
    {
    	for(VkFormat format : candidates)
    	{
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice.get, format, &props);
            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
    	}
        LOG_ERROR("[Vulkan] Failed to find Supported Image Format!");
        return {};
    }

    bool Vulkan::HasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    VkSampleCountFlagBits Vulkan::GetMaxUsableSampleCount()
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice.get, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

    void Vulkan::SetupColorResources()
    {
        VkFormat colorFormat = m_SwapChain.format.format;

        SetupImage(m_SwapChain.capabilities.currentExtent.width,
            m_SwapChain.capabilities.currentExtent.height, 
            colorFormat, 
            VK_IMAGE_TILING_OPTIMAL, 
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,1, 
            m_ColorImage, m_MSAASamples);
			SetupImageView(m_ColorImage, VK_IMAGE_ASPECT_COLOR_BIT,1);
    }

    void Vulkan::GenerateMipmaps(TricksterImage& image, uint32_t mipLevels)
    {
        // Check if image format supports linear blitting
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice.get, image.format, &formatProperties);

        if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
            LOG_ERROR("texture image format does not support linear blitting!");
        }
    	
        VkCommandBuffer commandBuffer = StartSingleUseCommand();
        
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image.get;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;
        int32_t mipWidth = image.width;
        int32_t mipHeight = image.height;

        for (uint32_t i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                image.get, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                image.get, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);


            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);
        EndSingleUseCommand(commandBuffer);
    }
}
