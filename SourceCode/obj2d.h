#ifndef _OBJ2D_H_
#define _OBJ2D_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
class CObj2D
{
public:
	CObj2D();
	~CObj2D();

	//��Ԋ֐�
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, char *TexPass,bool bUse);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	//�t�@�N�V�����֐�
	void SetPos(const D3DXVECTOR3 *pos);
	void SetSize(const D3DXVECTOR3 *size);
	void AddRot(const float rot);
	void SetAlpha(int nAlpha);
	void SetColor(int R , int G ,int B);
	void AddAlpha(int nValue);
	void AddPos(const D3DXVECTOR3 *pos);
	bool GetUse(void);
	void SetUse(bool bUse);
	bool PointInPolygon(const D3DXVECTOR3 *pos);                                           //�_���|���S���̒��ɂ��邩�ǂ���
	static void MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);

protected:
	void SpriteRorate(VERTEX_2D *pVtx);
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	float m_rot;
	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuffer;              //�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;                      //�e�N�X�`��
	int m_Color[4];
	bool m_bUse;
};

#endif