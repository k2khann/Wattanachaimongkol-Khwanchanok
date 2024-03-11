//=============================================================================
//
// �^�C�g����ʏ��� [story.h]
// Author : ���b�^�i�`���C�����R���@�J���`���m
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct STORY
{
	XMFLOAT3	pos;		// �|���S���̍��W
	XMFLOAT3	rot;				// ���f���̌���(��])
	XMFLOAT3	scl;				// ���f���̑傫��(�X�P�[��)
	float		w, h;		// ���ƍ���
	int			texNo;		// �g�p���Ă���e�N�X�`���ԍ�


	// ���`��ԗp

	XMFLOAT3	Direction;	// ���C�g�̕���
	XMFLOAT4	Diffuse;	// �g�U���̐F


	float		time;			// ���`��ԗp
	int			tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;			// ���̃e�[�u���̃f�[�^��

};

struct STORY_LOGO
{
	XMFLOAT3	pos;		// �|���S���̍��W
	BOOL		use;		//
	float		w, h;		// ���ƍ���
	int			texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	float	countAnim;
	int		patternAnim;
};

 // �X�g�[���[�̃y�[�W
enum
{
	STORY_PAGE01,
	STORY_PAGE02,
	STORY_PAGE03,

	STORY_MAX
};

enum
{
	SUBTITLE01,
	SUBTITLE02,
	SUBTITLE03,

	SUBTITLE_MAX,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStory(void);
void UninitStory(void);
void UpdateStory(void);
void DrawStory(void);



