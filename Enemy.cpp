#include "Enemy.h"

Enemy::Enemy(){
    active = false;
    alive = true;
}

Enemy::~Enemy() {}

void Enemy::reset(float x, float y) {
    sprite.setPosition(x, y);
    active = true;
    alive = true;
}

void Enemy::update(float dt, int height) {
    if (!alive || !active) return;
    sprite.move(0, speed * dt);
    if (sprite.getPosition().y > height) active = false; // missed
}

void Enemy::onHit(int& score) {
    alive = false;
    active = false;
    score += 1;
}

FloatRect Enemy::bounds() const { return sprite.getGlobalBounds(); }

void Enemy::draw(RenderWindow& window) {
    if (alive && active) window.draw(sprite);
}

//boss enemy
BossEnemy::BossEnemy() { 
    speed = 80.f;
    hp = 20;
    active = false;
    alive = true;
}

void BossEnemy::onHit(int& score) {
    hp--;
    if (hp <= 0) {
        alive = false;
        active = false;
        score += 10;
    }
}

void BossEnemy::draw(RenderWindow& window) {
    if (!alive || !active) return;
    window.draw(sprite);

    RectangleShape bg({50.f, 6.f});
    RectangleShape bar({(hp / 20.f) * 50.f, 6.f});
    bg.setFillColor(Color::Red);
    bar.setFillColor(Color::Green);

    Vector2f pos = sprite.getPosition();
    bg.setPosition(pos.x, pos.y - 10);
    bar.setPosition(pos.x, pos.y - 10);

    window.draw(bg);
    window.draw(bar);
}

BossEnemy::~BossEnemy() {}