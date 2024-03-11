//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "score.h"
#include "npc.h"
#include "boss.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(16)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(32)	// 

#define NUM_POS_X					(930)	// ������POS
#define NUM_POS_Y					(505)

#define SCORE_TEXTURE_WIDTH			(529 / 4)	// Score�̃��S�T�C�Y
#define SCORE_TEXTURE_HEIGHT		(179 / 4)

#define SCORE_POS_X					(720)	// Score�̃��S��POS
#define SCORE_POS_Y					(480)

//////////////////////////////////////////////
#define NPC_MAX						(8)

#define NUM_NPC_POS_X				(95)	// ������ NPC DOWN
#define NUM_NPC_POS_Y				(510)

#define NPC_LOGO_WIDTH				(233 / 4)
#define NPC_LOGO_HEIGHT				(230 / 4)

#define NPC_LOGO_POS_X				(20)
#define NPC_LOGO_POS_Y				(470)

//////////////////////////////////////////////

#define NUM_BOSS_POS_X				(130)
#define NUM_BOSS_POS_Y				(440)

#define BOSS_LOGO_WIDTH				(261 / 4)
#define BOSS_LOGO_HEIGHT			(337 / 4)
		
#define BOSS_LOGO_POS_X				(20)
#define BOSS_LOGO_POS_Y				(370)

//////////////////////////////////////////////


#define TEXTURE_MAX					(5)		// �e�N�X�`���̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/number01.png",
	"data/TEXTURE/number02.png",
	"data/TEXTURE/Score02.png",

	"data/TEXTURE/NPC_Count01.png",
	"data/TEXTURE/BOSS_Count01.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static int						g_Score;					// �X�R�A
static int						g_Down;						// NPC���|�����l��
static int						g_BossHP;					// BOSS��HP

static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(void)
{

	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �v���C���[�̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { 500.0f, 20.0f, 0.0f };
	g_TexNo = 0;

	g_Score = 0;			// �X�R�A�̏�����
	g_Down = NPC_MAX;		// NPC�̓|������
	g_BossHP = BOSS_LIFE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	BOSS* boss = GetBoss();

#ifdef _DEBUG	// �f�o�b�O����\������
	// PrintDebugProc("BOSS HP%d\n", boss->hp);
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	BOSS* boss = GetBoss();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[NUMBER01]);

	// ��������������
	int number = g_Score;
	for (int i = 0; i < SCORE_DIGIT; i++)
	{
		// ����\�����錅�̐���
		float x = (float)(number % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = NUM_POS_X - g_w*i;	// �X�R�A�̕\���ʒuX
		float py = NUM_POS_Y;			// �X�R�A�̕\���ʒuY
		float pw = g_w;					// �X�R�A�̕\����
		float ph = g_h;					// �X�R�A�̕\������

		float tw = 1.0f / 10;			// �e�N�X�`���̕�
		float th = 1.0f / 1;			// �e�N�X�`���̍���
		float tx = x * tw;				// �e�N�X�`���̍���X���W
		float ty = 0.0f;				// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// ���̌���
		number /= 10;
	}



	// NPC�̐l��
	// ��������������
	int numberDown = g_Down;
	for (int i = 0; i < DOWN_DIGIT; i++)
	{
		// ����\�����錅�̐���
		float x = (float)(numberDown % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = NUM_NPC_POS_X - g_w * i;	// �X�R�A�̕\���ʒuX
		float py = NUM_NPC_POS_Y;			// �X�R�A�̕\���ʒuY
		float pw = g_w;						// �X�R�A�̕\����
		float ph = g_h;						// �X�R�A�̕\������

		float tw = 1.0f / 10;				// �e�N�X�`���̕�
		float th = 1.0f / 1;				// �e�N�X�`���̍���
		float tx = x * tw;					// �e�N�X�`���̍���X���W
		float ty = 0.0f;					// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// ���̌���
		numberDown /= 10;
	}


	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[NUMBER02]);

		// BOSS��HP
		// ��������������

		int numberBoss = boss->hp;
		// numberBoss = 159;
		for (int i = 0; i < BOSS_DIGIT; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(numberBoss % 10);

			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = NUM_BOSS_POS_X - g_w * i;		// �X�R�A�̕\���ʒuX
			float py = NUM_BOSS_POS_Y;					// �X�R�A�̕\���ʒuY
			float pw = g_w;								// �X�R�A�̕\����
			float ph = g_h;								// �X�R�A�̕\������

			float tw = 1.0f / 10;						// �e�N�X�`���̕�
			float th = 1.0f / 1;						// �e�N�X�`���̍���
			float tx = x * tw;							// �e�N�X�`���̍���X���W
			float ty = 0.0f;							// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			numberBoss /= 10;
		}

	}


	// Score�̃��S�̕`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[SCORE_LOGO]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, SCORE_POS_X, SCORE_POS_Y, SCORE_TEXTURE_WIDTH, SCORE_TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}





	// NPC�̃��S�̕`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[NPC_LOGO]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, NPC_LOGO_POS_X, NPC_LOGO_POS_Y, NPC_LOGO_WIDTH, NPC_LOGO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	

	// BOSS�̃��S�̕`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BOSS_LOGO]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, BOSS_LOGO_POS_X, BOSS_LOGO_POS_Y, BOSS_LOGO_WIDTH, BOSS_LOGO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}


//=============================================================================
// �X�R�A�����Z����
// ����:add :�ǉ�����_���B�}�C�i�X���\
//=============================================================================
void AddScore(int add)
{
	g_Score += add;
	if (g_Score > SCORE_MAX)
	{
		g_Score = SCORE_MAX;
	}

}

//=============================================================================
// �X�R�A�����Z����
// ����:add :�}�C�i�X�ɂ���
//=============================================================================
void MinusScore(int add)
{
	g_Score -= add;
	if (g_Score < SCORE_MIN)
	{
		g_Score = SCORE_MIN;
	}
}


int GetScore(void)
{
	return g_Score;
}


void NpcDown(int add)
{
	g_Down -= add;
}





