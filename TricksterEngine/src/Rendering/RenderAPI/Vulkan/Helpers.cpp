#include "pch.h"
#include "Rendering/RenderAPI/Vulkan/Helpers.h"

#include "Rendering/RenderAPI/Vulkan/Vulkan.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader.h"
#include "Core/FilePaths.h"

using namespace TE;
    Vulkan* TricksterModel::owner = 0;
    Vulkan* TricksterSprite::owner = 0;

void TricksterDescriptor::Initialize(VkDevice a_Device, VkBuffer a_UniformBuffer)
{
    //ALLOCATE DESCRIPTOR SETS
    VkBuffer localBuffer;
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(glm::mat4);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result;
    result = vkCreateBuffer(a_Device, &bufferInfo, nullptr, &localBuffer);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("[Vulkan] Failed to create Buffer!");
    }
    VkMemoryRequirements memRequirements{};
    vkGetBufferMemoryRequirements(a_Device, localBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    	
    //TODO: left off here, this will be a single matrix descriptor set.
    /*  allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    result = vkAllocateMemory(m_Device.get, &allocInfo, nullptr, &bufferMemory);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("[Vulkan] Failed to allocate Buffer Memory!");
    }
    vkBindBufferMemory(m_Device.get, buffer, bufferMemory, 0);

    */
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 10;
        
    if (vkCreateDescriptorPool(a_Device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = 2;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.descriptorCount = 1;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;

    if (vkCreateDescriptorSetLayout(a_Device, &layoutInfo, nullptr, &set_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
    	
    //What I'll need: pool and setlayouts
    	
    // VkDescriptorSetAllocateInfo allocInfo{};
    /*allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &set_layout;

    sets.resize(1);
    VkResult result;
    result = vkAllocateDescriptorSets(a_Device, &allocInfo, sets.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("[Vulkan] Failed to allocate Descriptor Sets!");
    }
    */
    for (size_t i = 0; i < 1; i++) {

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = a_UniformBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(glm::mat4);
            

        std::vector<VkWriteDescriptorSet> descriptorWrites{};
        descriptorWrites.push_back({});
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = sets[0];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;
            



        vkUpdateDescriptorSets(a_Device,
            static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(),
            0,
            nullptr);

    }
}


VkResult TricksterImage::Load(const std::string a_ImagePath, Vulkan* a_Vulkan)
{
    int texWidth, texHeight, texChannels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* pixels = stbi_load((TexturePath + a_ImagePath).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    path = a_ImagePath;
    a_Vulkan->m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    if (!pixels)
    {
        LOG_ERROR("[Vulkan] Failed to load Texture Image");
    }
    TricksterBuffer StagingBuffer = {};
    a_Vulkan->SetupBuffer(
        imageSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        StagingBuffer.get,
        StagingBuffer.memory);

    void* data;
    vkMapMemory(a_Vulkan->m_Device.get, StagingBuffer.memory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(a_Vulkan->m_Device.get, StagingBuffer.memory);
    stbi_image_free(pixels);
    channels = texChannels;
    a_Vulkan->SetupImage(texWidth,
        texHeight,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        a_Vulkan->m_MipLevels,
        *this);
    a_Vulkan->TransitionImageLayout(get, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, a_Vulkan->m_MipLevels);
    a_Vulkan->CopyBufferToImage(StagingBuffer.get, get, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    // TransitionImageLayout(m_Image.get, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,m_MipLevels);
    a_Vulkan->CleanBuffer(StagingBuffer);
    a_Vulkan->SetupImageView(*this, VK_IMAGE_ASPECT_COLOR_BIT, a_Vulkan->m_MipLevels);

    a_Vulkan->GenerateMipmaps(*this, a_Vulkan->m_MipLevels);
    //Create descriptors



    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;




    std::array<VkDescriptorSetLayoutBinding, 1> bindings = { samplerLayoutBinding };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    VkResult result;
    result = vkCreateDescriptorSetLayout(a_Vulkan->m_Device.get, &layoutInfo, nullptr, &descriptor.set_layout);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("[Vulkan] Failed to create Descriptor Set Layout!");
    }
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;


    result = vkCreateDescriptorPool(a_Vulkan->m_Device.get, &poolInfo, nullptr, &descriptor.pool);
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("[Vulkan] Failed to create Descriptor Pool!");
    }

    std::vector<VkDescriptorSetLayout> layouts(1, descriptor.set_layout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptor.pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layouts.data();

    descriptor.sets.resize(1);
    result = vkAllocateDescriptorSets(a_Vulkan->m_Device.get, &allocInfo, descriptor.sets.data());
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("[Vulkan] Failed to allocate Descriptor Sets!");
    }


    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //Model image view

    imageInfo.imageView = view;

    imageInfo.sampler = a_Vulkan->m_TextureSampler;


    std::vector<VkWriteDescriptorSet> descriptorWrites{};

    descriptorWrites.push_back({});
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptor.sets[0];
    descriptorWrites[0].dstBinding = 1;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pImageInfo = &imageInfo;



    vkUpdateDescriptorSets(a_Vulkan->m_Device.get,
        static_cast<uint32_t>(descriptorWrites.size()),
        descriptorWrites.data(),
        0,
        nullptr);
    return result;
}

void TricksterModel::Load(std::string a_ModelPath)
{

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (ModelPath + a_ModelPath).c_str(), ModelPath.c_str())) {
        LOG_ERROR(warn + err);
    }
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            TricksterVertex vertex{};
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };
            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]

            };

            vertex.color = { 1.0f, 1.0f, 1.0f };
            vertices.push_back(vertex);

            indices.push_back(vertices.size() - 1);
        }
    }
    for(auto material: materials)
    {

        SetupTextureImage(material.diffuse_texname);
    }
    SetupVertexBuffer();
    SetupIndexBuffer();
}
void TricksterModel::SetupVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    TricksterBuffer StagingBuffer;
    StagingBuffer = {};
    VertexBuffer = {};


    owner->SetupBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        StagingBuffer.get,
        StagingBuffer.memory);

    void* data;
    vkMapMemory(owner->m_Device.get, StagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(owner->m_Device.get, StagingBuffer.memory);

    owner->SetupBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VertexBuffer.get,
        VertexBuffer.memory);

    owner->CopyBuffer(StagingBuffer, VertexBuffer, bufferSize);
    owner->CleanBuffer(StagingBuffer);
}

//https://vulkan-tutorial.com/Vertex_buffers/Index_buffer 
void TricksterModel::SetupIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    TricksterBuffer StagingBuffer = {};

    owner->SetupBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        StagingBuffer.get,
        StagingBuffer.memory);

    void* data;
    vkMapMemory(owner->m_Device.get, StagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(owner->m_Device.get, StagingBuffer.memory);

    owner->SetupBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        IndexBuffer.get,
        IndexBuffer.memory);

    owner->CopyBuffer(StagingBuffer, IndexBuffer, bufferSize);

    owner->CleanBuffer(StagingBuffer);
}
void TricksterModel::SetupTextureImage(std::string a_TexturePath)
{
    if (texture.Load(a_TexturePath, owner) != VK_SUCCESS)
    {
        LOG_WARNING("[TricksterModel] Failed to load texture from file:" + a_TexturePath);
    }
}

void TricksterModel::Draw(const VkCommandBuffer& CommandBuffer, const VkDescriptorSet& DescriptorSet)
{
    	
    //This is the draw function for a model
    VkBuffer vertexBuffers[] = { VertexBuffer.get };
    VkDeviceSize offsets[] = { 0 };

    // Binding point 0: Mesh vertex buffer
    vkCmdBindVertexBuffers(CommandBuffer, 0, 1, vertexBuffers, offsets);

    // Binding point 1 : Instance data buffer
    //vkCmdBindVertexBuffers(CommandBuffer, 1, 1, &instances->buffer.get, offsets);

    	
    vkCmdBindIndexBuffer(CommandBuffer, IndexBuffer.get, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, owner->m_Pipeline.layout, 0, 1, &DescriptorSet, 0, nullptr);
    //Very temporary
    vkCmdDrawIndexed(CommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        
    //LOG_USELESS("[Vulkan] Drawn model");
    //end draw function of a model
}

void TricksterSprite::Setup(const std::string a_TexturePath)
{
    if (!owner)
    {
        LOG_ERROR("[TricksterSprite] owner is NULL!");
    }
    LoadTexture(a_TexturePath);
    SetupVertexBuffer();
    SetupIndexBuffer();
}

void TricksterSprite::LoadTexture(const std::string a_TexturePath)
{
    if (texture.Load(a_TexturePath, owner) != VK_SUCCESS)
    {
        LOG_WARNING("[TricksterSprite] Failed to load texture from file:" + a_TexturePath);
    }
}

void TricksterSprite::SetupVertexBuffer()
{
    TricksterVertex vertices[4] = {};
    vertices[0].position = { 0.f, 0.f, 0.f };   //Top left
    vertices[1].position = { 1.f, 0.f, 0.f };   //Top Right
    vertices[2].position = { 1.f, 1.f, 0.f };   //Bottom Right
    vertices[3].position = { 0.f, 1.f, 0.f };   //Bottom left

    vertices[0].color = { 1.f,1.f,1.f };   //Top left
    vertices[1].color = { 1.f,1.f,1.f };   //Top Right
    vertices[2].color = { 1.f,1.f,1.f };   //Bottom Right
    vertices[3].color = { 1.f,1.f,1.f };   //Bottom left

    vertices[0].normal = { 0.f,0.f,0.f };   //Top left
    vertices[1].normal = { 0.f,0.f,0.f };   //Top Right
    vertices[2].normal = { 0.f,0.f,0.f };   //Bottom Right
    vertices[3].normal = { 0.f,0.f,0.f };   //Bottom left

    vertices[0].texCoord = { 0.f, 0.f };   //Top left
    vertices[1].texCoord = { 1.f, 0.f};   //Top Right
    vertices[2].texCoord = { 1.f, 1.f};   //Bottom Right
    vertices[3].texCoord = { 0.f, 1.f};   //Bottom left

    VkDeviceSize bufferSize = sizeof(vertices[0]) * 4;

    TricksterBuffer StagingBuffer;
    StagingBuffer = {};
    VertexBuffer = {};


    owner->SetupBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        StagingBuffer.get,
        StagingBuffer.memory);

    void* data;
    vkMapMemory(owner->m_Device.get, StagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data, &vertices[0], (size_t)bufferSize);
    vkUnmapMemory(owner->m_Device.get, StagingBuffer.memory);

    owner->SetupBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VertexBuffer.get,
        VertexBuffer.memory);

    owner->CopyBuffer(StagingBuffer, VertexBuffer, bufferSize);
    owner->CleanBuffer(StagingBuffer);
}

void TricksterSprite::SetupIndexBuffer()
{
    uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
    VkDeviceSize bufferSize = sizeof(indices[0]) * 6;

    TricksterBuffer StagingBuffer = {};

    owner->SetupBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        StagingBuffer.get,
        StagingBuffer.memory);

    void* data;
    vkMapMemory(owner->m_Device.get, StagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data, &indices[0], (size_t)bufferSize);
    vkUnmapMemory(owner->m_Device.get, StagingBuffer.memory);

    owner->SetupBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        IndexBuffer.get,
        IndexBuffer.memory);

    owner->CopyBuffer(StagingBuffer, IndexBuffer, bufferSize);

    owner->CleanBuffer(StagingBuffer);
}

void TricksterSprite::Draw(const VkCommandBuffer& CommandBuffer, const VkDescriptorSet& DescriptorSet)
{
    VkBuffer vertexBuffers[] = { VertexBuffer.get };
    VkDeviceSize offsets[] = { 0 };

    // Binding point 0: Mesh vertex buffer
    vkCmdBindVertexBuffers(CommandBuffer, 0, 1, vertexBuffers, offsets);

    // Binding point 1 : Instance data buffer
    //vkCmdBindVertexBuffers(CommandBuffer, 1, 1, &instances->buffer.get, offsets);


    vkCmdBindIndexBuffer(CommandBuffer, IndexBuffer.get, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, owner->m_Pipeline.layout, 0, 1, &DescriptorSet, 0, nullptr);
    vkCmdDrawIndexed(CommandBuffer, 6, 1, 0, 0, 0);

}