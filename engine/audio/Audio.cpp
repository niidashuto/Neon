#include "Audio.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

void Audio::Initialize()
{
    IXAudio2MasteringVoice* masterVoice;
    //XAudioエンジンのインスタンスを生成
    HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    //マスターボイスを生成
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
    //ファイル入力ストリームのインスタンス
    std::ifstream file;
    //.wavファイルをバイナリモードで開く
    file.open(filename, std::ios_base::binary);
    //ファイルオープン失敗を検出する
    assert(file.is_open());

    //RIFFヘッダー読み込み
    RiffHeader riff;
    file.read((char*)&riff, sizeof(riff));
    //ファイルがRIFFかチェック
    if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
        assert(0);
    }
    //タイプがWAVEかチェック
    if (strncmp(riff.type, "WAVE", 4) != 0) {
        assert(0);
    }
    //Formatチャンクの読み込み
    FormatChunk format = {};
    //チャンクヘッダーの確認
    file.read((char*)&format, sizeof(ChunkHeader));
    if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
        assert(0);
    }
    //チャンク本体の読み込み
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char*)&format.fmt, format.chunk.size);

    //Dataチャンクの読み込み
    ChunkHeader data;
    file.read((char*)&data, sizeof(data));
    //JUNKチャンクを検出した場合
    if (strncmp(data.id, "JUNK", 4) == 0) {
        //読み取り位置をJUNKチャンクの終わりまで進める
        file.seekg(data.size, std::ios_base::cur);
        //再読み込み
        file.read((char*)&data, sizeof(data));
    }
    if (strncmp(data.id, "data", 4) != 0) {
        assert(0);
    }

    //Dataチャンクのデータ部(波形データ)の読み込み
    char* pBuffer = new char[data.size];
    file.read(pBuffer, data.size);

    //Waveファイルを閉じる
    file.close();

    //returnするための音声データ
    SoundData soundData = {};

    soundData.wfex = format.fmt;
    soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
    soundData.bufferSize = data.size;

    soundDatas_.insert(std::make_pair(filename, soundData));
}

void Audio::SoundUnLoad(SoundData* soundData)
{
    //バッファのメモリを解放
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

    //波形フォーマットを元にsourcevoiceの生成
    IXAudio2SourceVoice* pSourceVoice = nullptr;
    result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
    assert(SUCCEEDED(result));

    //再生する波形データの設定
    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundData.pBuffer;
    buf.AudioBytes = soundData.bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    //波形データの再生
    result = pSourceVoice->SubmitSourceBuffer(&buf);
    result = pSourceVoice->Start();
}

Audio* Audio::GetInstance()
{
    static Audio ins;
    return &ins;
}
