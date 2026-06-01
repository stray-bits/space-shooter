#include "Ship.h"

void Ship::setTexture(Texture& tex) { sprite.setTexture(tex); }

void Ship::reset(float x, float y) {
    sprite.setPosition(x, y);
    lives = 3;
}

void Ship::update(float dt, int windowWidth) {
    if (Keyboard::isKeyPressed(Keyboard::A) && sprite.getPosition().x > 0)
        sprite.move(-speed * dt, 0);
    if (Keyboard::isKeyPressed(Keyboard::D) && sprite.getPosition().x < windowWidth - sprite.getGlobalBounds().width)
        sprite.move(speed * dt, 0);
}

FloatRect Ship::bounds() const { return sprite.getGlobalBounds(); }

void Ship::draw(RenderWindow& window) { window.draw(sprite); }