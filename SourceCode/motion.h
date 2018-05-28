#pragma once
#ifndef _MOTION_H_
#define _MOTION_H_

//ヘッダファイル
#include "main.h"

#define MATTEX_NUMMAX (10)
#define PART_MAX (20)

//クラス宣言
/*
	モデルのパーツクラス
*/

class CPart
{
public:
	CPart();
	~CPart();

	HRESULT Init( DWORD nNumMatModel);
	void SetInit(const WORLD_MATRIX *offsetInit, int nIndex, int nNumPart);                      //初期のオフセット位置と親の番号とパーツ番号の設定
	void SetKeyBefore(const WORLD_MATRIX *keyBefore);                                                  //前のキーフレーム
	void SetKeyAfter(const WORLD_MATRIX *keyAfter);                                                    //次のキーフレーム
	void SetModelPass(char *Modelpass);
	D3DXMATRIX GetResultMtx(void);
	bool GetUse(void);
	void SetUse(bool bUse);
	void SetParentID(int nID);
	int GetParentID(void);
	int GetIndex(void);
	int GetParent(void);
	void Update(int nMotionFrameNum,int nMotionFrameNow, const D3DXMATRIX *pParent);
	void Draw(void);
	void Uninit(void);
	HRESULT ModelChange(char *Modelpass, DWORD nNumMatModel);                                                              //モデルチェンジ
private:
	WORLD_MATRIX m_keyBefore;        //前のキーフレームのオフセット
	WORLD_MATRIX m_keyAfter;         //次のキーフレームのオフセット
	D3DXMATRIX m_MtxResult;          //MtxParent*m_OffsetVecの生成Mtx*m_MtxOffset(モデルマトリクス)
	D3DXMATRIX m_MtxResultOld;       //当り判定用Old座標
	WORLD_MATRIX m_OffsetInit;       //オフセットを生成するためのベクトル(Init時設定オフセット)
	char m_aModelpass[256];         //モデルパス
	int m_nIndex;                   //自分のパーツの番号(Init時読み込み)
	int m_nNumParent;               //親のパーツの番号(Init時読み込み)
	int m_nID;                       //親のID登録
	bool m_bUse;                    //使用しているフラグ(最初false設定,次にInit時設定)

	LPD3DXMESH m_pMesh;                               //メッシュインターフェースポインタ
	LPD3DXBUFFER m_pBuffMat;                          //マテリアル情報バッファ(D3DX汎用バッファインターフェース)
	LPDIRECT3DTEXTURE9 m_TexSlot[MATTEX_NUMMAX];                      //マテリアルテクスチャ番号を保存する配列
	DWORD m_NumMat;
};

/*
仕様:パーツを確認し,キャラに対応するモーションテキストを読み込み,
CHARACT構造体の「MotionName」を確認し,テキストからモーションネームを参照し相応のモーションデータを読み込み,
各パーツのオフセットデータを各パーツのOffsetVecに読み込み,MtxOffsetを作る
親子関係確認,親子関係によって親のMtxResultを自分のMtxParent代入,自分のMtxResult = MtxParent*MtxOffsetを計算し
(MtxResultOld - MtxResult)モーション差分を計算し,
「pD3DDevice->SetTransform(D3DTS_WORLD, &MtxResult);」を使って自分のワールドの位置を表示
*/
class CModel
{
public:
	typedef enum
	{
		MOTION_NULL = 0,
		MOTION_NORMAL,
		MOTION_SHOOT,
		MOTION_NORMALMOVE,
		MOTION_GUNMOVE,
		MOTION_RUN,
		MOTION_GNORMAL,
		MOTION_GSHOOT,
		MOITON_GNORMALMOVE,
		MOTION_GGUNMOVE,
		MOTION_GRUN,
		MOTION_GTHROW,
		MOTION_MAX,
	}CHAR_MODE;

	CModel();
	~CModel();
	HRESULT Init(char *filepass);
	void Update(void);                                       //親パーツからアップデートしないといけない
	void Draw(void);
	void Uninit(void);
	bool GetLoop(void);
	static D3DXMATRIX GetMtxParent(int nNumPart);                                           //取得する親マトリクスの番号
private:
	char aMotionTextPass[256];        //キャラに対応するモーションデータテキストの相対パス
	CHAR_MODE MotionName;             //モーション名
	int m_nMotionFrameNum;                 //モーションフレーム数(読み込み)
	int m_nMotionFrameNow;              //第何フレーム目を実行しているのか?(カウントアップ)
	int m_nKeyNum;                      //キーの数(読み込み)
	int m_nKeyNow;                      //今のキー(カウントアップ)
	bool m_bLoop;                       //ループフラグ[0:ループしない / 1:ループする]
	int m_nNumpart;                   //パーツの数

	static CPart m_partData[PART_MAX];
};

#endif