//=============================================================================
//
// obj���f������ [0bj.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "model.h"

#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "house.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX		(1)									// �e�N�X�`���̐�

#define	MODEL_HOUSE		"data/MODEL/HOUSE/Snow.obj"			// �ǂݍ��ރ��f����

#define MAX_HOUSE		(6)

#define HOUSE_OFFSET_Y		(1.0f)						// �G�l�~�[�̑��������킹��
#define HOUSE_OFFSET_Z		(300.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_���
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static HOUSE						g_House;				// �|���S���f�[�^
static int							g_TexNo;				// �e�N�X�`���ԍ�



//=============================================================================
// ����������
//=============================================================================
HRESULT InitHouse(void)
{

	LoadModel(MODEL_HOUSE, &g_House.model);


	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_House.pos = XMFLOAT3(0.0f, HOUSE_OFFSET_Y, HOUSE_OFFSET_Z);
	g_House.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_House.scl = XMFLOAT3(10.0f, 10.0f, 10.0f);

	g_TexNo = 0;
	g_House.load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHouse(void)
{

	// ���f���̉������
	if (g_House.load)
	{
		UnloadModel(&g_House.model);
		g_House.load = FALSE;

	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateHouse(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHouse(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;


	for (int i = 0; i < MAX_HOUSE / 2; i++)
	{
		//���̕`��
		//���[���h�}�g���b�N�X�̏�����y
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_House.rot.x, g_House.rot.y, g_House.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		 mtxTranslate = XMMatrixTranslation(g_House.pos.x - 300.0f + (i * 300.0f), g_House.pos.y, g_House.pos.z);
		 mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_House.mtxWorld, mtxWorld);

		//���f���`��
		DrawModel(&g_House.model);
	}



	for (int i = 0; i < MAX_HOUSE; i++)
	{
		//���̕`��
		//���[���h�}�g���b�N�X�̏�����y
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_House.rot.x, g_House.rot.y + 160.0f, g_House.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_House.pos.x - 400.0f + (i * 300.0f), g_House.pos.y, g_House.pos.z - 620.0f);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_House.mtxWorld, mtxWorld);

		//���f���`��
		DrawModel(&g_House.model);
	}



}