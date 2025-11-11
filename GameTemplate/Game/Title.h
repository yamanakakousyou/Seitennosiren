#pragma once
class Game;
class Describe;
class Title : public IGameObject
{
public:
    Title();
    ~Title();
    void Update();
    //描画関数。
    void Render(RenderContext& rc);
    //スプライトレンダ―。
    SpriteRender spriteRender;
    Game* m_game;
    Describe* m_describe;
};