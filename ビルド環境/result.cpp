//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(4)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(1392 / 2)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(475  / 2)			//

#define RESULT_LOGO_POS_X			(490)
#define RESULT_LOGO_POS_Y			(150)

#define TEXTURE_WIDTH_CUR			(40)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_CUR			(40)			// �I�����T�C�Y

#define RESULT_NUM_POS_X			(650)
#define RESULT_NUM_POS_Y			(340)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Result02.png",
	"data/TEXTURE/Result_Logo02.png",
	"data/TEXTURE/number03.png",
	"data/TEXTURE/chioce01.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

// Choice
int								cur2;

// �A�j���[�V����
int								animcnt;
BOOL							cntmax;
int								waitcnt;
int								digitcnt;

float							mawasu;

// Scroll
float							scrollwidth;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
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


	// �ϐ��̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_RESULT);

	// �A�j���[�V�����p
	animcnt = 1;
	waitcnt = 1;
	digitcnt = 0;

	// Scroll
	scrollwidth = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
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
void UpdateResult(void)
{

	// Choice
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, STICK_L_UP))
	{
		cur2 = (++cur2 + RESULT_TITLE_MAX) % RESULT_TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, STICK_L_DOWN))
	{
		cur2 = (--cur2 + RESULT_TITLE_MAX) % RESULT_TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}


	switch (cur2)
	{
	case RESULT_START:
		g_TexNo = 3;
		g_Pos = XMFLOAT3(350.0f, 400.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter��������A�X�e�[�W��؂�ւ���
				// SetMode(MODE_GAME);
			SetFade(FADE_OUT, MODE_TITLE);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case RESULT_EXIT:
		g_TexNo = 3;
		g_Pos = XMFLOAT3(350.0f, 450.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{
			GameLoop();
		}
		break;
	}



	// �A�j���[�V�����p
	if (waitcnt % 20 == 0);
	{
		animcnt += 1;

	}
	if (animcnt % 20 == 0)
	{
		digitcnt += 1;
	}

	waitcnt += 1;


	// Scroll
	scrollwidth += 3;
	if (scrollwidth >= TEXTURE_WIDTH_LOGO)
	{
		scrollwidth = TEXTURE_WIDTH_LOGO;
	}
	

#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
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

	// ���U���g�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// ���U���g�̃��S��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, scrollwidth, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



	// �X�R�A�\��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// ��������������
		int number = GetScore();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{

			float x;
			if (digitcnt > i)
			{
				x = (float)(number % 10);			// ����\�����錅�̐���
			}
			else
			{
				x = (float)(animcnt % 10);
			}


			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float pw = 16 * 4;									// �X�R�A�̕\����
			float ph = 32 * 4;									// �X�R�A�̕\������
			float px = RESULT_NUM_POS_X - i * pw - i * 10;		// �X�R�A�̕\���ʒuX
			float py = RESULT_NUM_POS_Y;						// �X�R�A�̕\���ʒuY

			float tw = 1.0f / 10;								// �e�N�X�`���̕�
			float th = 1.0f / 1;								// �e�N�X�`���̍���
			float tx = x * tw;									// �e�N�X�`���̍���X���W
			float ty = 0.0f;									// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			number /= 10;
		}

	}



	//Choice
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x + 440.0f, g_Pos.y + 30.0f, TEXTURE_WIDTH_CUR, TEXTURE_HEIGHT_CUR, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



}




