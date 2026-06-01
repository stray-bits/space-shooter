#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Ship {
public:
    Sprite sprite;
    float speed = 350.f;
    int lives = 3;

    void setTexture(Texture& tex);
    void reset(float x, float y);
    void update(float dt, int windowWidth);
    FloatRect bounds() const;
    void draw(RenderWindow& window);
};