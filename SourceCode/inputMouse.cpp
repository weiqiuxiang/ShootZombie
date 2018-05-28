//=============================================================================
//
// 入力処理 [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "inputMouse.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(4)	    // キーの最大数
#define	LIMIT_COUNT_REPEAT	(10)	// リピートカウントリミッター

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInputMouse(HINSTANCE hInstance, HWND hWnd);
void UninitInputMouse(void);
void UpdateInputMouse(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pInputMouse = NULL;			// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevMouse = NULL;				// 入力デバイス(マウス)へのポインタ
DIMOUSESTATE            g_zdiMouseState;                //マウス状態取得
BYTE					g_aMouseState[NUM_KEY_MAX];			// マウスの入力情報ワーク
BYTE					g_aMouseStateTrigger[NUM_KEY_MAX];	// マウスのトリガー情報ワーク
BYTE					g_aMouseStateRelease[NUM_KEY_MAX];	// マウスのリリース情報ワーク
BYTE					g_aMouseStateRepeat[NUM_KEY_MAX];		// マウスのリピート情報ワーク
int						g_aMouseStateRepeatCnt[NUM_KEY_MAX];	// マウスのリピートカウンタ

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInputMouse(HINSTANCE hInstance, HWND hWnd)
{
	if(g_pInputMouse == NULL)
	{
		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInputMouse, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInputMouse(void)
{
	if(g_pInputMouse != NULL)
	{// DirectInputオブジェクトの開放
		g_pInputMouse->Release();
		g_pInputMouse = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInputMouse(void)
{
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT ret = S_FALSE;
	// 入力処理の初期化
	if(FAILED(InitInputMouse(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if(FAILED(g_pInputMouse->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）(調べる必要がある)
	if(FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	if (FAILED(ret)) {
		// デバイスの設定に失敗
		MessageBox(hWnd, "デバイスの設定失敗", "警告！", MB_ICONWARNING);
		return false;
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	g_pDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void UninitMouse(void)
{
	if(g_pDevMouse != NULL)
	{// 入力デバイス(マウス)の開放
		// マウスへのアクセス権を開放(入力制御終了)
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	// 入力処理の終了処理
	UninitInputMouse();
}

//=============================================================================
// マウスの更新処理
//=============================================================================
void UpdateMouse(void)
{
	// 読取前の値を保持します
	DIMOUSESTATE zdiMouseState_bak;	// マウス情報(変化検知用)
	memcpy(&zdiMouseState_bak, &g_zdiMouseState, sizeof(zdiMouseState_bak));       //今のマウスの状態を取得


	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState)))
	{
		for(int nCnMouse = 0; nCnMouse < NUM_KEY_MAX; nCnMouse++)
		{
			// キープレス情報を保存
			//キートリガーリリース情報の作成
			g_aMouseStateTrigger[nCnMouse] = (zdiMouseState_bak.rgbButtons[nCnMouse] ^ g_zdiMouseState.rgbButtons[nCnMouse]) & g_zdiMouseState.rgbButtons[nCnMouse];                          //g_aMouseStateは前,aMouseStateは今
			g_aMouseStateTrigger[nCnMouse] = (zdiMouseState_bak.rgbButtons[nCnMouse] ^ g_zdiMouseState.rgbButtons[nCnMouse]) & zdiMouseState_bak.rgbButtons[nCnMouse];                        //g_aMouseStateは前,aMouseStateは今
			g_aMouseState[nCnMouse] = g_zdiMouseState.rgbButtons[nCnMouse];

			//キーリビーと情報の生成
			if(g_zdiMouseState.rgbButtons[nCnMouse])
			{
				if(g_aMouseStateRepeatCnt[nCnMouse] < LIMIT_COUNT_REPEAT)
				{
					g_aMouseStateRepeatCnt[nCnMouse]++;
					if(g_aMouseStateRepeatCnt[nCnMouse] == 1 || g_aMouseStateRepeatCnt[nCnMouse] >= LIMIT_COUNT_REPEAT) 
					{
						g_aMouseStateRepeat[nCnMouse] = g_zdiMouseState.rgbButtons[nCnMouse];
						g_aMouseStateRepeatCnt[nCnMouse] = 2;
					}

					else
					{
						g_aMouseStateRepeat[nCnMouse] = 0;
					}
				}
			}

			else
			{
				g_aMouseStateRepeat[nCnMouse] = 0;
				g_aMouseStateRepeatCnt[nCnMouse] = 0; 
			}
		}
	}

	//失敗した場合はもう一度マウスへのアスセスを要求する
	else
	{
		// マウスへのアクセス権を取得
		g_pDevMouse->Acquire();
	}
}

//=============================================================================
// マウスのプレス状態を取得
//=============================================================================
bool GetMousePress(int nMouse)
{
	return (g_aMouseState[nMouse] & 0x80) ? true: false;
}

//=============================================================================
// マウスのトリガー状態を取得
//=============================================================================
bool GetMouseTrigger(int nMouse)
{
	return (g_aMouseStateTrigger[nMouse] & 0x80) ? true: false;
}

//=============================================================================
// マウスのリピート状態を取得
//=============================================================================
bool GetMouseRepeat(int nMouse)
{
	return (g_aMouseStateRepeat[nMouse] & 0x80) ? true: false;
}

//=============================================================================
// マウスのリリ－ス状態を取得
//=============================================================================
bool GetMouseRelease(int nMouse)
{
	return (g_aMouseStateRelease[nMouse] & 0x80) ? true: false;
}

//=============================================================================
//マウス状態取得
//=============================================================================
DIMOUSESTATE *GetMouseStates(void) 
{
	return &g_zdiMouseState;
}
