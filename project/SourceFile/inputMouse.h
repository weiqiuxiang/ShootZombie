//=============================================================================
//
// ���͏��� [input.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include "main.h"

//�}�N��
#define MOUSE_LEFT (0)
#define MOUSE_RIGHT (1)
#define MOUSE_CENTER (2)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);

bool GetMousePress(int nKey);             //�ǂ̃{�^���������ꂽ��
bool GetMouseTrigger(int nKey);           //�{�^���������ꂽ�u��
bool GetMouseRepeat(int nKey);            //
bool GetMouseRelease(int nKey);           //�{�^�������ꂽ�u��
DIMOUSESTATE *GetMouseStates(void);       //�}�E�X��Ԏ擾

#endif