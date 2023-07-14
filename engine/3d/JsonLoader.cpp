#include "JsonLoader.h"

#include "json.hpp"
#include <fstream>
#include <cassert>

const std::string JsonLoader::kDefaultBaseDirectory = "Resources/jsons/";
const std::string JsonLoader::kExtension = ".json";

JsonData* JsonLoader::LoadFile(const std::string& fileName)
{
	//�A�g���ăt���p�X�𓾂�
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;

	//�t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fullpath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	file >> deserialized;

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	// ���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	JsonData* jsonData = new JsonData();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// ��ʂ��擾
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// �v�f�ǉ�
			jsonData->objects.emplace_back(JsonData::ObjectData{});
			// ���ǉ������v�f�̎Q�Ƃ𓾂�
			JsonData::ObjectData& objectData = jsonData->objects.back();

			if (object.contains("file_name")) {
				// �t�@�C����
				objectData.fileName = object["file_name"];
			}

			// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
			nlohmann::json& transform = object["transform"];
			// ���s�ړ�
			objectData.translation.m128_f32[0] = (float)transform["translation"][1];
			objectData.translation.m128_f32[1] = (float)transform["translation"][2];
			objectData.translation.m128_f32[2] = -(float)transform["translation"][0];
			objectData.translation.m128_f32[3] = 1.0f;
			// ��]�p
			objectData.rotation.m128_f32[0] = -(float)transform["rotation"][1];
			objectData.rotation.m128_f32[1] = -(float)transform["rotation"][2];
			objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];
			objectData.rotation.m128_f32[3] = 0.0f;
			// �X�P�[�����O
			objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scaling.m128_f32[3] = 0.0f;

			// TODO: �R���C�_�[�̃p�����[�^�ǂݍ���
		}

		// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
		if (object.contains("children")) {

		}
	}

	return jsonData;
}
