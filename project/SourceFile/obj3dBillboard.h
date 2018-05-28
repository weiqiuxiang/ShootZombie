#pragma once
#ifndef _OBJ3DBILLBOARD_H_
#define _OBJ3DBILLBOARD_H_

#include "main.h"

#define TEX_MAX (10)

class CObj3dBillboard
{
public:
	CObj3dBillboard();
	~CObj3dBillboard();
	HRESULT Init(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed,const D3DXVECTOR2 *psize, int TexNum);
	void Uninit(void);                                //頂点バッファ解放用
	void Update(void);
	void Draw(void);                                  //描画

	//ファクション関数
	void SetAlpha(int nAlpha);
	void SetColor(int R, int G, int B);
	void AddAlpha(int nValue);
	void SetSize(const D3DXVECTOR2 *psize);
	static HRESULT LoadTex(int TexNum, char *Texpass);
	static void UnloadTex(void);
private:
	void SpriteRorate(VERTEX_3D *pVtx);
	static void MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_MAX];                      //テクスチャメンバ

	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuffer;                              //バッファ
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_speed;
	D3DXVECTOR2 m_size;
	int m_TexNum;
	int m_Color[4];
};

#endif