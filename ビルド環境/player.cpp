//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "fade.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "debugproc.h"
#include "meshfield.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MODEL_PLAYER_BODY	"data/MODEL/P_Witch01/WITCH_BODY.obj"				// �ǂݍ��ރ��f����
#define MODEL_PLAYER_ARM_R	"data/MODEL/P_Witch01/WITCH_R_ARM.obj"			// �ǂݍ��ރ��f����
#define MODEL_PLAYER_ARM_L	"data/MODEL/P_Witch01/WITCH_L_ARM.obj"			// �ǂݍ��ރ��f����
#define MODEL_PLAYER_LEG_R	"data/MODEL/P_Witch01/WITCH_R_LEG.obj"			// �ǂݍ��ރ��f����
#define MODEL_PLAYER_LEG_L	"data/MODEL/P_Witch01/WITCH_L_LEG.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ��� ��
#define VALUE_MOVE_BOTH		(1.5f)							// �ړ��� ����
#define	VALUE_MOVE_DOWN		(0.5f)							// �ړ��� ��
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// ��]��

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(7.0f)							// �v���C���[�̑��������킹��

#define PLAYER_PARTS_MAX	(5)								// �v���C���[�̃p�[�c�̐�

enum TABLE_LIST
{
	// �ҋ@���
	WAIT_BODY,
	WAIT_R_ARM,
	WAIT_L_ARM,
	WAIT_R_LEG,
	WAIT_L_LEG,

	// �������
	fMOVE_BODY,
	fMOVE_R_ARM,
	fMOVE_L_ARM,
	fMOVE_R_LEG,
	fMOVE_L_LEG,

	// ������� ���E
	rMOVE_BODY,
	rMOVE_R_ARM,
	rMOVE_L_ARM,
	rMOVE_R_LEG,
	rMOVE_L_LEG,

	// ������� ����
	lMOVE_BODY,
	lMOVE_R_ARM,
	lMOVE_L_ARM,
	lMOVE_R_LEG,
	lMOVE_L_LEG,

	// �U�����
	ATTACK_BODY,
	ATTACK_R_ARM,
	ATTACK_L_ARM,
	ATTACK_R_LEG,
	ATTACK_L_LEG,

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

static float		roty = 0.0f;

static LIGHT		g_Light;





// �v���C���[�̊K�w�A�j���[�V�����f�[�^

static INTERPOLATION_DATA wait_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(5.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-6.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-6.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA wait_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};







static INTERPOLATION_DATA move_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI / 10, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 10, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(5.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-6.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-6.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};







static INTERPOLATION_DATA rmove_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f,-5.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.5f, -5.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-2.0f, 0.5f, -2.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-2.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -3.0f, -4.0f), XMFLOAT3(1.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -3.0f, -4.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA rmove_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -3.0f, 0.0f), XMFLOAT3(0.0f, -5.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -3.0f, 0.0f), XMFLOAT3(1.0f, -5.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};







static INTERPOLATION_DATA lmove_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f,4.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f,4.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.5f, 5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(4.0f,-0.5f, -5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(4.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 5.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(1.0f, 4.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 4.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA lmove_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -6.0f, 0.0f), XMFLOAT3(0.0f, 4.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -6.0f, 0.0f), XMFLOAT3(1.0f, 4.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
};





static INTERPOLATION_DATA attack_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_r_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, 3.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(5.0f, 3.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_l_arm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-6.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-6.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_r_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(3.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA attack_tbl_l_leg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-5.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{

	wait_tbl_body,
	wait_tbl_r_arm,
	wait_tbl_l_arm,
	wait_tbl_r_leg,
	wait_tbl_l_leg,

	move_tbl_body,
	move_tbl_r_arm,
	move_tbl_l_arm,
	move_tbl_r_leg,
	move_tbl_l_leg,

	rmove_tbl_body,
	rmove_tbl_r_arm,
	rmove_tbl_l_arm,
	rmove_tbl_r_leg,
	rmove_tbl_l_leg,

	lmove_tbl_body,
	lmove_tbl_r_arm,
	lmove_tbl_l_arm,
	lmove_tbl_r_leg,
	lmove_tbl_l_leg,

	attack_tbl_body,
	attack_tbl_r_arm,
	attack_tbl_l_arm,
	attack_tbl_r_leg,
	attack_tbl_l_leg,


};






//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	g_Player.load = TRUE;
	LoadModel(MODEL_PLAYER_BODY, &g_Player.model);

	g_Player.pos = XMFLOAT3(-50.0f, PLAYER_OFFSET_Y+50.0f, -250.0f);
	g_Player.rot = XMFLOAT3(0.0f, XM_PI , 0.0f);
	g_Player.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	
	g_Player.hp = PLAYER_LIFE;		// �v���C���[�̃��C�t HP

	g_Player.use = TRUE;			// TRUE:�����Ă�
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	g_Player.moving_f = FALSE;		// �����̊K�w�A�j���[�V�����p
	g_Player.attack = FALSE;		// �U���̊K�w�A�j���[�V�����p



	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	// �L�[�����������̃v���C���[�̌���
	roty = 0.0f;

	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������


	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �e�q�֌W
		g_Parts[i].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
		g_Parts[1].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
		g_Parts[2].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
		g_Parts[3].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
		g_Parts[4].parent = &g_Player;		// �� �����ɐe�̃A�h���X������

	//	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
	//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load = FALSE;
	}

	g_Parts[0].use = TRUE;
	g_Parts[0].parent = &g_Player;												// �e���Z�b�g
	g_Parts[0].tblNo = WAIT_BODY;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0].tblMax = sizeof(move_tbl_body) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[0].load = TRUE;
	LoadModel(MODEL_PLAYER_BODY, &g_Parts[0].model);

	g_Parts[1].use = TRUE;
	g_Parts[1].parent = &g_Player;												// �e���Z�b�g
	g_Parts[1].tblNo = WAIT_R_ARM;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[1].tblMax = sizeof(move_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[1].load = TRUE;
	LoadModel(MODEL_PLAYER_ARM_R, &g_Parts[1].model);

	g_Parts[2].use = TRUE;
	g_Parts[2].parent = &g_Player;												// �e���Z�b�g
	g_Parts[2].tblNo = WAIT_L_ARM;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[2].tblMax = sizeof(move_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[2].load = TRUE;
	LoadModel(MODEL_PLAYER_ARM_L, &g_Parts[2].model);

	g_Parts[3].use = TRUE;
	g_Parts[3].parent = &g_Player;												// �e���Z�b�g
	g_Parts[3].tblNo = WAIT_R_LEG;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[3].tblMax = sizeof(move_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[3].load = TRUE;
	LoadModel(MODEL_PLAYER_LEG_R, &g_Parts[3].model);

	g_Parts[4].use = TRUE;
	g_Parts[4].parent = &g_Player;												// �e���Z�b�g
	g_Parts[4].tblNo = WAIT_L_LEG;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[4].tblMax = sizeof(move_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[4].load = TRUE;
	LoadModel(MODEL_PLAYER_LEG_L, &g_Parts[4].model);


	// �N�H�[�^�j�I���̏�����
	XMStoreFloat4(&g_Player.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	if (g_Player.load == TRUE)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	// �p�[�c�̉������
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// �p�[�c�̉������
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	
	// �J�������[�N�i�v���C���[�̎��_�ɂ���j
	XMVECTOR vCameraAt = XMLoadFloat3(&cam->at);
	XMVECTOR vPlayerPos = XMLoadFloat3(&g_Player.pos);
	XMVECTOR vec = vPlayerPos - vCameraAt;
	float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]);

	// �v���C���[�̃X�s�[�h
	g_Player.spd *= 0.7f;

	// �v���C���[�̓������ǂ���
	g_Player.moving_f = FALSE;

	// �ړ�����
	if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, STICK_L_LEFT))
	{
		g_Player.spd = VALUE_MOVE_BOTH;
		//g_Player.pos.x -= g_Player.spd;
		roty = anglePlayer - XM_PIDIV2;

		g_Player.state = STATE_MOVE_LEFT;
		g_Player.moving_f = TRUE;
	}
	if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, STICK_L_RIGHT))
	{
		g_Player.spd = VALUE_MOVE_BOTH;
		roty = anglePlayer + XM_PIDIV2;

		g_Player.state = STATE_MOVE_RIGHT;
		g_Player.moving_f = TRUE;
	}
	if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, STICK_L_UP))
	{
		g_Player.spd = VALUE_MOVE;
		//g_Player.pos.z += g_Player.spd;
		roty = anglePlayer;

		g_Player.state = STATE_MOVE_FORWARD;
		g_Player.moving_f = TRUE;

	}
	if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, STICK_L_DOWN))
	{
		g_Player.spd = VALUE_MOVE_DOWN;
		roty = anglePlayer - XM_PI;

		g_Player.state = STATE_MOVE_FORWARD;
		g_Player.moving_f = TRUE;
	}


	if ((g_Player.moving_f == FALSE) && (g_Player.attack == FALSE))
	{
		g_Player.state = STATE_WAIT;
	}


	//////////////////////////////////////////////////////////////////////
	// �K�w�A�j���[�V�����p
	//////////////////////////////////////////////////////////////////////

	switch (g_Player.state)
	{
	case STATE_WAIT:
		SetWaitTbl();
		break;

	case STATE_MOVE_FORWARD:
		SetMoveTbl();
		break;

	case STATE_MOVE_RIGHT:
		SetMove_RTbl();
		break;

	case STATE_MOVE_LEFT:
		SetMove_LTbl();
		break;


	case STATE_ATTACK:
		SetAttackTbl();
		break;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.spd = 0.0f;
		roty = 0.0f;
	}


#endif


	{	// �����������Ƀv���C���[���ړ�������
		// �����������Ƀv���C���[���������Ă��鏊
		g_Player.rot.y = anglePlayer;

		g_Player.pos.x -= sinf(roty) * g_Player.spd;
		g_Player.pos.z -= cosf(roty) * g_Player.spd;
	}


	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 HitPosition;		// ��_
	XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
	BOOL ans = RayHitField(g_Player.pos, &HitPosition, &Normal);
	if (ans)
	{
		g_Player.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_Player.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}


	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_R))
	{
		SetBullet(g_Player.pos, g_Player.rot);

		g_Player.moving_f = FALSE;
		g_Player.state = STATE_ATTACK;
		g_Player.attack = TRUE;
		
	}

	// �v���C���[��HP���O�ɂȂ�����ABad Ending�ɑJ�ڂ���
	if (g_Player.hp == 0)
	{
		SetFade(FADE_OUT, MODE_BADEND);
	}

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);


	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// �g���Ă���Ȃ珈������
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	// ���`��Ԃ̏���
			int nowNo = (int)g_Parts[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Parts[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
			INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

			float nowTime = g_Parts[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
			Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

			// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

			// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

			// frame���g�Ď��Ԍo�ߏ���������
			g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
			if ((int)g_Parts[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				g_Parts[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
			}

		}

	}


	// �|�C���g���C�g�̃e�X�g
	{
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}



	//////////////////////////////////////////////////////////////////////
	// �p������
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Player.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	//quat = XMQuaternionIdentity();

	//quat = XMQuaternionRotationAxis(nvx, angle);
	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Player.Quaternion, quat);




#ifdef _DEBUG
	// �f�o�b�O�\��
	// PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z, Normal.y);
	// PrintDebugProc("Player RotY:%f\n", g_Player.rot.y);
	// PrintDebugProc("Player anglePlayer:%f\n", anglePlayer);
#endif

}


// �`�揈��
//=============================================================================
void DrawPlayer(void)
{

	if (g_Player.use == FALSE) return;

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// �����̐ݒ�
	SetFuchi(1);


	// ���f���`��
	// DrawModel(&g_Player.model);



	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
																			// ��
																			// g_Player.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);


		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}

	SetFuchi(0);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// �K�w�A�j���[�V�����p
//=============================================================================
void SetWaitTbl(void)
{
	g_Parts[0].tblNo = WAIT_BODY;
	g_Parts[0].tblMax = sizeof(wait_tbl_body) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[1].tblNo = WAIT_R_ARM;
	g_Parts[1].tblMax = sizeof(wait_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[2].tblNo = WAIT_L_ARM;
	g_Parts[2].tblMax = sizeof(wait_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[3].tblNo = WAIT_R_LEG;
	g_Parts[3].tblMax = sizeof(wait_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[4].tblNo = WAIT_L_LEG;
	g_Parts[4].tblMax = sizeof(wait_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


}


void SetMoveTbl(void)
{
	g_Parts[0].tblNo = fMOVE_BODY;
	g_Parts[0].tblMax = sizeof(move_tbl_body) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[1].tblNo = fMOVE_R_ARM;
	g_Parts[1].tblMax = sizeof(move_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[2].tblNo = fMOVE_L_ARM;
	g_Parts[2].tblMax = sizeof(move_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[3].tblNo = fMOVE_R_LEG;
	g_Parts[3].tblMax = sizeof(move_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[4].tblNo = fMOVE_L_LEG;
	g_Parts[4].tblMax = sizeof(move_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


}


void SetMove_RTbl(void)
{
	g_Parts[0].tblNo = rMOVE_BODY;
	g_Parts[0].tblMax = sizeof(rmove_tbl_body) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[1].tblNo = rMOVE_R_ARM;
	g_Parts[1].tblMax = sizeof(rmove_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[2].tblNo = rMOVE_L_ARM;
	g_Parts[2].tblMax = sizeof(rmove_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[3].tblNo = rMOVE_R_LEG;
	g_Parts[3].tblMax = sizeof(rmove_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[4].tblNo = rMOVE_L_LEG;
	g_Parts[4].tblMax = sizeof(rmove_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


}



void SetMove_LTbl(void)
{
	g_Parts[0].tblNo = lMOVE_BODY;
	g_Parts[0].tblMax = sizeof(lmove_tbl_body) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[1].tblNo = lMOVE_R_ARM;
	g_Parts[1].tblMax = sizeof(lmove_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[2].tblNo = lMOVE_L_ARM;
	g_Parts[2].tblMax = sizeof(lmove_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[3].tblNo = lMOVE_R_LEG;
	g_Parts[3].tblMax = sizeof(lmove_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[4].tblNo = lMOVE_L_LEG;
	g_Parts[4].tblMax = sizeof(lmove_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


}

void SetAttackTbl(void)
{
	g_Parts[0].tblNo = ATTACK_BODY;
	g_Parts[0].tblMax = sizeof(lmove_tbl_body) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[1].tblNo = ATTACK_R_ARM;
	g_Parts[1].tblMax = sizeof(lmove_tbl_r_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[2].tblNo = ATTACK_L_ARM;
	g_Parts[2].tblMax = sizeof(lmove_tbl_l_arm) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[3].tblNo = ATTACK_R_LEG;
	g_Parts[3].tblMax = sizeof(lmove_tbl_r_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Parts[4].tblNo = ATTACK_L_LEG;
	g_Parts[4].tblMax = sizeof(lmove_tbl_l_leg) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


}