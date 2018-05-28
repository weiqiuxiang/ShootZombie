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
#include "titleManager.h"
#include "input.h"
#include "obj2d.h"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////

CObj2D::CObj2D()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_rot = 0;
	m_pVtxBuffer = NULL;
	m_pTexture = NULL;
	m_bUse = false;
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_Color[nCnt] = 255;
	}
}

CObj2D::~CObj2D()
{
	
}

HRESULT CObj2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, char *TexPass,bool bUse)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	m_pos = pos;
	m_size = size;
	m_bUse = bUse;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_Color[nCnt] = 255;
	}

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&m_pVtxBuffer,NULL)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TexPass, &m_pTexture)))
	{
		MessageBox(NULL, "Obj2Dのテクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//頂点設定
	MakeVertexSetting(pD3DDevice, &pVtx[0]);

	//タイトルポリゴン
	m_pVtxBuffer-> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	SpriteRorate(&pVtx[0]);

	m_pVtxBuffer-> Unlock();                                     //バッファのアンロック
	return S_OK;
}

//ポリゴンの更新
void CObj2D::Update(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //頂点構造体のポインタ

	MakeVertexSetting(pD3DDevice, &pVtx[0]);

	//タイトルポリゴン
	m_pVtxBuffer-> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	SpriteRorate(&pVtx[0]);                                      //回転した後配置

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[1].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[2].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[3].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

	m_pVtxBuffer-> Unlock();                                     //バッファのアンロック
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void CObj2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0, m_pVtxBuffer,0,sizeof(VERTEX_2D));                   //VRAMからGPUにデータをストリーム
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	pD3DDevice -> SetTexture(0,m_pTexture);
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

//終了関数
void CObj2D::Uninit(void)
{
	if(m_pVtxBuffer != NULL)
	{
		SAFE_RELEASE(m_pVtxBuffer);
	}

	if(m_pTexture != NULL)
	{
		SAFE_RELEASE(m_pTexture);
	}
}

//背景ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void CObj2D::MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
{
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

void CObj2D::SetPos(const D3DXVECTOR3 *pos)
{
	m_pos = *pos;
}

void CObj2D::SetSize(const D3DXVECTOR3 *size)
{
	m_size = *size;
}

void CObj2D::AddRot(const float rot)
{
	m_rot += rot;
}

void CObj2D::SetAlpha(int Alpha)
{
	m_Color[3] = Alpha;

	if(m_Color[3] < 0)
	{
		m_Color[3] = 0;
	}

	if(m_Color[3] > 255)
	{
		m_Color[3] = 255;
	}
}

bool CObj2D::GetUse(void)
{
	return m_bUse;
}

void CObj2D::AddAlpha(int nValue)
{
	m_Color[3] += nValue;                                    //カラーの設定

	if(m_Color[3] >= 255)
	{
		m_Color[3] = 255;
	}

	if (m_Color[3] < 0)
	{
		m_Color[3] = 0;
	}
}

void CObj2D::SpriteRorate(VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //左上,右下座標
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;     //回転した後の座標
	double XC, YC;                                     //中心座標

	X1 = m_pos.x - 0.5*m_size.x;
	Y1 = m_pos.y - 0.5*m_size.y;
	X2 = m_pos.x + 0.5*m_size.x;
	Y2 = m_pos.y - 0.5*m_size.y;
	X3 = m_pos.x - 0.5*m_size.x;
	Y3 = m_pos.y + 0.5*m_size.y;
	X4 = m_pos.x + 0.5*m_size.x;
	Y4 = m_pos.y + 0.5*m_size.y;

	XC = m_pos.x;
	YC = m_pos.y;

	Xc1 = (X1 - XC)*cos(m_rot) - (Y1 - YC)*sin(m_rot) + XC;
	Yc1 = (X1 - XC)*sin(m_rot) + (Y1 - YC)*cos(m_rot) + YC;

	Xc2 = (X2 - XC)*cos(m_rot) - (Y2 - YC)*sin(m_rot) + XC;
	Yc2 = (X2 - XC)*sin(m_rot) + (Y2 - YC)*cos(m_rot) + YC;

	Xc3 = (X3 - XC)*cos(m_rot) - (Y3 - YC)*sin(m_rot) + XC;
	Yc3 = (X3 - XC)*sin(m_rot) + (Y3 - YC)*cos(m_rot) + YC;

	Xc4 = (X4 - XC)*cos(m_rot) - (Y4 - YC)*sin(m_rot) + XC;
	Yc4 = (X4 - XC)*sin(m_rot) + (Y4 - YC)*cos(m_rot) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1, Yc1, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2, Yc2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3, Yc3, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4, Yc4, 0.0f);
}

void CObj2D::AddPos(const D3DXVECTOR3 *pos)
{
	m_pos += *pos;
}

bool CObj2D::PointInPolygon(const D3DXVECTOR3 *pos)
{
	float X1 = m_pos.x - 0.5*m_size.x;
	float X2 = m_pos.x + 0.5*m_size.x;
	float Y1 = m_pos.y - 0.5*m_size.y;
	float Y2 = m_pos.y + 0.5*m_size.y;

	if(
		((pos->x >= X1) && (pos->x <= X2)) &&
		((pos->y >= Y1) && (pos->y <= Y2))
		)
	{
		return true;
	}

	return false;
}

void CObj2D::SetColor(int R, int G, int B)
{
	m_Color[0] = R;
	m_Color[1] = G;
	m_Color[2] = B;

	for(int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_Color[nCnt] > 255)
		{
			m_Color[nCnt] = 255;
		}

		if(m_Color[nCnt] < 0)
		{
			m_Color[nCnt] = 0;
		}
	}
}

void CObj2D::SetUse(bool bUse)
{
	m_bUse = bUse;
}