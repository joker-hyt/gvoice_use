// gvoice_test.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "gvoice_test.h"
#include "src/gvoice_proxy_interface.h"
#include "include/GCloudVoiceErrno.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
IGVioceProxy* gGvoiceProxy;                     // gVoiceProxy实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
extern "C" __declspec(dllexport) IGVioceProxy* CreateGVoiceProxy(HWND hWnd);
extern "C" __declspec(dllexport) void DeleteGVoiceProxy(IGVioceProxy* s);
void InitWindowControl(HWND hWnd, HINSTANCE hInstance);
void OperateCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
    ZeroMemory(&msg, sizeof(msg));
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GVOICE_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GVOICE_TEST));

	// 主消息循环:
    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            gGvoiceProxy->Update();
        }
    }

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GVOICE_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GVOICE_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 1600, 850, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   gGvoiceProxy = CreateGVoiceProxy(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
            OperateCommand(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_CREATE:
        InitWindowControl(hWnd, hInst);
        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void OperateCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    assert(message == WM_COMMAND);
    int wmId    = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);

    switch (wmId)
    {
    case IDM_GVOICE_INIT:
        {
            HWND openIDWnd = GetDlgItem(hWnd, IDC_GVOICE_OPENID);
            LRESULT sel = SendMessage(openIDWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->Init(ItoA(static_cast<int>(sel)).c_str());
        }
        break;
    case IDM_GVOICE_SET_MODE:
        {
            HWND modeWnd = GetDlgItem(hWnd, IDC_GVOICE_MODE);
            LRESULT sel = SendMessage(modeWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->SetMode(static_cast<int>(sel));
        }
        break;
    case IDM_GVOICE_APPLY_MESSAGE_KEY:
        {
            gGvoiceProxy->ApplyMessageKey();
        }
        break;
    case IDM_GVOICE_JOIN_TEAM_ROOM:
        {
            HWND roomIdWnd = GetDlgItem(hWnd, IDC_GVOICE_ROOM_ID);
            LRESULT sel = SendMessage(roomIdWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->JoinTeamRoom(("x51_" + ItoA(static_cast<int>(sel))).c_str());
        }
        break;
    case IDM_GVOICE_JOIN_NATIONAL_ROOM:
        {
            HWND roomIDWnd = GetDlgItem(hWnd, IDC_GVOICE_ROOM_ID);
            LRESULT roomIDSel = SendMessage(roomIDWnd, CB_GETCURSEL, 0, 0);
            HWND roleWnd = GetDlgItem(hWnd, IDC_GVOICE_ROLE);
            LRESULT roleSel = SendMessage(roleWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->JoinNationalRoom(("x51_" + ItoA(static_cast<int>(roomIDSel))).c_str(), static_cast<int>(roleSel));
        }
        break;
    case IDM_GVOICE_QUIT_ROOM:
        {
            HWND roomIDWnd = GetDlgItem(hWnd, IDC_GVOICE_ROOM_ID);
            LRESULT sel = SendMessage(roomIDWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->QuitRoom(("x51_" + ItoA(static_cast<int>(sel))).c_str());
        }
        break;
    case IDM_GVOICE_CHANGE_ROLE:
        {
            HWND roomIDWnd = GetDlgItem(hWnd, IDC_GVOICE_ROOM_ID);
            LRESULT roomIDSel = SendMessage(roomIDWnd, CB_GETCURSEL, 0, 0);
            HWND roleWnd = GetDlgItem(hWnd, IDC_GVOICE_ROLE);
            LRESULT roleSel = SendMessage(roleWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->ChangeRole(("x51_" + ItoA(static_cast<int>(roomIDSel))).c_str(), static_cast<int>(roleSel));
        }
        break;
    case IDM_GVOICE_OPEN_MIC:
        {
            gGvoiceProxy->OpenMic();
        }
        break;
    case IDM_GVOICE_CLOSE_MIC:
        {
            gGvoiceProxy->CloseMic();
        }
        break;
    case IDM_GVOICE_OPEN_SPEAKER:
        {
            gGvoiceProxy->OpenSpeaker();
        }
        break;
    case IDM_GVOICE_CLOSE_SPEAKER:
        {
            gGvoiceProxy->CloseSpeaker();
        }
        break;
    case IDM_GVOICE_START_RECORDING:
        {
            HWND filePathWnd = GetDlgItem(hWnd, IDC_GVOICE_FILE_PATH);
            LRESULT sel = SendMessage(filePathWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->StartRecording(("./x51_record_file_" + ItoA(static_cast<int>(sel))).c_str());
        }
        break;
    case IDM_GVOICE_STOP_RECORDING:
        {
            gGvoiceProxy->StopRecording();
        }
        break;
    case IDM_GVOICE_UPLOAD_RECORDED_FILE:
        {
            HWND filePathWnd = GetDlgItem(hWnd, IDC_GVOICE_FILE_PATH);
            LRESULT sel = SendMessage(filePathWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->UploadRecordedFile(("./x51_record_file_" + ItoA(static_cast<int>(sel))).c_str());
        }
        break;
    case IDM_GVOICE_DOWNLOAD_RECORDED_FILE:
        {
            HWND fileIDWnd = GetDlgItem(hWnd, IDC_GVOICE_FILE_ID);
            WCHAR fileID[512] = { 0 };
            GetWindowText(fileIDWnd, (LPWSTR)fileID, 512);
            HWND filePathWnd = GetDlgItem(hWnd, IDC_GVOICE_FILE_PATH);
            LRESULT filePathSel = SendMessage(filePathWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->DownloadRecordedFile(wchar2char(fileID).c_str(), ("./x51_record_file_" + ItoA(static_cast<int>(filePathSel))).c_str());
        }
        break;
    case IDM_GVOICE_PLAY_RECORDED_FILE:
        {
            HWND filePathWnd = GetDlgItem(hWnd, IDC_GVOICE_FILE_PATH);
            LRESULT sel = SendMessage(filePathWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->PlayRecordedFile(("./x51_record_file_" + ItoA(static_cast<int>(sel))).c_str());
        }
        break;
    case IDM_GVOICE_STOP_PLAY_FILE:
        {
            gGvoiceProxy->StopPlayFile();
        }
        break;
    case IDM_GVOICE_SPEECH_TO_TEXT:
        {
            HWND fileIDWnd = GetDlgItem(hWnd, IDC_GVOICE_FILE_ID);
            WCHAR fileID[512] = { 0 };
            GetWindowText(fileIDWnd, (LPWSTR)fileID, 512);
            gGvoiceProxy->SpeechToText(wchar2char(fileID).c_str());
        }
        break;
    case IDM_GVOICE_SET_MAX_MESSAGE_LENGTH:
        {
            HWND maxMessageLengthWnd = GetDlgItem(hWnd, IDC_GVOICE_MAX_MESSAGE_LENGTH);
            LRESULT sel = SendMessage(maxMessageLengthWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->SetMaxMessageLength(1000 * static_cast<int>(sel));
        }
        break;
    case IDM_GVOICE_GET_FILE_PARAM:
        {
            HWND filePathWnd = GetDlgItem(hWnd, IDC_GVOICE_FILE_PATH);
            LRESULT sel = SendMessage(filePathWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->GetFileParam(("./x51_record_file_" + ItoA(static_cast<int>(sel))).c_str());
        }
        break;
    case IDM_GVOICE_FORBID_MEMBER_VOICE:
        {
            HWND forbidMemberWnd = GetDlgItem(hWnd, IDC_GVOICE_FORBID_MEMBER);
            WCHAR forbidMember[512] = { 0 };
            GetWindowText(forbidMemberWnd, (LPWSTR)forbidMember, 512);
            HWND forbidMemberOnWnd = GetDlgItem(hWnd, IDC_GVOICE_FORBID_MEMEBER_ON);
            LRESULT forbidMemberOnSel = SendMessage(forbidMemberOnWnd, CB_GETCURSEL, 0, 0);
            HWND roomIDWnd = GetDlgItem(hWnd, IDC_GVOICE_ROOM_ID);
            LRESULT roomIDSel = SendMessage(roomIDWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->ForbidMemberVoice(atoi(wchar2char(forbidMember).c_str()), static_cast<int>(forbidMemberOnSel) == 0 ? false : true, ("x51_" + ItoA(static_cast<int>(roomIDSel))).c_str());
        }
        break;
    case IDM_GVOICE_SET_MIC_VOLUME:
        {
            HWND micVolumeWnd = GetDlgItem(hWnd, IDC_GVOICE_MIC_VOLUME);
            LRESULT sel = SendMessage(micVolumeWnd, CB_GETCURSEL, 0, 0);
            LRESULT data = SendMessage(micVolumeWnd, CB_GETITEMDATA, sel, 0);
            gGvoiceProxy->SetMicVolume(static_cast<int>(data));
        }
        break;
    case IDM_GVOICE_SET_SPEAKER_VOLUME:
        {
            HWND speakerVolumeWnd = GetDlgItem(hWnd, IDC_GVOICE_SPEAKER_VOLUME);
            LRESULT sel = SendMessage(speakerVolumeWnd, CB_GETCURSEL, 0, 0);
            LRESULT data = SendMessage(speakerVolumeWnd, CB_GETITEMDATA, sel, 0);
            gGvoiceProxy->SetSpeakerVolume(static_cast<int>(data));
        }
        break;
    case IDM_GVOICE_ENABLE_SPEAKER_ON:
        {
            HWND speakerOnWnd = GetDlgItem(hWnd, IDC_GVOICE_SPEAKER_ON);
            LRESULT sel = SendMessage(speakerOnWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->EnableSpeakerOn(static_cast<int>(sel) == 0 ? false : true);
        }
        break;
    case IDM_GVOICE_TEST_MIC:
        {
            gGvoiceProxy->TestMic();
        }
        break;
    case IDM_GVOICE_SET_VOICE_EFFECTS:
        {
            HWND voiceEffectsWnd = GetDlgItem(hWnd, IDC_GVOICE_VOICE_EFFECTS);
            LRESULT sel = SendMessage(voiceEffectsWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->SetVoiceEffect(static_cast<int>(sel));
        }
        break;
    case IDM_GVOICE_ENABLE_REVERB:
        {
            HWND reverbOnWnd = GetDlgItem(hWnd, IDC_GVOICE_REVERB_ON);
            LRESULT sel = SendMessage(reverbOnWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->EnableReverb(static_cast<int>(sel) == 0 ? false : true);
        }
        break;
    case IDM_GVOICE_SET_REVERB_MODE:
        {
            HWND reverbModeWnd = GetDlgItem(hWnd, IDC_GVOICE_REVERB_MODE);
            LRESULT sel = SendMessage(reverbModeWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->SetReverbMode(static_cast<int>(sel));
        }
        break;
    case IDM_GVOICE_ENABLE_NATIVE_BGM_PLAY:
        {
            HWND BGMOnWnd = GetDlgItem(hWnd, IDC_GVOICE_BGM_ON);
            LRESULT sel = SendMessage(BGMOnWnd, CB_GETCURSEL, 0, 0);
            gGvoiceProxy->EnableNativeBGMPlay(static_cast<int>(sel) == 0 ? false : true);
        }
        break;
    case IDM_GVOICE_SET_BGM_PATH:
        {
            HWND BGMPathWnd = GetDlgItem(hWnd, IDC_GVOICE_BGM_PATH);
            WCHAR BGMPath[512] = { 0 };
            GetWindowText(BGMPathWnd, (LPWSTR)BGMPath, 512);
            gGvoiceProxy->SetBGMPath(wchar2char(BGMPath).c_str());
        }
        break;
    case IDM_GVOICE_START_BGM_PLAY:
        {
            gGvoiceProxy->StartBGMPlay();
        }
        break;
    case IDM_GVOICE_STOP_BGM_PLAY:
        {
            gGvoiceProxy->StopBGMPlay();
        }
        break;
    case IDM_GVOICE_PAUSE_BGM_PLAY:
        {
            gGvoiceProxy->PauseBGMPlay();
        }
        break;
    case IDM_GVOICE_RESUME_BGM_PLAY:
        {
            gGvoiceProxy->ResumeBGMPlay();
        }
        break;
    case IDM_GVOICE_SET_BGM_VOL:
        {
            HWND BGMVolWnd = GetDlgItem(hWnd, IDC_GVOICE_BGM_VOL);
            LRESULT sel = SendMessage(BGMVolWnd, CB_GETCURSEL, 0, 0);
            LRESULT data = SendMessage(BGMVolWnd, CB_GETITEMDATA, sel, 0);
            gGvoiceProxy->SetBGMVol(static_cast<int>(data));
        }
        break;
    default:
        DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void InitWindowControl(HWND hWnd, HINSTANCE hInstance)
{
    int init_x = 10;
    int init_y = 10;
    int width = 160;
    int height = 25;
    int width_pan = width + 10;
    int height_pan = height + 10;
    int combobox_height = 250;

    {
        int x = init_x + 0 * width_pan;
        int y = init_y;

        CreateWindow(_T("STATIC"), _T("OpenID"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_OPENID_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("Mode"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_MODE_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("RoomID"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_ROOM_ID_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("Role"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_ROLE_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("MemberVoice"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_MEMBER_VOICE_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("FilePath"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_FILE_PATH_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("FileID"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_FILE_ID_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("MaxMessageLength"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_MAX_MESSAGE_LENGTH_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("ForbidMember"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_FORBID_MEMBER_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("ForbidMemberOn"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_FORBID_MEMEBER_ON_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("MicVolume"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_MIC_VOLUME_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("MicLevel"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_MIC_LEVEL_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("SpeakerVolume"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_SPEAKER_VOLUME_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("SpeakerLevel"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_SPEAKER_LEVEL_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("SpeakerOn"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_SPEAKER_ON_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("VoiceEffects"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_VOICE_EFFECTS_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("ReverbOn"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_REVERB_ON_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("ReverbMode"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_REVERB_MODE_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("BGMOn"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_BGM_ON_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("BGMPath"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_BGM_PATH_LABEL, NULL, NULL);
        CreateWindow(_T("STATIC"), _T("BGMVol"), WS_CHILD | WS_VISIBLE, x, y += height_pan, width, height,  hWnd, (HMENU)IDC_GVOICE_BGM_VOL_LABEL, NULL, NULL);
    }
    {
        int x = init_x + 1 * width_pan;
        int y = init_y;

        HWND openIDWnd = CreateWindow(_T("COMBOBOX"), _T("OpenID"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_OPENID, hInstance, NULL);
        for(int i = 0; i < 30; ++i)
        {
            SendMessage(openIDWnd, CB_ADDSTRING, 0, (LPARAM)char2wchar(ItoA(i).c_str()).c_str());
        }
        SendMessage(openIDWnd, CB_SETCURSEL, 0, 0);

        HWND modeWnd = CreateWindow(_T("COMBOBOX"), _T("Mode"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_MODE, hInstance, NULL);
        SendMessage(modeWnd, CB_ADDSTRING, 0, (LPARAM)_T("RealTime"));
        SendMessage(modeWnd, CB_ADDSTRING, 0, (LPARAM)_T("Messages"));
        SendMessage(modeWnd, CB_ADDSTRING, 0, (LPARAM)_T("Translation"));
        SendMessage(modeWnd, CB_ADDSTRING, 0, (LPARAM)_T("RSTT"));
        SendMessage(modeWnd, CB_SETCURSEL, 0, 0);

        HWND roomIDWnd = CreateWindow(_T("COMBOBOX"), _T("RoomID"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_ROOM_ID, hInstance, NULL);
        for(int i = 0; i < 30; ++i)
        {
            SendMessage(roomIDWnd, CB_ADDSTRING, 0, (LPARAM)char2wchar(ItoA(i).c_str()).c_str());
        }
        SendMessage(roomIDWnd, CB_SETCURSEL, 0, 0);

        HWND roleWnd = CreateWindow(_T("COMBOBOX"), _T("Role"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_ROLE, hInstance, NULL);
        SendMessage(roleWnd, CB_ADDSTRING, 0, (LPARAM)_T("Invalid"));
        SendMessage(roleWnd, CB_ADDSTRING, 0, (LPARAM)_T("Anchor"));
        SendMessage(roleWnd, CB_ADDSTRING, 0, (LPARAM)_T("Audience"));
        SendMessage(roleWnd, CB_SETCURSEL, 1, 0);

        HWND memberVoiceWnd = CreateWindow(_T("EDIT"), _T("MemberVoice"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y += height_pan, width, height, hWnd, (HMENU)IDC_GVOICE_MEMBER_VOICE, hInstance, NULL);

        HWND filePathWnd = CreateWindow(_T("COMBOBOX"), _T("FilePath"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_FILE_PATH, hInstance, NULL);
        for(int i = 0; i < 30; ++i)
        {
            SendMessage(filePathWnd, CB_ADDSTRING, 0, (LPARAM)char2wchar(ItoA(i).c_str()).c_str());
        }
        SendMessage(filePathWnd, CB_SETCURSEL, 0, 0);

        HWND fileIDWnd = CreateWindow(_T("EDIT"), _T("FileID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y += height_pan, width, height, hWnd, (HMENU)IDC_GVOICE_FILE_ID, hInstance, NULL);

        HWND maxMessageLengthWnd = CreateWindow(_T("COMBOBOX"), _T("MaxMessageLength"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_MAX_MESSAGE_LENGTH, hInstance, NULL);
        for(int i = 0; i < 300; ++i)
        {
            SendMessage(maxMessageLengthWnd, CB_ADDSTRING, 0, (LPARAM)char2wchar(ItoA(1000 * i).c_str()).c_str());
        }
        SendMessage(maxMessageLengthWnd, CB_SETCURSEL, 60, 0);

        HWND forbidMemberWnd = CreateWindow(_T("EDIT"), _T("ForbidMember"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y += height_pan, width, height, hWnd, (HMENU)IDC_GVOICE_FORBID_MEMBER, hInstance, NULL);

        HWND forbidMemberOnWnd = CreateWindow(_T("COMBOBOX"), _T("ForbidMemberOn"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_FORBID_MEMEBER_ON, hInstance, NULL);
        SendMessage(forbidMemberOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("false"));
        SendMessage(forbidMemberOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("true"));
        SendMessage(forbidMemberOnWnd, CB_SETCURSEL, 0, 0);

        HWND micVolumeWnd = CreateWindow(_T("COMBOBOX"), _T("MicVolume"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_MIC_VOLUME, hInstance, NULL);
        for(int i = 0; i < 21; ++i)
        {
            SendMessage(micVolumeWnd, CB_ADDSTRING, 0, (LPARAM)char2wchar(ItoA(100 * (i - 10)).c_str()).c_str());
            SendMessage(micVolumeWnd, CB_SETITEMDATA, i, 100 * (i - 10));
        }
        SendMessage(micVolumeWnd, CB_SETCURSEL, 20, 0);

        HWND micLevelWnd = CreateWindow(_T("EDIT"), _T("MicLevel"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y += height_pan, width, height, hWnd, (HMENU)IDC_GVOICE_MIC_LEVEL, hInstance, NULL);

        HWND speakerVolumeWnd = CreateWindow(_T("COMBOBOX"), _T("SpeakerVolume"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_SPEAKER_VOLUME, hInstance, NULL);
        for(int i = 0; i < 11; ++i)
        {
            SendMessage(speakerVolumeWnd, CB_ADDSTRING, 0, (LPARAM)char2wchar(ItoA(10 * i).c_str()).c_str());
            SendMessage(speakerVolumeWnd, CB_SETITEMDATA, i, 10 * i);
        }
        SendMessage(speakerVolumeWnd, CB_SETCURSEL, 10, 0);

        HWND speakerLevelWnd = CreateWindow(_T("EDIT"), _T("SpeakerLevel"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y += height_pan, width, height, hWnd, (HMENU)IDC_GVOICE_SPEAKER_LEVEL, hInstance, NULL);

        HWND speakerOnWnd = CreateWindow(_T("COMBOBOX"), _T("SpeakerOn"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_SPEAKER_ON, hInstance, NULL);
        SendMessage(speakerOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("false"));
        SendMessage(speakerOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("true"));
        SendMessage(speakerOnWnd, CB_SETCURSEL, 0, 0);

        HWND voiceEffectsWnd = CreateWindow(_T("COMBOBOX"), _T("VoiceEffects"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_VOICE_EFFECTS, hInstance, NULL);
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("教堂"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("剧院"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("地狱"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("机器人"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("男变女"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("女变男"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("醉酒"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("papi酱"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("松鼠"));
        SendMessage(voiceEffectsWnd, CB_ADDSTRING, 0, (LPARAM)_T("无变声效果"));
        SendMessage(voiceEffectsWnd, CB_SETCURSEL, 9, 0);

        HWND reverbOnWnd = CreateWindow(_T("COMBOBOX"), _T("ReverbOn"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_REVERB_ON, hInstance, NULL);
        SendMessage(reverbOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("false"));
        SendMessage(reverbOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("true"));
        SendMessage(reverbOnWnd, CB_SETCURSEL, 0, 0);

        HWND revertModeWnd = CreateWindow(_T("COMBOBOX"), _T("ReverbMode"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_REVERB_MODE, hInstance, NULL);
        SendMessage(revertModeWnd, CB_ADDSTRING, 0, (LPARAM)_T("语音增强"));
        SendMessage(revertModeWnd, CB_ADDSTRING, 0, (LPARAM)_T("无音效"));
        SendMessage(revertModeWnd, CB_ADDSTRING, 0, (LPARAM)_T("小房间音效"));
        SendMessage(revertModeWnd, CB_ADDSTRING, 0, (LPARAM)_T("大房间音效"));
        SendMessage(revertModeWnd, CB_ADDSTRING, 0, (LPARAM)_T("教堂音效"));
        SendMessage(revertModeWnd, CB_ADDSTRING, 0, (LPARAM)_T("剧院音效"));
        SendMessage(revertModeWnd, CB_SETCURSEL, 0, 0);

        HWND BGMOnWnd = CreateWindow(_T("COMBOBOX"), _T("BGMOn"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_BGM_ON, hInstance, NULL);
        SendMessage(BGMOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("false"));
        SendMessage(BGMOnWnd, CB_ADDSTRING, 0, (LPARAM)_T("true"));
        SendMessage(BGMOnWnd, CB_SETCURSEL, 0, 0);

        HWND BGMPathWnd = CreateWindow(_T("EDIT"), _T(".\\1.mp3"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y += height_pan, width, height, hWnd, (HMENU)IDC_GVOICE_BGM_PATH, hInstance, NULL);

        HWND BGMVolWnd = CreateWindow(_T("COMBOBOX"), _T("BGMVol"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, x, y += height_pan, width, combobox_height, hWnd, (HMENU)IDC_GVOICE_BGM_VOL, hInstance, NULL);
        for(int i = 0; i < 9; ++i)
        {
            SendMessage(BGMVolWnd, CB_ADDSTRING, 0, (LPARAM)char2wchar(ItoA(100 * i).c_str()).c_str());
            SendMessage(BGMVolWnd, CB_SETITEMDATA, i, 100 * i);
        }
        SendMessage(BGMVolWnd, CB_SETCURSEL, 8, 0);
    }
    {
        int x = init_x + 2 * width_pan;
        int y = init_y;

        CreateWindow(_T("BUTTON"), _T("Init"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_INIT, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("SetMode"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_MODE, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("ApplyMessageKey"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_APPLY_MESSAGE_KEY, hInstance, NULL);
    }
    {

        int x = init_x + 3 * width_pan;
        int y = init_y;

        CreateWindow(_T("BUTTON"), _T("JoinTeamRoom"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_JOIN_TEAM_ROOM, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("JoinNationalRoom"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_JOIN_NATIONAL_ROOM, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("QuitRoom"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_QUIT_ROOM, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("ChangeRole"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_CHANGE_ROLE, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("OpenMic"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_OPEN_MIC, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("CloseMic"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_CLOSE_MIC, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("OpenSpeaker"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_OPEN_SPEAKER, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("CloseSpeaker"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_CLOSE_SPEAKER, hInstance, NULL);
    }
    {

        int x = init_x + 4 * width_pan;
        int y = init_y;

        CreateWindow(_T("BUTTON"), _T("StartRecord"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_START_RECORDING, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("StopRecord"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_STOP_RECORDING, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("UploadRecordFile"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_UPLOAD_RECORDED_FILE, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("DownloadRecordFile"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_DOWNLOAD_RECORDED_FILE, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("PlayRecordFile"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_PLAY_RECORDED_FILE, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("StopPlayFile"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_STOP_PLAY_FILE, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("SpeechToText"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SPEECH_TO_TEXT, hInstance, NULL);
    }
    {
        int x = init_x + 5 * width_pan;
        int y = init_y;

        CreateWindow(_T("BUTTON"), _T("SetMaxMessageLength"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_MAX_MESSAGE_LENGTH, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("GetFileParam"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_GET_FILE_PARAM, hInstance, NULL);
    }
    {
        int x = init_x + 6 * width_pan;
        int y = init_y;

        CreateWindow(_T("BUTTON"), _T("ForbidMemberVoice"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_FORBID_MEMBER_VOICE, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("SetMicVolume"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_MIC_VOLUME, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("SetSpeakerVolume"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_SPEAKER_VOLUME, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("EnableSpeakerOn"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_ENABLE_SPEAKER_ON, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("TestMic"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_TEST_MIC, hInstance, NULL);
    }
    {
        int x = init_x + 7 * width_pan;
        int y = init_y;
        
        CreateWindow(_T("BUTTON"), _T("SetVoiceEffects"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_VOICE_EFFECTS, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("EnableReverb"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_ENABLE_REVERB, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("SetReverbMode"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_REVERB_MODE, hInstance, NULL);
    //}
    //{
        //int x = init_x + 8 * width_pan;
        //int y = init_y;

        CreateWindow(_T("BUTTON"), _T("EnableNativeBGMPlay"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_ENABLE_NATIVE_BGM_PLAY, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("SetBGMPath"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_BGM_PATH, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("StartBGMPlay"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_START_BGM_PLAY, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("StopBGMPlay"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_STOP_BGM_PLAY, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("PauseBGMPlay"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_PAUSE_BGM_PLAY, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("ResumeBGMPlay"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_RESUME_BGM_PLAY, hInstance, NULL);
        CreateWindow(_T("BUTTON"), _T("SetBGMVol"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y += height_pan, width, height, hWnd, (HMENU)IDM_GVOICE_SET_BGM_VOL, hInstance, NULL);
    }
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
