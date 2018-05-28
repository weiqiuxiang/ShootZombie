/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
メッセージBOX表示処理[main.cpp]

Author :
Data   :
----------------------------------------------------------------
■Update : 2016/04/18
 ・_tWinMainをWinMainに変更
 ・WinMainのWINAPIをAPIENTRYに変更
 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//-----------------------------------------------------------------------------------------
//インクルードヘッド
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
//マクロ定義
//-----------------------------------------------------------------------------------------
#define CLASS_NAME      "サンプル"                  //ウィンドウクラスの名前
#define WINDOW_NAME     "ウインドウ表示処理"        //ウィンドウの名前

#define ID_BUTTON000 (101)
#define ID_EDIT000 (201)
#define ID_EDIT001 (202)

#define ID_TIMER (301)
#define TIMER_INTERVAL (1000/60)

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;            //Direct3Dデバイスインターフェイス
bool g_CameraSwitch;                              //「FPS:flase」と「TPS:true」を切り替えスイッチ
LPD3DXFONT g_pGuideFont = NULL;                   //ガイド文字表示
GAMEMODE g_mode = MODE_TITLE;                         //画面遷移変数の作成

//クラス変数
CTitle g_CTitle;                                  //タイトルクラス変数
CTutorial g_CTutorial;                            //チュートリアルクラス変数
CScore g_CScore;
CResult g_CResult;
CGameUI g_CGameUI;
CScene3D g_CScene3D;

D3DXVECTOR2 g_MousePosInWindow;
//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
void Update(HWND hwnd);                                                             //プロトタイプ宣言の定義
void Draw(void);                                                               //描画処理
void DrawGuideFont(void);                                                      //ガイド文字表示
void LockCursor(HWND hwnd);                            //カーソルの座標ロック
void ShowCursorMy(bool bUse);                          //マウス表示,非表示の関数
void SaveMousePos(HWND hWnd);

//-----------------------------------------------------------------------------------------
//メイン関数
// (APIENTRY : Win32API関数を呼び出す時の規約[WINAPIでも良い])
// hInstance     : このアプリケーションのインスタンスハンドル(識別子)
// hPrevInstance : 常にNULL(16bit時代の名残り)
// lpCmdLine     : コマンドラインから受け取った引数へのポインタ
// nCmdShow      : アプリケーションの初期ウインドウ表示方法の指定
//-----------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//warningを表示させないため、なんも役が立たない
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
		sizeof(WNDCLASSEX),                     //WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,                             //表示するウィンドウのスタイルを指定
		WndProc,								//ウィンドウプロシージャのアドレス(関数名)を指定
		0,										//通常は使用しないので"0"を指定
		0,										//通常は使用しないので"0"を指定
		hInstance,								//WinMainのパラメータのインスタンスハンドルを指定
		NULL,									//使用するアイコンを指定(Windowsが持っているアイコンを使う)
		LoadCursor(NULL,IDC_ARROW),				//マウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),				//ウィンドウのクライアント領域の背景色を設定
		NULL,									//Windowにつけるメニューを設定
		CLASS_NAME,								//ウィンドウクラスの名前
		NULL									//小さいアイコンが設定された場合の情報を記述
	};

	//ウィンドウクラスの登録(オリジナルのウィンドウの名前の登録)
	RegisterClassEx(&wcex);

	HWND hWnd;                                  //hWnd:ウィンドウハンドル
	MSG msg;

	//ウィンドウを作成
	hWnd = CreateWindowEx(0,                    //拡張ウィンドウスタイル
		CLASS_NAME,								//ウィンドウクラスの名前
		WINDOW_NAME,							//ウィンドウの名前
		WS_OVERLAPPEDWINDOW ^ (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX),					//ウィンドウスタイル
		wx,							            //ウィンドウの左上X座標
		wy,							            //ウィンドウの左上Y座標
		ww,							            //ウィンドウの幅
		wh,							            //ウィンドウの高さ
		NULL,									//親ウィンドウのハンドル
		NULL,									//メニューハンドルまたは子ウィンドウID
		hInstance,								//インスタンスハンドル
		NULL);									//ウィンドウ作成データ
	
	
	//ウィンドウの表示
	ShowWindow(hWnd,nCmdShow);					//指定されたウインドウの表示
	UpdateWindow(hWnd);							//ウィンドウの状態直ちに返す

	//初期化
	Init(hInstance,hWnd,1);

	//分解能の設定
	timeBeginPeriod(1);

	//各カウンターの初期化
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;
	DWORD dwExecLastTime = timeGetTime();                  //現在の時刻をミリ秒単位
	DWORD dwFPSLastTime = dwExecLastTime;

	SetFade(FADE_IN, MODE_TITLE);
	//メッセージループ
	for(;;)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE) != 0 )
		{
			//Windowsの処理
			if(msg.message == WM_QUIT) break;
			else
			{
				TranslateMessage(&msg);               //呼び出し側スレッドのメッセージキューか
				DispatchMessage(&msg);                //メッセージの翻訳と送出
			}
		}

		//ゲームの処理
		else
		{
			dwCurrentTime = timeGetTime();

			//1フレームの時間が経ったら一回ゲームループ
			if( (dwCurrentTime - dwExecLastTime) >= (1000/60) )
			{
				dwExecLastTime = dwCurrentTime;

				Update(hWnd);        //更新処理
				Draw();          //描画処理
			}
		}
	}

	//分解能を戻す
	timeEndPeriod(1);

	//終了処理
	Uninit();

	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------------------
//ウィンドウプロシージャ
//(CALLBACK:Win32API関数を呼び出す時の規約)
//hWnd   :ウィンドウのハンドル
//uMsg   :メッセージの識別子
//wParam :メッセージの最初のパラメータ
//LParam :メッセージの2番目のパラメータ
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
		//MessageBox(NULL,"DESTROY","DESTROY",MB_OK); //ウィンドウ破棄のメッセージ
		PostQuitMessage(0);							//"WM_QUIT"メッセージを返す
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
			ID = MessageBox(NULL, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
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

	return DefWindowProc(hWnd,uMsg,wParam,lParam);  //既定の処理を提供(ウィンドウの基本の動きを提供する(これはないと危険！))
}

//失敗した時の内容を格納させていく
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;         //デバイスの動作を設定する構造体
	D3DDISPLAYMODE d3ddm;                //デバイスのIDirect3Device9インターフェイス

	//D3D9インターフェイスの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードの取得
	//FAILEDマクロ:DIRECTXプログラムが失敗か？というマクロ
	if ( FAILED( g_pD3D -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm) ) )
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメーターの設定
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.EnableAutoDepthStencil = TRUE;                           //DepthStencilを使えます
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                     //デブスバッファ16ビット
	d3dpp.Windowed = bWindow;                                      //ウィンドウモードかフルスクリーンモードを選択
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;    //フルスクリーンモードにおいて
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;      //フルスクリーンモードにおいて

	//Direct3Dデバイスの作成
	if(FAILED(g_pD3D -> CreateDevice(
		D3DADAPTER_DEFAULT,                                    //どの表示ハードウェアにしますか？(defaultなら自分のパソコンのスクリーン)
		D3DDEVTYPE_HAL,                                        //描画処理タイプ(ハードウェアでやるかソフトウェアでやるか？_HALならハードウェア)
		hWnd,                                                  //ウィンドウハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,                   //デバイス作成オプションフラグ(この場合:ハードウェアで頂点を処理する)
		&d3dpp,                                                //その他の設定
		&g_pD3DDevice)))                                       //Direct3Dデバイスインターフェイスのポインタのポインタ(間接かな)
	{
		if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
		{
			if(FAILED(g_pD3D -> CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pD3DDevice)))
			{
				MessageBox(NULL,"デバイスの取得が失敗しました","エラー",MB_OK|MB_ICONHAND);
				return E_FAIL;
			}
		}
	}

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pGuideFont);

	//サンプラーステートの設定
	g_pD3DDevice -> SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	g_pD3DDevice -> SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	srand((unsigned)time(NULL));                       //乱数取得

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

//終了関数(後片づけ)
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
//プロトタイプ宣言の定義
////////////////////////////////////////////////
void Update(HWND hwnd)
{
	SaveMousePos(hwnd);                                       //マウス座標データ保存

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
		SetProjectionMatrix();               //プロジェクションマトリクスセット
		LockCursor(hwnd);                    //マウス位置ロック
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
//描画処理
////////////////////////////////////////////////
void Draw(void)
{
	//バックバッファとZバッファのクリア
	g_pD3DDevice -> Clear(
		0,
		NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,          //色|深度情報
		D3DCOLOR_RGBA(0,0,0,0),              //クリア色
		1.0f,                                      //クリア深度値(0:手前 1:一番後ろ)
		0                                          //ステンシル値(0~255)
		);

	//描画の開始
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
			DrawBullet();                        //バレット描画
			DrawExplosion();                     //爆発描画
			CParticalManager::Draw();
			DrawHitRect();
			DrawHitCircle();
			DrawPolygon();
			g_CScore.Draw();
			g_CGameUI.Draw();
#ifdef _DEBUG
			//DrawGuideFont();                     //ガイド文字
#endif
			break;
		case MODE_RESULT:
			g_CResult.Draw();
			g_CScore.Draw();
			break;
		}

		DrawFade();

		//描画終了
		g_pD3DDevice -> EndScene();
	}

	g_pD3DDevice -> Present(NULL,NULL,NULL,NULL);
}

////////////////////////////////////////////////
//DIRECT3Dデバイスをゲット
////////////////////////////////////////////////
LPDIRECT3DDEVICE9 Get_Device(void)
{
	return g_pD3DDevice;
}

//カメラスイッチ取得
bool *GetCameraSwitch(void) 
{
	return &g_CameraSwitch;
}

void DrawGuideFont(void)
{
	CAMERA_FPS *pCameraFps = GetCameraTps();            //カメラ取得
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //カメラ注視点取得
	float fRayDi = GetRayDistance();
	D3DXVECTOR3 fRayInt = GetRayInt();
	AIRCRAFT *pAircraft = Get_Aircraft();             //プレーヤーデータ取得

	char aStr[512];
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	sprintf(&aStr[0], "\nモデルの移動:\n W:前 \n S:後 \n A:左 \n D:右 \n Q:上昇 \n E:降下\n プレーヤー位置: X:%f Y:%f Z:%f\n\nカメラ回転:マウスの移動\n\n弾発射:マウス左クリック\n\nカメラ水平角度:%.2f \n\nカメラ垂直角度:%.2f \n メッシュコリジョン表示 : F1キー",
		pAircraft->m_Matrix.pos.x, pAircraft->m_Matrix.pos.y, pAircraft->m_Matrix.pos.z,
		(pCameraFps->fAngleHorizon * 180 / PI),
		(pCameraFps->fAngleVertical * 180 / PI),
		g_pGuideFont);
	g_pGuideFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 255, 255));
}

void SetMode(GAMEMODE mode)
{
	//終了処理
	switch (mode)
	{
	case MODE_TITLE:
		g_CTitle.Uninit();                                //タイトル
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
		g_CResult.Uninit();                                //タイトル
		break;
	}

	//初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		g_CScore.Init(D3DXVECTOR3(SCREEN_WIDTH*0.6, SCREEN_HEIGHT*0.1, 0.0f));
		g_CTitle.Init();                                //タイトル
		ShowCursorMy(true);                         //マウス表示
		break;
	case MODE_TUTORIAL:
		g_CTutorial.Init();
		ShowCursorMy(true);                         //マウス表示
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
		ShowCursorMy(false);                         //マウス非表示
		break;
	case MODE_RESULT:
		g_CResult.Init();                                //タイトル
		ShowCursorMy(true);                         //マウス表示
		break;
	}

	//モード変換とフラグを立たせる
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

	//カーソル座標ポインタ
	int CurX = (wx + ww*0.5);
	int CurY = (wy + wh*0.5);
	POINT lpPoint;
	lpPoint.x = (LONG)CurX;
	lpPoint.y = (LONG)CurY;
	POINT lpPointOld = lpPoint;

	//マウスカーソルを束縛する
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

	int wx = windowInfo.rcWindow.left;                                               //Window左上x座標
	int wy = windowInfo.rcWindow.top;												 //Window左上y座標
	int ww = windowInfo.rcWindow.right - windowInfo.rcWindow.left;					 //Window幅
	int wh = windowInfo.rcWindow.bottom - windowInfo.rcWindow.top;					 //Window高さ

	POINT lpPoint;
	BOOL CursorCheck = GetCursorPos(&lpPoint);                                     //マウス座標取得
	g_MousePosInWindow.x = lpPoint.x - wx;                                         //マウスのウィンドウx座標計算
	g_MousePosInWindow.y = lpPoint.y - wy - 20.0f;                                         //マウスのウィンドウy座標計算
}

D3DXVECTOR3 GetMousePosInWindow(void)
{
	return g_MousePosInWindow;
}

CScore *GetScore(void)
{
	return &g_CScore;
}