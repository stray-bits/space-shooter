#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace sf;

const int MAX_BULLETS = 200;
const int MAX_ENEMIES_ON_SCREEN = 5;

void initializeGame(int& score, int& level, int& lives, Ship& player, int width, int height, sf::Texture& spaceshipTex);
void fireBullet(Ship& player, Bullet bullets[], int& bulletCount, Texture& bulletTex);
void updateBullet(Bullet& bullet, int height, float dt);
void spawnEnemies(Enemy enemies[], int total, int maxOnScreen, Texture& tex, int width);
void moveEnemies(Enemy enemies[], int numEnemies, int height, int level, float dt);
int countActiveEnemies(Enemy enemies[], int numEnemies);
bool checkCollisions(Ship& player, Bullet bullets[], int bulletCount, Enemy enemies[], int numEnemies, BossEnemy& boss, int& score, int& lives);
void readHighscore(int& highScore);
void saveHighscore(int highScore);
bool allEnemiesDead(Enemy enemies[], int numEnemies);
void clearBullets(Bullet bullets[], int& bulletCount);

void displayHUD(RenderWindow& window, Font& font, int score, int level, int lives, int highScore);
void displayQuitConfirmation(RenderWindow& window, Font& font);
void displayGameOver(RenderWindow& window, Font& font, int score);
void displayLevelComplete(RenderWindow& window, Font& font, int level);
void displayInstructions(RenderWindow& window, Font& font);
void displayHeading(RenderWindow& window, Font& font);
void loadTextures(sf::Texture& spaceship, sf::Texture& enemy, sf::Texture& bullet, sf::Texture& redEnemy);






