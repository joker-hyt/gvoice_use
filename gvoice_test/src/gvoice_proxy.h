#pragma once
#include "gvoice_proxy_interface.h"
#include "../include/GCloudVoice.h"

class CGVioceProxy : public IGVioceProxy, public gcloud_voice::IGCloudVoiceNotify
{
public:
    CGVioceProxy(HWND hWnd);
    virtual ~CGVioceProxy();

public:
    virtual void Update();

    virtual void Init(const char* openID);
    virtual void SetMode(int mode);
    virtual void ApplyMessageKey();

    virtual void JoinTeamRoom(const char* roomName);
    virtual void JoinNationalRoom(const char* roomName, int role);
    virtual void QuitRoom(const char* roomName);
    virtual void ChangeRole(const char* roomName, int role);
    virtual void OpenMic();
    virtual void CloseMic();
    virtual void OpenSpeaker();
    virtual void CloseSpeaker();

    virtual void StartRecording(const char* filePath);
    virtual void StopRecording();
    virtual void UploadRecordedFile(const char* filePath);
    virtual void DownloadRecordedFile(const char* fileID, const char* filePath);
    virtual void PlayRecordedFile(const char* filePath);
    virtual void StopPlayFile();
    virtual void SpeechToText(const char* fileID);

    virtual void SetMaxMessageLength(int msTime);
    virtual void GetFileParam(const char* filePath);

    virtual void ForbidMemberVoice(int member, bool bEnable, const char* roomName);
    virtual void SetMicVolume(int vol);
    virtual void SetSpeakerVolume(int vol);
    virtual void EnableSpeakerOn(bool bEnable);
    virtual void TestMic();

    virtual void SetVoiceEffect(int mode);
    virtual void EnableReverb(bool bEnable);
    virtual void SetReverbMode(int mode);

    virtual void EnableNativeBGMPlay(bool bEnable);
    virtual void SetBGMPath(const char* path);
    virtual void StartBGMPlay();
    virtual void StopBGMPlay();
    virtual void PauseBGMPlay();
    virtual void ResumeBGMPlay();
    virtual void SetBGMVol(int vol);

public:
    virtual void OnApplyMessageKey(gcloud_voice::GCloudVoiceCompleteCode code);

    virtual void OnJoinRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName, int memberID);
    virtual void OnQuitRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName);
    virtual void OnRoleChanged(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName, int memberID, int role);
    virtual void OnMemberVoice(const unsigned int* members, int count);
    virtual void OnMemberVoice(const char* roomName, unsigned int member, int status);
    virtual void OnStatusUpdate(gcloud_voice::GCloudVoiceCompleteCode status, const char* roomName, int memberID);

    virtual void OnRecording(const unsigned char* pAudioData, unsigned int nDataLength);
    virtual void OnUploadFile(gcloud_voice::GCloudVoiceCompleteCode code, const char* filePath, const char* fileID);
    virtual void OnDownloadFile(gcloud_voice::GCloudVoiceCompleteCode code, const char* filePath, const char* fileID);
    virtual void OnPlayRecordedFile(gcloud_voice::GCloudVoiceCompleteCode code, const char* filePath);
    virtual void OnSpeechToText(gcloud_voice::GCloudVoiceCompleteCode code, const char* fileID, const char* result);
    virtual void OnStreamSpeechToText(gcloud_voice::GCloudVoiceCompleteCode code, int error, const char* result, const char* voicePath);
	virtual void OnRoomMemberInfo(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName, int memid, const char* openID);

private:
    void Poll();
    void GetMicLevel();
    void GetSpeakLevel();
	void GetBGMPlayState();

private:
	void GetGVoiceRoomToken(std::string& token, int& time, const char* roomName);
	void GetGVoiceMessageToken(std::string& token, int& time);
	void GetGVoiceTranslationToken(std::string& token, int& time, const char* fileID);

private:
    HWND m_hWnd;
    std::ofstream m_logger;
    gcloud_voice::IGCloudVoiceEngine* m_gvoice;

	std::string m_openID;
	int m_member_id;
};
