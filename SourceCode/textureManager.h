#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

#define TEXTUREMAX (30)

typedef struct 
{
	LPDIRECT3DTEXTURE9 TextureSlot;                  //テクスチャのバッファ
	char aFileName[200];                             //ファイル名
	int nTextureNum;                                 //テクスチャの番号
	bool bUse;                                       //テクスチャバッファは使用しているか
}TEXTUREMANAGER;

//プロトタイプ宣言
void InitTextureManager(void);                         //テクスチャマネジャー初期化
void UpdateTextureManager(void);                       //テクスチャマネジャー更新
TEXTUREMANAGER *GetTextureManager(void);               //テクスチャマネジャーを取得
HRESULT SetTexture(int *Num, char aFileName[200]);     //テクスチャをセットする関数

#endif