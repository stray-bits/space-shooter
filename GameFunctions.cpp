#include "GameFunctions.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

// Fire bullets by reusing inactive bullets
void fireBullet(Ship& player, Bullet bullets[], int& bulletCount, Texture& bulletTex) {
    for (int i = 0; i < bulletCount; i++) {
        if (!bullets[i].active) {
            bullets[i].active = true;
            bullets[i].sprite.setTexture(bulletTex);
            bullets[i].x = player.sprite.getPosition().x + player.sprite.getGlobalBounds().width/2 - bullets[i].sprite.getGlobalBounds().width/2;
            bullets[i].y = player.sprite.getPosition().y;
            bullets[i].sprite.setPosition(bullets[i].x, bullets[i].y);
            return;
        }
    }
    if (bulletCount < MAX_BULLETS) {
        bullets[bulletCount].active = true;
        bullets[bulletCount].sprite.setTexture(bulletTex);
        bullets[bulletCount].x = player.sprite.getPosition().x + player.sprite.getGlobalBounds().width/2 - bullets[bulletCount].sprite.getGlobalBounds().width/2;
        bullets[bulletCount].y = player.sprite.getPosition().y;
        bullets[bulletCount].sprite.setPosition(bullets[bulletCount].x, bullets[bulletCount].y);
        bulletCount++;
    }
}

void updateBullet(Bullet& bullet, int height, float dt) {
    if (!bullet.active) return;
    float speed = 400.f;
    bullet.y -= speed * dt;
    bullet.sprite.setPosition(bullet.x, bullet.y);
    if (bullet.y < 0) bullet.active = false;
}

void spawnEnemies(Enemy enemies[], int total, int maxOnScreen, Texture& tex, int width) {
    for (int i = 0; i < total; i++) {
        enemies[i].sprite.setTexture(tex);
        enemies[i].alive = true;
        enemies[i].active = (i < maxOnScreen);
        float x = rand() % (width - 50);
        float y = enemies[i].active ? rand() % 200 : -100;
        enemies[i].sprite.setPosition(x, y);
    }
}

bool checkCollisions(Ship& player, Bullet bullets[], int bulletCount, Enemy enemies[], int numEnemies, BossEnemy& boss, int& score, int& lives) {
    for (int b = 0; b < bulletCount; b++) {
        if (!bullets[b].active) continue;
        for (int e = 0; e < numEnemies; e++) {
            if (!enemies[e].active || !enemies[e].alive) continue;
            if (bullets[b].sprite.getGlobalBounds().intersects(enemies[e].sprite.getGlobalBounds())) {
                bullets[b].active = false;
                enemies[e].onHit(score);
            }
        }
        if (boss.active && boss.alive && bullets[b].sprite.getGlobalBounds().intersects(boss.sprite.getGlobalBounds())) {
            bullets[b].active = false;
            boss.onHit(score);
        }
    }

    FloatRect playerBounds = player.sprite.getGlobalBounds();
    for (int e = 0; e < numEnemies; e++) {
        if (!enemies[e].active || !enemies[e].alive) continue;
        if (playerBounds.intersects(enemies[e].sprite.getGlobalBounds())) {
            enemies[e].alive = false;
            enemies[e].active = false;
            lives--;
        }
    }

    if (boss.active && boss.alive && playerBounds.intersects(boss.sprite.getGlobalBounds())) {
        lives--;
        boss.active = false;
        boss.alive = false;
    }

    return lives <= 0;
}

void saveHighscore(int highScore) {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore << endl;
        file.close();
    }
}

void displayQuitConfirmation(RenderWindow& window, Font& font) {
    Text text;
    text.setFont(font);
    text.setCharacterSize(36);
    text.setFillColor(Color::Green);
    text.setString("Do you want to quit?\nPress (Y) for Yes\nPress (N) for No");
    text.setPosition(100, 80);
    window.draw(text);
}

void displayGameOver(RenderWindow& window, Font& font, int score) {
    Text text;
    text.setFont(font);
    text.setCharacterSize(48);
    text.setFillColor(Color::Red);
    text.setString("GAME OVER\nYour score: " + std::to_string(score) + "\nPress 'Esc' to quit");
    text.setPosition(250, 200);
    window.draw(text);
}

void displayHUD(RenderWindow& window, Font& font, int score, int level, int lives, int highScore) {
    Text hud;
    hud.setFont(font);
    hud.setCharacterSize(24);
    hud.setFillColor(Color::White);
    hud.setString("Score: " + std::to_string(score) +
                  " | Level: " + std::to_string(level) +
                  " | Lives: " + std::to_string(lives) +
                  " | High Score: " + std::to_string(highScore));
    hud.setPosition(10, 10);
    window.draw(hud);
}

void displayLevelComplete(RenderWindow& window, Font& font, int level) {
    Text text;
    text.setFont(font);
    text.setCharacterSize(48);
    text.setFillColor(Color::Green);
    text.setString("Level " + std::to_string(level) + " Complete!");
    text.setPosition(200, 200);
    window.draw(text);
}

void displayInstructions(RenderWindow& window, Font& font) {
    Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(24);
    instructions.setFillColor(Color::White);
    instructions.setString("Use 'A' and 'D' to move left and right\n"
                           "Press 'Space' to fire\n"
                           "Different coloured enemies give different score\n"
                           "Press 'Esc' to quit");
    instructions.setPosition(200, 300);
    window.draw(instructions);
}

void displayHeading(RenderWindow& window, Font& font) {
    Text heading;
    heading.setFont(font);
    heading.setCharacterSize(72);
    heading.setFillColor(Color::Yellow);
    heading.setString("Space Shooter");
    heading.setPosition(250, 150);
    window.draw(heading);
}

void clearBullets(Bullet bullets[], int& bulletCount) {
    for (int i = 0; i < bulletCount; i++) {
        bullets[i].active = false;
    }
    bulletCount = 0;
}

void moveEnemies(Enemy enemies[], int numEnemies, int height, int level, float dt) {
    float speed = 50.f + level * 10.f;
    for (int i = 0; i < numEnemies; i++) {
        if (!enemies[i].alive || !enemies[i].active) continue;
        FloatRect bounds = enemies[i].sprite.getGlobalBounds();
        enemies[i].sprite.move(0, speed * dt);

        if (enemies[i].sprite.getPosition().y > height) {
            enemies[i].active = false;
            enemies[i].sprite.setPosition(bounds.left, -50); // reset off-screen
        }
    }
}

void loadTextures(Texture& spaceshipTex, Texture& enemyTex, Texture& bulletTex, Texture& redEnemyTex) {
    if (!spaceshipTex.loadFromFile("assets/spaceship.png") ||
        !enemyTex.loadFromFile("assets/enemy.png") ||
        !bulletTex.loadFromFile("assets/bullet.png") ||
        !redEnemyTex.loadFromFile("assets/enemy_red.png")) {
        cerr << "Error loading textures!" << std::endl;
        exit(1);
    }
}

void readHighscore(int& highscore) {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highscore;
        file.close();
    } else {
        highscore = 0;
    }
}

void initializeGame(int& score, int& level, int& lives, Ship& player, int width, int height, Texture& spaceshipTex) {
    score = 0;
    lives = 3;
    level = 1;
    player.setTexture(spaceshipTex);
    player.reset(width / 2.f - 25.f, height - 100.f);
}

int countActiveEnemies(Enemy enemies[], int numEnemies) {
    int count = 0;
    for (int i = 0; i < numEnemies; i++) {
        if (enemies[i].active && enemies[i].alive) count++;
    }
    return count;
}

bool allEnemiesDead(Enemy enemies[], int numEnemies) {
    for (int i = 0; i < numEnemies; i++) {
        if (enemies[i].alive) return false;
    }
    return true;
}