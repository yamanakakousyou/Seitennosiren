#pragma once
class BackGround:public IGameObject
{
public:
    //BackGround(int stageID);
    BackGround();
    // ステージ番号を指定して生成
    ~BackGround();
    void Render(RenderContext& rc);
    void Update();

    int GetStageID() const { return m_stageID; }  // ← ステージ番号を取得できるように
private:
    ModelRender modelRender;
    PhysicsStaticObject physicsStaticObject;

    int m_stageID = 0;
};
