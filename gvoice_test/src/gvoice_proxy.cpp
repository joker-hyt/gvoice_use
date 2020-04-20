#include "stdafx.h"
#include "gvoice_proxy.h"
#include "../Resource.h"

const int GVOICE_TIMEOUT = 10000;
const char* appID = "xxxxxx";
const char* appKey = "xxxxxx";
const char* appKeyDummy = "abc";

CGVioceProxy::CGVioceProxy(HWND hWnd)
    : m_hWnd(hWnd)
    , m_gvoice(NULL)
	, m_member_id(0)
{}

CGVioceProxy::~CGVioceProxy()
{
    m_logger.close();
}

void CGVioceProxy::Update()
{
    if(m_gvoice != NULL)
    {
        Poll();
        GetMicLevel();
        GetSpeakLevel();
		GetBGMPlayState();
    }
}

void CGVioceProxy::Init(const char* openID)
{
	m_openID = openID;

    m_gvoice = gcloud_voice::GetVoiceEngine();
    assert(m_gvoice != NULL);

    m_gvoice->SetAppInfo(appID, appKeyDummy, openID);
    m_gvoice->Init();
    m_gvoice->SetNotify(this);
    m_gvoice->EnableLog(true);
    m_logger.open(m_openID + ".log");

	//采集PC系统声音
	//m_gvoice->invoke(6107, 1, 0, NULL);
}

void CGVioceProxy::SetMode(int mode)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SetMode(static_cast<gcloud_voice::IGCloudVoiceEngine::GCloudVoiceMode>(mode));
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetMode"), MB_OK);
}

void CGVioceProxy::ApplyMessageKey()
{
	std::string token;
	int timestamp = 0;
	GetGVoiceMessageToken(token, timestamp);

    gcloud_voice::GCloudVoiceErrno code = m_gvoice->ApplyMessageKey(token.c_str(), timestamp, GVOICE_TIMEOUT);
    //gcloud_voice::GCloudVoiceErrno code = m_gvoice->ApplyMessageKey(GVOICE_TIMEOUT);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("ApplyMessageKey"), MB_OK);
}

void CGVioceProxy::JoinTeamRoom(const char* roomName)
{
	std::string token;
	int timestamp = 0;
	GetGVoiceRoomToken(token, timestamp, roomName);
	gcloud_voice::GCloudVoiceErrno code = m_gvoice->JoinTeamRoom(roomName, token.c_str(), timestamp, GVOICE_TIMEOUT);
    //gcloud_voice::GCloudVoiceErrno code = m_gvoice->JoinTeamRoom(roomName, GVOICE_TIMEOUT);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("JoinTeamRoom"), MB_OK);
}

void CGVioceProxy::JoinNationalRoom(const char* roomName, int role)
{
	std::string token;
	int timestamp = 0;
	GetGVoiceRoomToken(token, timestamp, roomName);
	gcloud_voice::GCloudVoiceErrno code = m_gvoice->JoinNationalRoom(roomName, static_cast<gcloud_voice::IGCloudVoiceEngine::GCloudVoiceMemberRole>(role), token.c_str(), timestamp, GVOICE_TIMEOUT);
    //gcloud_voice::GCloudVoiceErrno code = m_gvoice->JoinNationalRoom(roomName, static_cast<gcloud_voice::GCloudVoiceMemberRole>(role), GVOICE_TIMEOUT);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("JoinNationalRoom"), MB_OK);
}

void CGVioceProxy::QuitRoom(const char* roomName)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->QuitRoom(roomName, GVOICE_TIMEOUT);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("QuitRoom"), MB_OK);
}

void CGVioceProxy::ChangeRole(const char* roomName, int role)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->ChangeRole(static_cast<gcloud_voice::IGCloudVoiceEngine::GCloudVoiceMemberRole>(role), roomName);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("ChangeRole"), MB_OK);
}

void CGVioceProxy::OpenMic()
{
	//关闭vad（静音抑制）
	//m_gvoice->invoke(5016, 1, 0, NULL);

    gcloud_voice::GCloudVoiceErrno code = m_gvoice->OpenMic();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OpenMic"), MB_OK);
}

void CGVioceProxy::CloseMic()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->CloseMic();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("CloseMic"), MB_OK);
}

void CGVioceProxy::OpenSpeaker()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->OpenSpeaker();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OpenSpeaker"), MB_OK);
}

void CGVioceProxy::CloseSpeaker()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->CloseSpeaker();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("CloseSpeaker"), MB_OK);
}

void CGVioceProxy::StartRecording(const char* filePath)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->StartRecording(filePath);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("StartRecording"), MB_OK);
}

void CGVioceProxy::StopRecording()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->StopRecording();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("StopRecording"), MB_OK);
}

void CGVioceProxy::UploadRecordedFile(const char* filePath)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->UploadRecordedFile(filePath, GVOICE_TIMEOUT);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("UploadRecordedFile"), MB_OK);
}

void CGVioceProxy::DownloadRecordedFile(const char* fileID, const char* filePath)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->DownloadRecordedFile(fileID, filePath, GVOICE_TIMEOUT);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("DownloadRecordedFile"), MB_OK);
}

void CGVioceProxy::PlayRecordedFile(const char* filePath)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->PlayRecordedFile(filePath);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("PlayRecordedFile"), MB_OK);
}

void CGVioceProxy::StopPlayFile()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->StopPlayFile();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("StopPlayFile"), MB_OK);
}

void CGVioceProxy::SpeechToText(const char* fileID)
{
	std::string token;
	int timestamp = 0;
	GetGVoiceTranslationToken(token, timestamp, fileID);
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SpeechToText(fileID, token.c_str(), timestamp, GVOICE_TIMEOUT, gcloud_voice::IGCloudVoiceEngine::China);
	
    //gcloud_voice::GCloudVoiceErrno code = m_gvoice->SpeechToText(fileID, GVOICE_TIMEOUT, gcloud_voice::Chinese);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SpeechToText"), MB_OK);
}

void CGVioceProxy::SetMaxMessageLength(int msTime)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SetMaxMessageLength(msTime);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetMaxMessageLength"), MB_OK);
}

void CGVioceProxy::GetFileParam(const char* filePath)
{
    float seconds = 0.f;
    unsigned int bytes = 0;
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->GetFileParam(filePath, &bytes, &seconds);
    std::string str = "code:" + ItoA(code) + ", bytes:" + ItoA(bytes) + ", seconds:" + FtoA(seconds);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("GetFileParam"), MB_OK);
}

void CGVioceProxy::ForbidMemberVoice(int member, bool bEnable, const char* roomName)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->ForbidMemberVoice(member, bEnable, roomName);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("ForbidMemberVoice"), MB_OK);
}

void CGVioceProxy::SetMicVolume(int vol)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SetMicVolume(vol);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetMicVolume"), MB_OK);
}

void CGVioceProxy::SetSpeakerVolume(int vol)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SetSpeakerVolume(vol);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetSpeakerVolume"), MB_OK);
}

void CGVioceProxy::EnableSpeakerOn(bool bEnable)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->EnableSpeakerOn(bEnable);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("EnableSpeakerOn"), MB_OK);
}

void CGVioceProxy::TestMic()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->TestMic();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("TestMic"), MB_OK);
}

void CGVioceProxy::SetVoiceEffect(int mode)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SetVoiceEffects(static_cast<gcloud_voice::IGCloudVoiceEngine::SoundEffects>(mode));
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetVoiceEffect"), MB_OK);
}

void CGVioceProxy::EnableReverb(bool bEnable)
{
    int code = m_gvoice->EnableReverb(bEnable);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("EnableReverb"), MB_OK);
}

void CGVioceProxy::SetReverbMode(int mode)
{
    int code = m_gvoice->SetReverbMode(mode);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetReverbMode"), MB_OK);
}

void CGVioceProxy::EnableNativeBGMPlay(bool bEnable)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->EnableNativeBGMPlay(bEnable);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("EnableNativeBGMPlay"), MB_OK);
}

void CGVioceProxy::SetBGMPath(const char* path)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SetBGMPath(path);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetBGMPath"), MB_OK);
}

void CGVioceProxy::StartBGMPlay()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->StartBGMPlay();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("StartBGMPlay"), MB_OK);
}

void CGVioceProxy::StopBGMPlay()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->StopBGMPlay();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("StopBGMPlay"), MB_OK);
}

void CGVioceProxy::PauseBGMPlay()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->PauseBGMPlay();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("PauseBGMPlay"), MB_OK);
}

void CGVioceProxy::ResumeBGMPlay()
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->ResumeBGMPlay();
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("ResumeBGMPlay"), MB_OK);
}

void CGVioceProxy::SetBGMVol(int vol)
{
    gcloud_voice::GCloudVoiceErrno code = m_gvoice->SetBGMVol(vol);
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("SetBGMVol"), MB_OK);
}

void CGVioceProxy::OnApplyMessageKey(gcloud_voice::GCloudVoiceCompleteCode code) 
{
    std::string str = "code:" + ItoA(code);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnApplyMessageKey"), MB_OK);
}

void CGVioceProxy::OnJoinRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName, int memberID)
{
	m_member_id = memberID;
    std::string str = "code:" + ItoA(code) + ", roomName:" + roomName + ", memberID:" + ItoA(memberID);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnJoinRoom"), MB_OK);
}

void CGVioceProxy::OnQuitRoom(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName)
{
    std::string str = "code:" + ItoA(code) + ", roomName:" + roomName;
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnQuitRoom"), MB_OK);
}

void CGVioceProxy::OnRoleChanged(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName, int memberID, int role)
{
	m_member_id = memberID;
    std::string str = "code:" + ItoA(code) + ", roomName:" + roomName + ", memberID:" + ItoA(memberID) + ", role:" + ItoA(role);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnRoleChanged"), MB_OK);
}

void CGVioceProxy::OnMemberVoice(const unsigned int* members, int count)
{
    std::string str;
    str += "count:" + ItoA(count);
    for(int i = 0; i < count; ++i)
	{
		str += ", ID:" + ItoA(members[2 * i]) + ", status:" + ItoA(members[2 * i + 1]);
		//if(members[2 * i] != 0)
		//{
		//    HWND forbidMemberWnd = GetDlgItem(m_hWnd, IDC_GVOICE_FORBID_MEMBER);
		//    SetWindowText(forbidMemberWnd, char2wchar(ItoA(members[2 * i]).c_str()).c_str());
		//}
	}

	static std::string s_str;
	if(s_str != str)
	{
		s_str = str;

		HWND memberVoiceWnd = GetDlgItem(m_hWnd, IDC_GVOICE_MEMBER_VOICE);
		SetWindowText(memberVoiceWnd, char2wchar(str.c_str()).c_str());
		m_logger << str << std::endl;
		//MessageBox(m_hWnd, char2wchar(str.c_str()), _T("OnMemberVoice"), MB_OK);
	}
}

void CGVioceProxy::OnMemberVoice(const char* roomName, unsigned int member, int status)
{
    std::string str = "memberID:" + ItoA(member) + ", roomName:" + roomName + ", status:" + ItoA(status);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnMemberVoice"), MB_OK);
}

void CGVioceProxy::OnStatusUpdate(gcloud_voice::GCloudVoiceCompleteCode status, const char* roomName, int memberID)
{
    std::string str = "memberID:" + ItoA(memberID) + ", roomName:" + roomName + ", code:" + ItoA(status);
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnStatusUpdate"), MB_OK);
}

void CGVioceProxy::OnRecording(const unsigned char* pAudioData, unsigned int nDataLength)
{}

void CGVioceProxy::OnUploadFile(gcloud_voice::GCloudVoiceCompleteCode code, const char* filePath, const char* fileID)
{
    std::string str = "code:" + ItoA(code) + ", filePath:" + filePath + ", fileID:" + fileID;
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnUploadFile"), MB_OK);

    if(code == gcloud_voice::GV_ON_UPLOAD_RECORD_DONE)
    {
        HWND fileIDWnd = GetDlgItem(m_hWnd, IDC_GVOICE_FILE_ID);
        SetWindowText(fileIDWnd, char2wchar(fileID).c_str());
    }
}

void CGVioceProxy::OnDownloadFile(gcloud_voice::GCloudVoiceCompleteCode code, const char* filePath, const char* fileID)
{
    std::string str = "code:" + ItoA(code) + ", filePath:" + filePath + ", fileID:" + fileID;
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnDownloadFile"), MB_OK);
}

void CGVioceProxy::OnPlayRecordedFile(gcloud_voice::GCloudVoiceCompleteCode code, const char* filePath)
{
    std::string str = "code:" + ItoA(code) + ", filePath:" + filePath;
    MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnPlayRecordedFile"), MB_OK);
}

void CGVioceProxy::OnSpeechToText(gcloud_voice::GCloudVoiceCompleteCode code, const char* fileID, const char* result)
{
    std::string str = "code:" + ItoA(code) + ", fileID:" + fileID + ", result:" + result;
    MessageBox(m_hWnd, char2wchar(utf8ToGbk18030(result, strlen(result)).c_str()).c_str(), _T("OnSpeechToText"), MB_OK);
}

void CGVioceProxy::OnStreamSpeechToText(gcloud_voice::GCloudVoiceCompleteCode code, int error, const char* result, const char* voicePath)
{
    std::string str = "code:" + ItoA(code) + ", error:" + ItoA(error) + ", result:" + result + ", voicePath:" + voicePath;
    MessageBox(m_hWnd, char2wchar(utf8ToGbk18030(result, strlen(result)).c_str()).c_str(), _T("OnStreamSpeechToText"), MB_OK);
}

void CGVioceProxy::OnRoomMemberInfo(gcloud_voice::GCloudVoiceCompleteCode code, const char* roomName, int memid, const char* openID)
{
	std::string str = "code:" + ItoA(code) + ", roomName:" + roomName + ", memid:" + ItoA(memid) + ", openID:" + openID;
	MessageBox(m_hWnd, char2wchar(str.c_str()).c_str(), _T("OnRoomMemberInfo"), MB_OK);
}

void CGVioceProxy::Poll()
{
    m_gvoice->Poll();
}

void CGVioceProxy::GetMicLevel()
{
    int mic_level = m_gvoice->GetMicLevel();

    HWND micLevelWnd = GetDlgItem(m_hWnd, IDC_GVOICE_MIC_LEVEL);
    SetWindowText(micLevelWnd, char2wchar(ItoA(mic_level).c_str()).c_str());
}

void CGVioceProxy::GetSpeakLevel()
{
    int speaker_level = m_gvoice->GetSpeakerLevel();

    HWND speakerLevelWnd = GetDlgItem(m_hWnd, IDC_GVOICE_SPEAKER_LEVEL);
    SetWindowText(speakerLevelWnd, char2wchar(ItoA(speaker_level).c_str()).c_str());
}

void CGVioceProxy::GetBGMPlayState()
{
	int bgm_play_state = m_gvoice->GetBGMPlayState();
	//m_logger << "bgm_play_state:" << bgm_play_state << std::endl;
}

void CGVioceProxy::GetGVoiceRoomToken(std::string& token, int& time, const char* roomName)
{
	std::string business_id = appID;
	std::string business_key = appKey;
	std::string room_name = roomName;
	
	std::string open_id = m_openID;
	int nowt = time32::time(NULL).value(); 
	time = nowt;

	char check_str[1024] = { 0 };

	bibo_snprintf(check_str
		, sizeof(check_str)-1
		, "[room_name:%s][business_id:%s][open_id:%s][ip:127.0.0.1][net_type:0][time:%d][business_key:%s]"
		, room_name.c_str()
		, business_id.c_str()
		, open_id.c_str()
		, nowt
		, business_key.c_str()
		);
	std::string tmp_data = check_str;

	token = GenerateConfigMD5(tmp_data);
}

void CGVioceProxy::GetGVoiceMessageToken(std::string& token, int& time)
{
	std::string business_id = appID;
	std::string business_key = appKey;
	
	std::string open_id = m_openID;
	int nowt = time32::time(NULL).value(); 
	time = nowt;

	char check_str[1024] = { 0 };

	bibo_snprintf(check_str
		, sizeof(check_str)-1
		, "[business_id:%s][open_id:%s][ip:127.0.0.1][time:%d][business_key:%s]"
		, business_id.c_str()
		, open_id.c_str()
		, nowt
		, business_key.c_str()
		);
	std::string tmp_data = check_str;

	token = GenerateConfigMD5(tmp_data);
}

void CGVioceProxy::GetGVoiceTranslationToken(std::string& token, int& time, const char* fileID)
{
	std::string business_id = appID;
	std::string business_key = appKey;
	
	std::string file_key = fileID;
	int nowt = time32::time(NULL).value(); 
	time = nowt;

	char check_str[1024] = { 0 };

	bibo_snprintf(check_str
		, sizeof(check_str)-1
		, "[business_id:%s][file_key:%s][time:%d][business_key:%s]"
		, business_id.c_str()
		, file_key.c_str()
		, nowt
		, business_key.c_str()
		);
	std::string tmp_data = check_str;

	token = GenerateConfigMD5(tmp_data);
}

extern "C" __declspec(dllexport) IGVioceProxy* CreateGVoiceProxy(HWND hWnd)
{
    return new CGVioceProxy(hWnd);
}

extern "C" __declspec(dllexport) void DeleteGVoiceProxy(IGVioceProxy* s)
{
    delete s;
}
