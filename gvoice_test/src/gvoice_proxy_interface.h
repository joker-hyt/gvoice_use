#pragma once

class IGVioceProxy
{
public:
    virtual ~IGVioceProxy() {}

public:
    virtual void Update() = 0;

    virtual void Init(const char* openID) = 0;
    virtual void SetMode(int mode) = 0;
    virtual void ApplyMessageKey() = 0;

    virtual void JoinTeamRoom(const char* roomName) = 0;
    virtual void JoinNationalRoom(const char* roomName, int role) = 0;
    virtual void QuitRoom(const char* roomName) = 0;
    virtual void ChangeRole(const char* roomName, int role) = 0;
    virtual void OpenMic() = 0;
    virtual void CloseMic() = 0;
    virtual void OpenSpeaker() = 0;
    virtual void CloseSpeaker() = 0;

    virtual void StartRecording(const char* filePath) = 0;
    virtual void StopRecording() = 0;
    virtual void UploadRecordedFile(const char* filePath) = 0;
    virtual void DownloadRecordedFile(const char* fileID, const char* filePath) = 0;
    virtual void PlayRecordedFile(const char* filePath) = 0;
    virtual void StopPlayFile() = 0;
    virtual void SpeechToText(const char* fileID) = 0;

    virtual void SetMaxMessageLength(int msTime) = 0;
    virtual void GetFileParam(const char* filePath) = 0;

    virtual void ForbidMemberVoice(int member, bool bEnable, const char* roomName) = 0;
    virtual void SetMicVolume(int vol) = 0;
    virtual void SetSpeakerVolume(int vol) = 0;
    virtual void EnableSpeakerOn(bool bEnable) = 0;
    virtual void TestMic() = 0;

    virtual void SetVoiceEffect(int mode) = 0;
    virtual void EnableReverb(bool bEnable) = 0;
    virtual void SetReverbMode(int mode) = 0;

    virtual void EnableNativeBGMPlay(bool bEnable) = 0;
    virtual void SetBGMPath(const char* path) = 0;
    virtual void StartBGMPlay() = 0;
    virtual void StopBGMPlay() = 0;
    virtual void PauseBGMPlay() = 0;
    virtual void ResumeBGMPlay() = 0;
    virtual void SetBGMVol(int vol) = 0;
};
