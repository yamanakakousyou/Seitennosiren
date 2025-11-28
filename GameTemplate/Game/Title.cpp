#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Describe.h"

Title::Title()
{
	//画像を読み込む
	m_spriteRender.Init("Assets/sprite/Title2.dds", 1920.0f, 1080.0f);
}

bool Title::Start()
{
	//BGM.
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/Title.wav");
	m_soundSource = NewGO<SoundSource>(0);
	//ResistWaveFileBankで指定した番号。
	m_soundSource->Init(0);
	//BGMは曲をループさせる。
	m_soundSource->Play(true);
	return true;
}

Title::~Title()
{
	DeleteGO(m_soundSource);
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Describe>(0, "describe");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	//画像を描画する。
	m_spriteRender.Draw(rc);
}