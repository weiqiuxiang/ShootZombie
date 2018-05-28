#include "CScene3D.h"
#include "hitRectangle .h"

//モデルパスリスト
#define MODELPASS_BUILD01 "data/GAMEOBJ/XFILE/building01.x"
#define MODELPASS_BUILD02 "data/GAMEOBJ/XFILE/building02.x"
#define MODELPASS_BRIDGE "data/GAMEOBJ/XFILE/bridge.x"
#define MODELPASS_WALL01 "data/GAMEOBJ/XFILE/stone.x"

#define BUILDING_LEFT_POSX (-100.0f)
#define BUILDING_RIGHT_POSX (100.0f)
#define BUILDING_POSY (-45.0f)

#define BUILDING01_POSZ (200.0f)

#define BRIDGE_POSY (-45)
#define BRIDGE_POSZ (700)

#define BUILDING02_POSZ (BUILDING01_POSZ + BRIDGE_POSZ)

CScene3D::CScene3D()
{
	
}

CScene3D::~CScene3D()
{
	
}

void CScene3D::Init(void)
{
	WORLD_MATRIX Setting;
	D3DXVECTOR2 HW;

	/*************************************BUILDING01_R***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, 0.0f);
	Setting.rot = D3DXVECTOR3(0.0f, 0.5*PI, 0.0f);
	Setting.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Obj3D[0].Init(&Setting, MODELPASS_BUILD01, 2, true);     //オブジェセット
	m_Obj3D[0].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX - 30.0f, BUILDING_POSY, 0.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX + 30.0f, BUILDING_POSY, 0.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(y)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, -30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(y)
	/****************************************************************************/

	/*************************************BUILDING01_L***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, 0.0f);
	Setting.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	m_Obj3D[1].Init(&Setting, MODELPASS_BUILD02, 1, true);     //オブジェセット
	m_Obj3D[1].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 30.0f, BUILDING_POSY - 10.0f, 0.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(120, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 30.0f, BUILDING_POSY - 10.0f, 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 15.0f, BUILDING_POSY, -60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(30, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 16.0f, BUILDING_POSY, -28.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI*0.65, 0.0f);
	HW = D3DXVECTOR2(80, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット
	/****************************************************************************/

	/*************************************BUILDING02_R***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(0.0f, 0.5*PI, 0.0f);
	Setting.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Obj3D[2].Init(&Setting, MODELPASS_BUILD01, 2, true);     //オブジェセット
	m_Obj3D[2].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX - 30.0f, BUILDING_POSY, BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX + 30.0f, BUILDING_POSY, BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING01_POSZ + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING01_POSZ - 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)
	/****************************************************************************/

	/*************************************BUILDING03_R***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(0.0f, 0.5*PI, 0.0f);
	Setting.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Obj3D[3].Init(&Setting, MODELPASS_BUILD01, 2, true);     //オブジェセット
	m_Obj3D[3].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX - 30.0f, BUILDING_POSY, BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX + 30.0f, BUILDING_POSY, BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING01_POSZ * 2 + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING01_POSZ * 2 - 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)
	/****************************************************************************/

	/*************************************BUILDING02_L***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	m_Obj3D[4].Init(&Setting, MODELPASS_BUILD02, 1, true);     //オブジェセット
	m_Obj3D[4].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 30.0f, BUILDING_POSY - 10.0f, BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(120, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 30.0f, BUILDING_POSY - 10.0f, BUILDING01_POSZ + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING01_POSZ + 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 15.0f, BUILDING_POSY, BUILDING01_POSZ - 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(30, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 16.0f, BUILDING_POSY, BUILDING01_POSZ - 28.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI*0.65, 0.0f);
	HW = D3DXVECTOR2(80, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット
	/****************************************************************************/

	/*************************************BUILDING03_L***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING01_POSZ*2);
	Setting.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	m_Obj3D[5].Init(&Setting, MODELPASS_BUILD02, 1, true);     //オブジェセット
	m_Obj3D[5].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 30.0f, BUILDING_POSY - 10.0f, BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(120, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 30.0f, BUILDING_POSY - 10.0f, BUILDING01_POSZ * 2 + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING01_POSZ * 2 + 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 15.0f, BUILDING_POSY, BUILDING01_POSZ * 2 - 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(30, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 16.0f, BUILDING_POSY, BUILDING01_POSZ * 2 - 28.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI*0.65, 0.0f);
	HW = D3DXVECTOR2(80, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット
	/****************************************************************************/

	/*************************************BUILDING04_R***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ);
	Setting.rot = D3DXVECTOR3(0.0f, 0.5*PI, 0.0f);
	Setting.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Obj3D[6].Init(&Setting, MODELPASS_BUILD01, 2, true);     //オブジェセット
	m_Obj3D[6].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX - 30.0f, BUILDING_POSY, BUILDING02_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX + 30.0f, BUILDING_POSY, BUILDING02_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(y)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ - 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(y)
	/****************************************************************************/

	/*************************************BUILDING04_L***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING02_POSZ);
	Setting.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	m_Obj3D[7].Init(&Setting, MODELPASS_BUILD02, 1, true);     //オブジェセット
	m_Obj3D[7].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 30.0f, BUILDING_POSY - 10.0f, BUILDING02_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(120, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 30.0f, BUILDING_POSY - 10.0f, BUILDING02_POSZ + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING02_POSZ + 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 15.0f, BUILDING_POSY, BUILDING02_POSZ - 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(30, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 16.0f, BUILDING_POSY, BUILDING02_POSZ - 28.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI*0.65, 0.0f);
	HW = D3DXVECTOR2(80, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット
	/****************************************************************************/

	/*************************************BUILDING05_R***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(0.0f, 0.5*PI, 0.0f);
	Setting.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Obj3D[8].Init(&Setting, MODELPASS_BUILD01, 2, true);     //オブジェセット
	m_Obj3D[8].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX - 30.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX + 30.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ - 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)
	/****************************************************************************/

	/*************************************BUILDING06_R***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(0.0f, 0.5*PI, 0.0f);
	Setting.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Obj3D[9].Init(&Setting, MODELPASS_BUILD01, 2, true);     //オブジェセット
	m_Obj3D[9].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX - 30.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX + 30.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(X)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ * 2 + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)

	Setting.pos = D3DXVECTOR3(BUILDING_RIGHT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ * 2 - 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット(z)
	/****************************************************************************/

	/*************************************BUILDING05_L***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	m_Obj3D[10].Init(&Setting, MODELPASS_BUILD02, 1, true);     //オブジェセット
	m_Obj3D[10].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 30.0f, BUILDING_POSY -10.0f, BUILDING02_POSZ + BUILDING01_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(120, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 30.0f, BUILDING_POSY - 10.0f, BUILDING02_POSZ + BUILDING01_POSZ +30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ + 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 15.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ -60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(30, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 16.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ -28.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI*0.65, 0.0f);
	HW = D3DXVECTOR2(80, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット
	/****************************************************************************/

	/*************************************BUILDING06_L***************************************/
	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ*2);
	Setting.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	m_Obj3D[11].Init(&Setting, MODELPASS_BUILD02, 1, true);     //オブジェセット
	m_Obj3D[11].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 30.0f, BUILDING_POSY - 10.0f, BUILDING02_POSZ + BUILDING01_POSZ * 2);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(120, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 30.0f, BUILDING_POSY - 10.0f, BUILDING02_POSZ + BUILDING01_POSZ * 2 + 30.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ * 2 + 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(60, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX - 15.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ * 2 - 60.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(30, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(BUILDING_LEFT_POSX + 16.0f, BUILDING_POSY, BUILDING02_POSZ + BUILDING01_POSZ*2 - 28.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI*0.65, 0.0f);
	HW = D3DXVECTOR2(80, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット
	/****************************************************************************/

	/*************************************BRIDGE***************************************/
	Setting.pos = D3DXVECTOR3(0.0f, BRIDGE_POSY, BRIDGE_POSZ);
	Setting.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Obj3D[12].Init(&Setting, MODELPASS_BRIDGE, 2, true);     //オブジェセット
	m_Obj3D[12].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(48.0f, BRIDGE_POSY, BRIDGE_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(275, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(42.0f, BRIDGE_POSY, BRIDGE_POSZ+137.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(13, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(36.0f, BRIDGE_POSY, BRIDGE_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(275, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(42.0f, BRIDGE_POSY, BRIDGE_POSZ - 137.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(13, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(-37.0f, BRIDGE_POSY, BRIDGE_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.5*PI, 0.0f);
	HW = D3DXVECTOR2(275, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(-42.0f, BRIDGE_POSY, BRIDGE_POSZ + 137.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(13, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(-46.0f, BRIDGE_POSY, BRIDGE_POSZ);
	Setting.rot = D3DXVECTOR3(PI*0.5, -0.5*PI, 0.0f);
	HW = D3DXVECTOR2(275, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(-40.0f, BRIDGE_POSY, BRIDGE_POSZ - 137.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(13, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(0.0f, BRIDGE_POSY+27.0f, BRIDGE_POSZ-109.0f);
	Setting.rot = D3DXVECTOR3(-PI*0.23, 0.0f, 0.0f);
	HW = D3DXVECTOR2(90, 40);
	SetHitRect(&Setting, &HW, MESH_SLOPE);    //当り判定セット

	Setting.pos = D3DXVECTOR3(0.0f, BRIDGE_POSY + 47.5f, BRIDGE_POSZ - 74.0f);
	Setting.rot = D3DXVECTOR3(-PI*0.13, 0.0f, 0.0f);
	HW = D3DXVECTOR2(90, 55);
	SetHitRect(&Setting, &HW, MESH_SLOPE);    //当り判定セット

	Setting.pos = D3DXVECTOR3(0.0f, BRIDGE_POSY + 60.0f, BRIDGE_POSZ - 24);
	Setting.rot = D3DXVECTOR3(-PI*0.03, 0.0f, 0.0f);
	HW = D3DXVECTOR2(90, 50);
	SetHitRect(&Setting, &HW, MESH_SLOPE);    //当り判定セット

	Setting.pos = D3DXVECTOR3(0.0f, BRIDGE_POSY + 60.0f, BRIDGE_POSZ + 24);
	Setting.rot = D3DXVECTOR3(PI*0.03, 0.0f, 0.0f);
	HW = D3DXVECTOR2(90, 50);
	SetHitRect(&Setting, &HW, MESH_SLOPE);    //当り判定セット

	Setting.pos = D3DXVECTOR3(0.0f, BRIDGE_POSY + 46.5f, BRIDGE_POSZ + 74.0f);
	Setting.rot = D3DXVECTOR3(PI*0.13, 0.0f, 0.0f);
	HW = D3DXVECTOR2(90, 55);
	SetHitRect(&Setting, &HW, MESH_SLOPE);    //当り判定セット

	Setting.pos = D3DXVECTOR3(0.0f, BRIDGE_POSY + 27.0f, BRIDGE_POSZ + 109.0f);
	Setting.rot = D3DXVECTOR3(PI*0.23, 0.0f, 0.0f);
	HW = D3DXVECTOR2(90, 40);
	SetHitRect(&Setting, &HW, MESH_SLOPE);    //当り判定セット

	//歩道橋回りの手すり
	Setting.pos = D3DXVECTOR3(187.0f, BRIDGE_POSY + 16.0f, BRIDGE_POSZ + 105.0f);
	Setting.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Obj3D[13].Init(&Setting, MODELPASS_WALL01, 1, true);     //オブジェセット
	m_Obj3D[13].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(-187.0f, BRIDGE_POSY + 16.0f, BRIDGE_POSZ + 105.0f);
	Setting.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Obj3D[14].Init(&Setting, MODELPASS_WALL01, 1, true);     //オブジェセット
	m_Obj3D[14].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(187.0f, BRIDGE_POSY + 16.0f, BRIDGE_POSZ - 105.0f);
	Setting.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Obj3D[15].Init(&Setting, MODELPASS_WALL01, 1, true);     //オブジェセット
	m_Obj3D[15].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(-187.0f, BRIDGE_POSY + 16.0f, BRIDGE_POSZ - 105.0f);
	Setting.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Obj3D[16].Init(&Setting, MODELPASS_WALL01, 1, true);     //オブジェセット
	m_Obj3D[16].SetMatrialUse(true);

	Setting.pos = D3DXVECTOR3(-150.0f, BRIDGE_POSY, BRIDGE_POSZ + 105.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(220, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(150.0f, BRIDGE_POSY, BRIDGE_POSZ + 105.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, 0.0f, 0.0f);
	HW = D3DXVECTOR2(220, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(-150.0f, BRIDGE_POSY, BRIDGE_POSZ - 105.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(220, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	Setting.pos = D3DXVECTOR3(150.0f, BRIDGE_POSY, BRIDGE_POSZ - 105.0f);
	Setting.rot = D3DXVECTOR3(PI*0.5, PI, 0.0f);
	HW = D3DXVECTOR2(220, 300);
	SetHitRect(&Setting, &HW, MESH_WALL);    //当り判定セット

	/****************************************************************************/


}

void CScene3D::Uninit(void)
{
	for(int nCnt = 0; nCnt < OBJ3D_NUM;nCnt++)
	{
		m_Obj3D[nCnt].Uninit();
	}
}

void CScene3D::Update(void)
{
	for (int nCnt = 0; nCnt < OBJ3D_NUM; nCnt++)
	{
		if (m_Obj3D[nCnt].GetUse() == true)
		{
			m_Obj3D[nCnt].Update();
		}
	}
}

void CScene3D::Draw(void)
{
	for (int nCnt = 0; nCnt < OBJ3D_NUM; nCnt++)
	{
		if (m_Obj3D[nCnt].GetUse() == true)
		{
			m_Obj3D[nCnt].Draw();
		}
	}
}