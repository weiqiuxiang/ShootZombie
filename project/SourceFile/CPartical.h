#pragma once
#ifndef _CPARTICAL_H_
#define _CPARTICAL_H_

#include "main.h"

#define TEX_MAX (10)

class CPartical
{
public:
	typedef enum
	{
		PARTICAL_NORMAL = 0,                            //一般のパーティクル
		PARTICAL_GRAVITY,                               //重力効果ありパーティクル
		PARTICAL_MAX
	}PARTICAL_TYPE;

	CPartical();
	~CPartical();
	HRESULT Init(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed, float fRadius, int nLife, int TexNum, PARTICAL_TYPE type,float fGravity);
	void Uninit(void);                                //頂点バッファ解放用
	void Update(void);
	void Draw(void);                                  //描画

	//ファクション関数
	int GetLife(void);
	void SetAlpha(int nAlpha);
	void SetColor(int R, int G, int B);
	void AddAlpha(int nValue);
	void AddRadius(float fValue);
	static HRESULT LoadTex(int TexNum,char *Texpass);
	static void UnloadTex(void);
private:
	void SpriteRorate(VERTEX_3D *pVtx);
	static void MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_MAX];                      //テクスチャメンバ

	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuffer;                              //バッファ
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_speed;
	float m_fRadius;
	int m_nLife;
	int m_TexNum;
	int m_Color[4];
	int m_nAlphaSpeed;
	float m_fRadiusSpeed;
	float m_fGravity;
	PARTICAL_TYPE m_type;
};

#endif