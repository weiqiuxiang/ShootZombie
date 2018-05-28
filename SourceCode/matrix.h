#pragma once
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "main.h"

void SetMatrix(D3DXMATRIX *pWorldMatrix, WORLD_MATRIX ChangeMatrix);                              //マトリクスセット
void SetViewMatrix(const D3DXVECTOR3 *CameraPos, const D3DXVECTOR3 *LookatPos, const D3DXVECTOR3* VecUp);
void SetProjectionMatrix(void);
void SetBillboardMatrix(D3DXMATRIX *pWorldMatrix, WORLD_MATRIX ChangeMatrix);
void SetChangeVertex(D3DXVECTOR3 *Vertex, const WORLD_MATRIX *ChangeMatrix);
void SetChangeNormarlize(D3DXVECTOR3 *Vertex, const WORLD_MATRIX *ChangeMatrix);

#endif // !_MATRIX_H_
