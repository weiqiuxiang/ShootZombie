#include "hitCircleMesh.h"
#include "hitcheckMesh.h"
#include "hitCircle.h"

bool CircleAndRect(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, float fRadius/*����͈͔��a*/)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //���ʃx�N�g��������
	D3DXVECTOR3 MoveSpeedVector;             //�ړ��ʃx�N�g��
	float WallMoveSpeed;                         //�ǂɉ����ړ���
	HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bGround = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�
	bool bSlope = false;
	bool bWall = false;

	//�ړ��ʃx�N�g���v�Z
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y�����̒l��0�ɂ���

											//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			//�_�ƕ��ʂ̋���������
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//�_�ƕ��ʂ̋����͔��蔼�a�ȉ��Ȃ�
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //�~�̊ђʓ_
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //�~�̊ђʓ_�̌Â����W
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������


				//�@�����萬��
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //�ђʓ_�����߂�

					//����OR�E�H�[��
					if (((pHitRect + nCnt)->meshType == MESH_FLOOR))
					{
						//
						//XZ���ʂŐ����`�Ɛ����`����
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //�ϐ��錾

						//�l�pA�̌v�Z
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

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
								pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001 + fRadius;

								bGround = true;
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

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//�_�ƕ��ʂ̋���������
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//�_�ƕ��ʂ̋����͔��蔼�a�ȉ��Ȃ�
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //�~�̊ђʓ_
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //�~�̊ђʓ_�̌Â����W
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������


				//�@�����萬��
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //�ђʓ_�����߂�

					//�΂�
					if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
					{
						//�ђʓ_�����ʏ�ɂ��邩���肷��
						if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
						{
							WallMoveSpeed = D3DXVec3Length(&MoveSpeedVector);                     //�X�s�[�h�̑傫�����v�Z����
							D3DXVec3Normalize(&MoveSpeedVector, &MoveSpeedVector);                           //�x�N�g���P�ʉ�

							Intersection += WallMoveSpeed*cos((pHitRect + nCnt)->m_HitRectMatrix.rot.x)*MoveSpeedVector;                //�ړ���̊ђʓ_�v�Z

							Intersection.y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001; //�ђʓ_�̍����v�Z

							*pVectorEnd = Intersection + fRadius*(pHitRect + nCnt)->VectexC[0].normal;             //�I�_�x�N�g���ʒu�Z�o

							bSlope = true;
							bGround = true;
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
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//�_�ƕ��ʂ̋���������
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//�_�ƕ��ʂ̋����͔��蔼�a�ȉ��Ȃ�
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //�~�̊ђʓ_
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //�~�̊ђʓ_�̌Â����W
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������


				//�@�����萬��
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //�ђʓ_�����߂�

					if ((pHitRect + nCnt)->meshType == MESH_WALL)
					{
						//
						//XZ���ʂŐ����`�Ɛ����`����
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //�ϐ��錾

						//�l�pA�̌v�Z
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

						//�l�pB�̌v�Z
						RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
						RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

						//���b�V���̃^�C�v��WALL�̎�

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
							pVectorEnd->x = pVectorStart->x;
							pVectorEnd->z = pVectorStart->z;

							//XZ���ʂł̈ړ����x��ǉ����ړ��x�N�g���ƈړ��ʃx�N�g���̓��ςŎZ�o
							WallMoveSpeed = D3DXVec3Dot(&MoveSpeedVector, &(pHitRect + nCnt)->VecXZ);

							*pVectorEnd += (pHitRect + nCnt)->VecXZ * WallMoveSpeed;                    //�ǉ����ړ�
							nWallCollision++;
							bWall = true;
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
	//�����̕ǂƔ��肪����ꍇ,�O�̍��W�ɖ߂�
	if ( (nWallCollision > 1) || (bSlope && bWall))
	{
		*pVectorEnd = *pVectorStart;
	}

	return bGround;
}

bool CircleAndRectSSV(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, float fRadius/*����͈͔��a*/)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //���ʃx�N�g��������
	D3DXVECTOR3 MoveSpeedVector;             //�ړ��ʃx�N�g��
	float WallMoveSpeed;                         //�ǂɉ����ړ���
	HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bGround = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�
	bool bSlope = false;
	bool bWall = false;

	//�ړ��ʃx�N�g���v�Z
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y�����̒l��0�ɂ���

											//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			//���b�V���̃^�C�v��FLOOR�̎�
			if ((pHitRect + nCnt)->meshType == MESH_FLOOR)
			{
				float timePlane = CircleAndPlaneHitTime(pVectorStart, pVectorEnd, &(pHitRect + nCnt)->VectexC[0].pos, &(pHitRect + nCnt)->VectexC[0].normal, fRadius);

				if (timePlane >= 0)
				{
					D3DXVECTOR3 Intersection = *pVectorStart + timePlane*(*pVectorEnd - *pVectorStart);

					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))                                 //�_�����ʏ�ɂ��邩
					{
						pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001 + fRadius;
						bGround = true;
					}
				}
			}
		}
	}

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
			{
				float timePlane = CircleAndPlaneHitTime(pVectorStart, pVectorEnd, &(pHitRect + nCnt)->VectexC[0].pos, &(pHitRect + nCnt)->VectexC[0].normal, fRadius);

				if (timePlane >= 0)
				{
					D3DXVECTOR3 Intersection = *pVectorStart + timePlane*(*pVectorEnd - *pVectorStart);                                   //�ђʓ_�����߂�

					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))                                 //�_�����ʏ�ɂ��邩
					{
						WallMoveSpeed = D3DXVec3Length(&MoveSpeedVector);                     //�X�s�[�h�̑傫�����v�Z����
						D3DXVec3Normalize(&MoveSpeedVector, &MoveSpeedVector);                           //�x�N�g���P�ʉ�

						Intersection += WallMoveSpeed*cos((pHitRect + nCnt)->m_HitRectMatrix.rot.x)*MoveSpeedVector;                //�ړ���̊ђʓ_�v�Z

						Intersection.y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001; //�ђʓ_�̍����v�Z

						*pVectorEnd = Intersection + fRadius*(pHitRect + nCnt)->VectexC[0].normal;             //�I�_�x�N�g���ʒu�Z�o

						bSlope = true;
						bGround = true;
					}
				}
			}
		}
	}

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			if ((pHitRect + nCnt)->meshType == MESH_WALL)
			{
				float timePlane = CircleAndPlaneHitTime(pVectorStart, pVectorEnd, &(pHitRect + nCnt)->VectexC[0].pos, &(pHitRect + nCnt)->VectexC[0].normal, fRadius);

				if (timePlane >= 0)
				{
					D3DXVECTOR3 Intersection = *pVectorStart + timePlane*(*pVectorEnd - *pVectorStart);                                   //�ђʓ_�����߂�

					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))                                 //�_�����ʏ�ɂ��邩
					{
						pVectorEnd->x = pVectorStart->x;
						pVectorEnd->z = pVectorStart->z;

						//XZ���ʂł̈ړ����x��ǉ����ړ��x�N�g���ƈړ��ʃx�N�g���̓��ςŎZ�o
						WallMoveSpeed = D3DXVec3Dot(&MoveSpeedVector, &(pHitRect + nCnt)->VecXZ);

						*pVectorEnd += (pHitRect + nCnt)->VecXZ * WallMoveSpeed;                    //�ǉ����ړ�
						nWallCollision++;
						bWall = true;
					}
				}
			}
		}
	}
	//�����̕ǂƔ��肪����ꍇ,�O�̍��W�ɖ߂�
	if ((nWallCollision > 1) || (bSlope && bWall))
	{
		*pVectorEnd = *pVectorStart;
	}

	return bGround;
}

bool CircleAndRectRepel(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, float fRadius/*����͈͔��a*/,float elasticity , D3DXVECTOR3 *pspeedOut)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //���ʃx�N�g��������
	D3DXVECTOR3 MoveSpeedVector;             //�ړ��ʃx�N�g��
	D3DXVECTOR3 Normalize = D3DXVECTOR3(0.0f,0.0f,0.0f);
	float WallMoveSpeed;                         //�ǂɉ����ړ���
	HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bGround = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�
	bool bSlope = false;
	bool bWall = false;

	//�ړ��ʃx�N�g���v�Z
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y�����̒l��0�ɂ���

											//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			//�_�ƕ��ʂ̋���������
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//�_�ƕ��ʂ̋����͔��蔼�a�ȉ��Ȃ�
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //�~�̊ђʓ_
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //�~�̊ђʓ_�̌Â����W
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������


																						  //�@�����萬��
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //�ђʓ_�����߂�

																																		   //����OR�E�H�[��
					if (((pHitRect + nCnt)->meshType == MESH_FLOOR))
					{
						//
						//XZ���ʂŐ����`�Ɛ����`����
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //�ϐ��錾

																				 //�l�pA�̌v�Z
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

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
								pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001 + fRadius;
								Normalize = (pHitRect + nCnt)->VectexC[0].normal;
								bGround = true;
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

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//�_�ƕ��ʂ̋���������
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//�_�ƕ��ʂ̋����͔��蔼�a�ȉ��Ȃ�
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //�~�̊ђʓ_
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //�~�̊ђʓ_�̌Â����W
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������


																						  //�@�����萬��
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //�ђʓ_�����߂�

																																		   //�΂�
					if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
					{
						//�ђʓ_�����ʏ�ɂ��邩���肷��
						if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
						{
							WallMoveSpeed = D3DXVec3Length(&MoveSpeedVector);                     //�X�s�[�h�̑傫�����v�Z����
							D3DXVec3Normalize(&MoveSpeedVector, &MoveSpeedVector);                           //�x�N�g���P�ʉ�

							Intersection += WallMoveSpeed*cos((pHitRect + nCnt)->m_HitRectMatrix.rot.x)*MoveSpeedVector;                //�ړ���̊ђʓ_�v�Z

							Intersection.y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001; //�ђʓ_�̍����v�Z

							*pVectorEnd = Intersection + fRadius*(pHitRect + nCnt)->VectexC[0].normal;             //�I�_�x�N�g���ʒu�Z�o
							Normalize = (pHitRect + nCnt)->VectexC[0].normal;
							bSlope = true;
							bGround = true;
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
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//�_�ƕ��ʂ̋���������
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//�_�ƕ��ʂ̋����͔��蔼�a�ȉ��Ȃ�
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //�~�̊ђʓ_
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //�~�̊ђʓ_�̌Â����W
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������


																						  //�@�����萬��
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //�ђʓ_�����߂�

					if ((pHitRect + nCnt)->meshType == MESH_WALL)
					{
						//
						//XZ���ʂŐ����`�Ɛ����`����
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //�ϐ��錾

																				 //�l�pA�̌v�Z
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

						//�l�pB�̌v�Z
						RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
						RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

						//���b�V���̃^�C�v��WALL�̎�

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
							pVectorEnd->x = pVectorStart->x;
							pVectorEnd->z = pVectorStart->z;

							//XZ���ʂł̈ړ����x��ǉ����ړ��x�N�g���ƈړ��ʃx�N�g���̓��ςŎZ�o
							WallMoveSpeed = D3DXVec3Dot(&MoveSpeedVector, &(pHitRect + nCnt)->VecXZ);

							*pVectorEnd += (pHitRect + nCnt)->VecXZ * WallMoveSpeed;                    //�ǉ����ړ�
							Normalize = (pHitRect + nCnt)->VectexC[0].normal;
							nWallCollision++;
							bWall = true;
							bGround = true;
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
	//�����̕ǂƔ��肪����ꍇ,�O�̍��W�ɖ߂�
	if ((nWallCollision > 1) || (bSlope && bWall))
	{
		*pVectorEnd = *pVectorStart;
	}

	if(bGround == true)
	{
		if(elasticity < 0)
		{
			elasticity = 0;
		}

		if (elasticity > 1)
		{
			elasticity = 1;
		}

		*pspeedOut += Normalize*D3DXVec3Length(pspeedOut)*elasticity;                               //���˕Ԃ����x�v�Z
	}

	return bGround;
}

void CircleAndRay(D3DXVECTOR3 *pOut, float *pfOutDistance, const D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, const D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, const D3DXVECTOR3 *pCirclePos/*�~�̈ʒu*/, float fRadius/*����͈͔��a*/, const D3DXVECTOR3 *pModelPos)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //�����x�N�g��������
	D3DXVECTOR3 VectorK = *pVectorStart - *pCirclePos;                //���C�n�_�Ɖ~���S���W�̃x�N�g��
	D3DXVECTOR3 VecModelCircle = *pModelPos - *pCirclePos;            //�~�̒��S�ƃ��f�����W�̃x�N�g��
	D3DXVec3Normalize(&resultVector, &resultVector);                      //�����x�N�g���P�ʉ�
	
	if (D3DXVec3Length(&VecModelCircle) > fRadius)
	{
		//0 = fDistance^2*A + 2*fDistance*B + C �̗v�fA,B,C������
		float fDistance = 100000;                                            //���C�n�_�ƕ\�����_�̋���
		float A = D3DXVec3Dot(&resultVector, &resultVector);
		float B = D3DXVec3Dot(&VectorK, &resultVector);
		float C = D3DXVec3Dot(&VectorK, &VectorK) - fRadius*fRadius;

		float B2AC = B*B - A*C;           //B^2 - A*C�����߂�

		//���������_�����݂����
		if (B2AC > 0)
		{
			float fDistance1 = (-B - sqrt(B2AC)) / A;
			float fDistance2 = (-B + sqrt(B2AC)) / A;

			fDistance = fabs(fDistance1);
			*pfOutDistance = fDistance;                      //�������
			*pOut = fDistance*resultVector + *pVectorStart;  //�����_���W�����߂�
		}
	}
}

//���f���~�Ɠ��蔻��~�̋���������,���f�������蔻��~�͈͓̔��ɓ����Ă�����true��Ԃ�
bool BulletCircleAndCircle(const D3DXVECTOR3 *pModelpos, float fModelRadius,BULLETTYPE Btype)
{
	bool bCheck = false;
	HITCIRCLE *pHitCircle = Get_HitCircle(0);
	for (int nCnt = 0; nCnt < HITCIRCLE_MAX; nCnt++)
	{
		if ((pHitCircle + nCnt)->bUse == true)
		{
			if ((pHitCircle + nCnt)->m_type == TYPE_ENEMY)
			{
				D3DXVECTOR3 VecModelCircle = *pModelpos - (pHitCircle + nCnt)->m_Matrix.pos;
				float fDistanceModelCircle = D3DXVec3Length(&VecModelCircle);                           //���f���Ɠ��蔻��~�̋���

				if (fDistanceModelCircle < (fModelRadius + (pHitCircle + nCnt)->m_skydome.fRadius))
				{
					if(Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //���蔻��~�̓��蔻��t���O�̗L��
					if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
					bCheck = true;
					break;
				}
			}
		}
	}

	return bCheck;
}

bool BulletCircleAndCircleSSV(D3DXVECTOR3 *pOutModelpos,const D3DXVECTOR3 *pModelposOld,const D3DXVECTOR3 *pModelpos, float fModelRadius, BULLETTYPE Btype)
{
	bool bCheck = false;
	HITCIRCLE *pHitCircle = Get_HitCircle(0);

	for (int nCnt = 0; nCnt < HITCIRCLE_MAX; nCnt++)
	{
		if ((pHitCircle + nCnt)->bUse == true)
		{
			if ((pHitCircle + nCnt)->m_type == TYPE_ENEMY)
			{
				D3DXVECTOR3 C1 = *pModelpos - (pHitCircle + nCnt)->m_Matrix.pos;
				D3DXVECTOR3 C0 = *pModelposOld - (pHitCircle + nCnt)->m_posOld;
				D3DXVECTOR3 D = C1 - C0;
				float P = D3DXVec3LengthSq(&D);
				float Q = D3DXVec3Dot(&C0, &D);
				float R = D3DXVec3LengthSq(&C0);
				float Radius_SumSq = (fModelRadius + (pHitCircle + nCnt)->m_skydome.fRadius) * (fModelRadius + (pHitCircle + nCnt)->m_skydome.fRadius);

				float K = Q*Q - P*(R - Radius_SumSq);

				if (K >= 0)
				{
					float t1 = (-Q - sqrt(K) ) / P;
					float t2 = (-Q + sqrt(K) ) / P;

					bool bt1Check = false;
					bool bt2Check = false;

					if ((t1 >= 0.0f) && (t1 <= 1.0f)) bt1Check = true;
					if ((t2 >= 0.0f) && (t2 <= 1.0f)) bt2Check = true;
						
					if( (bt1Check == true) || (bt2Check == true) )
					{
						if ((bt1Check == true) && (bt2Check == true))
						{
							*pOutModelpos = *pModelposOld + t1*(*pModelpos - *pModelposOld);                                     //�ʒu����
							if (Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //���蔻��~�̓��蔻��t���O�̗L��
							if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
							bCheck = true;
							break;
						}

						else
						{
							if(bt1Check == true)
							{
								*pOutModelpos = *pModelposOld + t1*(*pModelpos - *pModelposOld);                                     //�ʒu����
								if (Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //���蔻��~�̓��蔻��t���O�̗L��
								if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
								bCheck = true;
								break;
							}

							else
							{
								*pOutModelpos = *pModelposOld + t2*(*pModelpos - *pModelposOld);                                     //�ʒu����
								if (Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //���蔻��~�̓��蔻��t���O�̗L��
								if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
								bCheck = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	return bCheck;
}

float CircleAndPlaneHitTime(const D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, const D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/,const D3DXVECTOR3 *pPlanePoint, const D3DXVECTOR3 *pPlaneNormal/*���ʖ@��*/, float fRadius/*����͈͔��a*/)
{
	D3DXVECTOR3 C0 = *pVectorStart - *pPlanePoint;
	D3DXVECTOR3 d = *pVectorEnd - *pVectorStart;
	float DotCN = D3DXVec3Dot(&C0,pPlaneNormal);
	float DotdN = D3DXVec3Dot(&d, pPlaneNormal);
	float t = -1;
	//�Փˎ���t�����߂�
	if (DotdN < 0)
	{
		t = (fRadius - DotCN) / DotdN;

		if( (t >= 0.0f) && (t <= 1.0f) )
		{
			return t;
		}
	}

	return -1;
}