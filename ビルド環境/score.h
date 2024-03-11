//=============================================================================
//
// �X�R�A���� [score.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_MAX			(99999)		// �X�R�A�̍ő�l
#define SCORE_MIN			(0)			// �X�R�A�̍ŏ��l
#define SCORE_DIGIT			(5)			// ����

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
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore(int add);
void MinusScore(int add);
int GetScore(void);

void NpcDown(int add);
