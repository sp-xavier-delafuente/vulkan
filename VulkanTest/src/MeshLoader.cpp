/*
* Mesh loader for creating Vulkan resources from models loaded with ASSIMP
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "MeshLoader.hpp"

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
	m_Entries.clear();
}

bool MeshLoader::LoadMesh(const std::string& filename, int flags)
{
	pScene = Importer.ReadFile(filename.c_str(), flags);

	if (pScene)
	{
		m_Entries.clear();
		m_Entries.resize(pScene->mNumMeshes);
		// Read in all meshes in the scene
		for (auto i = 0; i < m_Entries.size(); i++)
		{
			m_Entries[i].vertexBase = numVertices;
			numVertices += pScene->mMeshes[i]->mNumVertices;
			const aiMesh* paiMesh = pScene->mMeshes[i];
			InitMesh(&m_Entries[i], paiMesh, pScene);
		}
		return true;
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
		return false;
	}
}

void MeshLoader::InitMesh(MeshEntry *meshEntry, const aiMesh* paiMesh, const aiScene* pScene)
{
	meshEntry->MaterialIndex = paiMesh->mMaterialIndex;

	aiColor3D pColor(0.f, 0.f, 0.f);
	pScene->mMaterials[paiMesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, pColor);

	aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		aiVector3D* pPos = &(paiMesh->mVertices[i]);
		aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		aiVector3D* pTexCoord = (paiMesh->HasTextureCoords(0)) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		aiVector3D* pTangent = (paiMesh->HasTangentsAndBitangents()) ? &(paiMesh->mTangents[i]) : &Zero3D;
		aiVector3D* pBiTangent = (paiMesh->HasTangentsAndBitangents()) ? &(paiMesh->mBitangents[i]) : &Zero3D;

		Vertex v(
			glm::vec3(pPos->x, -pPos->y, pPos->z),
			glm::vec2(pTexCoord->x, pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z),
			glm::vec3(pTangent->x, pTangent->y, pTangent->z),
			glm::vec3(pBiTangent->x, pBiTangent->y, pBiTangent->z),
			glm::vec3(pColor.r, pColor.g, pColor.b)
		);

		dim.max.x = fmax(pPos->x, dim.max.x);
		dim.max.y = fmax(pPos->y, dim.max.y);
		dim.max.z = fmax(pPos->z, dim.max.z);

		dim.min.x = fmin(pPos->x, dim.min.x);
		dim.min.y = fmin(pPos->y, dim.min.y);
		dim.min.z = fmin(pPos->z, dim.min.z);

		meshEntry->Vertices.push_back(v);
	}

	dim.size = dim.max - dim.min;

	uint32_t indexBase = static_cast<uint32_t>(meshEntry->Indices.size());
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		if (Face.mNumIndices != 3)
			continue;
		meshEntry->Indices.push_back(indexBase + Face.mIndices[0]);
		meshEntry->Indices.push_back(indexBase + Face.mIndices[1]);
		meshEntry->Indices.push_back(indexBase + Face.mIndices[2]);
	}
}