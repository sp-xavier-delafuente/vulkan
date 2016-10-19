/*
* Mesh loader for creating Vulkan resources from models loaded with ASSIMP
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#include <string>
#include <stdio.h>
#include <vector>

#include "vulkan/vulkan.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vkMeshLoader 
{
	typedef enum VertexLayout {
		VERTEX_LAYOUT_POSITION = 0x0,
		VERTEX_LAYOUT_NORMAL = 0x1,
		VERTEX_LAYOUT_COLOR = 0x2,
		VERTEX_LAYOUT_UV = 0x3,
		VERTEX_LAYOUT_TANGENT = 0x4,
		VERTEX_LAYOUT_BITANGENT = 0x5,
		VERTEX_LAYOUT_DUMMY_FLOAT = 0x6,
		VERTEX_LAYOUT_DUMMY_VEC4 = 0x7
	} VertexLayout;

	struct MeshBufferInfo 
	{
		VkBuffer buf = VK_NULL_HANDLE;
		VkDeviceMemory mem = VK_NULL_HANDLE;
		size_t size = 0;
	};

	/** @brief Stores a mesh's vertex and index descriptions */
	struct MeshDescriptor
	{
		uint32_t vertexCount;
		uint32_t indexBase;
		uint32_t indexCount;
	};

	/** @brief Mesh representation storing all data required to generate buffers */
	struct MeshBuffer 
	{
		std::vector<MeshDescriptor> meshDescriptors;
		MeshBufferInfo vertices;
		MeshBufferInfo indices;
		uint32_t indexCount;
		glm::vec3 dim;
	};

	/** @brief Holds parameters for mesh creation */
	struct MeshCreateInfo
	{
		glm::vec3 center;
		glm::vec3 scale;
		glm::vec2 uvscale;
	};

	/** 
	* Get the size of a vertex layout	
	* 
	* @param layout VertexLayout to get the size for
	*
	* @return Size of the vertex layout in bytes
	*/
	static uint32_t vertexSize(std::vector<vkMeshLoader::VertexLayout> layout)
	{
		uint32_t vSize = 0;
		for (auto& layoutDetail : layout)
		{
			switch (layoutDetail)
			{
			// UV only has two components
			case VERTEX_LAYOUT_UV: 
				vSize += 2 * sizeof(float);
				break;
			default:
				vSize += 3 * sizeof(float);
			}
		}
		return vSize;
	}

	// Note: Always assumes float formats
	/**
	* Generate vertex attribute descriptions for a layout at the given binding point
	*
	* @param layout VertexLayout from which to generate the descriptions 
	* @param attributeDescriptions Refernce to a vector of the descriptions to generate
	* @param binding Index of the attribute description binding point
	*
	* @note Always assumes float formats
	*/
	static void getVertexInputAttributeDescriptions(std::vector<vkMeshLoader::VertexLayout> layout, std::vector<VkVertexInputAttributeDescription> &attributeDescriptions, uint32_t binding)
	{
		uint32_t offset = 0;
		uint32_t location = 0;
		for (auto& layoutDetail : layout)
		{
			VkVertexInputAttributeDescription inputAttribDescription = {};
			inputAttribDescription.binding = binding;
			inputAttribDescription.location = location;
			inputAttribDescription.offset = offset;

			switch (layoutDetail)
			{
			// UV only has two components
			case VERTEX_LAYOUT_UV:
				offset += 2 * sizeof(float);
				inputAttribDescription.format = VK_FORMAT_R32G32_SFLOAT;
				break;
			default:
				offset += 3 * sizeof(float);
				inputAttribDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
			}
			attributeDescriptions.push_back(inputAttribDescription);
			location++;
		}
	}

	// Stores some additonal info and functions for 
	// specifying pipelines, vertex bindings, etc.
	class Mesh
	{
	public:
		MeshBuffer buffers;

		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

		uint32_t vertexBufferBinding = 0;

		VkPipelineVertexInputStateCreateInfo vertexInputState;
		VkVertexInputBindingDescription bindingDescription;
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	};

	static void freeMeshBufferResources(VkDevice device, vkMeshLoader::MeshBuffer *meshBuffer)
	{
		vkDestroyBuffer(device, meshBuffer->vertices.buf, nullptr);
		vkFreeMemory(device, meshBuffer->vertices.mem, nullptr);
		if (meshBuffer->indices.buf != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, meshBuffer->indices.buf, nullptr);
			vkFreeMemory(device, meshBuffer->indices.mem, nullptr);
		}
	}
}

// Simple mesh class for getting all the necessary stuff from models loaded via ASSIMP
class MeshLoader 
{
private:
	static const int defaultFlags = aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;

	struct Vertex
	{
		glm::vec3 m_pos;
		glm::vec2 m_tex;
		glm::vec3 m_normal;
		glm::vec3 m_color;
		glm::vec3 m_tangent;
		glm::vec3 m_binormal;

		Vertex() {}

		Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& bitangent, const glm::vec3& color)
		{
			m_pos = pos;
			m_tex = tex;
			m_normal = normal;
			m_color = color;
			m_tangent = tangent;
			m_binormal = bitangent;
		}
	};

	struct MeshEntry {
		uint32_t NumIndices;
		uint32_t MaterialIndex;
		uint32_t vertexBase;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
	};

public:
	std::vector<MeshEntry> m_Entries;

	struct Dimension 
	{
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(-FLT_MAX);
		glm::vec3 size;
	} dim;

	uint32_t numVertices = 0;

	Assimp::Importer Importer;
	const aiScene* pScene;

	/**
	* Default constructor
	*/
	MeshLoader();

	/**
	* Default destructor
	*
	* @note Does not free any Vulkan resources
	*/
	~MeshLoader();

	/** 
	* Load a scene from a supported 3D file format
	*
	* @param filename Name of the file (or asset) to load
	* @param flags (Optional) Set of ASSIMP processing flags
	*
	* @return Returns true if the scene has been loaded
	*/
	bool LoadMesh(const std::string& filename, int flags = defaultFlags);

	/**
	* Read mesh data from ASSIMP mesh to an internal mesh representation that can be used to generate Vulkan buffers
	*
	* @param meshEntry Pointer to the target MeshEntry strucutre for the mesh data
	* @param paiMesh ASSIMP mesh to get the data from
	* @param pScene Scene file of the ASSIMP mesh
	*/
	void InitMesh(MeshEntry *meshEntry, const aiMesh* paiMesh, const aiScene* pScene);
};