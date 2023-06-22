#include "Audio.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

void Audio::Initialize()
{
    IXAudio2MasteringVoice* masterVoice;
    //XAudio�G���W���̃C���X�^���X�𐶐�
    HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    //�}�X�^�[�{�C�X�𐶐�
    result = xAudio2_->CreateMasteringVoice(&masterVoice);
}

void Audio::Finalize()
{
    xAudio2_.Reset();
    std::map<std::string, SoundData>::iterator it = soundDatas_.begin();

    for (; it != soundDatas_.end(); ++it)
    {
        SoundUnLoad(&it->second);
    }

}

void Audio::SoundLoadWave(const char* filename)
{
    if (soundDatas_.find(filename) != soundDatas_.end())
    {
        return;
    }

    HRESULT result;
    //�t�@�C�����̓X�g���[���̃C���X�^���X
    std::ifstream file;
    //.wav�t�@�C�����o�C�i�����[�h�ŊJ��
    file.open(filename, std::ios_base::binary);
    //�t�@�C���I�[�v�����s�����o����
    assert(file.is_open());

    //RIFF�w�b�_�[�ǂݍ���
    RiffHeader riff;
    file.read((char*)&riff, sizeof(riff));
    //�t�@�C����RIFF���`�F�b�N
    if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
        assert(0);
    }
    //�^�C�v��WAVE���`�F�b�N
    if (strncmp(riff.type, "WAVE", 4) != 0) {
        assert(0);
    }
    //Format�`�����N�̓ǂݍ���
    FormatChunk format = {};
    //�`�����N�w�b�_�[�̊m�F
    file.read((char*)&format, sizeof(ChunkHeader));
    if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
        assert(0);
    }
    //�`�����N�{�̂̓ǂݍ���
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char*)&format.fmt, format.chunk.size);

    //Data�`�����N�̓ǂݍ���
    ChunkHeader data;
    file.read((char*)&data, sizeof(data));
    //JUNK�`�����N�����o�����ꍇ
    if (strncmp(data.id, "JUNK", 4) == 0) {
        //�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
        file.seekg(data.size, std::ios_base::cur);
        //�ēǂݍ���
        file.read((char*)&data, sizeof(data));
    }
    if (strncmp(data.id, "data", 4) != 0) {
        assert(0);
    }

    //Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
    char* pBuffer = new char[data.size];
    file.read(pBuffer, data.size);

    //Wave�t�@�C�������
    file.close();

    //return���邽�߂̉����f�[�^
    SoundData soundData = {};

    soundData.wfex = format.fmt;
    soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
    soundData.bufferSize = data.size;

    soundDatas_.insert(std::make_pair(filename, soundData));
}

void Audio::SoundUnLoad(SoundData* soundData)
{
    //�o�b�t�@�̃����������
    delete[] soundData->pBuffer;

    soundData->pBuffer = 0;
    soundData->bufferSize = 0;
    soundData->wfex = {};
}

void Audio::SoundPlayWave(std::string filename)
{
    HRESULT result;

    std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
    assert(it != soundDatas_.end());

    SoundData& soundData = it->second;

    //�g�`�t�H�[�}�b�g������sourcevoice�̐���
    IXAudio2SourceVoice* pSourceVoice = nullptr;
    result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
    assert(SUCCEEDED(result));

    //�Đ�����g�`�f�[�^�̐ݒ�
    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundData.pBuffer;
    buf.AudioBytes = soundData.bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    //�g�`�f�[�^�̍Đ�
    result = pSourceVoice->SubmitSourceBuffer(&buf);
    result = pSourceVoice->Start();
}

Audio* Audio::GetInstance()
{
    static Audio ins;
    return &ins;
}
