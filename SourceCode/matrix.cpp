#include "matrix.h"
#include "camera.h"

D3DXMATRIX g_ViewMtx;

//���ʃ}�g���N�X
void SetMatrix(D3DXMATRIX *pWorldMatrix, WORLD_MATRIX ChangeMatrix)
{
	/*******************************************�ϐ��錾***********************************************/
	//���[���h�s��̍쐬
	D3DXMATRIX mtxScale/*�g��s��*/, mtxRot/*��]�s��*/, mtxMove/*�ړ��s��*/, mtxResult/*���ʍs��*/;
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();

	/*******************************************�f�o�C�X�ݒ�***********************************************/
	//���[���h�s��̐ݒ�
	D3DXMatrixScaling(&mtxScale, ChangeMatrix.scl.x, ChangeMatrix.scl.y, ChangeMatrix.scl.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, ChangeMatrix.rot.y/*y��*/, ChangeMatrix.rot.x/*x��*/, ChangeMatrix.rot.z/*z��*/); //��]�s���ݒ�
	D3DXMatrixTranslation(&mtxMove, ChangeMatrix.pos.x, ChangeMatrix.pos.y, ChangeMatrix.pos.z);
	mtxResult = mtxScale*mtxRot*mtxMove;                     //���ʂ̕ϊ��s��̌v�Z
	D3DXMatrixIdentity(pWorldMatrix);                    //���[���h�s���P�ʍs��ŏ�����
	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxResult);  //��]�s��ƃ��[���h�s��̊|���Z
	pD3DDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);              //���[���h�ϊ��s��
}

void SetViewMatrix(const D3DXVECTOR3 *CameraPos, const D3DXVECTOR3 *LookatPos,const D3DXVECTOR3* VecUp)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();

	//�r���[�s��̐ݒ�
	D3DXMatrixLookAtLH(&g_ViewMtx, CameraPos, LookatPos, VecUp);

	pD3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMtx);      //�r���[�ϊ��s��
}

void SetProjectionMatrix(void)
{
	D3DXMATRIX mtxProj;
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();

	D3DXMatrixPerspectiveFovLH(&mtxProj/*�v���W�F�N�V�����s��*/, D3DX_PI / 3.0f /*��p*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT/*�A�X�y�N�g��*/, 1.0f/*near(near>0�K�{)*/, 8000.0f/*far*/);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);     //�v���W�F�N�V�����ϊ��s��Z�b�g
}

void SetBillboardMatrix(D3DXMATRIX *pWorldMatrix, WORLD_MATRIX ChangeMatrix)
{
	/*******************************************�ϐ��錾***********************************************/
	//���[���h�s��̍쐬
	D3DXMATRIX mtxScale/*�g��s��*/, mtxRot/*��]�s��*/, mtxMove/*�ړ��s��*/, mtxResult/*���ʍs��*/;
	D3DXMATRIX mtxViewInverse;               //�r���[�s��̋t�s��

	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();        //�f�o�C�X�擾

	/*******************************************�f�o�C�X�ݒ�***********************************************/
	//���[���h�s��̐ݒ�
	D3DXMatrixIdentity(pWorldMatrix);                    //���[���h�s���P�ʍs��ŏ�����
	D3DXMatrixScaling(&mtxScale, ChangeMatrix.scl.x, ChangeMatrix.scl.y, ChangeMatrix.scl.z);
	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxScale);  //�X�P�[��

	D3DXMatrixTranspose(&mtxViewInverse, &g_ViewMtx);                              //�]�u�s��𐶐�

	mtxViewInverse._14 = 0.0f;                                                    //���s�ړ��������J�b�g
	mtxViewInverse._24 = 0.0f;													  //���s�ړ��������J�b�g
	mtxViewInverse._34 = 0.0f;													  //���s�ړ��������J�b�g

	mtxViewInverse._41 = 0.0f;                                                    //���s�ړ��������J�b�g
	mtxViewInverse._42 = 0.0f;													  //���s�ړ��������J�b�g
	mtxViewInverse._43 = 0.0f;													  //���s�ړ��������J�b�g

	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxViewInverse);                //�t�s��*���[���h�s��(��])

	D3DXMatrixTranslation(&mtxMove, ChangeMatrix.pos.x, ChangeMatrix.pos.y, ChangeMatrix.pos.z);
	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxMove);                                 //�ړ�

	pD3DDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);                //���[���h�ϊ��s��
}

void SetChangeVertex(D3DXVECTOR3 *Vertex, const WORLD_MATRIX *ChangeMatrix)
{
	//���[���h�s��̍쐬
	D3DXMATRIX mtxScale/*�g��s��*/, mtxRot/*��]�s��*/, mtxMove/*�ړ��s��*/, mtxResult/*���ʍs��*/;

	D3DXMatrixScaling(&mtxScale, ChangeMatrix->scl.x, ChangeMatrix->scl.y, ChangeMatrix->scl.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, ChangeMatrix->rot.y/*y��*/, ChangeMatrix->rot.x/*x��*/, ChangeMatrix->rot.z/*z��*/); //��]�s���ݒ�
	D3DXMatrixTranslation(&mtxMove, ChangeMatrix->pos.x, ChangeMatrix->pos.y, ChangeMatrix->pos.z);
	mtxResult = mtxScale*mtxRot*mtxMove;                     //���ʂ̕ϊ��s��̌v�Z

	D3DXVec3TransformCoord(Vertex, Vertex, &mtxResult);
}

void SetChangeNormarlize(D3DXVECTOR3 *Vertex, const WORLD_MATRIX *ChangeMatrix)
{
	//���[���h�s��̍쐬
	D3DXMATRIX mtxScale/*�g��s��*/, mtxRot/*��]�s��*/, mtxMove/*�ړ��s��*/, mtxResult/*���ʍs��*/;

	D3DXMatrixScaling(&mtxScale, ChangeMatrix->scl.x, ChangeMatrix->scl.y, ChangeMatrix->scl.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, ChangeMatrix->rot.y/*y��*/, ChangeMatrix->rot.x/*x��*/, ChangeMatrix->rot.z/*z��*/); //��]�s���ݒ�
	mtxResult = mtxScale*mtxRot;                     //���ʂ̕ϊ��s��̌v�Z

	D3DXVec3TransformCoord(Vertex, Vertex, &mtxResult);
}