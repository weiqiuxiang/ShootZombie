#pragma once
#ifndef _CSCORE_H_
#define _CSCORE_H_

#include "main.h"
#include "CNumber.h"

#define DIGIT_NUM (8)

class CScore
{
public:
	CScore();
	~CScore();

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetScore(void);
	void AddScore(int nValue);
private:
	CNumber m_Number[DIGIT_NUM];
	D3DXVECTOR3 m_pos;
	int m_nScore;
};

#endif