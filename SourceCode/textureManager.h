#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

#define TEXTUREMAX (30)

typedef struct 
{
	LPDIRECT3DTEXTURE9 TextureSlot;                  //�e�N�X�`���̃o�b�t�@
	char aFileName[200];                             //�t�@�C����
	int nTextureNum;                                 //�e�N�X�`���̔ԍ�
	bool bUse;                                       //�e�N�X�`���o�b�t�@�͎g�p���Ă��邩
}TEXTUREMANAGER;

//�v���g�^�C�v�錾
void InitTextureManager(void);                         //�e�N�X�`���}�l�W���[������
void UpdateTextureManager(void);                       //�e�N�X�`���}�l�W���[�X�V
TEXTUREMANAGER *GetTextureManager(void);               //�e�N�X�`���}�l�W���[���擾
HRESULT SetTexture(int *Num, char aFileName[200]);     //�e�N�X�`�����Z�b�g����֐�

#endif