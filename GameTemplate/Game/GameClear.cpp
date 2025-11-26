#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"
#include "Enemy.h"
#include "Boss.h"


GameClear::GameClear()
{

}

GameClear::~GameClear()
{
	DeleteGO(m_soundSource);
}

bool GameClear::Start()
{
	m_spriteRender.Init("Assets/sprite/GameClear.dds", 1920.0f, 1080.0f);
	//m_game = FindGO<Game>("game");

	//BGM.
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/Gameclear .wav");
	m_soundSource = NewGO<SoundSource>(0);
	//ResistWaveFileBankで指定した番号。
	m_soundSource->Init(3);
	//BGMは曲をループさせる。
	m_soundSource->Play(true);
	return true;
}

//更新処理
void GameClear::Update()
{
	m_spriteRender.Update();

	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//タイトルのオブジェクトをつくる
		NewGO<Title>(0, "title");
		//自身を削除する
		DeleteGO(this);
	}
}

//描画処理
void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}