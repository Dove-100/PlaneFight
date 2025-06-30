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
// 定义是否运行测试的宏
#define RUN_TESTS true

int main()
{
    // 初始化随机数种子
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // 初始化窗口并获取窗口尺寸
    sf::RenderWindow window = initWindow();
    sf::Vector2u windowSize = window.getSize();

    // 加载菜单背景音乐
    sf::Music menuMusic;
    menuMusic.openFromFile("danceWeapon4.mp3");  // 加载音乐文件
    menuMusic.setVolume(10);  // 设置音量
    menuMusic.play();  // 播放音乐
    menuMusic.setLooping(true);  // 设置循环播放

    // 加载游戏背景音乐
    sf::Music gameMusic;
    gameMusic.openFromFile("track1.mp3");
    gameMusic.setVolume(15);

    // 加载玩家角色
    Player player = loadPlayer(window);

    // 加载背景
    Background background = loadBackground();

    // 初始化游戏分数
    int score = 0;

    // 加载激光音效
    sf::Music laserSound;
    laserSound.openFromFile("hit_sound.mp3");

    // 加载碰撞音效
    sf::Music hitSound;
    hitSound.openFromFile("pencil_hit.mp3");

    // 加载书本纹理
    sf::Texture bookTextures[3];
    bookTextures[0].loadFromFile("PNG/googly-a.png");
    bookTextures[1].loadFromFile("PNG/googly-b.png");
    bookTextures[2].loadFromFile("PNG/googly-c.png");

    // 创建存储书本和铅笔的向量
    std::vector<Book> balls;
    std::vector<Pencil> pencils;

    // 创建第一个书本对象
    Book firstBook(bookTextures[std::rand() % 3]);  // 随机选择纹理
    float scale = 0.1f + ((float)(std::rand() % 5 + 1) / 60); // 随机缩放比例(0.1到0.15之间)
    firstBook.setScale({ scale, scale });
    balls.push_back(firstBook);

    // 加载第一个铅笔对象
    sf::Texture pencilTex;
    pencilTex.loadFromFile("red_star.png");
    Pencil pencil(pencilTex, player.getPosition());
    pencils.push_back(pencil);

    // 初始化计时器用于控制生成间隔
    sf::Clock ball_spawn_clock;  // 书本生成计时器
    sf::Clock pencil_spawn_clock;  // 铅笔生成计时器
    sf::Clock hitRestrictor;  // 击中限制计时器

    // 加载字体
    sf::Font font;
    font.openFromFile("PressStart2P-Regular.ttf");
    // 创建文本对象
    sf::Text scoreLabel(font);  // 分数显示
    sf::Text healthLabel(font);  // 生命值显示
    sf::Text highScoreLable(font);  // 最高分显示

    // 设置文本样式
    loadTexts(window, font, scoreLabel, healthLabel, highScoreLable);

    // 设置初始生成速率
    float bookSpawnRate = 4;  // 书本生成速率(秒)
    float pencilSpawnRate = 50.f;  // 铅笔生成速率(毫秒)

    // 显示开始菜单
    Menu menu(background.getTexture(), player.getTexture(), font, windowSize);
    menu.startMenu(window);

    // 停止菜单音乐，开始游戏音乐
    menuMusic.stop();
    gameMusic.play();
    gameMusic.setLooping(true);

    // 如果启用测试，运行所有测试用例
    if (RUN_TESTS)
    {
        TestCases test;
        test.testAll();
    }

    // 主游戏循环
    while (window.isOpen())
    {
        // 事件处理循环
        while (const std::optional event = window.pollEvent())
        {
            // 按ESC键关闭窗口
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
                window.close();
        }

        // 书本生成逻辑
        if (ball_spawn_clock.getElapsedTime().asSeconds() >= bookSpawnRate)
        {
            // 创建新书本
            Book newBall(bookTextures[std::rand() % 3]);
            float scale = 0.1f + ((float)(std::rand() % 5 + 1) / 60);
            newBall.setScale({ scale, scale });
            balls.push_back(newBall);
            ball_spawn_clock.restart();
            // 逐渐增加难度(减少生成间隔)
            if (bookSpawnRate > 1) bookSpawnRate -= 0.1f;
            std::cout << "Number of balls: " << balls.size() << std::endl;
        }

        // 铅笔生成逻辑
        if (pencils[pencils.size() - 1].getIsShooting() == true &&
            pencil_spawn_clock.getElapsedTime().asMilliseconds() >= pencilSpawnRate)
        {
            Pencil newPencil(pencilTex, player.getPosition());
            pencils.push_back(newPencil);
            pencil_spawn_clock.restart();
        }

        // 更新所有游戏对象 //

        // 更新所有书本
        for (auto& ball : balls)
        {
            ball.update(window.getSize());
            if (ball.getHealth() <= 0)  // 如果书本生命值耗尽
            {
                score += ball.getOriginalHealth() * 8;  // 增加分数
                ball.setHealth(0);
                // 从向量中移除被摧毁的书本
                balls.erase(std::remove_if(balls.begin(), balls.end(),
                    [](const Book& b) { return b.getHealth() <= 0; }), balls.end());
                scoreLabel.setString("Score: " + std::to_string(score));  // 更新分数显示
            }
        }

        // 更新所有铅笔
        for (auto& pencil : pencils)
        {
            pencil.update(window, laserSound, player);
            // 如果铅笔击中目标或飞出屏幕，且铅笔数量大于2
            if ((pencil.getIsHit() || pencil.getPosition().y < 0) && pencils.size() > 2)
            {
                // 从向量中移除铅笔
                pencils.erase(std::remove_if(pencils.begin(), pencils.end(),
                    [](const Pencil& p) { return p.getIsHit() || p.getPosition().y < 0; }), pencils.end());
            }
        }

        // 碰撞检测
        for (auto& ball : balls)
        {
            // 检测玩家是否被书本击中
            if (checkIfPlayerIsHit(player, ball) && hitRestrictor.getElapsedTime().asSeconds() >= 1)
            {
                player.setHealth(player.getHealth() - 1);  // 减少生命值
                healthLabel.setString(std::to_string(player.getHealth()) + " HP");  // 更新生命值显示
                hitRestrictor.restart();
                player.setColor(sf::Color::Red);  // 玩家变红表示受伤
            }

            // 检测铅笔是否击中书本
            for (auto& pencil : pencils)
            {
                if (checkShotHit(pencil, ball)) hitSound.play();  // 播放击中音效
            }

            // 受伤后恢复颜色
            if (player.getColor() == sf::Color::Red && hitRestrictor.getElapsedTime().asSeconds() >= .5)
            {
                player.setColor(sf::Color::White);
            }
        }

        // 更新玩家位置
        player.movePlayer(window.getSize());

        // 清屏
        window.clear();

        // 绘制所有游戏对象 //

        // 绘制背景和文本
        window.draw(background);
        window.draw(scoreLabel);
        window.draw(healthLabel);
        window.draw(highScoreLable);

        // 绘制所有书本
        for (const auto& ball : balls)
        {
            window.draw(ball);
        }

        // 绘制所有铅笔
        for (const auto& pencil : pencils)
        {
            window.draw(pencil);
        }

        // 绘制玩家
        window.draw(player);

        // 显示绘制内容
        window.display();

        // 游戏结束检测
        if (player.getHealth() <= 0)
        {
            std::cout << "Game Over!" << std::endl;

            // 重置玩家位置和状态
            player.setPosition({ (float)windowSize.x / 2.f, (float)windowSize.y / 1.5f });
            window.clear();

            // 显示游戏结束画面
            GameOverScreen gameOverScreen(background.getTexture(), player.getTexture(), font, windowSize);
            gameOverScreen.run(window);

            // 重置游戏状态
            bookSpawnRate = 5;
            highScoreLable.setString("High Score: " + std::to_string(checkHighScore(score)));
            score = 0;
            scoreLabel.setString("Score: 0");
            player.setHealth(5);
            healthLabel.setString("5 HP");
            balls.clear();  // 清空所有书本
        }
    }

    return 0;
}