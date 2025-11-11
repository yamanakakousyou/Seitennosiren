#include "stdafx.h"
#include "Rule.h"
#include "Game.h"

Rule::Rule()
{
	m_spriteRender.Init("Assets/sprite/Rule.dds", 1920.0f, 1080.0f);
}

Rule::~Rule()
{
}

bool Rule::Start()
{
	return true;
}

void Rule::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Rule::Render(RenderContext&rc)
{
	m_spriteRender.Draw(rc);
}
