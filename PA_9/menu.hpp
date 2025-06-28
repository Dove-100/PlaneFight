#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "player.hpp"
#include "background.hpp"


class Menu
{
public:
    // ���캯��
    // ����:
    //   backgroundTexture - ������������
    //   playerTexture - �����������
    //   font - ��������
    //   windowSize - ���ڴ�С
    Menu(const sf::Texture& backgroundTexture, const sf::Texture& playerTexture, const sf::Font& font, sf::Vector2u windowSize)
        : background(backgroundTexture), player({ (float)windowSize.x, (float)windowSize.y }, playerTexture),// ��ʼ�����������
        gameOverText(font, "BOOK BLASTER", 100), escapeCont(font, "Press Space to Play"), menuCont(font, "Menu", 50)//�����ı�����ʾ�ı�
    {
        //���ñ�����ȫ��
        background.setPosition({ 0,0.0 });
        background.setScale({
            (float)windowSize.x / background.getTextureRect().size.x,
            (float)windowSize.y / background.getTextureRect().size.y
            });

        //�������
        player.setPosition({ (float)windowSize.x / 2, (float)windowSize.y / 2 });//��Ļ�м�
        player.setScale({ 0.5f, 0.5f }); // ���ű���
        player.setOrigin({ player.getGlobalBounds().size.x / 1.0f, player.getGlobalBounds().size.y / 2 });//������ҽ�ɫ����

        //set up the "Game Over" text  ��Ϸ����
        gameOverText.setFillColor(sf::Color::Black);
        gameOverText.setOrigin({ gameOverText.getGlobalBounds().size.x / 2, gameOverText.getGlobalBounds().size.y / 2 });
        gameOverText.setPosition({ (float)windowSize.x / 2, (float)windowSize.y / 5.5f });

        //set up escape text           ��ʾ�ı�
        escapeCont.setFillColor(sf::Color::Black);
        escapeCont.setOrigin({ escapeCont.getGlobalBounds().size.x / 2, escapeCont.getGlobalBounds().size.y / 2 });
        escapeCont.setPosition({ (float)windowSize.x / 2, (float)windowSize.y / 3.45f });



        //set up go-to-menu option text �˵���ť
        menuCont.setFillColor(sf::Color::Red);
        menuCont.setOrigin({ (float)menuCont.getGlobalBounds().size.x / 2, (float)menuCont.getGlobalBounds().size.y / 2 });
        menuCont.setPosition({ (float)windowSize.x / 2, (float)windowSize.y / 3.0f });

    }


    //function to run the game over screen
    int startMenu(sf::RenderWindow& window) {
        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
                    window.close();
            }



            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                return 1;
            }

            //draw the game over screen
            window.clear();
            window.draw(background);
            window.draw(player);
            window.draw(gameOverText);
            window.draw(escapeCont);
            window.draw(menuCont);
            window.display();

        }
        return 0; //if the window is closed, do not restart
    }


private:
    Background background;//����
    Player player;//���
    sf::Text gameOverText;//��Ϸ�����ı�
    sf::Text escapeCont;//
    sf::Text menuCont;//

};