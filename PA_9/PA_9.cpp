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
//你好
//set false if you dont want to run tests
#define RUN_TESTS true

int main()
{

    //Seed random time
    std::srand(static_cast<unsigned>(std::time(nullptr)));


    //Open window and return dimensions
     sf::RenderWindow window = initWindow();  
    //sf::RenderWindow window(sf::VideoMode({ 1800,900 }), "sfml");
     sf::Vector2u windowSize = window.getSize();


    //load menu background music
    sf::Music menuMusic;
    menuMusic.openFromFile("danceWeapon4.mp3");
    menuMusic.setVolume(10);
    menuMusic.play();
    menuMusic.setLooping(true);


    //load in-game background music
    sf::Music gameMusic;
    gameMusic.openFromFile("track1.mp3");
    gameMusic.setVolume(15);


    //load player
    Player player = loadPlayer(window);


    //load background
    Background background = loadBackground();


    //track gamescore
    int score = 0;


    //load laser sound
    sf::Music laserSound;
    laserSound.openFromFile("LaserGunPew.wav");

    //load collision sounds
    sf::Music hitSound;
    hitSound.openFromFile("pencil_hit.mp3");


    //load book textures
    sf::Texture bookTextures[3];
    bookTextures[0].loadFromFile("Book.png");
    bookTextures[1].loadFromFile("Book2.png");
    bookTextures[2].loadFromFile("Book3.png");


    //vectors to store books and pencils
    std::vector<Book> balls;
    std::vector<Pencil> pencils;


    //first book
    Book firstBook(bookTextures[std::rand() % 3]);
    float scale = 0.1f + ((float)(std::rand() % 5 + 1) / 60); // Between 0.1 and .15
    firstBook.setScale({ scale, scale });
    balls.push_back(firstBook);

    //load first pencil in
    sf::Texture pencilTex;
    pencilTex.loadFromFile("pencil.png");
    Pencil pencil(pencilTex, player.getPosition());
    pencils.push_back(pencil);


    //initialize clocks to keep track of spawn times
    sf::Clock ball_spawn_clock;
    sf::Clock pencil_spawn_clock;
    sf::Clock hitRestrictor;


    //load font
    sf::Font font;
    font.openFromFile("PressStart2P-Regular.ttf");
    sf::Text scoreLabel(font);
    sf::Text healthLabel(font);
    sf::Text highScoreLable(font);

    //set text settings
    loadTexts(window, font, scoreLabel, healthLabel, highScoreLable);

    //defines starting spawn rate of balls. decreases down to 3 seconds for difficulty increase
    float bookSpawnRate = 4;
    float pencilSpawnRate = 50.f;


    //Start game in the menu
    Menu menu(background.getTexture(), player.getTexture(), font, windowSize);
    menu.startMenu(window);

    menuMusic.stop();

    gameMusic.play();
    gameMusic.setLooping(true);



    //run test cases before gameloop begins
    if (RUN_TESTS)
    {
        TestCases test;
        test.testAll();
    }


    while (window.isOpen())
    {
        //keep program running until window closes
        while (const std::optional event = window.pollEvent())
        {
         //   if (event->is<sf::Event::Closed>())
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))//开始游戏后，按E键盘直接退出
                window.close();
        }

        //spawn a new ball every spawnRate seconds
        if (ball_spawn_clock.getElapsedTime().asSeconds() >= bookSpawnRate)
        {
            Book newBall(bookTextures[std::rand() % 3]);
            float scale = 0.1f + ((float)(std::rand() % 5 + 1) / 60); // Between 0.1 and .15
            newBall.setScale({ scale, scale });
            balls.push_back(newBall);
            ball_spawn_clock.restart();
            if (bookSpawnRate > 1) bookSpawnRate -= 0.1f; // Decrease spawn rate to increase difficulty
            std::cout << "Number of balls: " << balls.size() << std::endl;
        }

        //spawn in a new pencil if pencil is shot
        if (pencils[pencils.size() - 1].getIsShooting() == true && pencil_spawn_clock.getElapsedTime().asMilliseconds() >= pencilSpawnRate)
        {
            Pencil newPencil(pencilTex, player.getPosition());
            pencils.push_back(newPencil);
            pencil_spawn_clock.restart();
        }


        //update all objects//


        //update all balls
        for (auto& ball : balls)
        {
            ball.update(window.getSize());
            if (ball.getHealth() <= 0)
            {
                score += ball.getOriginalHealth() * 8;
                //remove the ball from the vector if destoryed
                ball.setHealth(0);
                balls.erase(std::remove_if(balls.begin(), balls.end(), [](const Book& b) { return b.getHealth() <= 0; }), balls.end());
                scoreLabel.setString("Score: " + std::to_string(score));
            }
        }



        //update all pencils
        for (auto& pencil : pencils)
        {
            pencil.update(window, laserSound, player);
            if ((pencil.getIsHit() || pencil.getPosition().y < 0) && pencils.size() > 2)
            {
                //remove the pencil from the vector if it hits a book
                pencils.erase(std::remove_if(pencils.begin(), pencils.end(), [](const Pencil& p) { return p.getIsHit() || p.getPosition().y < 0; }), pencils.end());
            }
        }


        //check if pencils have hit a ball and if balls have hit the player
        for (auto& ball : balls)
        {
            if (checkIfPlayerIsHit(player, ball) && hitRestrictor.getElapsedTime().asSeconds() >= 1)
            {
                player.setHealth(player.getHealth() - 1);
                healthLabel.setString(std::to_string(player.getHealth()) + " HP");
                hitRestrictor.restart();
                player.setColor(sf::Color::Red);
            }
            for (auto& pencil : pencils)
            {
                if (checkShotHit(pencil, ball)) hitSound.play();
            }
            if (player.getColor() == sf::Color::Red && hitRestrictor.getElapsedTime().asSeconds() >= .5)
            {
                player.setColor(sf::Color::White);
            }
        }

        //update player location & check hit
        player.movePlayer(window.getSize());

        //clear screen between frames
        window.clear();


        //draw all objects back to screen//


        window.draw(background);
        window.draw(scoreLabel);
        window.draw(healthLabel);
        window.draw(highScoreLable);

        //draw all balls
        for (const auto& ball : balls)
        {
            window.draw(ball);
        }

        //draw all pencils
        for (const auto& pencil : pencils)
        {
            window.draw(pencil);
        }
        window.draw(player);

        //display window to screen
        window.display();

        //check if player has died
        if (player.getHealth() <= 0)
        {
            std::cout << "Game Over!"<< std::endl;

            //reset player position and stats and go to game over screen
            player.setPosition({ (float)windowSize.x / 2.f, (float)windowSize.y / 1.5f });
            window.clear();
            GameOverScreen gameOverScreen(background.getTexture(), player.getTexture(), font, windowSize);
            gameOverScreen.run(window);
            bookSpawnRate = 5;
            highScoreLable.setString("High Score: " + std::to_string(checkHighScore(score)));
            score = 0;
            scoreLabel.setString("Score: 0");
            player.setHealth(5);
            healthLabel.setString("5 HP");
            balls.clear();
        }
    }

    return 0;

}