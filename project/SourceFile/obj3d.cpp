/*
cpp名:titleobj.cpp
タイトルの2D画像処理用cpp
*/

//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "obj3d.h"
#include "textureManager.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////

CObj3D::CObj3D()
{
	D3DXMatrixIdentity(&m_WorldMatrix);
	m_PosRotScl.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosRotScl.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosRotScl.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	//調整用マテリアル設定
	m_Material.Diffuse.r = 255;
	m_Material.Diffuse.g = 0;
	m_Material.Diffuse.b = 0;
	m_Material.Diffuse.a = 50;
	m_Material.Ambient = m_Material.Diffuse;
	m_Material.Specular.r = 0;
	m_Material.Specular.g = 0;
	m_Material.Specular.b = 0;
	m_Material.Specular.a = 0;
	m_Material.Emissive = m_Material.Specular;
	m_Material.Power = 0.0f;

	m_UseMatrial = false;
	m_bUse = false;

	m_pMesh = NULL;
	m_pBuffMat = NULL;

	m_nNumMat = 0;                               //マテリアル数

	for (int nCnt = 0; nCnt < OBJ3D_TEX_MAX; nCnt++)
	{
		m_TexNUM[nCnt] = -1;
	}
}

CObj3D::~CObj3D()
{

}

HRESULT CObj3D::Init(const WORLD_MATRIX *pPosRotScl, char *ModelPass,DWORD nNumMatEB, bool bUse)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	m_PosRotScl = *pPosRotScl;                        //データ代入
	m_nNumMat = nNumMatEB;                                     //マテリアル数代入
	m_UseMatrial = false;
	m_bUse = bUse;
	D3DXMatrixIdentity(&m_WorldMatrix);

	//モデルデータの読み込み
	if(FAILED(D3DXLoadMeshFromX(ModelPass, D3DXMESH_MANAGED/*指数メモリ*/, pD3DDevice/*デバイス*/, NULL/*隣接バッファ*/, &m_pBuffMat/*マテリアル情報を格納する*/, NULL, &m_nNumMat/*マテリアル数*/, &m_pMesh/*メッシュ*/)))
	{
		MessageBox(NULL, "モデルMODEL_NAMEの読み込みが失敗した", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();   //マテリアル情報のポインタとしてバッファのアドレスを取得

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < m_nNumMat; nCnt++)
	{
		SetTexture(&m_TexNUM[nCnt], pMat[nCnt].pTextureFilename);
	}

	//調整用マテリアル設定
	m_Material = pMat[0].MatD3D;
	m_Material.Ambient.r = 1;
	m_Material.Ambient.g = 1;
	m_Material.Ambient.b = 1;
	m_Material.Ambient.a = 0.5;

	return S_OK;
}

//ポリゴンの更新
void CObj3D::Update(void)
{
	
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void CObj3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	TEXTUREMANAGER *pTexManager = GetTextureManager();

	D3DMATERIAL9 matDef;
	pD3DDevice->GetMaterial(&matDef);                                                 //デバイスのマテリアルを取得し、保存する

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	TEXTUREMANAGER *pTextureM = GetTextureManager();                                  //テクスチャマネージャー取得

	if (m_bUse == true)
	{
		SetMatrix(&m_WorldMatrix, m_PosRotScl);                                            //マトリクスの計算

		for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
		{
			if (m_UseMatrial == true) pD3DDevice->SetMaterial(&m_Material);
			else pD3DDevice->SetMaterial(&pMat[nCnt].MatD3D);
			if(pMat[nCnt].pTextureFilename != NULL) pD3DDevice->SetTexture(0, (pTexManager+ m_TexNUM[nCnt])->TextureSlot);
			else pD3DDevice->SetTexture(0,NULL);
			m_pMesh->DrawSubset(nCnt);
		}
	}
	pD3DDevice->SetMaterial(&matDef);                                                //マテリアルを戻す
}

//終了関数
void CObj3D::Uninit(void)
{
	if (m_pMesh != NULL)
	{
		SAFE_RELEASE(m_pMesh);
	}

	if (m_pBuffMat != NULL)
	{
		SAFE_RELEASE(m_pBuffMat);
	}
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void CObj3D::MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CObj3D::SetPos(const D3DXVECTOR3 *pos)
{
	m_PosRotScl.pos = *pos;
}

void CObj3D::SetRot(const D3DXVECTOR3 *rot)
{
	m_PosRotScl.rot = *rot;
}

void CObj3D::AddRotY(float rot)
{
	m_PosRotScl.rot.y += rot;
}

void CObj3D::SetMaterial(const D3DMATERIAL9 *Material)
{
	m_Material = *Material;
}

bool CObj3D::GetUse(void)
{
	return m_bUse;
}

void CObj3D::AddPos(const D3DXVECTOR3 *pos)
{
	m_PosRotScl.pos += *pos;
}

void CObj3D::SetUse(bool bUse)
{
	m_bUse = bUse;
}

void CObj3D::SetMatrialUse(bool bUse)
{
	m_UseMatrial = bUse;
}