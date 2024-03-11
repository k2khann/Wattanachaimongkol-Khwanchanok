//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�

#define	PLAYER_SIZE		(5.0f)				// �����蔻��̑傫��

#define PLAYER_LIFE		(4)					// �v���C���[�̃��C�t HP


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	float				spd;				// �ړ��X�s�[�h

	int					hp;					// �v���C���[�̃��C�t HP
	
	BOOL				load;
	DX11_MODEL			model;				// ���f�����


	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	BOOL				use;

	float				size;

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��


	//�K�w�A�j���[�V�����p�̓������
	int					state;
	BOOL				moving_f;
	BOOL				attack;

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	PLAYER				*parent;			// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	// �N�H�[�^�j�I��
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// �����������Ă��鏊

};

enum
{
	STATE_WAIT,

	STATE_MOVE_FORWARD,
	// STATE_MOVE_BACK,
	STATE_MOVE_RIGHT,
	STATE_MOVE_LEFT,

	STATE_ATTACK,

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

// �K�w�A�j���[�V�����p
void SetWaitTbl(void);
void SetMoveTbl(void);
void SetMove_RTbl(void);
void SetMove_LTbl(void);
void SetAttackTbl(void);
