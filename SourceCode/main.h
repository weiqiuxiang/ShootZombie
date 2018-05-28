#ifndef _MAIN_H_
#define _MAIN_H_

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")             //システム時刻取得に必要
#pragma comment (lib,"dinput8.lib")

//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include <dinput.h>
#include <XAudio2.h>

//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define SCREEN_WIDTH (800)                          //ウィンドウの幅
#define SCREEN_HEIGHT (600)                         //ウィンドウの高さ
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)                     //2D頂点フォーマット
#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_NORMAL)          //3D頂点フォーマット
#define PI (3.14159265359f)
#define GRAVITY (0.03f)

//セーフリリースマクロ
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( memory) { if(memory != NULL) { memory -> Release(); memory = NULL; } }  //メモリのクリア
#endif

//前方宣言
class CScore;

//-----------------------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------------------
//頂点構造体(↑のファーマットの型に合わせること)
typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX,
}GAMEMODE;

typedef struct
{
	D3DXVECTOR3 pos;                         //
	float rhw;								 //座標変換済み頂点フラグ(必ず1.0f)
	D3DCOLOR color;						     //頂点カラー
	D3DXVECTOR2 tex;                         //テクスチャ座標(UV or ST)
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;                         //
	D3DXVECTOR3 normal;                      //ベクトルの方向
	D3DCOLOR color;							 //
	D3DXVECTOR2 tex;                         //テクスチャ座標(UV or ST)
}VERTEX_3D;

//マトリクス配列
typedef struct
{
	D3DXVECTOR3 pos;   //モデル移動(座標)
	D3DXVECTOR3 rot;   //モデル回転
	D3DXVECTOR3 scl;   //モデル拡大率
}WORLD_MATRIX;

typedef struct
{
	D3DXVECTOR3 p0;
	D3DXVECTOR3 p1;
	D3DXVECTOR3 p2;
	D3DXVECTOR3 p3;
}RECT_3D;

typedef struct
{
	D3DXVECTOR2 p[4];
}RECT_2D;

typedef struct
{
	D3DXVECTOR3 p0;
	D3DXVECTOR3 p1;
	D3DXVECTOR3 p2;
}TRIANGLE_3D;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void Uninit(void);                                                             //終了(後片付け関数)
LPDIRECT3DDEVICE9 Get_Device(void);
bool *GetCameraSwitch(void);
void SetMode(GAMEMODE mode);
D3DXVECTOR3 GetMousePosInWindow(void);

CScore *GetScore(void);
#endif