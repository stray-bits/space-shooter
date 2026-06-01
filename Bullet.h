#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

struct Bullet {
    float x, y;
    bool active = false;
    Sprite sprite;
};
