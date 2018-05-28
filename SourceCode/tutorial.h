#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "obj2d.h"

#define TUTORIALOBJ_NUM (20)

class CTutorial
{
public:
	CTutorial();
	~CTutorial();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObj2D m_TutorialObj[TUTORIALOBJ_NUM];
	int m_nDrawNum;
};

#endif