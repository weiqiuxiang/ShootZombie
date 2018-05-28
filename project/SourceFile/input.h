//=============================================================================
//
// 入力処理 [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);             //どのボタンが押されたか
bool GetKeyboardTrigger(int nKey);           //ボタンが押された瞬間
bool GetKeyboardRepeat(int nKey);            //
bool GetKeyboardRelease(int nKey);           //ボタンが離れた瞬間

#endif