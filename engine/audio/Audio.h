#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <map>
#include <string>

/**
*Audio.h

* �I�[�f�B�I
*/
class Audio
{
public:
    //�`�����N�w�b�_
    struct ChunkHeader
    {
        char id[4];//�`�����N���Ƃ�ID
        int32_t size;//�`�����N�T�C�Y
    };
    //RIFF�w�b�_�`�����N
    struct RiffHeader
    {
        ChunkHeader chunk;//"RIFF"
        char type[4];//"WAVE"
    };
    //FMT�`�����N
    struct FormatChunk
    {
        ChunkHeader chunk;//"fmt"
        WAVEFORMATEX fmt;//�g�`�t�H�[�}�b�g
    };
    //�����f�[�^
    struct SoundData
    {
        //�g�`�t�H�[�}�b�g
        WAVEFORMATEX wfex;
        //�o�b�t�@�̐擪�A�h���X
        BYTE* pBuffer;
        //�o�b�t�@�̃T�C�Y
        unsigned int bufferSize;
    };

public:
    //������
    void Initialize();
    //�폜
    void Finalize();
    //�T�E���h�ǂݍ���
    void SoundLoadWave(const char* filename);
    //�T�E���h�ǂݍ��ݒ�~
    void SoundUnLoad(SoundData* soundData);
    //�T�E���h�Đ�
    void SoundPlayWave(std::string filename);
    //Audio�C���X�^���X
    static Audio* GetInstance();

private:
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
    
    std::map<std::string, SoundData>soundDatas_;
};

