/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
メッセージBOX表示処理[camera.cpp]

Author :
Data   :
----------------------------------------------------------------
■Update : 2016/04/18
カメラの操作
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/////////////////////////////////インクルードヘッド//////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "textureManager.h"

/////////////////////////////////グロバール変数/////////////////////////////////
TEXTUREMANAGER g_TextureManager[TEXTUREMAX];

/////////////////////////////////テクスチャマネジャー初期化関数//////////////////////////////////
void InitTextureManager(void)
{
	//テクスチャマネジャーパラメータ初期化
	for (int nCnt = 0; nCnt < TEXTUREMAX; nCnt++)
	{
		g_TextureManager[nCnt].nTextureNum = nCnt;
		g_TextureManager[nCnt].TextureSlot = NULL;
		g_TextureManager[nCnt].aFileName[0] = '\0';
		g_TextureManager[nCnt].bUse = false;
	}
}

/////////////////////////////////テクスチャマネジャー更新関数//////////////////////////////////
void UpdateTextureManager(void) 
{
	
}

/////////////////////////////////テクスチャマネジャー取得//////////////////////////////////
TEXTUREMANAGER *GetTextureManager(void) 
{
	return &g_TextureManager[0];
}

/////////////////////////////////テクスチャをセットする関数//////////////////////////////////
HRESULT SetTexture(int *Num,char aFileName[200])
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();         //デバイス取得

	if (aFileName != NULL)
	{
		for (int nCnt = 0; nCnt < TEXTUREMAX; nCnt++)
		{
			//テクスチャのスロットが使われる
			if (g_TextureManager[nCnt].bUse == true)
			{
				//文字列比較
				if (strcmp(g_TextureManager[nCnt].aFileName, aFileName) == 0)
				{
					//テクスチャすでに読み込まれている
					*Num = g_TextureManager[nCnt].nTextureNum;            //テクスチャ番号発行
					break;
				}
			}

			//テクスチャのスロットが使われていない
			else
			{
				g_TextureManager[nCnt].bUse = true;                     //使用中表記
				g_TextureManager[nCnt].nTextureNum = nCnt;
				*Num = g_TextureManager[nCnt].nTextureNum;               //テクスチャ番号登録
				strcpy(g_TextureManager[nCnt].aFileName, aFileName);    //テクスチャパス登録
				//テクスチャ読み込む
				if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, aFileName, &g_TextureManager[nCnt].TextureSlot)))
				{
					MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
					return E_FAIL;
				}
				break;
			}
		}
	}

	else
	{
		return E_FAIL;
	}

	return S_OK;
}