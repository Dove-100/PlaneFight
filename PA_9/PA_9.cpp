#include <SFML/Graphics.hpp>
#include <SFML/Window/Window.hpp>
#include "menu.hpp"
#include "player.hpp"
#include "background.hpp"
#include "Book.hpp"
#include "Windows.h"
#include "MiscFunctions.hpp"
#include <vector>
#include <string>
#include "pencil.hpp"
#include "GameOverScreen.hpp"
#include "testCases.hpp"
//0989
// �����Ƿ����в��Եĺ�
#define RUN_TESTS true

int main()
{
    // ��ʼ�����������
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // ��ʼ�����ڲ���ȡ���ڳߴ�
    sf::RenderWindow window = initWindow();
    sf::Vector2u windowSize = window.getSize();

    // ���ز˵���������
    sf::Music menuMusic;
    menuMusic.openFromFile("danceWeapon4.mp3");  // ���������ļ�
    menuMusic.setVolume(10);  // ��������
    menuMusic.play();  // ��������
    menuMusic.setLooping(true);  // ����ѭ������

    // ������Ϸ��������
    sf::Music gameMusic;
    gameMusic.openFromFile("track1.mp3");
    gameMusic.setVolume(15);

    // ������ҽ�ɫ
    Player player = loadPlayer(window);

    // ���ر���
    Background background = loadBackground();

    // ��ʼ����Ϸ����
    int score = 0;

    // ���ؼ�����Ч
    sf::Music laserSound;
    laserSound.openFromFile("hit_sound.mp3");

    // ������ײ��Ч
    sf::Music hitSound;
    hitSound.openFromFile("pencil_hit.mp3");

    // �����鱾����
    sf::Texture bookTextures[3];
    bookTextures[0].loadFromFile("PNG/googly-a.png");
    bookTextures[1].loadFromFile("PNG/googly-b.png");
    bookTextures[2].loadFromFile("PNG/googly-c.png");

    // �����洢�鱾��Ǧ�ʵ�����
    std::vector<Book> balls;
    std::vector<Pencil> pencils;

    // ������һ���鱾����
    Book firstBook(bookTextures[std::rand() % 3]);  // ���ѡ������
    float scale = 0.1f + ((float)(std::rand() % 5 + 1) / 60); // ������ű���(0.1��0.15֮��)
    firstBook.setScale({ scale, scale });
    balls.push_back(firstBook);

    // ���ص�һ��Ǧ�ʶ���
    sf::Texture pencilTex;
    pencilTex.loadFromFile("red_star.png");
    Pencil pencil(pencilTex, player.getPosition());
    pencils.push_back(pencil);

    // ��ʼ����ʱ�����ڿ������ɼ��
    sf::Clock ball_spawn_clock;  // �鱾���ɼ�ʱ��
    sf::Clock pencil_spawn_clock;  // Ǧ�����ɼ�ʱ��
    sf::Clock hitRestrictor;  // �������Ƽ�ʱ��

    // ��������
    sf::Font font;
    font.openFromFile("PressStart2P-Regular.ttf");
    // �����ı�����
    sf::Text scoreLabel(font);  // ������ʾ
    sf::Text healthLabel(font);  // ����ֵ��ʾ
    sf::Text highScoreLable(font);  // ��߷���ʾ

    // �����ı���ʽ
    loadTexts(window, font, scoreLabel, healthLabel, highScoreLable);

    // ���ó�ʼ��������
    float bookSpawnRate = 4;  // �鱾��������(��)
    float pencilSpawnRate = 50.f;  // Ǧ����������(����)

    // ��ʾ��ʼ�˵�
    Menu menu(background.getTexture(), player.getTexture(), font, windowSize);
    menu.startMenu(window);

    // ֹͣ�˵����֣���ʼ��Ϸ����
    menuMusic.stop();
    gameMusic.play();
    gameMusic.setLooping(true);

    // ������ò��ԣ��������в�������
    if (RUN_TESTS)
    {
        TestCases test;
        test.testAll();
    }

    // ����Ϸѭ��
    while (window.isOpen())
    {
        // �¼�����ѭ��
        while (const std::optional event = window.pollEvent())
        {
            // ��ESC���رմ���
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
                window.close();
        }

        // �鱾�����߼�
        if (ball_spawn_clock.getElapsedTime().asSeconds() >= bookSpawnRate)
        {
            // �������鱾
            Book newBall(bookTextures[std::rand() % 3]);
            float scale = 0.1f + ((float)(std::rand() % 5 + 1) / 60);
            newBall.setScale({ scale, scale });
            balls.push_back(newBall);
            ball_spawn_clock.restart();
            // �������Ѷ�(�������ɼ��)
            if (bookSpawnRate > 1) bookSpawnRate -= 0.1f;
            std::cout << "Number of balls: " << balls.size() << std::endl;
        }

        // Ǧ�������߼�
        if (pencils[pencils.size() - 1].getIsShooting() == true &&
            pencil_spawn_clock.getElapsedTime().asMilliseconds() >= pencilSpawnRate)
        {
            Pencil newPencil(pencilTex, player.getPosition());
            pencils.push_back(newPencil);
            pencil_spawn_clock.restart();
        }

        // ����������Ϸ���� //

        // ���������鱾
        for (auto& ball : balls)
        {
            ball.update(window.getSize());
            if (ball.getHealth() <= 0)  // ����鱾����ֵ�ľ�
            {
                score += ball.getOriginalHealth() * 8;  // ���ӷ���
                ball.setHealth(0);
                // ���������Ƴ����ݻٵ��鱾
                balls.erase(std::remove_if(balls.begin(), balls.end(),
                    [](const Book& b) { return b.getHealth() <= 0; }), balls.end());
                scoreLabel.setString("Score: " + std::to_string(score));  // ���·�����ʾ
            }
        }

        // ��������Ǧ��
        for (auto& pencil : pencils)
        {
            pencil.update(window, laserSound, player);
            // ���Ǧ�ʻ���Ŀ���ɳ���Ļ����Ǧ����������2
            if ((pencil.getIsHit() || pencil.getPosition().y < 0) && pencils.size() > 2)
            {
                // ���������Ƴ�Ǧ��
                pencils.erase(std::remove_if(pencils.begin(), pencils.end(),
                    [](const Pencil& p) { return p.getIsHit() || p.getPosition().y < 0; }), pencils.end());
            }
        }

        // ��ײ���
        for (auto& ball : balls)
        {
            // �������Ƿ��鱾����
            if (checkIfPlayerIsHit(player, ball) && hitRestrictor.getElapsedTime().asSeconds() >= 1)
            {
                player.setHealth(player.getHealth() - 1);  // ��������ֵ
                healthLabel.setString(std::to_string(player.getHealth()) + " HP");  // ��������ֵ��ʾ
                hitRestrictor.restart();
                player.setColor(sf::Color::Red);  // ��ұ���ʾ����
            }

            // ���Ǧ���Ƿ�����鱾
            for (auto& pencil : pencils)
            {
                if (checkShotHit(pencil, ball)) hitSound.play();  // ���Ż�����Ч
            }

            // ���˺�ָ���ɫ
            if (player.getColor() == sf::Color::Red && hitRestrictor.getElapsedTime().asSeconds() >= .5)
            {
                player.setColor(sf::Color::White);
            }
        }

        // �������λ��
        player.movePlayer(window.getSize());

        // ����
        window.clear();

        // ����������Ϸ���� //

        // ���Ʊ������ı�
        window.draw(background);
        window.draw(scoreLabel);
        window.draw(healthLabel);
        window.draw(highScoreLable);

        // ���������鱾
        for (const auto& ball : balls)
        {
            window.draw(ball);
        }

        // ��������Ǧ��
        for (const auto& pencil : pencils)
        {
            window.draw(pencil);
        }

        // �������
        window.draw(player);

        // ��ʾ��������
        window.display();

        // ��Ϸ�������
        if (player.getHealth() <= 0)
        {
            std::cout << "Game Over!" << std::endl;

            // �������λ�ú�״̬
            player.setPosition({ (float)windowSize.x / 2.f, (float)windowSize.y / 1.5f });
            window.clear();

            // ��ʾ��Ϸ��������
            GameOverScreen gameOverScreen(background.getTexture(), player.getTexture(), font, windowSize);
            gameOverScreen.run(window);

            // ������Ϸ״̬
            bookSpawnRate = 5;
            highScoreLable.setString("High Score: " + std::to_string(checkHighScore(score)));
            score = 0;
            scoreLabel.setString("Score: 0");
            player.setHealth(5);
            healthLabel.setString("5 HP");
            balls.clear();  // ��������鱾
        }
    }

    return 0;
}