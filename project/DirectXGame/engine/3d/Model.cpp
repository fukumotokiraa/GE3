#include "Model.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "SrvManager.h"


void Model::Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename)
{
	//ModelCommonのポインタを引数からメンバ変数に記録する
	modelCommon_ = modelCommon;
	//モデル読み込み
	modelData_ = LoadModelFile(directorypath, filename);
	//頂点データの初期化
	vertexResource = modelCommon_->GetDXCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData->position.x *= -1.0f;
	vertexData->normal.x *= -1.0f;
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());//頂点データをリソースにコピー
	//マテリアルの初期化
	materialResource = modelCommon_->GetDXCommon()->CreateBufferResource(sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//白
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//spriteはlightingしないのでfalseを設定する
	materialData->enableLighting = false;
	materialData->uvTransform = MakeIdentity4x4();
	//テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	//テクスチャ番号を取得してメンバ変数に書き込む
	modelData_.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData_.material.textureFilePath);
	//VertexBufferViewを設定
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();//リソースの先頭アドレスから使う
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());//使用するリソースのサイズは頂点のサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);//１頂点あたりのサイズ
}

void Model::Draw()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = {
	TextureManager::GetInstance()->GetSrvManager()->GetDescriptorHeap().Get()
	};
	modelCommon_->GetDXCommon()->GetCommandlist()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	//マテリアルCBufferの場所を設定
	modelCommon_->GetDXCommon()->GetCommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	modelCommon_->GetDXCommon()->GetCommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);//VBVを設定
	//SRVのDescriptorTableの先頭を設定
	modelCommon_->GetDXCommon()->GetCommandlist()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));
	//描画(DrawCall)
	modelCommon_->GetDXCommon()->GetCommandlist()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);

}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	//1.中で必要となる変数の宣言
	MaterialData materialData;//構築するMaterialData
	std::string line;//ファイルから読んだ1行を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開けなかったら止める

	//3.実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd")
		{
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	//4.MaterialDataを返す
	return materialData;
}

ModelData Model::LoadModelFile(const std::string& directoryPath, const std::string& filename) {
	//1.中で必要となる変数の宣言
	ModelData modelData;
	Assimp::Importer importer;

	//2.ファイルを開く
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(),aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());//メッシュがないのは対応しない

	//3.mesh解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));
		for(uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			for(uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };

				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
                modelData.vertices.push_back(vertex);
			}
		}
	}

	//material解析
	for(uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) !=0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	modelData.rootNode = ReadNode(scene->mRootNode);

	//4.ModelDataを返す
	return modelData;
}

Node Model::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; //nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); //列ベクトル形式を行ベクトルに転置
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	result.localMatrix.m[0][1] = aiLocalMatrix[0][1];
	result.localMatrix.m[0][2] = aiLocalMatrix[0][2];
	result.localMatrix.m[0][3] = aiLocalMatrix[0][3];
	result.localMatrix.m[1][0] = aiLocalMatrix[1][0];
	result.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	result.localMatrix.m[1][2] = aiLocalMatrix[1][2];
	result.localMatrix.m[1][3] = aiLocalMatrix[1][3];
	result.localMatrix.m[2][0] = aiLocalMatrix[2][0];
	result.localMatrix.m[2][1] = aiLocalMatrix[2][1];
	result.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	result.localMatrix.m[2][3] = aiLocalMatrix[2][3];
	result.localMatrix.m[3][0] = aiLocalMatrix[3][0];
	result.localMatrix.m[3][1] = aiLocalMatrix[3][1];
	result.localMatrix.m[3][2] = aiLocalMatrix[3][2];
	result.localMatrix.m[3][3] = aiLocalMatrix[3][3];
	result.name = node->mName.C_Str(); //Node名を格納
	result.children.resize(node->mNumChildren); //子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		//再起的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}
