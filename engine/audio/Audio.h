#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <map>
#include <string>

/**
*Audio.h

* オーディオ
*/
class Audio
{
public:
    //チャンクヘッダ
    struct ChunkHeader
    {
        char id[4];//チャンクごとのID
        int32_t size;//チャンクサイズ
    };
    //RIFFヘッダチャンク
    struct RiffHeader
    {
        ChunkHeader chunk;//"RIFF"
        char type[4];//"WAVE"
    };
    //FMTチャンク
    struct FormatChunk
    {
        ChunkHeader chunk;//"fmt"
        WAVEFORMATEX fmt;//波形フォーマット
    };
    //音声データ
    struct SoundData
    {
        //波形フォーマット
        WAVEFORMATEX wfex;
        //バッファの先頭アドレス
        BYTE* pBuffer;
        //バッファのサイズ
        unsigned int bufferSize;
    };

public:
    //初期化
    void Initialize();
    //削除
    void Finalize();
    //サウンド読み込み
    void SoundLoadWave(const char* filename);
    //サウンド読み込み停止
    void SoundUnLoad(SoundData* soundData);
    //サウンド再生
    void SoundPlayWave(std::string filename);
    //Audioインスタンス
    static Audio* GetInstance();

private:
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
    
    std::map<std::string, SoundData>soundDatas_;
};

