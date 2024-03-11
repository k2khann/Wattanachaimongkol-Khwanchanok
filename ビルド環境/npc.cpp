//=============================================================================
//
// �G�l�~�[���f������ [npc.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
#include "player.h"
#include "npc.h"
#include "bullet.h"
#include "shadow.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_NPC			"data/MODEL/P_NPC/P_NPC.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)								// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)						// ��]��

#define NPC_SHADOW_SIZE		(0.4f)								// �e�̑傫��
#define NPC_OFFSET_Y		(0.0f)								// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static NPC			g_Npc[MAX_NPC];

int g_Npc_load = 0;



static INTERPOLATION_DATA g_MoveTbl0[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-290.0f,	NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60},
	{ XMFLOAT3(-290.0f,	NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(-290.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(-290.0f,	NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl1[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120},
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(5.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 60.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(300.0f, NPC_OFFSET_Y, 290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl3[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 60},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl4[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120},
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(-90.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};


static INTERPOLATION_DATA g_MoveTbl5[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 3.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 240},
	{ XMFLOAT3(195.0f, NPC_OFFSET_Y, -290.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 180},
};

static INTERPOLATION_DATA g_MoveTbl6[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y,-20.0f), XMFLOAT3(0.0f,-2.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},
	{ XMFLOAT3(400.0f, NPC_OFFSET_Y, -20.0f), XMFLOAT3(0.0f,2.5f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},
};

static INTERPOLATION_DATA g_MoveTbl7[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(400.0f, NPC_OFFSET_Y, -20.0f), XMFLOAT3(0.0f,2.5f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},
	{ XMFLOAT3(-400.0f, NPC_OFFSET_Y,-20.0f), XMFLOAT3(0.0f,-2.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 600},

};



static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,
	g_MoveTbl3,
	g_MoveTbl4,
	g_MoveTbl5,
	g_MoveTbl6,
	g_MoveTbl7,

};



//=============================================================================
// ����������
//=============================================================================
HRESULT InitNpc(void)
{
	for (int i = 0; i < MAX_NPC; i++)
	{
		LoadModel(MODEL_NPC, &g_Npc[i].model);
		g_Npc[i].load = TRUE;

		g_Npc[i].pos = XMFLOAT3(0.0f + i * 30.0f, NPC_OFFSET_Y, 0.0f);
		g_Npc[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Npc[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Npc[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Npc[i].size = NPC_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Npc[i].model, &g_Npc[i].diffuse[0]);

		XMFLOAT3 pos = g_Npc[i].pos;
		pos.y -= (NPC_OFFSET_Y - 0.1f);
		g_Npc[i].shadowIdx = CreateShadow(pos, NPC_SHADOW_SIZE, NPC_SHADOW_SIZE);
		
		g_Npc[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Npc[i].tblNo = 0;				// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Npc[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_Npc[i].use = TRUE;			// TRUE:�����Ă�


	}

	// 0�Ԃ̐��`��Ԃœ�����
	g_Npc[0].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[0].tblNo = 0;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 1�Ԃ̐��`��Ԃœ�����
	g_Npc[1].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[1].tblNo = 1;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԃ̐��`��Ԃœ�����
	g_Npc[2].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[2].tblNo = 2;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 3�Ԃ̐��`��Ԃœ�����
	g_Npc[3].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[3].tblNo = 3;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[3].tblMax = sizeof(g_MoveTbl3) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 4�Ԃ̐��`��Ԃœ�����
	g_Npc[4].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[4].tblNo = 4;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[4].tblMax = sizeof(g_MoveTbl4) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 5�Ԃ̐��`��Ԃœ�����
	g_Npc[5].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[5].tblNo = 5;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[5].tblMax = sizeof(g_MoveTbl5) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 6�Ԃ̐��`��Ԃœ�����
	g_Npc[6].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[6].tblNo = 6;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[6].tblMax = sizeof(g_MoveTbl6) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 7�Ԃ̐��`��Ԃœ�����
	g_Npc[7].time = 0.0f;													// ���`��ԗp�̃^�C�}�[���N���A
	g_Npc[7].tblNo = 7;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Npc[7].tblMax = sizeof(g_MoveTbl7) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitNpc(void)
{

	for (int i = 0; i < MAX_NPC; i++)
	{
		if (g_Npc[i].load)
		{
			UnloadModel(&g_Npc[i].model);
			g_Npc[i].load = FALSE;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateNpc(void)
{
	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_NPC; i++)
	{
		if (g_Npc[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes

			// �ړ�����
			if (g_Npc[i].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Npc[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Npc[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Npc[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = g_Npc[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Npc[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Npc[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Npc[i].scl, nowScl + Scl);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Npc[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Npc[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Npc[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}

			}

			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Npc[i].pos;
			pos.y -= (NPC_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Npc[i].shadowIdx, pos);
		}
	}




#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// ���f���̐F��ύX�ł����I�������ɂ��ł����B
		for (int j = 0; j < g_Npc[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Npc[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		for (int j = 0; j < g_Npc[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Npc[0].model, j, g_Npc[0].diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawNpc(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_NPC; i++)
	{
		if (g_Npc[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Npc[i].scl.x, g_Npc[i].scl.y, g_Npc[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Npc[i].rot.x, g_Npc[i].rot.y + XM_PI, g_Npc[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Npc[i].pos.x, g_Npc[i].pos.y, g_Npc[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Npc[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Npc[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
NPC *GetNpc()
{
	return &g_Npc[0];
}

