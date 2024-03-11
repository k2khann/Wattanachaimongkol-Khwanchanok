//=============================================================================
//
// ���U���g��ʏ��� [badend.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "badend.h"
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

#define TEXTURE_WIDTH_LOGO			(1521 / 2)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(554 / 2)			//

#define TEXTURE_WIDTH_CUR			(60)			// �I�����T�C�Y
#define TEXTURE_HEIGHT_CUR			(40)			// �I�����T�C�Y

#define SCROLL_MAX					(300)			// �X�N���[���}�b�N�X

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/ending02.png",
	"data/TEXTURE/ending_logo.png",
	"data/TEXTURE/choice04.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

// Choice
int								cur3;

// �X�N���[���A�j���[�V����
float							scrolldown;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBadEnd(void)
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
	PlaySound(SOUND_LABEL_SE_GAMEOVER);
	
	// �X�N���[���A�j���[�V����
	scrolldown = 0;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBadEnd(void)
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
void UpdateBadEnd(void)
{

	// Choice
	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, STICK_L_DOWN))
	{
		cur3 = (++cur3 + BADEND_TITLE_MAX) % BADEND_TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}
	else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, STICK_L_UP))
	{
		cur3 = (--cur3 + BADEND_TITLE_MAX) % BADEND_TITLE_MAX;
		PlaySound(SOUND_LABEL_SE_CLICK01);
	}


	switch (cur3)
	{
	case BADEND_RETRY:

		g_Pos = XMFLOAT3(350.0f, 350.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter��������A�X�e�[�W��؂�ւ���
				// SetMode(MODE_GAME);
			SetFade(FADE_OUT, MODE_GAME);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case BADEND_START:
		g_TexNo = 3;
		g_Pos = XMFLOAT3(350.0f, 400.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{		// Enter��������A�X�e�[�W��؂�ւ���
		
			SetFade(FADE_OUT, MODE_TITLE);
			PlaySound(SOUND_LABEL_SE_PICK);
		}
		break;

	case BADEND_EXIT:
		g_TexNo = 3;
		g_Pos = XMFLOAT3(350.0f, 450.0f, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonPressed(0, BUTTON_A))
		{
			GameLoop();
		}
		break;
	}




	scrolldown += 1;
	if (scrolldown >= SCROLL_MAX)
	{
		scrolldown = SCROLL_MAX;
	}


	

#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBadEnd(void)
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

	// �o�b�h�G���h�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �o�b�h�G���h�̃��S��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSprite(g_VertexBuffer, 470.0f, scrolldown, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// Choice
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x + 420.0f, g_Pos.y + 30.0f, TEXTURE_WIDTH_CUR, TEXTURE_HEIGHT_CUR, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}



}




