//=============================================================================
//
// ���͏��� [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);             //�ǂ̃{�^���������ꂽ��
bool GetKeyboardTrigger(int nKey);           //�{�^���������ꂽ�u��
bool GetKeyboardRepeat(int nKey);            //
bool GetKeyboardRelease(int nKey);           //�{�^�������ꂽ�u��

#endif