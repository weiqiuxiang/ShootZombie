#define _CRT_SECURE_NO_WARNINGS
#include "motion.h"
#include "textureManager.h"
#include "matrix.h"
#include "model_aircraft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PART_NUM (15)

CPart CModel::m_partData[PART_MAX];

/************************************************************CPartクラス部分***************************************************************************/
/**********************************************************************開始***************************************************************************/
CPart::CPart()
{
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	for(int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		m_TexSlot[nCnt] = NULL;
	}
}

CPart::~CPart()
{
	
}

HRESULT CPart::Init(DWORD nNumMatModel)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	//モデルデータの読み込み
	if (FAILED(D3DXLoadMeshFromX(m_aModelpass, D3DXMESH_MANAGED/*指数メモリ*/, pD3DDevice/*デバイス*/, NULL/*隣接バッファ*/, &m_pBuffMat/*マテリアル情報を格納する*/, NULL, &nNumMatModel/*マテリアル数*/, &m_pMesh/*メッシュ*/)))
	{
		MessageBox(NULL, "モデルMODEL_NAMEの読み込みが失敗した", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();   //マテリアル情報のポインタとしてバッファのアドレスを取得

	for (int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		m_TexSlot[nCnt] = NULL;
	}
	
	//テクスチャ読み込む
	for (int nCnt = 0; nCnt < nNumMatModel; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, pMat[nCnt].pTextureFilename, &m_TexSlot[nCnt])))
			{
				MessageBox(NULL, "モデルパーツのテクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
				return E_FAIL;
			}
		}
	}
	
	m_NumMat = nNumMatModel;
	m_nIndex = -1;
	m_nNumParent = -1;
	m_bUse = false;

	return S_OK;
}

void CPart::SetInit(const WORLD_MATRIX *offsetInit, int nIndex, int nNumParent)
{
	m_OffsetInit = *offsetInit;
	m_nIndex = nIndex;
	m_nNumParent = nNumParent;
	m_bUse = true;
}

void CPart::SetKeyBefore(const WORLD_MATRIX *keyBefore)
{
	m_keyBefore = *keyBefore;
}

void CPart::SetKeyAfter(const WORLD_MATRIX *keyAfter)
{
	m_keyAfter = *keyAfter;
}

void CPart::SetModelPass(char *Modelpass)
{
	strcpy(m_aModelpass, Modelpass);
}

D3DXMATRIX CPart::GetResultMtx(void)
{
	return m_MtxResult;
}

bool CPart::GetUse(void)
{
	return m_bUse;
}

void CPart::SetUse(bool bUse)
{
	m_bUse = bUse;
}

int CPart::GetIndex(void)
{
	return m_nIndex;
}

int CPart::GetParent(void)
{
	return m_nNumParent;
}

void CPart::SetParentID(int nID)
{
	m_nID = nID;
}

int CPart::GetParentID(void)
{
	return m_nID;
}

HRESULT CPart::ModelChange(char *Modelpass,DWORD nNumMatModel)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	strcpy(m_aModelpass, Modelpass);
	m_NumMat = nNumMatModel;
	Uninit();

	//モデルデータの読み込み
	if (FAILED(D3DXLoadMeshFromX(m_aModelpass, D3DXMESH_MANAGED/*指数メモリ*/, pD3DDevice/*デバイス*/, NULL/*隣接バッファ*/, &m_pBuffMat/*マテリアル情報を格納する*/, NULL, &nNumMatModel/*マテリアル数*/, &m_pMesh/*メッシュ*/)))
	{
		MessageBox(NULL, "モデルMODEL_NAMEの読み込みが失敗した", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();   //マテリアル情報のポインタとしてバッファのアドレスを取得

	for (int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		m_TexSlot[nCnt] = NULL;
	}

	//テクスチャ読み込む
	for (int nCnt = 0; nCnt < nNumMatModel; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, pMat[nCnt].pTextureFilename, &m_TexSlot[nCnt])))
			{
				MessageBox(NULL, "モデルパーツのテクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
				return E_FAIL;
			}
		}
	}
}

void CPart::Update(int nMotionFrameNum, int nMotionFrameNow, const D3DXMATRIX *pParentMtx)
{
	D3DXMATRIX  MtxInit,mtxParent, mtxOffsetNow;
	D3DXMATRIX mtxRotOffset, mtxMoveOffset,mtxRotInit,mtxMoveInit;
	
	WORLD_MATRIX MtxOffset;

	MtxOffset.pos = m_keyAfter.pos - m_keyBefore.pos;
	MtxOffset.rot = m_keyAfter.rot - m_keyBefore.rot;
	MtxOffset.pos = m_keyBefore.pos + MtxOffset.pos*(float)nMotionFrameNow / (float)nMotionFrameNum;
	MtxOffset.rot = m_keyBefore.rot + MtxOffset.rot*(float)nMotionFrameNow / (float)nMotionFrameNum;

	D3DXMatrixRotationYawPitchRoll(&mtxRotOffset, MtxOffset.rot.y/*y軸*/, MtxOffset.rot.x/*x軸*/, MtxOffset.rot.z/*z軸*/); //回転行列を設定
	D3DXMatrixTranslation(&mtxMoveOffset, MtxOffset.pos.x, MtxOffset.pos.y, MtxOffset.pos.z);                         //移動行列設定
	mtxOffsetNow = mtxRotOffset*mtxMoveOffset;                                       //前キーフレームのマトリクス

	D3DXMatrixRotationYawPitchRoll(&mtxRotInit, m_OffsetInit.rot.y/*y軸*/, m_OffsetInit.rot.x/*x軸*/, m_OffsetInit.rot.z/*z軸*/); //回転行列を設定
	D3DXMatrixTranslation(&mtxMoveInit, m_OffsetInit.pos.x, m_OffsetInit.pos.y, m_OffsetInit.pos.z);                         //移動行列設定
	MtxInit = mtxRotInit*mtxMoveInit;                                       //前キーフレームのマトリクス

	mtxParent = *pParentMtx;

	m_MtxResultOld = m_MtxResult;                 //フレーム変化前に保存

	m_MtxResult = MtxInit*mtxParent;         //最終のリザルトマトリクス算出
	m_MtxResult = mtxOffsetNow*m_MtxResult;
}

void CPart::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	D3DMATERIAL9 matDef;
	pD3DDevice->GetMaterial(&matDef);                                                 //デバイスのマテリアルを取得し、保存する

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	TEXTUREMANAGER *pTextureM = GetTextureManager();                                  //テクスチャマネージャー取得
	
	AIRCRAFT *pAircraft = Get_Aircraft();

	D3DMATERIAL9 DamageMat;
	DamageMat.Diffuse.r = 0;
	DamageMat.Diffuse.g = 255;
	DamageMat.Diffuse.b = 0;
	DamageMat.Diffuse.a = 50;
	DamageMat.Ambient = DamageMat.Diffuse;
	DamageMat.Specular.r = 0;
	DamageMat.Specular.g = 0;
	DamageMat.Specular.b = 0;
	DamageMat.Specular.a = 0;
	DamageMat.Emissive = DamageMat.Specular;
	DamageMat.Power = 1.0f;

	if (m_bUse == true)
	{
		pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxResult);

		for (int nCnt = 0; nCnt < (int)m_NumMat; nCnt++)
		{
			pD3DDevice->SetMaterial(&pMat[nCnt].MatD3D);
			if (pAircraft->m_NoDamageTime > 0) pD3DDevice->SetMaterial(&DamageMat);          //無敵時間内
			pD3DDevice->SetTexture(0, m_TexSlot[nCnt]);
			m_pMesh->DrawSubset(nCnt);
		}
	}

	pD3DDevice->SetMaterial(&matDef);                                                //マテリアルを戻す
}

void CPart::Uninit(void)
{
	if(m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if(m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	for (int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		if (m_TexSlot[nCnt] != NULL)
		{
			m_TexSlot[nCnt]->Release();
			m_TexSlot[nCnt] = NULL;
		}
	}
}

/**********************************************************************終了***************************************************************************/
CModel::CModel()
{
	for (int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		m_partData[nCnt] = {};
	}
}

CModel::~CModel()
{
	
}

HRESULT CModel::Init(char *filepass)
{
	FILE *fp;
	char buf[256];
	int nReal = 0;

	m_nMotionFrameNum = 0;
	m_nMotionFrameNow = 0;
	m_nKeyNum = 0;
	m_nKeyNow = 0;

	//初期化
	for (int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		m_partData[nCnt].SetUse(false);
	}

	strcpy(aMotionTextPass, filepass);                           //ファイルパスコピー

	if( (fp = fopen(aMotionTextPass,"r")) == NULL)
	{
		MessageBox(NULL, "モーションテキストの読み込み失敗", "エラー", MB_OK | MB_ICONHAND);
	}

	//パーツ数の読み込み
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

		//読み飛ばし部分
		nReal = 0;
		if( (strncmp(buf, "#", 1) == 0) || (strcmp(buf,"\n") == 0) ) continue;                          //コメントアウトと改行を読み飛ばす
		for(int nCnt = 0;nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;                                  //データ部分まで読み込むとループから抜く
				break;
			}
		}
		
		if (strncmp((buf+ nReal), "NUM_MODEL =", 11) == 0)
		{
			sscanf((buf + nReal), "%*11c%d", &m_nNumpart);                    //パーツ数を読み込む
			break;
		}

	} while(strncmp((buf + nReal), "NUM_MODEL =", 11) != 0);

	//パーツのモデルパスの読み込み
	for (int nCnt = 0; nCnt < m_nNumpart;nCnt++)
	{
		char agomi[256];                                           //読み捨て用
		char aData[256];

		do
		{
			fgets(buf, sizeof(buf), fp);

			//読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf+nReal), "MODEL_FILENAME =", 16) == 0) break;
		} while (strncmp((buf + nReal), "MODEL_FILENAME =", 16) != 0);

		sscanf(buf, "%*16c%s%s", &aData[0], &agomi[0]);
		m_partData[nCnt].SetModelPass(&aData[0]);                                                      //モデルパス読み込む
	}

	//モデルのデータ初期化とマテリアル数設定
	for (int nCnt = 0; nCnt < PART_NUM; nCnt++)
	{
		m_partData[nCnt].Init(1);
	}

	//モーション初期データの読み込み
	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		int nIndex = -2;
		int nParent = -2;
		WORLD_MATRIX mTx;
		mTx.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		//PARTSSETを読み込むことが確認できるまで読み込み続く
		do
		{
			fgets(buf, sizeof(buf), fp);

			//読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;

				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf+ nReal), "PARTSSET", 8) == 0)
			{
				fgets(buf, sizeof(buf), fp);                  //次の行列のデータを読み込む
				break;
			}
		} while (strncmp((buf + nReal), "PARTSSET", 8) != 0);

		//インデックス番号読み込み
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if((strncmp(buf + nReal, "INDEX =", 7) == 0) )
		{
			sscanf(buf + nReal, "%*7c%d", &nIndex);                                       //自分の番号読み込み(インデックス)
			fgets(buf, sizeof(buf), fp);                  //次の行列のデータを読み込む
		}
		else
		{
			MessageBox(NULL, "INDEXの読み込み失敗", "エラー", MB_OK | MB_ICONHAND);
		}

		//親のパーツ番号読み込み
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if ((strncmp(buf+ nReal, "PARENT =", 8) == 0))
		{
			sscanf(buf+ nReal, "%*8c%d", &nParent);                                      //親の番号読み込み
			fgets(buf, sizeof(buf), fp);                  //次の行列のデータを読み込む
		}
		else
		{
			MessageBox(NULL, "PARENTの読み込み失敗", "エラー", MB_OK | MB_ICONHAND);
		}

		//POS
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if((strncmp(buf + nReal, "POS =", 5) == 0))
		{
			sscanf(buf + nReal, "%*5c%f%f%f", &mTx.pos.x,&mTx.pos.y, &mTx.pos.z);
			fgets(buf, sizeof(buf), fp);                  //次の行列のデータを読み込む
		}
		else
		{
			MessageBox(NULL, "POSの読み込み失敗", "エラー", MB_OK | MB_ICONHAND);
		}

		//ROT
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if ((strncmp(buf + nReal, "ROT =", 5) == 0))
		{
			sscanf(buf + nReal, "%*5c%f%f%f", &mTx.rot.x, &mTx.rot.y,&mTx.rot.z);
			fgets(buf, sizeof(buf), fp);                  //次の行列のデータを読み込む
		}
		else
		{
			MessageBox(NULL, "ROTの読み込み失敗", "エラー", MB_OK | MB_ICONHAND);
		}

		m_partData[nCnt].SetInit(&mTx, nIndex, nParent);                          //初期オフセットなどのデータをセット
	}

	fclose(fp);

	//ID登録
	for(int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nNumpart; nCnt2++)
		{
			if (nCnt != 0)
			{
				if(m_partData[nCnt].GetParent() == m_partData[nCnt2].GetIndex())
				{
					m_partData[nCnt].SetParentID(nCnt2);
					break;
				}
			}
		}
	}

	return S_OK;
}

void CModel::Update(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();

	D3DXMATRIX WorldMtx = pAircraft->m_MtxWorld;

	FILE *fp;
	char buf[256];
	int nReal = 0;

	MotionName = pAircraft->m_MotionNow;                    //モーション取得

	if(pAircraft->m_GunType != pAircraft->m_GunTypeOld)
	{
		if (pAircraft->m_GunType == GUNTYPE_GRENADE)
		{
			m_partData[14].ModelChange("data/MODEL/16_Grenade.x",2);
		}

		else
		{
			m_partData[14].ModelChange("data/MODEL/15_gun.x", 1);
		}
	}

	if (pAircraft->m_MotionNow != pAircraft->m_MotionBefore)
	{
		m_nKeyNow = 0;                                //モーション変換の時開始キーを0にする
		m_nMotionFrameNow = 0;
	}

	//テキスト読み込み
	if ((fp = fopen(aMotionTextPass, "r")) == NULL)
	{
		MessageBox(NULL, "モーションテキストの読み込み失敗", "エラー", MB_OK | MB_ICONHAND);
	}

	//モーション番号によりモーションの読み込み
	for (int nCnt = 0; nCnt < (int)MotionName; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

																									   //読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "MOTIONSET", 9) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "MOTIONSET", 9) != 0);
	}

	//LOOPするか?
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

		//読み飛ばし部分
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}

		if (strncmp((buf + nReal), "LOOP =", 6) == 0)
		{
			int nLoop = -2;
			sscanf((buf + nReal), "%*6c%d", &nLoop);                    //パーツ数を読み込む
			if (nLoop = 0) m_bLoop = false;
			else m_bLoop = true;
			break;
		}

	} while (strncmp((buf + nReal), "LOOP =", 6) != 0);

	//モーションのキー数
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

																								   //読み飛ばし部分
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}

		if (strncmp((buf + nReal), "NUM_KEY =", 9) == 0)
		{
			sscanf((buf + nReal), "%*9c%d", &m_nKeyNum);                    //パーツ数を読み込む
			break;
		}

	} while (strncmp((buf + nReal), "NUM_KEY =", 9) != 0);

	//モーションの現在のキーを読み込み
	for (int nCnt = 0; nCnt < m_nKeyNow+1; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

																									   //読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "KEYSET", 6) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "KEYSET", 6) != 0);
	}
	
	//現在キーのフレーム数を読み込み
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

		//読み飛ばし部分
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}

		if (strncmp((buf + nReal), "FRAME =", 7) == 0)
		{
			sscanf(buf + nReal, "%*7c%d", &m_nMotionFrameNum);
			break;
		}

	} while (strncmp((buf + nReal), "FRAME =", 7) != 0);

	//キーの対応パーツ読み込み**
	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		WORLD_MATRIX KeyBefore;
		KeyBefore.scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

			//読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "POS =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyBefore.pos.x, &KeyBefore.pos.y, &KeyBefore.pos.z);
				break;
			}

		} while (strncmp((buf + nReal), "POS =", 5) != 0);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

			//読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "ROT =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyBefore.rot.x, &KeyBefore.rot.y, &KeyBefore.rot.z);
				break;
			}

		} while (strncmp((buf + nReal), "ROT =", 5) != 0);

		m_partData[nCnt].SetKeyBefore(&KeyBefore);
	}

	fclose(fp);                                                                                                     //ファイルを閉じる

	int KeyAfterNum = (m_nKeyNow+1) % m_nKeyNum;

	//テキスト読み込み
	if ((fp = fopen(aMotionTextPass, "r")) == NULL)
	{
		MessageBox(NULL, "モーションテキストの読み込み失敗", "エラー", MB_OK | MB_ICONHAND);
	}

	//モーション番号によりモーションの読み込み
	for (int nCnt = 0; nCnt < (int)MotionName; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

																									   //読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "MOTIONSET", 9) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "MOTIONSET", 9) != 0);
	}

	//モーションの次のキーを読み込み
	for (int nCnt = 0; nCnt < KeyAfterNum + 1; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

			//読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "KEYSET", 6) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "KEYSET", 6) != 0);
	}

	//キーの対応パーツ読み込み
	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		WORLD_MATRIX KeyAfter;
		KeyAfter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

																									   //読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "POS =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyAfter.pos.x, &KeyAfter.pos.y, &KeyAfter.pos.z);
				break;
			}

		} while (strncmp((buf + nReal), "POS =", 5) != 0);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //行ごと読み込み

																									   //読み飛ばし部分
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //コメントアウトと改行を読み飛ばす
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //空白とタブを読み飛ばし
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "ROT =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyAfter.rot.x, &KeyAfter.rot.y, &KeyAfter.rot.z);
				break;
			}

		} while (strncmp((buf + nReal), "ROT =", 5) != 0);

		m_partData[nCnt].SetKeyAfter(&KeyAfter);
	}

	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		D3DXMATRIX ParentMtx;

		if(m_partData[nCnt].GetParent() == -1)
		{
			ParentMtx = WorldMtx;
		}

		else
		{
			int nNumparent = m_partData[nCnt].GetParentID();
			ParentMtx = m_partData[nNumparent].GetResultMtx();
		}
		m_partData[nCnt].Update(m_nMotionFrameNum,m_nMotionFrameNow,&ParentMtx);
	}

	m_nMotionFrameNow++;
	if(m_nMotionFrameNow >= m_nMotionFrameNum)
	{
		m_nMotionFrameNow = 0;
		m_nKeyNow = (m_nKeyNow + 1) % m_nKeyNum;
	}

	fclose(fp);
}

void CModel::Draw(void)
{
	for (int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		if (m_partData[nCnt].GetUse() == true)
		{
			m_partData[nCnt].Draw();
		}
	}
}

void CModel::Uninit(void)
{
	for(int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		m_partData[nCnt].Uninit();
	}
}

D3DXMATRIX CModel::GetMtxParent(int nNumPart)
{
	if (m_partData[nNumPart].GetUse() == true)
	{
		MessageBox(NULL, "このパーツは使われておりません", "エラー", MB_OK | MB_ICONHAND);
	}

	return m_partData[nNumPart].GetResultMtx();
}

bool CModel::GetLoop(void)
{
	return m_bLoop;
}