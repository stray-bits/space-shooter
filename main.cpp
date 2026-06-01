#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include "Ship.h"
#include "Bullet.h"
#include "Enemy.h"
#include "GameFunctions.h"

using namespace sf;
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    srand(static_cast<unsigned>(time(0)));
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Space Shooter");
    window.setFramerateLimit(60);

    Ship player;
    Bullet bullets[100];
    Enemy enemies[100];
    BossEnemy boss;

    int bulletCount = 0;
    int score = 0;
    int level = 1;
    int lives = 3;
    int highscore = 0;

    Texture spaceshipTex, enemyTex, bulletTex, redEnemyTex;
    loadTextures(spaceshipTex, enemyTex, bulletTex, redEnemyTex);
    Font font;
    if (!font.loadFromFile("assets/LilitaOne-Regular.ttf")) {
        cerr << "Error loading font!" << endl;
        return -1;
    }

    //player initialization
    player.setTexture(spaceshipTex);
    player.reset(WIDTH / 2.f - 25.f, HEIGHT - 100.f);

    //enemies initialization
    int totalEnemies = level * 10;
    int maxOnScreen = 1;
    spawnEnemies(enemies, totalEnemies, maxOnScreen, enemyTex, WIDTH);

    //game music
    sf::SoundBuffer shootBuffer, gameOverBuffer;
    sf::Sound shootSound, gameOverSound;
    sf::Music backgroundMusic;

    if (!shootBuffer.loadFromFile("shoot.wav")) {
        std::cerr << "Failed to load shoot sound\n";
    }
    shootSound.setBuffer(shootBuffer);
    shootSound.setVolume(70);

    if (!backgroundMusic.openFromFile("background.ogg")){
        cerr << "Failed to load background music\n";
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(40);
    backgroundMusic.play();

    if (!gameOverBuffer.loadFromFile("gameover.wav")){
        cerr << "Failed to load gameover sound\n";
    }
    gameOverSound.setBuffer(gameOverBuffer);
    gameOverSound.setVolume(70);

    Clock clock;
    bool moveLeft = false, moveRight = false;
    bool gameStarted = false, gamePaused = false, showQuit = false, levelComplete = false;

    readHighscore(highscore);
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed){ window.close();}
            if (showQuit) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Y) window.close();
                    else if (event.key.code == Keyboard::N) {
                        showQuit = false;
                        gamePaused = false;
                    }
                }
                continue;
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    gamePaused = true;
                    showQuit = true;
                }
                if (!gamePaused) {
                    if (!gameStarted && event.key.code == Keyboard::Space) {
                        gameStarted = true;
                        initializeGame(score, level, lives, player, WIDTH, HEIGHT, spaceshipTex);
                        totalEnemies = level * 10;
                        maxOnScreen = 1;
                        spawnEnemies(enemies, totalEnemies, maxOnScreen, enemyTex, WIDTH);
                    }
                    if (event.key.code == Keyboard::A){ moveLeft = true;}
                    if (event.key.code == Keyboard::D){ moveRight = true;}
                    if (event.key.code == Keyboard::Space){
                        fireBullet(player, bullets, bulletCount, bulletTex);
                        shootSound.play();
                    }
                    if (event.key.code == Keyboard::R) {
                        initializeGame(score, level, lives, player, WIDTH, HEIGHT, spaceshipTex);
                        totalEnemies = level * 10;
                        maxOnScreen = 1;
                        spawnEnemies(enemies, totalEnemies, maxOnScreen, enemyTex, WIDTH);
                        clearBullets(bullets, bulletCount);
                        gameStarted = true;
                    }
                }
            }
            if (event.type == Event::KeyReleased) {
                if (event.key.code == Keyboard::A) moveLeft = false;
                if (event.key.code == Keyboard::D) moveRight = false;
            }
        }

        if (gameStarted && !gamePaused && lives > 0) {
            player.update(dt, WIDTH);
            moveEnemies(enemies, totalEnemies, HEIGHT, level, dt);
            int activeCount = countActiveEnemies(enemies, totalEnemies);
            maxOnScreen = min(level, totalEnemies);
            for (int i = 0; i<totalEnemies && activeCount<maxOnScreen; i++) {
                if (enemies[i].alive && !enemies[i].active) {
                    enemies[i].reset(rand() % (WIDTH - 50), -50);
                    activeCount++;
                }
            }
            //bullet fire and collision check
            for (int i = 0; i < 100; i++) {
                if (bullets[i].active) updateBullet(bullets[i], HEIGHT, dt);
            }
            bool playerDead = checkCollisions(player, bullets, bulletCount, enemies, totalEnemies, boss, score, lives);

            //boss enemy
            if (level > 6 && allEnemiesDead(enemies, totalEnemies) && !boss.alive) {
                boss.reset(WIDTH / 2.f - 50.f, 50.f);
                boss.hp = 10 + level * 2;
                boss.sprite.setTexture(redEnemyTex);
            }

            if (playerDead){
                lives = 0;
                backgroundMusic.stop();
                gameOverSound.play();
            }

            if (!levelComplete && (allEnemiesDead(enemies, totalEnemies))) {
                levelComplete = true;
                level++;
                totalEnemies = level * 10;
                maxOnScreen = 1;
                spawnEnemies(enemies, totalEnemies, maxOnScreen, enemyTex, WIDTH);
                clearBullets(bullets, bulletCount);
            }
        }

        //display screens
        window.clear();
        if (!gameStarted) {
            displayHeading(window, font);
            displayInstructions(window, font);
        } else {
            if (lives > 0) {
                if (levelComplete) {
                    displayLevelComplete(window, font, level - 1);
                    //level++;
                    window.display();
                    sf::sleep(sf::seconds(1));
                    levelComplete = false;
                } else {
                    player.draw(window);
                    for (int i = 0; i < 100; i++) if (bullets[i].active) window.draw(bullets[i].sprite);
                    for (int i = 0; i < totalEnemies; i++) if (enemies[i].alive && enemies[i].active) enemies[i].draw(window);
                    if (level > 6 && boss.alive) boss.draw(window);
                    displayHUD(window, font, score, level, lives, highscore);
                }
            } else {
                displayGameOver(window, font, score);
            }
        }
        if (showQuit) displayQuitConfirmation(window, font);
        window.display();
    }

    saveHighscore(highscore);
    return 0;
}
