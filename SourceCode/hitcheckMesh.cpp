#include "hitcheckMesh.h"

//�}�N��
#define NUM_QUICKSORT (10)
#define ONEDEGREE_TO_RADIAN (0.0174533)

float MinAB(float A, float B)
{
	if(A > B) return B;
	else return A;
}

bool Rects2D(D3DXVECTOR2 const *fRectA_pos, D3DXVECTOR2 const *fRectA_HW, D3DXVECTOR2 const *fRectB_pos, D3DXVECTOR2 const *fRectB_HW)
{
	if(
		(fRectA_pos->x <= (fRectB_pos->x + fRectB_HW->x) ) &&
		(fRectA_pos->y <= (fRectB_pos->y + fRectB_HW->y) ) &&
		(fRectB_pos->x <= (fRectA_pos->x + fRectA_HW->x) ) &&
		(fRectB_pos->y <= (fRectA_pos->y + fRectA_HW->y) ) 
		)
	{
		return true;
	}

	else
	{
		return false;
	}
}

float DistancePointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect)
{
	float fDistance;             //�|�C���g�ƃ|���S���̋���
	D3DXVECTOR3 Vec = *pPoint - pRect->VectexC[0].pos;             //���_�ƃ|�C���g�̃x�N�g��
	fDistance = D3DXVec3Dot(&pRect->VectexC[0].normal,&Vec) / D3DXVec3Length(&pRect->VectexC[0].normal);      //�@���ƃx�N�g���̓��� / �@���̒���
	return fabs(fDistance);
}

D3DXVECTOR3 GetVecPlaneIntersection(const D3DXVECTOR3 *pStartPoint,const D3DXVECTOR3 *pEndPoint, const HITRECT *pRect)
{
	//
	//�錾����
	///////////////////////////
	float dStart, dEnd;           //�x�N�g���̎n�_�A�I�_�Ɣ|���S���̋���
	float s;      //������
	D3DXVECTOR3 VecStart, VecEnd;           //���_�Ǝn�_�A�I�_�����x�N�g��
	D3DXVECTOR3 Intersection;        //�ђʓ_
	D3DXVECTOR3 IntersectionVec;        //�ђʓ_�x�N�g��

	//
	//�v�Z����
	///////////////////////////
	dStart = DistancePointPlane(pStartPoint, pRect);
	dEnd   = DistancePointPlane(pEndPoint, pRect);
	VecStart = *pStartPoint - pRect->VectexC[0].pos;
	VecEnd = *pEndPoint - pRect->VectexC[0].pos;
	s = dStart / (dStart + dEnd);    //������v�Z
	D3DXVec3Lerp(&IntersectionVec, &VecStart , &VecEnd, s);         //�ђʓ_�v�Z
	Intersection = IntersectionVec + pRect->VectexC[0].pos;

	return Intersection;
}

bool Rects3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect)
{
	D3DXVECTOR3 Vec01, Vec13 , Vec32, Vec20;
	D3DXVECTOR3 Vec0P,Vec1P, Vec2P, Vec3P;
	D3DXVECTOR3 n[4],Normarlize;

	//
	//�v�Z����
	////////////////////////
	//�|���S���̉E����x�N�g��
	Vec01 = pRect->VectexC[1].pos - pRect->VectexC[0].pos;
	Vec13 = pRect->VectexC[3].pos - pRect->VectexC[1].pos;
	Vec32 = pRect->VectexC[2].pos - pRect->VectexC[3].pos;
	Vec20 = pRect->VectexC[0].pos - pRect->VectexC[2].pos;

	//���_�ƃ|�C���g�̃x�N�g��
	Vec1P = *pPoint - pRect->VectexC[1].pos;
	Vec2P = *pPoint - pRect->VectexC[2].pos;
	Vec3P = *pPoint - pRect->VectexC[3].pos;
	Vec0P = *pPoint - pRect->VectexC[0].pos;

	//�O�ς�����
	D3DXVec3Cross(&n[0], &Vec01, &Vec1P);
	D3DXVec3Cross(&n[1], &Vec13, &Vec3P);
	D3DXVec3Cross(&n[2], &Vec32, &Vec2P);
	D3DXVec3Cross(&n[3], &Vec20, &Vec0P);

	//���x����
	for (int i = 0; i < 4; i++)
	{
		//�P�ʃx�N�g����
		D3DXVec3Normalize(&n[i], &n[i]);
	}

	Normarlize = pRect->VectexC[0].normal;

	float Dot[4];
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		Dot[nCnt] = D3DXVec3Dot(&n[nCnt],&Normarlize);             //���ʖ@���ƃ|���S���̖@���̓��ς�����
	}

	//����(4���ʖ@���x�N�g���A�e�ƃ|���S���@���̓��ς̌��ʁA�܂�cos()�̒l�͖�����1�ɋ߂��Ȃ�,���萬��)
	if(
		(Dot[0] > 0.99) &&
		(Dot[1] > 0.99) &&
		(Dot[2] > 0.99) &&
		(Dot[3] > 0.99)
		)
	{
		return true;
	}

	return false;
}

bool Rect2D_Vector(const D3DXVECTOR2 *pPoint, const RECT_2D *pRect)
{
	D3DXVECTOR2 Vec01, Vec13, Vec32, Vec20;
	D3DXVECTOR2 Vec0P, Vec1P, Vec2P, Vec3P;
	float Value[4];

	RECT_2D Rect2D = *pRect;

	//�v�Z�p�x�N�g���Z�o
	Vec01 = Rect2D.p[1] - Rect2D.p[0];
	Vec13 = Rect2D.p[3] - Rect2D.p[1];
	Vec32 = Rect2D.p[2] - Rect2D.p[3];
	Vec20 = Rect2D.p[0] - Rect2D.p[2];

	Vec0P = *pPoint - Rect2D.p[0];
	Vec1P = *pPoint - Rect2D.p[1];
	Vec2P = *pPoint - Rect2D.p[2];
	Vec3P = *pPoint - Rect2D.p[3];

	Value[0] = D3DXVec2CCW(&Vec01, &Vec0P);
	Value[1] = D3DXVec2CCW(&Vec13, &Vec1P);
	Value[2] = D3DXVec2CCW(&Vec32, &Vec3P);
	Value[3] = D3DXVec2CCW(&Vec20, &Vec2P);

	//����
	if (
		((Value[0] <= 0) &&
		 (Value[1] <= 0) &&
		 (Value[2] <= 0) &&
		 (Value[3] <= 0))||

		((Value[0] >= 0) &&
		(Value[1] >= 0) &&
		(Value[2] >= 0) &&
		(Value[3] >= 0)) 
		)
	{
		return true;
	}

	return false;
}

bool Rect3D_Vector(const D3DXVECTOR3 *pPoint, const RECT_3D *pRect)
{
	int nCntTure = 0;
	//
	//�����_�ȉ���������x�ۂ߂���
	////////////////////////////////////////////////
	D3DXVECTOR3 Point = *pPoint;
	RECT_3D Rect = *pRect;

	//
	//XY���ʂ̔���
	///////////////////////////////////////////////////
	//point��Rect2D�����
	RECT_2D Rect2DXY;
	D3DXVECTOR2 PointXY = D3DXVECTOR2(Point.x, Point.y);
	Rect2DXY.p[0] = D3DXVECTOR2(Rect.p0.x, Rect.p0.y);
	Rect2DXY.p[1] = D3DXVECTOR2(Rect.p1.x, Rect.p1.y);
	Rect2DXY.p[2] = D3DXVECTOR2(Rect.p2.x, Rect.p2.y);
	Rect2DXY.p[3] = D3DXVECTOR2(Rect.p3.x, Rect.p3.y);

	//2d�l�ӌ`�ƃ|�C���g�̓��蔻����s��
	if (Rect2D_Vector(&PointXY, &Rect2DXY) == true)
	{
		nCntTure++;
	}

	//
	//YZ���ʂ̔���
	///////////////////////////////////////////////////
	//point��Rect2D�����
	RECT_2D Rect2DYZ;
	D3DXVECTOR2 PointYZ = D3DXVECTOR2(Point.y, Point.z);
	Rect2DYZ.p[0] = D3DXVECTOR2(Rect.p0.y, Rect.p0.z);
	Rect2DYZ.p[1] = D3DXVECTOR2(Rect.p1.y, Rect.p1.z);
	Rect2DYZ.p[2] = D3DXVECTOR2(Rect.p2.y, Rect.p2.z);
	Rect2DYZ.p[3] = D3DXVECTOR2(Rect.p3.y, Rect.p3.z);
	//2d�l�ӌ`�ƃ|�C���g�̓��蔻����s��
	if (Rect2D_Vector(&PointYZ, &Rect2DYZ) == true)
	{
		nCntTure++;
	}

	//
	//XY���ʂ̔���
	///////////////////////////////////////////////////
	//point��Rect2D�����
	RECT_2D Rect2DXZ;
	D3DXVECTOR2 PointXZ = D3DXVECTOR2(Point.x, Point.z);
	Rect2DXZ.p[0] = D3DXVECTOR2(Rect.p0.x, Rect.p0.z);
	Rect2DXZ.p[1] = D3DXVECTOR2(Rect.p1.x, Rect.p1.z);
	Rect2DXZ.p[2] = D3DXVECTOR2(Rect.p2.x, Rect.p2.z);
	Rect2DXZ.p[3] = D3DXVECTOR2(Rect.p3.x, Rect.p3.z);
	//2d�l�ӌ`�ƃ|�C���g�̓��蔻����s��
	if (Rect2D_Vector(&PointXZ, &Rect2DXZ) == true)
	{
		nCntTure++;
	}

	if (nCntTure == 3) return true;
	return false;
}

bool Wall3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect)
{
	RECT_2D RectXY, RectYZ;           //�|���S����XY���ʂ̓��e��YZ���ʂ̓��e
	D3DXVECTOR2 PointXY,PointYZ;                //����_��XY���ʂ̓��e��YZ���ʂ̓��e
	//�t���O
	bool bXYCheck = false;
	bool bYZCheck = false;

	for (int i = 0; i < 4; i++) 
	{
		RectXY.p[i] = D3DXVECTOR2(pRect->VectexC[i].pos.x, pRect->VectexC[i].pos.y);
		RectYZ.p[i] = D3DXVECTOR2(pRect->VectexC[i].pos.y, pRect->VectexC[i].pos.z);
	}

	PointXY = D3DXVECTOR2(pPoint->x, pPoint->y);
	PointYZ = D3DXVECTOR2(pPoint->y, pPoint->z);

	bXYCheck = Rect2D_Vector(&PointXY,&RectXY);
	bYZCheck = Rect2D_Vector(&PointYZ, &RectYZ);

	if(bXYCheck && bYZCheck)
	{
		return true;
	}

	return false;
}

bool VectorAndRect(D3DXVECTOR3 *pVectorStart, D3DXVECTOR3 *pVectorEnd)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //���ʃx�N�g��������
	D3DXVECTOR3 MoveSpeedVector;             //�ړ��ʃx�N�g��
	float WallMoveSpeed;                         //�ǂɉ����ړ���
	HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bCheck = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[

	//�ړ��ʃx�N�g���v�Z
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y�����̒l��0�ɂ���

	//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			float a, b;
			D3DXVECTOR3 VecA = *pVectorStart - (pHitRect + nCnt)->VectexC[0].pos;
			D3DXVECTOR3 VecB = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].pos;
			a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
			b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������

																					  //�@�����萬��
			if ((a*b) <= 0)
			{
				D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorStart, pVectorEnd, (pHitRect + nCnt));    //�ђʓ_�����߂�
				//�΂�
				if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
				{
					//�ђʓ_�����ʏ�ɂ��邩���肷��
					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
					{
						pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
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
					RectA_Pos.x = MinAB(pVectorStart->x, pVectorEnd->x);
					RectA_Pos.y = MinAB(pVectorStart->z, pVectorEnd->z);
					RectA_HW.x = fabs(pVectorEnd->x - pVectorStart->x);
					RectA_HW.y = fabs(pVectorEnd->z - pVectorStart->z);

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
							pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
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
							pVectorEnd->x = pVectorStart->x;
							pVectorEnd->z = pVectorStart->z;
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

	return bCheck;
}

bool VectorAndRect_GroundCheck(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //���ʃx�N�g��������
	D3DXVECTOR3 MoveSpeedVector;             //�ړ��ʃx�N�g��
	float WallMoveSpeed;                         //�ǂɉ����ړ���
	HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bGround = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�

	//�ړ��ʃx�N�g���v�Z
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y�����̒l��0�ɂ���

	//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			float a, b;
			D3DXVECTOR3 VecA = *pVectorStart - (pHitRect + nCnt)->VectexC[0].pos;
			D3DXVECTOR3 VecB = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].pos;
			a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
			b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������

																					  //�@�����萬��
			if ((a*b) <= 0)
			{
				D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorStart, pVectorEnd, (pHitRect + nCnt));    //�ђʓ_�����߂�
				//�΂�
				if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
				{
					//�ђʓ_�����ʏ�ɂ��邩���肷��
					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
					{
						pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
						bGround = true;
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
					RectA_Pos.x = MinAB(pVectorStart->x, pVectorEnd->x);
					RectA_Pos.y = MinAB(pVectorStart->z, pVectorEnd->z);
					RectA_HW.x = fabs(pVectorEnd->x - pVectorStart->x);
					RectA_HW.y = fabs(pVectorEnd->z - pVectorStart->z);

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
							pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
							bGround = true;
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
							pVectorEnd->x = pVectorStart->x;
							pVectorEnd->z = pVectorStart->z;

							//XZ���ʂł̈ړ����x��ǉ����ړ��x�N�g���ƈړ��ʃx�N�g���̓��ςŎZ�o
							WallMoveSpeed = D3DXVec3Dot(&MoveSpeedVector, &(pHitRect + nCnt)->VecXZ);

							*pVectorEnd += (pHitRect + nCnt)->VecXZ * WallMoveSpeed;                    //�ǉ����ړ�

							nWallCollision++;
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

		//�����̕ǂƔ��肪����ꍇ,�O�̍��W�ɖ߂�
		if(nWallCollision > 1)
		{
			*pVectorEnd = *pVectorStart;
		}

	}

	return bGround;
}

//�J�����ƃ��b�V���̔���
void VectorAndRect_TpsCamera(D3DXVECTOR3 *pVectorLookat/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorCamera/*�x�N�g���I�_�|�C���^*/)
{
	D3DXVECTOR3 resultVector = *pVectorCamera - *pVectorLookat;           //���ʃx�N�g��������
	
	HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	bool bCheck = false;  //�n�ʏՓ˃`�F�b�N�t���O
	int nCnt;              //�J�E���^�[
	int nWallCollision = 0;  //MESH_WALL�Ƃ̏Փː�
	float fDistance = 0;                                //�J�����ƏՓ˕��ʂ̋���
	float fMinDistance = 100000;                           //�J�����Ƒ����ʂ̏Փ˂ɑ΂��ăJ�����ƕ��ʂ̍ŒZ����

	//�J����-�����_�x�N�g����P�ʉ�
	D3DXVec3Normalize(&resultVector,&resultVector);

	//����`�F�b�N
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����l�p�|���S���g�p���Ȃ�
		if ((pHitRect + nCnt)->bUse == true)
		{
			float a, b;
			D3DXVECTOR3 VecA = *pVectorLookat - (pHitRect + nCnt)->VectexC[0].pos;
			D3DXVECTOR3 VecB = *pVectorCamera - (pHitRect + nCnt)->VectexC[0].pos;
			a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //�n�_�Ɩ@������
			b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //�I�_�Ɩ@������

			//�@�����萬��
			if ((a*b) <= 0)
			{
				D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorLookat, pVectorCamera, (pHitRect + nCnt));    //�ђʓ_�����߂�

				//�ŒZ�����_--�Փ˃��b�V�������v�Z
				fDistance = DistancePointPlane(pVectorLookat,(pHitRect + nCnt));          //�����_�ƏՓ˃��b�V���̋���
				
				//�΂�
				if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
				{
					//�ђʓ_�����ʏ�ɂ��邩���肷��
					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
					{
						if (fDistance < fMinDistance)
						{
							fMinDistance = fDistance;                              //�ŒZ�����ۑ�
							*pVectorCamera = Intersection - resultVector*0.1;     //���C��
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
					RectA_Pos.x = MinAB(pVectorLookat->x, pVectorCamera->x);
					RectA_Pos.y = MinAB(pVectorLookat->z, pVectorCamera->z);
					RectA_HW.x = fabs(pVectorCamera->x - pVectorLookat->x);
					RectA_HW.y = fabs(pVectorCamera->z - pVectorLookat->z);

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
								*pVectorCamera = Intersection - resultVector*1.0;     //���C��
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
								*pVectorCamera = Intersection - resultVector*1.0;     //���C��
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

float GetElevation(int nID,const D3DXVECTOR3 *pModelPos)
{
	HITRECT *pHitRect = Get_HitRect();                                //����l�p�|���S���f�[�^�擾
	D3DXVECTOR3 Vec = (pHitRect + nID)->VectexC[0].pos - *pModelPos;   //�v�Z�p�x�N�g��
	float fElevation;   //�W��

	//��������̒n�ʂ����݂���
	if( (pHitRect + nID)->bUse == true )
	{
		//�W���v�Z
		fElevation = (pHitRect + nID)->VectexC[0].pos.y + ((pHitRect + nID)->VectexC[0].normal.x*Vec.x + (pHitRect + nID)->VectexC[0].normal.z*Vec.z) / (pHitRect + nID)->VectexC[0].normal.y;
	}

	return fElevation;
}


void Quick_Sort(int nLeftID, int nRightID,float *a)
{
	//---------------------------
	//�ϐ��錾
	//---------------------------
	int nCnt = 0;           //�J�E���g
	int nTotalNum = nRightID - nLeftID + 1;              //����
	int nMiddleID = (nLeftID + nRightID) / 2;
	int nLeftNum_BE = nMiddleID - nLeftID;           //�����O�̒��Ԓl�̍����̔z�񃁃��o��
	int nRightNum_BE = nRightID - nMiddleID;         //�����O�̒��Ԓl�̉E���̔z�񃁃��o��
	int nLeftNum_AF = 0; //������̒��Ԓl�̍����̔z�񃁃��o��
	int nRightNum_AF = 0; //������̒��Ԓl�̉E���̔z�񃁃��o��

	float aDistanceL[NUM_QUICKSORT];                    //�p�[�e�B�N������J�����܂ł̋������z��
	float aDistanceR[NUM_QUICKSORT];                    //�p�[�e�B�N������J�����܂ł̋����E�z��

	//---------------------------
	//���ƉE����
	//---------------------------
	for (int i = 1; i <= nLeftNum_BE; i++)
	{
		//---------------------------
		//���Ԓl�O�̃����o�l�`�F�b�N
		//---------------------------
		//���z��ɓ����
		if (a[nMiddleID - i] < a[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = a[nMiddleID - i];          //�����z��ɓ����
			nLeftNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
		//�E�z��ɓ����
		else
		{
			aDistanceR[nRightNum_AF] = a[nMiddleID - i];  //�z��ɓ����
			nRightNum_AF++;                                         //�����o���J�E���g�A�b�v
		}

		//---------------------------
		//���Ԓl���̃����o�l�`�F�b�N
		//---------------------------
		//���z��ɓ����
		if (a[nMiddleID + i] < a[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = a[nMiddleID + i];          //�����z��ɓ����
			nLeftNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
		//�E�z��ɓ����
		else
		{
			aDistanceR[nRightNum_AF] = a[nMiddleID + i];  //�z��ɓ����
			nRightNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
	}

	//�����������̏ꍇ
	if (nRightNum_BE > nLeftNum_BE)
	{
		//���z��ɓ����
		if (a[nRightID] < a[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = a[nRightID];          //�����z��ɓ����
			nLeftNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
		//�E�z��ɓ����
		else
		{
			aDistanceR[nRightNum_AF] = a[nRightID];  //�z��ɓ����
			nRightNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
	}

	//----------------------------
	//�����̌�̔z���z��ɔ��f����
	//----------------------------
	nCnt = nLeftID;              //�J�E���g������
	float MIDistance = a[nMiddleID];      //���Ԓl�ۑ�

											   //������
	for (int j = 0; j < nLeftNum_AF; j++)
	{
		a[nCnt] = aDistanceL[j];        //�������
		nCnt++;            //�J�E���g�A�b�v
	}

	//���Ԓl
	a[nCnt] = MIDistance;        //�������
	nCnt++;       //�J�E���g�A�b�v

				  //�E����
	for (int j = 0; j < nRightNum_AF; j++)
	{
		a[nCnt] = aDistanceR[j];        //�������
		nCnt++;            //�J�E���g�A�b�v
	}

	//------------------
	//�ċA����
	//------------------
	//������
	if (nLeftNum_AF > 1)
	{
		Quick_Sort(nLeftID, nLeftID + nLeftNum_AF - 1,a);
	}

	//�E����
	if (nRightNum_AF > 1)
	{
		Quick_Sort(nRightID - nRightNum_AF + 1, nRightID,a);
	}
}