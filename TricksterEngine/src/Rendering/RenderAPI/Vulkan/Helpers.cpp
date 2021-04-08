#include "pch.h"
#include "Rendering/RenderAPI/Vulkan/Helpers.h"

#include "Rendering/RenderAPI/Vulkan/Vulkan.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader.h"
#include "Core/FilePaths.h"

namespace Trickster {
    Vulkan* TricksterModel::owner = 0;
    void Trickster::TricksterModel::Load(std::string a_ModelPath)
    {

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (ModelPath + a_ModelPath).c_str(), ModelPath.c_str())) {
            LOG_ERROR(warn + err);
        }
        std::unordered_map<TricksterVertex, uint32_t> uniqueVertices{};
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
                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
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
        int texWidth, texHeight, texChannels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* pixels = stbi_load((TexturePath +a_TexturePath).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;
        texturePath = a_TexturePath;
        owner->m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
        if (!pixels)
        {
            LOG_ERROR("[Vulkan] Failed to load Texture Image");
        }
        texture = {};
        TricksterBuffer StagingBuffer = {};
        owner->SetupBuffer(
            imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            StagingBuffer.get,
            StagingBuffer.memory);

        void* data;
        vkMapMemory(owner->m_Device.get, StagingBuffer.memory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(owner->m_Device.get, StagingBuffer.memory);
        stbi_image_free(pixels);
        texture.channels = texChannels;
        owner->SetupImage(texWidth,
            texHeight,
            VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            owner->m_MipLevels,
            texture);
        owner->TransitionImageLayout(texture.get, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, owner->m_MipLevels);
        owner->CopyBufferToImage(StagingBuffer.get, texture.get, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        // TransitionImageLayout(m_Image.get, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,m_MipLevels);
        owner->CleanBuffer(StagingBuffer);
        owner->SetupImageView(texture, VK_IMAGE_ASPECT_COLOR_BIT, owner->m_MipLevels);

        owner->GenerateMipmaps(texture, owner->m_MipLevels);
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

}
