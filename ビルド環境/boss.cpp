//=============================================================================
//
// �G�l�~�[���f������ [boss.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "boss.h"
#include "player.h"
#include "bullet.h"
#include "shadow.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BOSS			"data/MODEL/YetiGold/YetiGold.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define BOSS_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define BOSS_OFFSET_Y		(3.0f)						// �G�l�~�[�̑��������킹��

#define BOSS_SPAWN_RANGE_X	(800)
#define BOSS_SPAWN_RANGE_Z	(800)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BOSS			g_Boss[MAX_BOSS];				// �G�l�~�[

int g_Boss_load = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBoss(void)
{
	for (int i = 0; i < MAX_BOSS; i++)
	{
		LoadModel(MODEL_BOSS, &g_Boss[i].model);
		g_Boss[i].load = TRUE;

		g_Boss[i].pos = XMFLOAT3(-60.0f, BOSS_OFFSET_Y, 0.0f);
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(10.0f, 10.0f, 10.0f);

		g_Boss[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Boss[i].size = BOSS_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Boss[i].model, &g_Boss[i].diffuse[0]);

		XMFLOAT3 pos = g_Boss[i].pos;
		pos.y -= (BOSS_OFFSET_Y - 0.1f);
		g_Boss[i].shadowIdx = CreateShadow(pos, BOSS_SHADOW_SIZE, BOSS_SHADOW_SIZE);
		
		g_Boss[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Boss[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Boss[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_Boss[i].use = TRUE;			// TRUE:�����Ă�

		g_Boss[i].hp = BOSS_LIFE;

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].load)
		{
			UnloadModel(&g_Boss[i].model);
			g_Boss[i].load = FALSE;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{

	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes


			// Boss��HP���O�ɂȂ�����A�Q�[�����I���
			if (g_Boss[i].hp == 0)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}



			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Boss[i].pos;
			pos.y -= (BOSS_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Boss[i].shadowIdx, pos);
		}
	}






#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// ���f���̐F��ύX�ł����I�������ɂ��ł����B
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, g_Boss[0].diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Boss[i].scl.x, g_Boss[i].scl.y, g_Boss[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Boss[i].rot.x, g_Boss[i].rot.y + XM_PI, g_Boss[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Boss[i].pos.x, g_Boss[i].pos.y, g_Boss[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Boss[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Boss[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
BOSS *GetBoss()
{
	return &g_Boss[0];
}

