#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Enemy {
public:
    Sprite sprite;
    float speed = 60.f;
    bool alive = true;
    bool active = false;

    Enemy();
    virtual ~Enemy();
    virtual void reset(float x, float y);
    virtual void update(float dt, int height);
    virtual void onHit(int& score);
    FloatRect bounds() const;
    virtual void draw(RenderWindow& window);
};

class BossEnemy : public Enemy {
public:
    int hp = 20;
    BossEnemy();
    ~BossEnemy();

    void onHit(int& score) override;
    void draw(RenderWindow& window) override;
};