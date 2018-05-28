#include "tpsAim.h"
#include "hitRectangle .h"
#include "hitcheckMesh.h"
#include "hitCircle.h"
#include "hitCircleMesh.h"
#include "TpsCamera.h"

//�O���[�o���ϐ�
float g_fDistance;
D3DXVECTOR3 g_Intersection;

//���@1
void RayMesh(D3DXVECTOR3 *pOut,const D3DXVECTOR3 *pVectorLookat/*�x�N�g���n�_�|�C���^*/, const D3DXVECTOR3 *pVectorCamera/*�x�N�g���I�_�|�C���^*/)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //���ʃx�N�g��������
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector*100000;
	const HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bCheck = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�
	float fDistance = 0;                                //�J�����ƏՓ˕��ʂ̋���
	float fMinDistance = 1000000;                           //�J�����Ƒ����ʂ̏Փ˂ɑ΂��ăJ�����ƕ��ʂ̍ŒZ����

	//�J����-�����_�x�N�g����P�ʉ�
	D3DXVec3Normalize(&resultVector, &resultVector);

	//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			D3DXVECTOR3 InverNor = -(pHitRect + nCnt)->VectexC[0].normal;      //�@���̋t�@��

			//���C�Ɩ@���̋t�x�N�g���̓��ς�0�傫���ȏꍇ
			if (D3DXVec3Dot(&resultVector, &InverNor) > 0)
			{
				float fAngle = D3DXVec3Dot(&resultVector, &InverNor) / (D3DXVec3Length(&resultVector) * D3DXVec3Length(&InverNor));  //�����p�x
				fDistance = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));          //�J�����ƏՓ˃��b�V���̋���
				fDistance = fDistance / cos(fAngle);                   //���C�ƃ��b�V���̋���
				D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);
				if (fDistance > D3DXVec3Length(&resultVectorCal) )
				{

					D3DXVECTOR3 EndVec = (fDistance+1.0f)*resultVector + *pVectorCamera;    //�ђʓ_�����߂�
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorCamera, &EndVec, (pHitRect + nCnt));    //�ђʓ_�����߂�

					//�΂�
					if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
					{
						//�ђʓ_�����ʏ�ɂ��邩���肷��
						if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
						{
							if (fDistance < fMinDistance)
							{
								fMinDistance = fDistance;                              //�ŒZ�����ۑ�
								RayIntersection = Intersection;     //�C��
							}
							bCheck = true;
						}
					}

					//����OR�E�H�[��
					if (
						((pHitRect + nCnt)->meshType == MESH_WALL) ||
						((pHitRect + nCnt)->meshType == MESH_FLOOR)
						)
					{
						//
						//XZ���ʂŐ����`�Ɛ����`����
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //�ϐ��錾

																				 //�l�pA�̌v�Z
						RectA_Pos.x = MinAB(EndVec.x, pVectorCamera->x);
						RectA_Pos.y = MinAB(EndVec.z, pVectorCamera->z);
						RectA_HW.x = fabs(pVectorCamera->x - EndVec.x);
						RectA_HW.y = fabs(pVectorCamera->z - EndVec.z);

						//�l�pB�̌v�Z
						RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
						RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

						//���b�V���̃^�C�v��FLOOR�̎�
						if ((pHitRect + nCnt)->meshType == MESH_FLOOR)
						{
							if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW))
							{
								//
								if (fDistance < fMinDistance)
								{
									fMinDistance = fDistance;                              //�ŒZ�����ۑ�
									RayIntersection = Intersection;     //�C��
								}
								bCheck = true;
							}
						}

						//���b�V���̃^�C�v��WALL�̎�
						else
						{
							//���_��Y���W���\�[�g����
							float aSort[4];
							bool bYCheck = false;
							for (int j = 0; j < 4; j++)
							{
								aSort[j] = (pHitRect + nCnt)->VectexC[j].pos.y;
							}

							Quick_Sort(0, 3, aSort);

							if ((Intersection.y >= aSort[0]) && (Intersection.y <= aSort[3])) bYCheck = true;

							//�l�p���蔻��
							if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW) && bYCheck)
							{
								if (fDistance < fMinDistance)
								{
									fMinDistance = fDistance;                              //�ŒZ�����ۑ�
									RayIntersection = Intersection;     //�C��
								}
								bCheck = true;
							}
						}
					}
				}

				//�@�����萬�����Ȃ�
				else
				{
					continue;
				}
			}
		}
	}

	*pOut = RayIntersection;
}

//���@2
void RayMeshRect(D3DXVECTOR3 *pOut,float *fDistanceOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //���ʃx�N�g��������
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bCheck = false;  //�n�ʏՓ˃`�F�b�N�t���O
	float fMinDistance = 1000000;                           //�J�����Ƒ����ʂ̏Փ˂ɑ΂��ăJ�����ƕ��ʂ̍ŒZ����

	//�J����-�����_�x�N�g����P�ʉ�
	D3DXVec3Normalize(&resultVector, &resultVector);

	//����`�F�b�N
	for (int nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			D3DXVECTOR3 InverNor = -(pHitRect + nCnt)->VectexC[0].normal;      //�@���̋t�@��
			//nCnt == 4
			//���C�Ɩ@���̋t�x�N�g���̓��ς�0�傫���ȏꍇ
			if (D3DXVec3Dot(&resultVector, &InverNor) > 0)
			{
				D3DXVECTOR3 VecP0_S = (pHitRect + nCnt)->VectexC[0].pos - *pVectorCamera;
				float fDot1 = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));
				float fDot2 = D3DXVec3Dot(&resultVector,&InverNor);
				g_fDistance = fDot1 / fDot2;
				D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);

				if (g_fDistance > D3DXVec3Length(&resultVectorCal))
				{
					g_Intersection = *pVectorCamera + g_fDistance*resultVector;    //�ђʓ_�����߂�

					/*
					RECT_3D MeshRect;
					MeshRect.p0 = (pHitRect + nCnt)->VectexC[0].pos;
					MeshRect.p1 = (pHitRect + nCnt)->VectexC[1].pos;
					MeshRect.p2 = (pHitRect + nCnt)->VectexC[2].pos;
					MeshRect.p3 = (pHitRect + nCnt)->VectexC[3].pos;
					*/
					bCheck = Rects3DPointPlane(&g_Intersection, (pHitRect + nCnt) );                //3D���蔻����s��

					if(bCheck == true)
					{
						if (g_fDistance < fMinDistance)
						{
							fMinDistance = g_fDistance;                              //�ŒZ�����ۑ�
							RayIntersection = g_Intersection;     //�C��
						}
					}
				}

				//�@�����萬�����Ȃ�
				else
				{
					continue;
				}
			}
		}
	}
	g_fDistance = fMinDistance;               //�ŒZ�����ۑ�
	g_Intersection = RayIntersection;         //�����_�ۑ�
	*fDistanceOut = g_fDistance;
	*pOut = RayIntersection;
}

void RayCircle(D3DXVECTOR3 *pOut, float *fDistanceOut,const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera,const D3DXVECTOR3 *pModelPos)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //���ʃx�N�g��������
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITCIRCLE *pHitCircle = Get_HitCircle(0);                                //���蓖�蔻��~�f�[�^�擾
	float fMinDistance = 1000000;                           //�J�����Ƒ����ʂ̏Փ˂ɑ΂��ăJ�����ƕ��ʂ̍ŒZ����

	for (int nCnt = 0; nCnt < HITCIRCLE_MAX; nCnt++)
	{
		if ( (pHitCircle + nCnt)->bUse == true)
		{
			if ((pHitCircle + nCnt)->m_type != TYPE_PLAYER)
			{
				CircleAndRay(&g_Intersection, &g_fDistance, pVectorCamera, pVectorLookat, &(pHitCircle + nCnt)->m_Matrix.pos, (pHitCircle + nCnt)->m_skydome.fRadius, pModelPos);

				if (g_fDistance < fMinDistance)
				{
					fMinDistance = g_fDistance;                              //�ŒZ�����ۑ�
					RayIntersection = g_Intersection;     //�C��
				}
			}
		}
	}

	g_fDistance = fMinDistance;               //�ŒZ�����ۑ�
	g_Intersection = RayIntersection;         //�����_�ۑ�
	*fDistanceOut = g_fDistance;
	*pOut = RayIntersection;
}

void RayCheck(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera, const D3DXVECTOR3 *pModelPos)
{
	float fDistance01 = 0;
	float fDistance02 = 0;
	D3DXVECTOR3 pOut01;
	D3DXVECTOR3 pOut02;
	g_fDistance = 1000000;
	RayMeshRect(&pOut01, &fDistance01,pVectorLookat, pVectorCamera);
	RayCircle(&pOut02, &fDistance02, pVectorLookat, pVectorCamera, pModelPos);

	if(fDistance01 < fDistance02)
	{
		*pOut = pOut01;
	}

	else
	{
		*pOut = pOut02;
	}
}

/*
void RayMeshRect(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //���ʃx�N�g��������
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bCheck = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�
	float fMinDistance = 1000000;                           //�J�����Ƒ����ʂ̏Փ˂ɑ΂��ăJ�����ƕ��ʂ̍ŒZ����

															//�J����-�����_�x�N�g����P�ʉ�
	D3DXVec3Normalize(&resultVector, &resultVector);

	//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			D3DXVECTOR3 InverNor = -(pHitRect + nCnt)->VectexC[0].normal;      //�@���̋t�@��

																			   //���C�Ɩ@���̋t�x�N�g���̓��ς�0�傫���ȏꍇ
			if (D3DXVec3Dot(&resultVector, &InverNor) > 0)
			{
				float fAngle = D3DXVec3Dot(&resultVector, &InverNor) / (D3DXVec3Length(&resultVector) * D3DXVec3Length(&InverNor));  //�����p�x
				g_fDistance = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));          //�J�����ƏՓ˃��b�V���̋���
				g_fDistance = g_fDistance / fAngle;                   //���C�ƃ��b�V���̋���
				D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);

				if (g_fDistance > D3DXVec3Length(&resultVectorCal))
				{

					D3DXVECTOR3 EndVec = (g_fDistance)*resultVector + *pVectorCamera;    //�ђʓ_�����߂�
					g_Intersection = GetVecPlaneIntersection(pVectorCamera, &EndVec, (pHitRect + nCnt));    //�ђʓ_�����߂�

					RECT_3D MeshRect;
					MeshRect.p0 = (pHitRect + nCnt)->VectexC[0].pos;
					MeshRect.p1 = (pHitRect + nCnt)->VectexC[1].pos;
					MeshRect.p2 = (pHitRect + nCnt)->VectexC[2].pos;
					MeshRect.p3 = (pHitRect + nCnt)->VectexC[3].pos;

					bCheck = Rect3D_Vector(&g_Intersection, &MeshRect);                //3D���蔻����s��

					if (bCheck == true)
					{
						if (g_fDistance < fMinDistance)
						{
							fMinDistance = g_fDistance;                              //�ŒZ�����ۑ�
							RayIntersection = g_Intersection;     //�C��
						}
					}
				}

				//�@�����萬�����Ȃ�
				else
				{
					continue;
				}
			}
		}
	}
	*pOut = RayIntersection;
}
*/
/*
void RayMeshRect(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //���ʃx�N�g��������
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bCheck = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�
	float fMinDistance = 1000000;                           //�J�����Ƒ����ʂ̏Փ˂ɑ΂��ăJ�����ƕ��ʂ̍ŒZ����

															//�J����-�����_�x�N�g����P�ʉ�
	D3DXVec3Normalize(&resultVector, &resultVector);

	//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			float dotRN = D3DXVec3Dot(&resultVector, &(pHitRect + nCnt)->VectexC[0].normal);
			//���C�Ɩ@���̋t�x�N�g���̓��ς�0�傫���ȏꍇ
			if (dotRN != 0)
			{
				float t;
				float D3DXVec3Dot();

				if ()
				{
					float fAngle = D3DXVec3Dot(&resultVector, &InverNor) / (D3DXVec3Length(&resultVector) * D3DXVec3Length(&InverNor));  //�����p�x
					g_fDistance = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));          //�J�����ƏՓ˃��b�V���̋���
					g_fDistance = g_fDistance / fAngle;                   //���C�ƃ��b�V���̋���
					D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);

					if (g_fDistance > D3DXVec3Length(&resultVectorCal))
					{

						D3DXVECTOR3 EndVec = (g_fDistance)*resultVector + *pVectorCamera;    //�ђʓ_�����߂�
						g_Intersection = GetVecPlaneIntersection(pVectorCamera, &EndVec, (pHitRect + nCnt));    //�ђʓ_�����߂�

						RECT_3D MeshRect;
						MeshRect.p0 = (pHitRect + nCnt)->VectexC[0].pos;
						MeshRect.p1 = (pHitRect + nCnt)->VectexC[1].pos;
						MeshRect.p2 = (pHitRect + nCnt)->VectexC[2].pos;
						MeshRect.p3 = (pHitRect + nCnt)->VectexC[3].pos;

						bCheck = Rect3D_Vector(&g_Intersection, &MeshRect);                //3D���蔻����s��

						if (bCheck == true)
						{
							if (g_fDistance < fMinDistance)
							{
								fMinDistance = g_fDistance;                              //�ŒZ�����ۑ�
								RayIntersection = g_Intersection;     //�C��
							}
						}
					}

					//�@�����萬�����Ȃ�
					else
					{
						continue;
					}
				}
			}
		}
	}
	*pOut = RayIntersection;
}
*/
float GetRayDistance(void)
{
	return g_fDistance;
}

D3DXVECTOR3 GetRayInt(void)
{
	return g_Intersection;
}