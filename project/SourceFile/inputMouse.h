//=============================================================================
//
// 入力処理 [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include "main.h"

//マクロ
#define MOUSE_LEFT (0)
#define MOUSE_RIGHT (1)
#define MOUSE_CENTER (2)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);

bool GetMousePress(int nKey);             //どのボタンが押されたか
bool GetMouseTrigger(int nKey);           //ボタンが押された瞬間
bool GetMouseRepeat(int nKey);            //
bool GetMouseRelease(int nKey);           //ボタンが離れた瞬間
DIMOUSESTATE *GetMouseStates(void);       //マウス状態取得

#endif