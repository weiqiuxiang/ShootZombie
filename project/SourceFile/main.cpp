/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
���b�Z�[�WBOX�\������[main.cpp]

Author :
Data   :
----------------------------------------------------------------
��Update : 2016/04/18
 �E_tWinMain��WinMain�ɕύX
 �EWinMain��WINAPI��APIENTRY�ɕύX
 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//-----------------------------------------------------------------------------------------
//�C���N���[�h�w�b�h
//-----------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "titleManager.h"
#include "sound.h"
#include "polygon.h"
#include "model.h"
#include "MeshField.h"
#include "skydome.h"
#include "camera.h"
#include "TpsCamera.h"
#include "light.h"
#include "input.h"
#include "inputMouse.h"
#include "model_aircraft.h"
#include "EnemyBall.h"
#include "shadow.h"
#include "bullet.h"
#include "explosion.h"
#include "hitRectangle .h"
#include "tpsAim.h"
#include "matrix.h"
#include "hitCircle.h"
#include "fade.h"
#include "tutorial.h"
#include "result.h"
#include "CScore.h"
#include "CGameUI.h"
#include "CScene3D.h"
#include "particalManager.h"
#include "wave.h"
#include "motion.h"

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define CLASS_NAME      "�T���v��"                  //�E�B���h�E�N���X�̖��O
#define WINDOW_NAME     "�E�C���h�E�\������"        //�E�B���h�E�̖��O

#define ID_BUTTON000 (101)
#define ID_EDIT000 (201)
#define ID_EDIT001 (202)

#define ID_TIMER (301)
#define TIMER_INTERVAL (1000/60)

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;            //Direct3D�f�o�C�X�C���^�[�t�F�C�X
bool g_CameraSwitch;                              //�uFPS:flase�v�ƁuTPS:true�v��؂�ւ��X�C�b�`
LPD3DXFONT g_pGuideFont = NULL;                   //�K�C�h�����\��
GAMEMODE g_mode = MODE_TITLE;                         //��ʑJ�ڕϐ��̍쐬

//�N���X�ϐ�
CTitle g_CTitle;                                  //�^�C�g���N���X�ϐ�
CTutorial g_CTutorial;                            //�`���[�g���A���N���X�ϐ�
CScore g_CScore;
CResult g_CResult;
CGameUI g_CGameUI;
CScene3D g_CScene3D;

D3DXVECTOR2 g_MousePosInWindow;
//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
void Update(HWND hwnd);                                                             //�v���g�^�C�v�錾�̒�`
void Draw(void);                                                               //�`�揈��
void DrawGuideFont(void);                                                      //�K�C�h�����\��
void LockCursor(HWND hwnd);                            //�J�[�\���̍��W���b�N
void ShowCursorMy(bool bUse);                          //�}�E�X�\��,��\���̊֐�
void SaveMousePos(HWND hWnd);

//-----------------------------------------------------------------------------------------
//���C���֐�
// (APIENTRY : Win32API�֐����Ăяo�����̋K��[WINAPI�ł��ǂ�])
// hInstance     : ���̃A�v���P�[�V�����̃C���X�^���X�n���h��(���ʎq)
// hPrevInstance : ���NULL(16bit����̖��c��)
// lpCmdLine     : �R�}���h���C������󂯎���������ւ̃|�C���^
// nCmdShow      : �A�v���P�[�V�����̏����E�C���h�E�\�����@�̎w��
//-----------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//warning��\�������Ȃ����߁A�Ȃ�����������Ȃ�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RECT cr = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	AdjustWindowRect(&cr,WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX),FALSE);

	RECT dr;
	GetWindowRect(GetDesktopWindow(),&dr);

	int dw = dr.right -dr.left;
	int dh = dr.bottom -dr.top;

	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	int wx = ww > dw ? 0 : (dw-ww)/2;
	int wy = wh > dh ? 0 : (dh-wh)/2;

	WNDCLASSEX wcex = 
	{
		sizeof(WNDCLASSEX),                     //WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,                             //�\������E�B���h�E�̃X�^�C�����w��
		WndProc,								//�E�B���h�E�v���V�[�W���̃A�h���X(�֐���)���w��
		0,										//�ʏ�͎g�p���Ȃ��̂�"0"���w��
		0,										//�ʏ�͎g�p���Ȃ��̂�"0"���w��
		hInstance,								//WinMain�̃p�����[�^�̃C���X�^���X�n���h�����w��
		NULL,									//�g�p����A�C�R�����w��(Windows�������Ă���A�C�R�����g��)
		LoadCursor(NULL,IDC_ARROW),				//�}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),				//�E�B���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,									//Window�ɂ��郁�j���[��ݒ�
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		NULL									//�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	};

	//�E�B���h�E�N���X�̓o�^(�I���W�i���̃E�B���h�E�̖��O�̓o�^)
	RegisterClassEx(&wcex);

	HWND hWnd;                                  //hWnd:�E�B���h�E�n���h��
	MSG msg;

	//�E�B���h�E���쐬
	hWnd = CreateWindowEx(0,                    //�g���E�B���h�E�X�^�C��
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,							//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX),					//�E�B���h�E�X�^�C��
		wx,							            //�E�B���h�E�̍���X���W
		wy,							            //�E�B���h�E�̍���Y���W
		ww,							            //�E�B���h�E�̕�
		wh,							            //�E�B���h�E�̍���
		NULL,									//�e�E�B���h�E�̃n���h��
		NULL,									//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,								//�C���X�^���X�n���h��
		NULL);									//�E�B���h�E�쐬�f�[�^
	
	
	//�E�B���h�E�̕\��
	ShowWindow(hWnd,nCmdShow);					//�w�肳�ꂽ�E�C���h�E�̕\��
	UpdateWindow(hWnd);							//�E�B���h�E�̏�Ԓ����ɕԂ�

	//������
	Init(hInstance,hWnd,1);

	//����\�̐ݒ�
	timeBeginPeriod(1);

	//�e�J�E���^�[�̏�����
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;
	DWORD dwExecLastTime = timeGetTime();                  //���݂̎������~���b�P��
	DWORD dwFPSLastTime = dwExecLastTime;

	SetFade(FADE_IN, MODE_TITLE);
	//���b�Z�[�W���[�v
	for(;;)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0 )
		{
			//Windows�̏���
			if(msg.message == WM_QUIT) break;
			else
			{
				TranslateMessage(&msg);               //�Ăяo�����X���b�h�̃��b�Z�[�W�L���[��
				DispatchMessage(&msg);                //���b�Z�[�W�̖|��Ƒ��o
			}
		}

		//�Q�[���̏���
		else
		{
			dwCurrentTime = timeGetTime();

			//1�t���[���̎��Ԃ��o��������Q�[�����[�v
			if( (dwCurrentTime - dwExecLastTime) >= (1000/60) )
			{
				dwExecLastTime = dwCurrentTime;

				Update(hWnd);        //�X�V����
				Draw();          //�`�揈��
			}
		}
	}

	//����\��߂�
	timeEndPeriod(1);

	//�I������
	Uninit();

	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------------------
//�E�B���h�E�v���V�[�W��
//(CALLBACK:Win32API�֐����Ăяo�����̋K��)
//hWnd   :�E�B���h�E�̃n���h��
//uMsg   :���b�Z�[�W�̎��ʎq
//wParam :���b�Z�[�W�̍ŏ��̃p�����[�^
//LParam :���b�Z�[�W��2�Ԗڂ̃p�����[�^
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static float x = 0;
	static float y = 0;

	static char buf[256];
	HDC hDC;
	PAINTSTRUCT ps;
	const RECT rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	char temp[256];
	UINT ID;
	switch(uMsg)
	{
	case WM_DESTROY:
		//MessageBox(NULL,"DESTROY","DESTROY",MB_OK); //�E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);							//"WM_QUIT"���b�Z�[�W��Ԃ�
		break;
		
	case WM_CREATE:
		
		SetTimer(hWnd,ID_TIMER,TIMER_INTERVAL,NULL);

		break;

	case WM_COMMAND:
		
		break;

	case WM_PAINT:
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			ShowCursor(TRUE);
			ID = MessageBox(NULL, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
			if (ID == IDYES) DestroyWindow(hWnd);
			else ShowCursor(FALSE);
			break;
		}
		break;
	case WM_CLOSE:
		//MessageBox(NULL,"CLOSE","CLOSE",MB_YESNO);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);  //����̏������(�E�B���h�E�̊�{�̓�����񋟂���(����͂Ȃ��Ɗ댯�I))
}

//���s�������̓��e���i�[�����Ă���
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;         //�f�o�C�X�̓����ݒ肷��\����
	D3DDISPLAYMODE d3ddm;                //�f�o�C�X��IDirect3Device9�C���^�[�t�F�C�X

	//D3D9�C���^�[�t�F�C�X�̎擾
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h�̎擾
	//FAILED�}�N��:DIRECTX�v���O���������s���H�Ƃ����}�N��
	if ( FAILED( g_pD3D -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm) ) )
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�[�̐ݒ�
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.EnableAutoDepthStencil = TRUE;                           //DepthStencil���g���܂�
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                     //�f�u�X�o�b�t�@16�r�b�g
	d3dpp.Windowed = bWindow;                                      //�E�B���h�E���[�h���t���X�N���[�����[�h��I��
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;    //�t���X�N���[�����[�h�ɂ�����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;      //�t���X�N���[�����[�h�ɂ�����

	//Direct3D�f�o�C�X�̍쐬
	if(FAILED(g_pD3D -> CreateDevice(
		D3DADAPTER_DEFAULT,                                    //�ǂ̕\���n�[�h�E�F�A�ɂ��܂����H(default�Ȃ玩���̃p�\�R���̃X�N���[��)
		D3DDEVTYPE_HAL,                                        //�`�揈���^�C�v(�n�[�h�E�F�A�ł�邩�\�t�g�E�F�A�ł�邩�H_HAL�Ȃ�n�[�h�E�F�A)
		hWnd,                                                  //�E�B���h�E�n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,                   //�f�o�C�X�쐬�I�v�V�����t���O(���̏ꍇ:�n�[�h�E�F�A�Œ��_����������)
		&d3dpp,                                                //���̑��̐ݒ�
		&g_pD3DDevice)))                                       //Direct3D�f�o�C�X�C���^�[�t�F�C�X�̃|�C���^�̃|�C���^(�Ԑڂ���)
	{
		if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
		{
			if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
			{
				MessageBox(NULL,"�f�o�C�X�̎擾�����s���܂���","�G���[",MB_OK|MB_ICONHAND);
				return E_FAIL;
			}
		}
	}

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pGuideFont);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice -> SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	g_pD3DDevice -> SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	srand((unsigned)time(NULL));                       //�����擾

	g_CameraSwitch = false;
	InitSound(hWnd);
	InitHitRect();
	InitHitCircle();
	InitPolygon();
	InitTpsCamera();
	InitModel();
	InitAircraft();
	InitEB();
	InitMeshField();
	g_CScene3D.Init();
	InitSkydome();
	InitShadow();
	InitBullet();
	InitExplosion();
	InitCamera();
	InitLight();
	InitKeyboard(hInstance,hWnd);
	InitMouse(hInstance, hWnd);
	InitWave();

	g_CTitle.Init();
	g_CTutorial.Init();
	g_CScore.Init(D3DXVECTOR3(SCREEN_WIDTH*0.7,SCREEN_HEIGHT*0.05,0.0f));
	g_CResult.Init();
	CParticalManager::Init();
	g_CGameUI.Init();
	InitFade();

	return S_OK;
}

//�I���֐�(��ЂÂ�)
void Uninit(void)
{
	g_CTitle.Uninit();
	g_CTutorial.Uninit();
	g_CScore.Uninit();
	g_CResult.Uninit();
	g_CGameUI.Uninit();
	g_CScene3D.Uninit();
	CParticalManager::Uninit();
	UninitSound();
	UninitPolygon();
	UninitModel();
	UninitAircraft();
	UninitEB();
	UninitMeshField();
	UninitHitRect();
	UninitHitCircle();
	UninitSkydome();
	UninitKeyboard();
	UninitMouse();
	UninitLight();
	UninitBullet();
	UninitExplosion();
	UninitShadow();
	UninitFade();
	SAFE_RELEASE(g_pGuideFont);
	SAFE_RELEASE (g_pD3DDevice);
	SAFE_RELEASE (g_pD3D);
}

////////////////////////////////////////////////
//�v���g�^�C�v�錾�̒�`
////////////////////////////////////////////////
void Update(HWND hwnd)
{
	SaveMousePos(hwnd);                                       //�}�E�X���W�f�[�^�ۑ�

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateKeyboard();
		UpdateMouse();
		g_CTitle.Update();
		UpdateFade();
		break;
	case MODE_TUTORIAL:
		UpdateKeyboard();
		UpdateMouse();
		g_CTutorial.Update();
		UpdateFade();
		break;
	case MODE_GAME:
		SetProjectionMatrix();               //�v���W�F�N�V�����}�g���N�X�Z�b�g
		LockCursor(hwnd);                    //�}�E�X�ʒu���b�N
		UpdateWave();
		UpdateKeyboard();
		UpdateMouse();
		UpdateCamera();
		UpdateAircraft();
		g_CScene3D.Update();
		UpdateEB();
		UpdateTpsCamera();
		UpdateModel();
		UpdateShadow();
		UpdateBullet();
		UpdateExplosion();
		UpdatePolygon();
		UpdateMeshField();
		UpdateHitRect();
		UpdateHitCircle();
		UpdateSkydome();
		UpdateLight();
		UpdateFade();
		g_CScore.Update();
		g_CGameUI.Update();
		CParticalManager::Update();
		break;
	case MODE_RESULT:
		UpdateKeyboard();
		UpdateMouse();
		g_CResult.Update();
		UpdateFade();
		break;
	}
}

////////////////////////////////////////////////
//�`�揈��
////////////////////////////////////////////////
void Draw(void)
{
	//�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	g_pD3DDevice -> Clear(
		0,
		NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,          //�F|�[�x���
		D3DCOLOR_RGBA(0,0,0,0),              //�N���A�F
		1.0f,                                      //�N���A�[�x�l(0:��O 1:��Ԍ��)
		0                                          //�X�e���V���l(0~255)
		);

	//�`��̊J�n
	if(SUCCEEDED(g_pD3DDevice -> BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:
			g_CTitle.Draw();
			break;
		case MODE_TUTORIAL:
			g_CTutorial.Draw();
			break;
		case MODE_GAME:
			DrawAircraft();
			g_CScene3D.Draw();
			DrawEB();
			DrawMeshField();
			DrawShadow();
			DrawSkydome();
			DrawBullet();                        //�o���b�g�`��
			DrawExplosion();                     //�����`��
			CParticalManager::Draw();
			DrawHitRect();
			DrawHitCircle();
			DrawPolygon();
			g_CScore.Draw();
			g_CGameUI.Draw();
#ifdef _DEBUG
			//DrawGuideFont();                     //�K�C�h����
#endif
			break;
		case MODE_RESULT:
			g_CResult.Draw();
			g_CScore.Draw();
			break;
		}

		DrawFade();

		//�`��I��
		g_pD3DDevice -> EndScene();
	}

	g_pD3DDevice -> Present(NULL,NULL,NULL,NULL);
}

////////////////////////////////////////////////
//DIRECT3D�f�o�C�X���Q�b�g
////////////////////////////////////////////////
LPDIRECT3DDEVICE9 Get_Device(void)
{
	return g_pD3DDevice;
}

//�J�����X�C�b�`�擾
bool *GetCameraSwitch(void) 
{
	return &g_CameraSwitch;
}

void DrawGuideFont(void)
{
	CAMERA_FPS *pCameraFps = GetCameraTps();            //�J�����擾
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //�J���������_�擾
	float fRayDi = GetRayDistance();
	D3DXVECTOR3 fRayInt = GetRayInt();
	AIRCRAFT *pAircraft = Get_Aircraft();             //�v���[���[�f�[�^�擾

	char aStr[512];
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	sprintf(&aStr[0], "\n���f���̈ړ�:\n W:�O \n S:�� \n A:�� \n D:�E \n Q:�㏸ \n E:�~��\n �v���[���[�ʒu: X:%f Y:%f Z:%f\n\n�J������]:�}�E�X�̈ړ�\n\n�e����:�}�E�X���N���b�N\n\n�J���������p�x:%.2f \n\n�J���������p�x:%.2f \n ���b�V���R���W�����\�� : F1�L�[",
		pAircraft->m_Matrix.pos.x, pAircraft->m_Matrix.pos.y, pAircraft->m_Matrix.pos.z,
		(pCameraFps->fAngleHorizon * 180 / PI),
		(pCameraFps->fAngleVertical * 180 / PI),
		g_pGuideFont);
	g_pGuideFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 255, 255));
}

void SetMode(GAMEMODE mode)
{
	//�I������
	switch (mode)
	{
	case MODE_TITLE:
		g_CTitle.Uninit();                                //�^�C�g��
		break;

	case MODE_TUTORIAL:
		g_CTutorial.Uninit();
		break;

	case MODE_GAME:
		g_CScore.Uninit();
		g_CGameUI.Uninit();
		g_CScene3D.Uninit();
		UninitPolygon();
		UninitModel();
		UninitAircraft();
		UninitEB();
		UninitMeshField();
		UninitHitRect();
		UninitHitCircle();
		UninitSkydome();
		UninitLight();
		UninitBullet();
		UninitExplosion();
		UninitShadow();
		CParticalManager::Uninit();
		break;
	case MODE_RESULT:
		g_CResult.Uninit();                                //�^�C�g��
		break;
	}

	//����������
	switch (mode)
	{
	case MODE_TITLE:
		g_CScore.Init(D3DXVECTOR3(SCREEN_WIDTH*0.6, SCREEN_HEIGHT*0.1, 0.0f));
		g_CTitle.Init();                                //�^�C�g��
		ShowCursorMy(true);                         //�}�E�X�\��
		break;
	case MODE_TUTORIAL:
		g_CTutorial.Init();
		ShowCursorMy(true);                         //�}�E�X�\��
		break;
	case MODE_GAME:
		g_CScore.Init(D3DXVECTOR3(SCREEN_WIDTH*0.7, SCREEN_HEIGHT*0.05, 0.0f));
		g_CGameUI.Init();
		InitWave();
		InitHitRect();
		InitHitCircle();
		InitPolygon();
		InitTpsCamera();
		InitModel();
		InitAircraft();
		InitEB();
		g_CScene3D.Init();
		InitMeshField();
		InitSkydome();
		InitShadow();
		InitBullet();
		InitExplosion();
		InitCamera();
		InitLight();
		CParticalManager::Init();
		ShowCursorMy(false);                         //�}�E�X��\��
		break;
	case MODE_RESULT:
		g_CResult.Init();                                //�^�C�g��
		ShowCursorMy(true);                         //�}�E�X�\��
		break;
	}

	//���[�h�ϊ��ƃt���O�𗧂�����
	g_mode = mode;
}

void LockCursor(HWND hwnd)
{
	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);

	GetWindowInfo(hwnd, &windowInfo);

	int wx = windowInfo.rcWindow.left;
	int wy = windowInfo.rcWindow.top;
	int ww = windowInfo.rcWindow.right - windowInfo.rcWindow.left;
	int wh = windowInfo.rcWindow.bottom - windowInfo.rcWindow.top;

	//�J�[�\�����W�|�C���^
	int CurX = (wx + ww*0.5);
	int CurY = (wy + wh*0.5);
	POINT lpPoint;
	lpPoint.x = (LONG)CurX;
	lpPoint.y = (LONG)CurY;
	POINT lpPointOld = lpPoint;

	//�}�E�X�J�[�\���𑩔�����
	lpPointOld = lpPoint;
	BOOL CursorCheck = GetCursorPos(&lpPoint);
	if (CursorCheck == TRUE)
	{
		if ((lpPoint.x <= wx + ww*0.25) ||
			(lpPoint.x >= wx + ww*0.75))
		{
			lpPoint.x = lpPointOld.x;
		}

		if ((lpPoint.y <= wy + wh*0.25) ||
			(lpPoint.y >= wy + wh*0.75))
		{
			lpPoint.y = lpPointOld.y;
		}
	}
	SetCursorPos(lpPoint.x, lpPoint.y);
}

void ShowCursorMy(bool bUse)
{
	int c;
	if(bUse == true)
	{
		do
		{
			c = ShowCursor(TRUE);
		} while (c < 0);
	}

	else
	{
		do
		{
			c = ShowCursor(FALSE);
		} while (c >= 0);
	}
}

void SaveMousePos(HWND hwnd)
{
	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);

	GetWindowInfo(hwnd, &windowInfo);

	int wx = windowInfo.rcWindow.left;                                               //Window����x���W
	int wy = windowInfo.rcWindow.top;												 //Window����y���W
	int ww = windowInfo.rcWindow.right - windowInfo.rcWindow.left;					 //Window��
	int wh = windowInfo.rcWindow.bottom - windowInfo.rcWindow.top;					 //Window����

	POINT lpPoint;
	BOOL CursorCheck = GetCursorPos(&lpPoint);                                     //�}�E�X���W�擾
	g_MousePosInWindow.x = lpPoint.x - wx;                                         //�}�E�X�̃E�B���h�Ex���W�v�Z
	g_MousePosInWindow.y = lpPoint.y - wy - 20.0f;                                         //�}�E�X�̃E�B���h�Ey���W�v�Z
}

D3DXVECTOR3 GetMousePosInWindow(void)
{
	return g_MousePosInWindow;
}

CScore *GetScore(void)
{
	return &g_CScore;
}