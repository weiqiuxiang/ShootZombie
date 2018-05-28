#include "matrix.h"
#include "camera.h"

D3DXMATRIX g_ViewMtx;

//普通マトリクス
void SetMatrix(D3DXMATRIX *pWorldMatrix, WORLD_MATRIX ChangeMatrix)
{
	/*******************************************変数宣言***********************************************/
	//ワールド行列の作成
	D3DXMATRIX mtxScale/*拡大行列*/, mtxRot/*回転行列*/, mtxMove/*移動行列*/, mtxResult/*結果行列*/;
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();

	/*******************************************デバイス設定***********************************************/
	//ワールド行列の設定
	D3DXMatrixScaling(&mtxScale, ChangeMatrix.scl.x, ChangeMatrix.scl.y, ChangeMatrix.scl.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, ChangeMatrix.rot.y/*y軸*/, ChangeMatrix.rot.x/*x軸*/, ChangeMatrix.rot.z/*z軸*/); //回転行列を設定
	D3DXMatrixTranslation(&mtxMove, ChangeMatrix.pos.x, ChangeMatrix.pos.y, ChangeMatrix.pos.z);
	mtxResult = mtxScale*mtxRot*mtxMove;                     //結果の変換行列の計算
	D3DXMatrixIdentity(pWorldMatrix);                    //ワールド行列を単位行列で初期化
	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxResult);  //回転行列とワールド行列の掛け算
	pD3DDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);              //ワールド変換行列
}

void SetViewMatrix(const D3DXVECTOR3 *CameraPos, const D3DXVECTOR3 *LookatPos,const D3DXVECTOR3* VecUp)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();

	//ビュー行列の設定
	D3DXMatrixLookAtLH(&g_ViewMtx, CameraPos, LookatPos, VecUp);

	pD3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMtx);      //ビュー変換行列
}

void SetProjectionMatrix(void)
{
	D3DXMATRIX mtxProj;
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();

	D3DXMatrixPerspectiveFovLH(&mtxProj/*プロジェクション行列*/, D3DX_PI / 3.0f /*画角*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT/*アスペクト比*/, 1.0f/*near(near>0必須)*/, 8000.0f/*far*/);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);     //プロジェクション変換行列セット
}

void SetBillboardMatrix(D3DXMATRIX *pWorldMatrix, WORLD_MATRIX ChangeMatrix)
{
	/*******************************************変数宣言***********************************************/
	//ワールド行列の作成
	D3DXMATRIX mtxScale/*拡大行列*/, mtxRot/*回転行列*/, mtxMove/*移動行列*/, mtxResult/*結果行列*/;
	D3DXMATRIX mtxViewInverse;               //ビュー行列の逆行列

	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();        //デバイス取得

	/*******************************************デバイス設定***********************************************/
	//ワールド行列の設定
	D3DXMatrixIdentity(pWorldMatrix);                    //ワールド行列を単位行列で初期化
	D3DXMatrixScaling(&mtxScale, ChangeMatrix.scl.x, ChangeMatrix.scl.y, ChangeMatrix.scl.z);
	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxScale);  //スケール

	D3DXMatrixTranspose(&mtxViewInverse, &g_ViewMtx);                              //転置行列を生成

	mtxViewInverse._14 = 0.0f;                                                    //平行移動成分をカット
	mtxViewInverse._24 = 0.0f;													  //平行移動成分をカット
	mtxViewInverse._34 = 0.0f;													  //平行移動成分をカット

	mtxViewInverse._41 = 0.0f;                                                    //平行移動成分をカット
	mtxViewInverse._42 = 0.0f;													  //平行移動成分をカット
	mtxViewInverse._43 = 0.0f;													  //平行移動成分をカット

	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxViewInverse);                //逆行列*ワールド行列(回転)

	D3DXMatrixTranslation(&mtxMove, ChangeMatrix.pos.x, ChangeMatrix.pos.y, ChangeMatrix.pos.z);
	D3DXMatrixMultiply(pWorldMatrix, pWorldMatrix, &mtxMove);                                 //移動

	pD3DDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);                //ワールド変換行列
}

void SetChangeVertex(D3DXVECTOR3 *Vertex, const WORLD_MATRIX *ChangeMatrix)
{
	//ワールド行列の作成
	D3DXMATRIX mtxScale/*拡大行列*/, mtxRot/*回転行列*/, mtxMove/*移動行列*/, mtxResult/*結果行列*/;

	D3DXMatrixScaling(&mtxScale, ChangeMatrix->scl.x, ChangeMatrix->scl.y, ChangeMatrix->scl.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, ChangeMatrix->rot.y/*y軸*/, ChangeMatrix->rot.x/*x軸*/, ChangeMatrix->rot.z/*z軸*/); //回転行列を設定
	D3DXMatrixTranslation(&mtxMove, ChangeMatrix->pos.x, ChangeMatrix->pos.y, ChangeMatrix->pos.z);
	mtxResult = mtxScale*mtxRot*mtxMove;                     //結果の変換行列の計算

	D3DXVec3TransformCoord(Vertex, Vertex, &mtxResult);
}

void SetChangeNormarlize(D3DXVECTOR3 *Vertex, const WORLD_MATRIX *ChangeMatrix)
{
	//ワールド行列の作成
	D3DXMATRIX mtxScale/*拡大行列*/, mtxRot/*回転行列*/, mtxMove/*移動行列*/, mtxResult/*結果行列*/;

	D3DXMatrixScaling(&mtxScale, ChangeMatrix->scl.x, ChangeMatrix->scl.y, ChangeMatrix->scl.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, ChangeMatrix->rot.y/*y軸*/, ChangeMatrix->rot.x/*x軸*/, ChangeMatrix->rot.z/*z軸*/); //回転行列を設定
	mtxResult = mtxScale*mtxRot;                     //結果の変換行列の計算

	D3DXVec3TransformCoord(Vertex, Vertex, &mtxResult);
}