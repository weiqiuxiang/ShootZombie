//-----------------------------------------------------------------------------------------
//�p�[�e�B�N��,����Ȃǂ̏���[ParticalCenter.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "partical.h"
#include "particalCenter.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define FRAMEPARTICAL (4)                //1�t���[�����˂���p�[�e�B�N���̐�

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
PARTICALCENTER g_ParticalCenter[NUM_PARTICALCENTERPOLYGON];
int g_ParticalFunction = 0;                //�p�[�e�B�N�����j���[�K�w�R���g���[���ϐ�

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
//�p�[�e�B�N������
void ParticalBase(PARTICALCENTER *);                         //�p�[�e�B�N����{
void ParticalBaseControl(PARTICALCENTER *pParticalCenter);   //�p�[�e�B�N����{�p�����[�^�̃R���g���[��
void ParticalExplosion(PARTICALCENTER *);                    //�p�[�e�B�N�������G�t�F�N�g
void ParticalFire(PARTICALCENTER *pParticalCenter);          //�p�[�e�B�N����

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B
//-----------------------------------------------------------------------------------------

//�e�ϐ��̏�����
HRESULT InitParticalCenter(void)
{
	srand((unsigned)time(NULL));        //�����̎�𐶐�����

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		//�p�[�e�B�N���̏����ʒu
		g_ParticalCenter[nCnt].pos.x = 0;
		g_ParticalCenter[nCnt].pos.y = 0;
		//�p�[�e�B�N���̏����X�s�[�h
		g_ParticalCenter[nCnt].speed.x = 10;
		g_ParticalCenter[nCnt].speed.y = 9;
		//�p�[�e�B�N���̌Â��ʒu
		g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
		g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
		//�p�[�e�B�N���̏W�����ˊp�x�Ɗg�U�p�x
		g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
		g_ParticalCenter[nCnt].fRangeAngel = PI * 0.01;
		//�p�[�e�B�N���̔�ъ�����Ɣ��a
		g_ParticalCenter[nCnt].fDistance = 200;
		g_ParticalCenter[nCnt].fRadius = 0;
		//�v���[���[�g�p
		g_ParticalCenter[nCnt].bUse = false;

		//�p�[�e�B�N�����͊�
		g_ParticalCenter[nCnt].fGravity = 0;                 //�d��
		g_ParticalCenter[nCnt].fWindPower = 0;               //����
		g_ParticalCenter[nCnt].fWindAngel = 0;               //���������p�x
	}

	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	

	return S_OK;
}
//�I������
void UninitParticalCenter(void)
{
	
}

//�p�[�e�B�N���f�[�^�̍X�V����
void UpdateParticalCenter(void)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON;nCnt++)
	{
		//�����p�[�e�B�N�����S�͎g�p�̏�ԂȂ�
		if (g_ParticalCenter[nCnt].bUse == true)
		{
			//�p�[�e�B�N���̑O�̍��W���L�^
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;

			//-----------------------------------------------------------------------------------------
			//�p�[�e�B�N���̈ړ�����
			//-----------------------------------------------------------------------------------------
			//�v���[���[�̈ʒu���󂯎��

			//-----------------------------------------------------------------------------------------
			//�p�[�e�B�N���̃G�t�F�N�g���X�g
			//-----------------------------------------------------------------------------------------
			switch(g_ParticalCenter[nCnt].EffectMode)
			{
			case EFFECTMODE_BASE:
				//�p�[�e�B�N��:��{
				ParticalBase(&g_ParticalCenter[nCnt]);
				break;
			case EFFECTMODE_EXPLOSION:
				//�p�[�e�B�N��:����
				ParticalExplosion(&g_ParticalCenter[nCnt]);
				break;
			case EFFECTMODE_FIRE:
				ParticalFire(&g_ParticalCenter[nCnt]);
				break;
			default:
				//�p�[�e�B�N��:��{
				ParticalBase(&g_ParticalCenter[nCnt]);
				break;
			}

			//-----------------------------------------------------------------------------------------
			//�p�[�e�B�N��
			//-----------------------------------------------------------------------------------------
			//�ړ��͈͐���
			if ((g_ParticalCenter[nCnt].pos.x < 0) || ( (g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH) > SCREEN_WIDTH))
			{
				g_ParticalCenter[nCnt].pos.x = g_ParticalCenter[nCnt].posOld.x;
			}

			if ((g_ParticalCenter[nCnt].pos.y < 0) || ((g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT) > SCREEN_HEIGHT))
			{
				g_ParticalCenter[nCnt].pos.y = g_ParticalCenter[nCnt].posOld.y;
			}
		}
	}
}


//�p�[�e�B�N���|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonParticalCenter(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�|���S���`�悷��O�Ƀe�N�X�`�����Z�b�g
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

PARTICALCENTER *GetParticalCenter(void)
{
	return &g_ParticalCenter[0];
}

//-----------------------
//�p�[�e�B�N��:��{
//-----------------------
void ParticalBase(PARTICALCENTER *pParticalCenter)
{
		//���ˊp�x�v�Z
		float fHantenAngle = 2 * PI * ( (pParticalCenter->fShootAngel / (2*PI) ) - (int)(pParticalCenter->fShootAngel / (2*PI) ) );

		ParticalBaseControl(pParticalCenter);      //�p�[�e�B�N���p�����[�^�̃R���g���[��

		//-----------------------------------------------------------------------------------------
		//�p�[�e�B�N���̔���
		//-----------------------------------------------------------------------------------------
		float fDistance = 0;                //�p�[�e�B�N���̔�ы���
		float fDistanceZ = 0;               //�p�[�e�B�N��Z���̔�ы���
		float fAngelXY = 0;                   //�p�x
		D3DXVECTOR3 speed;                  //�p�[�e�B�N���̃X�s�[�h

		fDistance = pParticalCenter->fDistance + rand()%50 + 50;       //��ы����v�Z
		fDistance = fDistance*0.001;//(0.05-0.1)
		fDistanceZ = 0.001*0.5* (pParticalCenter->fDistance + rand() % 50 + 50);//(0.025-0.05)
		fAngelXY = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //��ъp�x�v�Z
		speed.x = fDistance*cos(fAngelXY);        //x�����̃X�s�[�h
		speed.y = fDistance*sin(fAngelXY);        //y�����̃X�s�[�h
		speed.z = fDistanceZ;

		//�p�[�e�B�N���𔭎�
		SetPartical(speed.x,speed.y, speed.z,pParticalCenter->fRadius,120);
}

//�p�[�e�B�N����{�R���g���[��
void ParticalBaseControl(PARTICALCENTER *pParticalCenter)
{
	//���ˊp�x���t���v���ɉ����ĉ��
	if (GetKeyboardPress(DIK_LEFT))
	{
		pParticalCenter->fShootAngel -= 0.01*PI;
	}

	//���ˊp�x�����v���ɉ����ĉ��
	if (GetKeyboardPress(DIK_RIGHT))
	{
		pParticalCenter->fShootAngel += 0.01*PI;
	}

	//���ˊp�x��0�ȏ�m��(�ő�l��10*PI)
	if (pParticalCenter->fShootAngel <= -10 * PI)
	{
		pParticalCenter->fShootAngel = -10 * PI;
	}

	if (pParticalCenter->fShootAngel >= 10 * PI)
	{
		pParticalCenter->fShootAngel = 10 * PI;
	}

	//�g�U�p�x�g��
	if (GetKeyboardPress(DIK_R))
	{
		pParticalCenter->fRangeAngel += 0.01*PI;
	}

	//�g�U�p�x�k��
	if (GetKeyboardPress(DIK_F))
	{
		pParticalCenter->fRangeAngel -= 0.01*PI;
	}

	//�g�U�p�x�͈̔͂�0~PI�Ɏ��܂�
	if (pParticalCenter->fRangeAngel < 0.01)
	{
		pParticalCenter->fRangeAngel = 0.01;
	}

	if (pParticalCenter->fRangeAngel > PI)
	{
		pParticalCenter->fRangeAngel = PI;
	}

	//�p�[�e�B�N���̔�ъ�������R���g���[��
	if (GetKeyboardPress(DIK_T))  //��ы����L�т�
	{
		pParticalCenter->fDistance += 3;
	}

	if (GetKeyboardPress(DIK_G))  //��ы����k��
	{
		pParticalCenter->fDistance -= 3;
	}

	//��ы����͈͐���
	if (pParticalCenter->fDistance < 0)
	{
		pParticalCenter->fDistance = 0;
	}

	if (pParticalCenter->fDistance > 100)
	{
		pParticalCenter->fDistance = 100;
	}
	/*
	//-----------------------------------------------------------------------------------------
	//���͌v�Z
	//-----------------------------------------------------------------------------------------
	//���̊p�x�v�Z
	if (GetKeyboardPress(DIK_I))
	{
		pParticalCenter->fWindAngel += 0.01*PI;
	}

	if (GetKeyboardPress(DIK_J))
	{
		pParticalCenter->fWindAngel -= 0.01*PI;
	}

	//�p�x�͈͐���(-2*PI ~ 2*PI)
	if (pParticalCenter->fWindAngel < -2 * PI)
	{
		pParticalCenter->fWindAngel = -2 * PI;
	}

	if (pParticalCenter->fWindAngel > 2 * PI)
	{
		pParticalCenter->fWindAngel = 2 * PI;
	}

	//���͌v�Z
	if (GetKeyboardPress(DIK_U))
	{
		pParticalCenter->fWindPower += 0.001;
	}

	if (GetKeyboardPress(DIK_H))
	{
		pParticalCenter->fWindPower -= 0.001;
	}

	//���͐���(0 ~ 10)
	if (pParticalCenter->fWindPower < 0)
	{
		pParticalCenter->fWindPower = 0;
	}

	if (pParticalCenter->fWindPower > 0.05)
	{
		pParticalCenter->fWindPower = 0.05;
	}
	*/
	//-----------------------------------------------------------------------------------------
	//�d�͌v�Z
	//-----------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_Y))
	{
		pParticalCenter->fGravity += 0.0001;
	}

	if (GetKeyboardPress(DIK_H))
	{
		pParticalCenter->fGravity -= 0.0001;
	}

	//�d�͐���(0 ~ 15)
	if (pParticalCenter->fGravity < 0)
	{
		pParticalCenter->fGravity = 0;
	}

	if (pParticalCenter->fGravity > 0.005 )
	{
		pParticalCenter->fGravity = 0.005;
	}
}

//----------------------------------------
//�p�[�e�B�N����������
//----------------------------------------
void ParticalExplosion(PARTICALCENTER *pParticalCenter)
{
	float fDistance = 0;                //�p�[�e�B�N���̔�ы���
	float fAngel = 0;                   //�p�x
	D3DXVECTOR3 speed;                  //�p�[�e�B�N���̃X�s�[�h
	D3DXVECTOR3 test;

	//��ы����͈͐���
	if(pParticalCenter->fDistance < 0)
	{
		pParticalCenter->fDistance = 0;
	}

	if(pParticalCenter->fDistance > 20)
	{
		pParticalCenter->fDistance = 20;
	}

	
	for(int nCnt = 0; nCnt < 120; nCnt++)
	{
		//-----------------------------------------------------------------------------------------
		//�p�[�e�B�N���̔���
		//-----------------------------------------------------------------------------------------
		fDistance = pParticalCenter->fDistance + rand()%20;       //��ы����v�Z
		fAngel = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //��ъp�x�v�Z
		speed.x = fDistance*cos(fAngel)/20;        //x�����̃X�s�[�h
		speed.y = fDistance*sin(fAngel)/20;        //y�����̃X�s�[�h

		test.x = (fDistance+10)*cos(fAngel)/20;        //x�����̃X�s�[�h
		test.y = (fDistance+10)*sin(fAngel)/20;        //y�����̃X�s�[�h

		//�p�[�e�B�N���𔭎�
		SetPartical(speed.x,speed.y,speed.z,pParticalCenter->fRadius,20);
	}

	pParticalCenter -> bUse = false;
}

//----------------------------------------
//�p�[�e�B�N��������
//----------------------------------------
void ParticalFire(PARTICALCENTER *pParticalCenter)
{
	//���ˊp�x�v�Z
	float FirePosX = (rand()%100)*0.01;                //�p�[�e�B�N���̔�ы���
	D3DXVECTOR3 speed;                  //�p�[�e�B�N���̃X�s�[�h
	
	speed.x = 0;
	speed.y = 0.01*(rand()%50 + 50);  //0.5-1
	speed.z = 0;

	//�p�[�e�B�N���𔭎�
	SetFirePartical(pParticalCenter,FirePosX,speed.x, speed.y, speed.z, 40);
}

//----------------------------------------
//�p�[�e�B�N�����S�Z�b�g
//----------------------------------------
void SetParticalCenter(float fposX,float fposY,float fposZ,float fRadius,float fDistance, EFFECTMODE effectMode)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		if(g_ParticalCenter[nCnt].bUse == false)
		{
			//�p�[�e�B�N���̏����ʒu
			g_ParticalCenter[nCnt].pos.x = fposX;
			g_ParticalCenter[nCnt].pos.y = fposY;
			g_ParticalCenter[nCnt].pos.z = fposZ;
			//�p�[�e�B�N���̏����X�s�[�h
			g_ParticalCenter[nCnt].speed.x = 1;
			g_ParticalCenter[nCnt].speed.y = 1;
			//�p�[�e�B�N���̌Â��ʒu
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
			//�p�[�e�B�N���̏W�����ˊp�x�Ɗg�U�p�x
			g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
			g_ParticalCenter[nCnt].fRangeAngel = PI * 0.01;
			//�p�[�e�B�N���̔�ъ�����Ɣ��a
			g_ParticalCenter[nCnt].fDistance = fDistance;
			g_ParticalCenter[nCnt].fRadius = fRadius;
			g_ParticalCenter[nCnt].EffectMode = effectMode;
			//�v���[���[�g�p
			g_ParticalCenter[nCnt].bUse = true;
			g_ParticalCenter[nCnt].fGravity = 0.002;//�d��
			break;
		}
	}
}