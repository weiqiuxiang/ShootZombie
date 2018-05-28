#pragma once
#ifndef _CSCENE3D_H_
#define _CSCENE3D_H_

//ヘッダファイル
#include "main.h"
#include "obj3d.h"

//マクロ
#define OBJ3D_NUM (20)

//先方宣言
class CObj3D;

class CScene3D
{
public:
	CScene3D();
	~CScene3D();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObj3D m_Obj3D[OBJ3D_NUM];
	int m_nFrame;                                //経過フレーム
};

#endif