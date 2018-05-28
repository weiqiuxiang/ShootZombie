#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "obj2d.h"

#define RESULTOBJ_NUM (20)

class CResult
{
public:
	CResult();
	~CResult();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObj2D m_ResultObj[RESULTOBJ_NUM];
};

#endif