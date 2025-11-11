#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Describe.h"

Title::Title()
{
	//‰æ‘œ‚ğ“Ç‚İ‚Ş
	spriteRender.Init("Assets/sprite/Title2.dds", 1920.0f, 1080.0f);
}

Title::~Title()
{

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
	//‰æ‘œ‚ğ•`‰æ‚·‚éB
	spriteRender.Draw(rc);
}