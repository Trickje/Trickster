#include "pch.h"
#include "Rendering/RenderAPI/Vulkan.h"


#include "Core/Application.h"
#include "Core/Version.h"

namespace Trickster {
    Trickster::Vulkan::Vulkan()
    {
        validationLayers = {"VK_LAYER_LUNARG_monitor"
#ifdef DETAILED_CONSOLE
        	, "VK_LAYER_LUNARG_api_dump"
#endif
        };
    }

    Trickster::Vulkan::~Vulkan()
    {
        vkDestroyCommandPool(m_Device.get, m_CommandPool.get, nullptr);
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    	for(auto view : m_SwapChain.image_views)
    	{
            vkDestroyImageView(m_Device.get, view, nullptr);
    	}
    	for(auto shader : m_Shaders)
    	{
            vkDestroyShaderModule(m_Device.get, shader.vertex, nullptr);
            vkDestroyShaderModule(m_Device.get, shader.fragment, nullptr);
    	}
        vkDestroySwapchainKHR(m_Device.get, m_SwapChain.get, nullptr);
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
        SetupApp("Trickster Engine");
        //Getting the physical device
        SetupPhysicalDevice();
        LOG(std::string("[Vulkan] Using GPU ") + m_PhysicalDevice.properties.deviceName);
        SetupDevice();
        SetupWindow();
        SetupCommandPool();
        SetupSwapChain();
        SetupGraphicsPipeline();


    	
        //Descriptor stuff that needs to move to it's seperate function that I can call
    	//But should also be optimized to use multiple buffers instead of one
    	//Because otherwise it would make no difference using OpenGL if I don't use the
    	//optimizations that Vulkan has to offer.
    	
        VkDescriptorPool descriptor_pool;
        VkDescriptorSetLayout descriptor_set_layout;
        VkDescriptorSet descriptor_set;
        VkWriteDescriptorSet descriptor_write_set;

        {
            VkDescriptorPoolSize pool_size = {};
            pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pool_size.descriptorCount = 1;

            VkDescriptorPoolCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            create_info.poolSizeCount = 1;
            create_info.pPoolSizes = &pool_size;
        	//For example how many unique textures do you want to bind to the GPU
            create_info.maxSets = 1;

            VkResult create_descriptor_pool = vkCreateDescriptorPool(
                m_Device.get,
                &create_info,
                nullptr,
                &descriptor_pool
                );
        	if(create_descriptor_pool != VK_SUCCESS)
        	{
                LOG_ERROR("[Vulkan] Failed to create descriptor pool.");
        	}
        }
    	//Creating descriptor pool layout
        {
            VkDescriptorSetLayoutBinding layout_binding = {};
        	//This is the layout number. This should not be 0 in the real implementation
            layout_binding.binding = 0;
            layout_binding.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            layout_binding.descriptorCount = 1;
        	//Which shader will the descriptor be visible for
            layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            VkDescriptorSetLayoutCreateInfo layout_info = {};
            layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layout_info.bindingCount = 1;
            layout_info.pBindings = &layout_binding;

            VkResult create_descriptor_result = vkCreateDescriptorSetLayout(
                m_Device.get,
                &layout_info,
                nullptr,
                &descriptor_set_layout
                );
        	if(create_descriptor_result != VK_SUCCESS)
        	{
                LOG_ERROR("[Vulkan] Failed to create descriptor set layout.");
        	}
        }

    	//Create Descriptor set
        {
            VkDescriptorSetAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            alloc_info.descriptorPool = descriptor_pool;
            alloc_info.descriptorSetCount = 1;
            alloc_info.pSetLayouts = &descriptor_set_layout;

            VkResult allocate_result = vkAllocateDescriptorSets(
                m_Device.get,
                &alloc_info,
                &descriptor_set
                );
        	if(allocate_result != VK_SUCCESS)
        	{
                LOG_ERROR("[Vulkan] Failed to allocate descriptor sets");
        	}
        }

        {
            VkDescriptorBufferInfo buffer_info = {};
            buffer_info.buffer = NULL;
            buffer_info.offset = 0;
            buffer_info.range = NULL;

            descriptor_write_set = {};
            descriptor_write_set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write_set.dstSet = descriptor_set;
        	//Id on the shader
        	//Must be the same as the binding of layout_binding
            descriptor_write_set.dstBinding = 0;
            descriptor_write_set.dstArrayElement = 0;
            descriptor_write_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptor_write_set.descriptorCount = 1;
            descriptor_write_set.pBufferInfo = &buffer_info;

            vkUpdateDescriptorSets(
                m_Device.get,
                1,
                &descriptor_write_set,
                0,
                NULL
                );
        	
        }

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
                        m_PhysicalDevice.features = features;
                        m_PhysicalDevice.memory_properties = memory_properties;
                        m_PhysicalDevice.queue_family_index = queue_family_index;
                    }

    			}
    			
    		}
    	}

    	
    }

    void Vulkan::SetupApp(const char* a_ApplicationName)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = a_ApplicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Trickster Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
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
        m_CommandPool.get = command_pool;
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
        auto shader = AddShader("vert.spv", "frag.spv");
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

    	//TODO some stuff here. I left off here, and last line from previous hasn't been saved https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Fixed_functions
    }

	//The filenames are already in the shader directory of Application:: ShaderPath
    Trickster::Vulkan::TricksterShader& Vulkan::AddShader(const std::string& filenameVertexShader, const std::string& filenameFragmentShader)
    {
        m_Shaders.push_back(
            {
            CreateShaderModule(
                ReadShaderFile(
                    Application::Get()->ShaderPath + filenameVertexShader))
            ,
            CreateShaderModule(
                ReadShaderFile(
                    Application::Get()->ShaderPath + filenameFragmentShader))
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

    Vulkan::TricksterSwapChain Vulkan::QuerySwapChainInfo()
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
                info.present_mode = present_mode;
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
}
