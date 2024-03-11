//=============================================================================
//
// スコア処理 [score.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX			(99999)		// スコアの最大値
#define SCORE_MIN			(0)			// スコアの最小値
#define SCORE_DIGIT			(5)			// 桁数

#define DOWN_DIGIT			(1)

#define BOSS_DIGIT			(3)

enum
{
	NUMBER01,
	NUMBER02,
	SCORE_LOGO,
	NPC_LOGO,
	BOSS_LOGO,

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore(int add);
void MinusScore(int add);
int GetScore(void);

void NpcDown(int add);
