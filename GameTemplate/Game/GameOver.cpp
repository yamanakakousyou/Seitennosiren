#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Game.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	//�Q�[���I�[�o�[�̉摜��ǂݍ���
	m_spriteRender.Init("Assets/sprite/Gameover.dds", 1920.0f, 1080.0f);
	//m_game = FindGO<Game>("game");
	return true;
}

void GameOver::Update()
{
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//�^�C�g���̃I�u�W�F�N�g������
		NewGO<Title>(0, "title");
		////���g���폜����
		DeleteGO(this);
		DeleteGO(m_game);
	}
}

//�`�揈��
void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
