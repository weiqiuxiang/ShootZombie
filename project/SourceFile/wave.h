#pragma once

#include "main.h"
#define WAVE_NUM (10)

typedef struct
{
	int m_nCnt;
	bool m_Wave[WAVE_NUM];
	bool m_Clear[WAVE_NUM];
}WAVE;

void InitWave(void);

void UpdateWave(void);

bool ClearFlag(void);