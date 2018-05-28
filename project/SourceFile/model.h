#ifndef _MODEL_H_
#define _MODEL_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "CParticalSetting.h"

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define NUM_MODELVERTEX (4)                        //頂点数
#define NUM_MODELPOLYGON (2)                       //ポリゴン数

#define MODEL_POS_X (300.0f)
#define MODEL_POS_Y (100.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define MODEL_NAME "data/ja.x"

//-----------------------------------------------------------------------------------------
//構造体変数
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitModel(void);                                                     //ポリゴン(頂点)の初期化
void DrawModel(void);                                                        //ポリゴンの描画
void UpdateModel(void);                                                      //ポリゴンの更新
void UninitModel(void);                                                      //ポリゴンの終了

void ModelRotate(void);                             //モデルの回転
void ModelMove(void);                               //モデルの移動

CSpawn *GetSpawnPoint(void);

WORLD_MATRIX *Get_ModelMatrix(void);

#endif