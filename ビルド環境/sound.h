//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_sample000,	// BGM0
	SOUND_LABEL_BGM_sample001,	// BGM1
	SOUND_LABEL_BGM_sample002,	// BGM2
	SOUND_LABEL_SE_bomb000,		// ������
	SOUND_LABEL_SE_defend000,	// 
	SOUND_LABEL_SE_defend001,	// 
	SOUND_LABEL_SE_hit000,		// 
	SOUND_LABEL_SE_laser000,	// 
	SOUND_LABEL_SE_lockon000,	// 
	SOUND_LABEL_SE_shot000,		// 
	SOUND_LABEL_SE_shot001,		// 
	
	SOUND_LABEL_BGM_maou,		// BGM Maou
	SOUND_LABEL_BGM_SNOWPOI,	// BGM 
	SOUND_LABEL_BGM_STORY,		// BGM Story Background
	SOUND_LABEL_BGM_TITLE,		// BGM Title
	SOUND_LABEL_BGM_RESULT,		// BGM Result

	SOUND_LABEL_SE_PICK,		// SE ���� Sound 
	SOUND_LABEL_SE_CLICK01,		// SE Click 01 Sound
	SOUND_LABEL_SE_NPCDOWN,		// SE NPC Down
	SOUND_LABEL_SE_GAMEOVER,	// SE Game Over
	SOUND_LABEL_SE_LASER,		// Shot Sound

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

