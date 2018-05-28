//=============================================================================
//
// ���͏��� [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "inputMouse.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(4)	    // �L�[�̍ő吔
#define	LIMIT_COUNT_REPEAT	(10)	// ���s�[�g�J�E���g���~�b�^�[

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInputMouse(HINSTANCE hInstance, HWND hWnd);
void UninitInputMouse(void);
void UpdateInputMouse(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pInputMouse = NULL;			// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevMouse = NULL;				// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
DIMOUSESTATE            g_zdiMouseState;                //�}�E�X��Ԏ擾
BYTE					g_aMouseState[NUM_KEY_MAX];			// �}�E�X�̓��͏�񃏁[�N
BYTE					g_aMouseStateTrigger[NUM_KEY_MAX];	// �}�E�X�̃g���K�[��񃏁[�N
BYTE					g_aMouseStateRelease[NUM_KEY_MAX];	// �}�E�X�̃����[�X��񃏁[�N
BYTE					g_aMouseStateRepeat[NUM_KEY_MAX];		// �}�E�X�̃��s�[�g��񃏁[�N
int						g_aMouseStateRepeatCnt[NUM_KEY_MAX];	// �}�E�X�̃��s�[�g�J�E���^

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInputMouse(HINSTANCE hInstance, HWND hWnd)
{
	if(g_pInputMouse == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInputMouse, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInputMouse(void)
{
	if(g_pInputMouse != NULL)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pInputMouse->Release();
		g_pInputMouse = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInputMouse(void)
{
}

//=============================================================================
// �}�E�X�̏�����
//=============================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT ret = S_FALSE;
	// ���͏����̏�����
	if(FAILED(InitInputMouse(hInstance, hWnd)))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInputMouse->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j(���ׂ�K�v������)
	if(FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	if (FAILED(ret)) {
		// �f�o�C�X�̐ݒ�Ɏ��s
		MessageBox(hWnd, "�f�o�C�X�̐ݒ莸�s", "�x���I", MB_ICONWARNING);
		return false;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// �}�E�X�̏I������
//=============================================================================
void UninitMouse(void)
{
	if(g_pDevMouse != NULL)
	{// ���̓f�o�C�X(�}�E�X)�̊J��
		// �}�E�X�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	// ���͏����̏I������
	UninitInputMouse();
}

//=============================================================================
// �}�E�X�̍X�V����
//=============================================================================
void UpdateMouse(void)
{
	// �ǎ�O�̒l��ێ����܂�
	DIMOUSESTATE zdiMouseState_bak;	// �}�E�X���(�ω����m�p)
	memcpy(&zdiMouseState_bak, &g_zdiMouseState, sizeof(zdiMouseState_bak));       //���̃}�E�X�̏�Ԃ��擾


	// �f�o�C�X����f�[�^���擾
	if(SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_zdiMouseState)))
	{
		for(int nCnMouse = 0; nCnMouse < NUM_KEY_MAX; nCnMouse++)
		{
			// �L�[�v���X����ۑ�
			//�L�[�g���K�[�����[�X���̍쐬
			g_aMouseStateTrigger[nCnMouse] = (zdiMouseState_bak.rgbButtons[nCnMouse] ^ g_zdiMouseState.rgbButtons[nCnMouse]) & g_zdiMouseState.rgbButtons[nCnMouse];                          //g_aMouseState�͑O,aMouseState�͍�
			g_aMouseStateTrigger[nCnMouse] = (zdiMouseState_bak.rgbButtons[nCnMouse] ^ g_zdiMouseState.rgbButtons[nCnMouse]) & zdiMouseState_bak.rgbButtons[nCnMouse];                        //g_aMouseState�͑O,aMouseState�͍�
			g_aMouseState[nCnMouse] = g_zdiMouseState.rgbButtons[nCnMouse];

			//�L�[���r�[�Ə��̐���
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

	//���s�����ꍇ�͂�����x�}�E�X�ւ̃A�X�Z�X��v������
	else
	{
		// �}�E�X�ւ̃A�N�Z�X�����擾
		g_pDevMouse->Acquire();
	}
}

//=============================================================================
// �}�E�X�̃v���X��Ԃ��擾
//=============================================================================
bool GetMousePress(int nMouse)
{
	return (g_aMouseState[nMouse] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetMouseTrigger(int nMouse)
{
	return (g_aMouseStateTrigger[nMouse] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetMouseRepeat(int nMouse)
{
	return (g_aMouseStateRepeat[nMouse] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�̃����|�X��Ԃ��擾
//=============================================================================
bool GetMouseRelease(int nMouse)
{
	return (g_aMouseStateRelease[nMouse] & 0x80) ? true: false;
}

//=============================================================================
//�}�E�X��Ԏ擾
//=============================================================================
DIMOUSESTATE *GetMouseStates(void) 
{
	return &g_zdiMouseState;
}
