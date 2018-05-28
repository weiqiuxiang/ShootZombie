#pragma once
#ifndef _CNUMBER_H_
#define _CNUMBER_H_

#include "main.h"
#include "obj2d.h"

class CNumber
{
public:
	CNumber();
	~CNumber();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 TexPos, D3DXVECTOR3 TexSize);
	void Draw(void);
	void Update(void);
	void Uninit(void);
	void SetTexPos(const D3DXVECTOR3 *pos);
	void SetTexSize(const D3DXVECTOR3 *pos);
	void SpriteRorate(VERTEX_2D *pVtx);
	static void MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx);
	static HRESULT LoadTex(char *TexPass);
	static void UnloadTex(void);
protected:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_TexPos;
	D3DXVECTOR3 m_TexSize;
	float m_rot;
	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuffer;              //バッファ
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;                      //テクスチャ
};

class CBullet : public CNumber
{
public:
	CBullet();
	~CBullet();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 TexPos, D3DXVECTOR3 TexSize);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	static HRESULT LoadTex(char *TexPass);
	static void UnloadTex(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;                      //テクスチャ
};
#endif