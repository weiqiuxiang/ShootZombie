#pragma once
#ifndef _TITLEMA_H_
#define _TITLEMA_H_

#include "main.h"
#include "obj2d.h"

#define TITLEOBJ_NUM (20)

class CTitle
{
public:
	CTitle();
	~CTitle();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObj2D m_TitleObj[TITLEOBJ_NUM];
	int m_nFrame;
};

#endif
