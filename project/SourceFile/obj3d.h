#ifndef _OBJ3D_H_
#define _OBJ3D_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//マグロ
//-----------------------------------------------------------------------------------------
#define OBJ3D_TEX_MAX (5)

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
class CObj3D
{
public:
	CObj3D();
	~CObj3D();

	//定番関数
	HRESULT Init(const WORLD_MATRIX *pPosRotScl, char *ModelPass, DWORD nNumMatEB, bool bUse);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	//ファクション関数
	void SetPos(const D3DXVECTOR3 *pos);                                                   //位置設定
	void SetRot(const D3DXVECTOR3 *rot);                                                   //初期回転角度設定
	void AddRotY(float fRot);                                                              //Y軸回転
	void SetMaterial(const D3DMATERIAL9 *Material);                                        //マテリアル設定
	void AddPos(const D3DXVECTOR3 *pos);                                                   //Pos増加
	bool GetUse(void);                                                                     //使用状態取得
	void SetUse(bool bUse);                                                                //使用状態設定
	void SetMatrialUse(bool bUse);                                                         //調整用マテリアル状態設定
	static void MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);             //レンダステータス設定

protected:
	D3DXMATRIX m_WorldMatrix;                         //ワールドマトリクス
	WORLD_MATRIX m_PosRotScl;                         //オブジェPos,Rot,Scl,この三つを使ってモデリング行列を生成する
	D3DMATERIAL9 m_Material;                          //変化用マテリアル(オブジェ自体がトラブル起こる時の対策用マテリアル)
	LPD3DXMESH m_pMesh;                               //メッシュインターフェースポインタ
	LPD3DXBUFFER m_pBuffMat;                          //マテリアル情報バッファ(D3DX汎用バッファインターフェース)
	DWORD m_nNumMat;                                //マテリアルの数
	int m_TexNUM[OBJ3D_TEX_MAX];                    //テクスチャ発行番号
	bool m_UseMatrial;                                //調整用マテリアルの使用状態(trueは使用)
	bool m_bUse;                                      //使用状態フラグ
};

#endif