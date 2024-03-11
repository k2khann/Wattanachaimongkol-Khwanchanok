//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "npc.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
// #include "tree.h"
#include "bullet.h"
#include "score.h"
#include "particle.h"
#include "collision.h"
#include "ui.h"
#include "house.h"

#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();
	InitBoss();

	// NPC�̏���������
	InitNpc();

	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// �؂𐶂₷
	// InitTree();

	// �e�̏�����
	InitBullet();

	// �X�R�A�̏�����
	InitScore();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// UI�̏���������
	InitUi();

	// �C�O���[�̏�����
	InitHouse();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_SNOWPOI);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �X�R�A�̏I������
	UninitScore();

	// �e�̏I������
	UninitBullet();

	// �؂̏I������
	// UninitTree();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	// NPC�̏I������
	UninitNpc();

	// �G�l�~�[�̏I������
	UninitEnemy();
	UninitBoss();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

	// UI�̏I������
	UninitUi();

	// �C�O���[�̏I������
	UninitHouse();


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif

	if(g_bPause == FALSE)
		return;

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();
	UpdateBoss();

	// NPC�̍X�V����
	UpdateNpc();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	// �؂̍X�V����
	// UpdateTree();

	// �e�̍X�V����
	UpdateBullet();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle();

	// �e�̍X�V����
	UpdateShadow();

	// �����蔻�菈��
	CheckHit();

	// �X�R�A�̍X�V����
	UpdateScore();

	// UI�̍X�V����
	UpdateUi();

	// �C�O���[�̍X�V����
	UpdateHouse();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame0(void)
{
	// 3D�̕���`�悷�鏈��

	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// NPC�̕`�揈��
	DrawNpc();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();
	DrawBoss();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawBullet();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �؂̕`�揈��
	// DrawTree();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �C�O���[�̕`�揈��
	DrawHouse();




	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// �X�R�A�̕`�揈��
	DrawScore();

	// UI�̕`�揈��
	DrawUi();


	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// �v���C���[���_
	pos = GetPlayer()->pos;
	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	NPC* npc = GetNpc();			// NPC�̃|�C���^�[�̏�����
	ENEMY *enemy = GetEnemy();		// �G�l�~�[�̃|�C���^�[��������
	BOSS* boss = GetBoss();
	PLAYER *player = GetPlayer();	// �v���C���[�̃|�C���^�[��������
	BULLET *bullet = GetBullet();	// �e�̃|�C���^�[��������

	// �G�ƃv���C���[�L����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (enemy[i].use == FALSE)
			continue;

		//BC�̓����蔻��
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))
		{
			// �G�L�����N�^�[�͓|�����
			enemy[i].use = FALSE;
			ReleaseShadow(enemy[i].shadowIdx);

			// �X�R�A�����炷
			MinusScore(10);

			// �v���C���[�̃��C�t�����炷
			player[0].hp -= 1;

			
		}
	}






	// �v���C���[�̒e�ƓG
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (bullet[i].use == FALSE)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[i].pos, enemy[j].pos, bullet[i].fWidth, enemy[j].size))
			{
				// �����������疢�g�p�ɖ߂�
				bullet[i].use = FALSE;
				ReleaseShadow(bullet[i].shadowIdx);

				// �G�L�����N�^�[�͓|�����
				enemy[j].use = FALSE;
				ReleaseShadow(enemy[j].shadowIdx);

				// �X�R�A�𑫂�
				AddScore(10);


				// �v���C���[�̃��C�t�𑝂₷
				player[0].hp += 1;

				// �v���C���[�̃��C�t��2�ȏゾ������AMAX�̂܂܂ɂ���
				if (player[0].hp > PLAYER_LIFE)
				{
					player[0].hp = PLAYER_LIFE;
					
				}

			}
		}






		// �G�Ɠ������Ă邩���ׂ�
		for (int b = 0; b < MAX_BOSS; b++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (boss[b].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[i].pos, boss[b].pos, bullet[i].fWidth, boss[b].size))
			{
				// �����������疢�g�p�ɖ߂�
				bullet[i].use = FALSE;
				ReleaseShadow(bullet[i].shadowIdx);

				// �G�L�����N�^�[�͓|�����
				//boss[b].use = FALSE;
				//ReleaseShadow(boss[b].shadowIdx);

				// �X�R�A�𑫂�
				AddScore(20);

				boss[b].hp -= 1;

			}
		}









		// NPC�Ɠ������Ă邩���ׂ�
		for (int n = 0; n < MAX_NPC; n++)
		{

			//�G�̗L���t���O���`�F�b�N����
			if (npc[n].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[i].pos, npc[n].pos, bullet[i].fWidth, npc[n].size))
			{
				// �����������疢�g�p�ɖ߂�
				bullet[i].use = FALSE;
				ReleaseShadow(bullet[i].shadowIdx);

				// �G�L�����N�^�[�͓|�����
				npc[n].use = FALSE;
				ReleaseShadow(npc[n].shadowIdx);

				// �X�R�A�����炷
				// NPC�̏ꍇ�́A���C�t���炳�Ȃ�
				MinusScore(100);
				NpcDown(1);

				// �T�E�h
				PlaySound(SOUND_LABEL_SE_NPCDOWN);

			}
		}
	}












	


	// �G�l�~�[���S�����S�������ԑJ��
	int enemy_count = 0;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use == FALSE) continue;
		enemy_count++;
	}






	// NPC���S�����S�������ԑJ��
	int npc_count = 0;
	for (int n = 0; n < MAX_NPC; n++)
	{
		if (npc[n].use == FALSE) continue;
		npc_count++;
	}

	// NPC���O�l�ɂȂ�����ABad Ending�ɑJ��
	if (npc_count == 0)
	{
		SetFade(FADE_OUT, MODE_BADEND);
	}
}


