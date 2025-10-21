#pragma once
class BackGround:public IGameObject
{
public:
    //BackGround(int stageID);
    BackGround();
    // �X�e�[�W�ԍ����w�肵�Đ���
    ~BackGround();
    void Render(RenderContext& rc);
    void Update();

    int GetStageID() const { return m_stageID; }  // �� �X�e�[�W�ԍ����擾�ł���悤��
private:
    ModelRender modelRender;
    PhysicsStaticObject physicsStaticObject;

    int m_stageID = 0;
};
