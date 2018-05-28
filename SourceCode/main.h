#ifndef _MAIN_H_
#define _MAIN_H_

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")             //�V�X�e�������擾�ɕK�v
#pragma comment (lib,"dinput8.lib")

//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include <dinput.h>
#include <XAudio2.h>

//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define SCREEN_WIDTH (800)                          //�E�B���h�E�̕�
#define SCREEN_HEIGHT (600)                         //�E�B���h�E�̍���
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)                     //2D���_�t�H�[�}�b�g
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_NORMAL)          //3D���_�t�H�[�}�b�g
#define PI (3.14159265359f)
#define GRAVITY (0.03f)

//�Z�[�t�����[�X�}�N��
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( memory) { if(memory != NULL) { memory -> Release(); memory = NULL; } }  //�������̃N���A
#endif

//�O���錾
class CScore;

//-----------------------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------------------
//���_�\����(���̃t�@�[�}�b�g�̌^�ɍ��킹�邱��)
typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX,
}GAMEMODE;

typedef struct
{
	D3DXVECTOR3 pos;                         //
	float rhw;								 //���W�ϊ��ςݒ��_�t���O(�K��1.0f)
	D3DCOLOR color;						     //���_�J���[
	D3DXVECTOR2 tex;                         //�e�N�X�`�����W(UV or ST)
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;                         //
	D3DXVECTOR3 normal;                      //�x�N�g���̕���
	D3DCOLOR color;							 //
	D3DXVECTOR2 tex;                         //�e�N�X�`�����W(UV or ST)
}VERTEX_3D;

//�}�g���N�X�z��
typedef struct
{
	D3DXVECTOR3 pos;   //���f���ړ�(���W)
	D3DXVECTOR3 rot;   //���f����]
	D3DXVECTOR3 scl;   //���f���g�嗦
}WORLD_MATRIX;

typedef struct
{
	D3DXVECTOR3 p0;
	D3DXVECTOR3 p1;
	D3DXVECTOR3 p2;
	D3DXVECTOR3 p3;
}RECT_3D;

typedef struct
{
	D3DXVECTOR2 p[4];
}RECT_2D;

typedef struct
{
	D3DXVECTOR3 p0;
	D3DXVECTOR3 p1;
	D3DXVECTOR3 p2;
}TRIANGLE_3D;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void Uninit(void);                                                             //�I��(��Еt���֐�)
LPDIRECT3DDEVICE9 Get_Device(void);
bool *GetCameraSwitch(void);
void SetMode(GAMEMODE mode);
D3DXVECTOR3 GetMousePosInWindow(void);

CScore *GetScore(void);
#endif