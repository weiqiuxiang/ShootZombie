/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
���b�Z�[�WBOX�\������[camera.cpp]

Author :
Data   :
----------------------------------------------------------------
��Update : 2016/04/18
�J�����̑���
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/////////////////////////////////�C���N���[�h�w�b�h//////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "textureManager.h"

/////////////////////////////////�O���o�[���ϐ�/////////////////////////////////
TEXTUREMANAGER g_TextureManager[TEXTUREMAX];

/////////////////////////////////�e�N�X�`���}�l�W���[�������֐�//////////////////////////////////
void InitTextureManager(void)
{
	//�e�N�X�`���}�l�W���[�p�����[�^������
	for (int nCnt = 0; nCnt < TEXTUREMAX; nCnt++)
	{
		g_TextureManager[nCnt].nTextureNum = nCnt;
		g_TextureManager[nCnt].TextureSlot = NULL;
		g_TextureManager[nCnt].aFileName[0] = '\0';
		g_TextureManager[nCnt].bUse = false;
	}
}

/////////////////////////////////�e�N�X�`���}�l�W���[�X�V�֐�//////////////////////////////////
void UpdateTextureManager(void) 
{
	
}

/////////////////////////////////�e�N�X�`���}�l�W���[�擾//////////////////////////////////
TEXTUREMANAGER *GetTextureManager(void) 
{
	return &g_TextureManager[0];
}

/////////////////////////////////�e�N�X�`�����Z�b�g����֐�//////////////////////////////////
HRESULT SetTexture(int *Num,char aFileName[200])
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();         //�f�o�C�X�擾

	if (aFileName != NULL)
	{
		for (int nCnt = 0; nCnt < TEXTUREMAX; nCnt++)
		{
			//�e�N�X�`���̃X���b�g���g����
			if (g_TextureManager[nCnt].bUse == true)
			{
				//�������r
				if (strcmp(g_TextureManager[nCnt].aFileName, aFileName) == 0)
				{
					//�e�N�X�`�����łɓǂݍ��܂�Ă���
					*Num = g_TextureManager[nCnt].nTextureNum;            //�e�N�X�`���ԍ����s
					break;
				}
			}

			//�e�N�X�`���̃X���b�g���g���Ă��Ȃ�
			else
			{
				g_TextureManager[nCnt].bUse = true;                     //�g�p���\�L
				g_TextureManager[nCnt].nTextureNum = nCnt;
				*Num = g_TextureManager[nCnt].nTextureNum;               //�e�N�X�`���ԍ��o�^
				strcpy(g_TextureManager[nCnt].aFileName, aFileName);    //�e�N�X�`���p�X�o�^
				//�e�N�X�`���ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, aFileName, &g_TextureManager[nCnt].TextureSlot)))
				{
					MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
					return E_FAIL;
				}
				break;
			}
		}
	}

	else
	{
		return E_FAIL;
	}

	return S_OK;
}