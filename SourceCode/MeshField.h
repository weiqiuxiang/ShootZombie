#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define MESHFIELD_TEXTURENAME "data/TEXTURE/863.jpg"

//-----------------------------------------------------------------------------------------
//メッシュwall構造体
//-----------------------------------------------------------------------------------------
typedef enum
{
	MESH_NULL = 0,
	MESH_WALL,
	MESH_FLOOR,
	MESH_SLOPE,
	MESH_MAX,
}MESHTYPE;

typedef struct
{
	D3DXMATRIX m_MeshFieldMtxWorld;                           // ワールドマトリクス
	WORLD_MATRIX m_MeshFieldMatrix;                           //ワールドマトリクスの変換行列
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBufferMeshField;           //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
	LPDIRECT3DINDEXBUFFER9 m_pIdxBufferFieldMeshField;       //インデックスバッファを操作するためのポインタ
	D3DXVECTOR2 HW;
	float fSlopeAngle;          //メッシュの属性がMESH_SLOPEの時,斜面の斜率
	int m_nVertexNum;     //頂点数
	int m_nIndexNum;      //インデックス数
	int m_nPolygonNum;    //ポリゴン数
	int m_TexNum;         //使用テクスチャ番号
	int nID_HR;             //使用している四角判定ポリゴンの番号
	bool bUse;              //使用フラグ
	MESHTYPE meshType;
}MESHWALL;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitMeshField(void);                                                     //ポリゴン(頂点)の初期化
void DrawMeshField(void);                                                        //ポリゴンの描画
void UpdateMeshField(void);                                                      //ポリゴンの更新
void UninitMeshField(void);                                                      //ポリゴンの終了
void MakeVertexMeshFieldBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_3D *pVtx);           //ポリゴン設定
//XZ平面のメッシュウォール
void InitMeshWall_XZ(D3DXVECTOR3 pos, D3DXVECTOR3 rot, UINT nNumBlock_Honrizon, UINT nNumBlock_Vertical, float fSizeBlock_Honrizon, float fSizeBlock_Vertical,  MESHTYPE meshType,char *TexPass);
MESHWALL *GetMesh(void);    //メッシュを返す
#endif