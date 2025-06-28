#pragma once
#include <SFML/Graphics.hpp>
#include "background.hpp"
#include "player.hpp"
#include <cstdlib>
#include <ctime>
#include "pencil.hpp"
#include "Book.hpp"
#include <iostream>
#include <fstream>

using sf::Text;

/**
 * ��ʼ����Ϸ����
 * @return ȫ��ģʽ����Ⱦ���ںʹ��ڳߴ�
 */
sf::RenderWindow initWindow()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();// ��ȡ����ֱ���
	sf::RenderWindow window(desktop, "Book Blaster", sf::Style::None);//�����ޱ߿򴰿�
	window.setFramerateLimit(120);//����֡��
	sf::Vector2u windowSize = window.getSize();//��ȡ���ڴ�С
	return window;
}


/**
*������ҽ�ɫ
* @param window ��Ϸ�������ã����ڻ�ȡ���ڳߴ磩
* @return ��ʼ�������Ҷ���
* */
Player loadPlayer(sf::RenderWindow& window)
{
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Player.png");//�����������
	Player playerSprite({ (float)window.getSize().x, (float)window.getSize().y }, playerTexture);//������Ҷ���
	return playerSprite;
}


/**
 * ������Ϸ����
 * @return ��ʼ����ı�������
 */
Background loadBackground()
{
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("Backdrop.png");//���ر�������
	Background backgroundSprite(backgroundTexture, { 0.f, 0.f });
	return backgroundSprite;
}


/**
 * �����鼮�����ˣ����󣨵�ǰ�����޷���ֵ��������Ҫ�޸ģ�
 */
void loadBall()
{
	sf::Texture bookTextures[3];// �����鼮����
	bookTextures[0].loadFromFile("Book.png");
	bookTextures[1].loadFromFile("Book2.png");
	bookTextures[2].loadFromFile("Book3.png");
	Book ball(bookTextures[std::rand() % 3]);// ���ѡ��һ���������鼮
	float scale = 0.05f + (static_cast<float>(std::rand() % 5 + 1) / 20); //Between 0.05 and 0.2
	ball.setHealth(rand() % 5 + 8);// �����������ֵ��8~12��
	ball.setScale({ scale, scale });//����
}

/**
 * ���Ǧ�ʣ��ӵ����Ƿ�����鼮
 * @param bullet Ǧ�ʶ���
 * @param enemy �鼮����
 * @return �Ƿ����
 */
static bool checkShotHit(Pencil& bullet, Book& enemy)
{
	if (bullet.getIsShooting())// ֻ�з���״̬��Ǧ�ʲż��
	{
		// ��ȡ����λ��
		float bulletX = bullet.getPosition().x;
		float enemyX = enemy.getPosition().x;

		float bulletY = bullet.getPosition().y;
		float enemyY = enemy.getPosition().y;


		float tolerance = 50.f; // ��ײ����ݲ�ɵ������п��С��
		// �򵥾�����ײ���
		if (std::abs(bulletX - enemyX) < tolerance && std::abs(bulletY - enemyY) < tolerance)
		{
			std::cout << "TARGET HIT!\n" << std::endl;
			enemy.setHealth(enemy.getHealth() - 1); // �����鼮����ֵ
			bullet.setIsHit(); // �ӵ��ѻ���
			return true;
		}
	}
	return false;
}


/**
 * �������Ƿ��鼮����
 * @param player ��Ҷ���
 * @param ball �鼮����
 * @return �Ƿ�����ײ
 */
static bool checkIfPlayerIsHit(Player& player, const Book& ball)
{
	// ��ȡ���ߵ�ȫ�ֱ߽��
	sf::FloatRect playerBounds = player.getGlobalBounds();
	sf::FloatRect ballBounds = ball.getGlobalBounds();
	// ʹ��SFML���õ���ײ���
	if (auto intersection = playerBounds.findIntersection(ballBounds)) {
		return true;
	}
	return false;
}

/**
 * ��鲢������߷�
 * @param cur ��ǰ����
 * @return �µ���߷�
 */
int checkHighScore(int cur)
{
	// ��ȡ��ʷ��߷�
	std::ifstream file("HighScore.txt");
	if (!file.is_open())
	{
		std::cerr << "Error opening high score file." << std::endl;
		return -1;
	}
	int highScore;
	file >> highScore;
	file.close();
	// �����ǰ�������ߣ����¼�¼
	if (cur > highScore)
	{
		std::ofstream file("HighScore.txt");
		if (!file.is_open())
		{
			std::cerr << "Error opening high score file." << std::endl;
			return -1;
		}
		file << cur;
		file.close();
		return cur;
	}
	else
	{
		return highScore;
	}
}

/**
 * ���ز���ʼ����Ϸ�ı���ǩ
 * @param window ��Ϸ����
 * @param font �������
 * @param scoreLabel ������ǩ
 * @param healthLabel ����ֵ��ǩ
 * @param highScoreLable ��߷ֱ�ǩ
 */
void loadTexts(sf::RenderWindow& window, sf::Font& font, sf::Text& scoreLabel, sf::Text& healthLabel, sf::Text& highScoreLable)
{

	//determine scale based on monitor
	sf::Vector2u windowSize = window.getSize();
	float scale = (float)windowSize.x / 1920;

	//set font and scale for score and health labels
	scoreLabel.setFont(font);
	scoreLabel.setString("Score: 0");
	scoreLabel.setCharacterSize(30);
	scoreLabel.setFillColor(sf::Color::Green);
	scoreLabel.setOrigin({ scoreLabel.getGlobalBounds().size.x / 2, scoreLabel.getGlobalBounds().size.y / 2 });
	scoreLabel.setPosition({ (float)windowSize.x / 2, (float)windowSize.y * .93f });

	healthLabel.setString("5 HP");
	healthLabel.setFont(font);
	healthLabel.setCharacterSize(30);
	healthLabel.setFillColor(sf::Color::Red);
	healthLabel.setOrigin({ healthLabel.getGlobalBounds().size.x / 2, healthLabel.getGlobalBounds().size.y / 2 });
	healthLabel.setPosition({ (float)windowSize.x / 4, (float)windowSize.y * .93f });


	//open highscore file to read
	std::ifstream file("HighScore.txt");
	std::string line;
	std::getline(file, line);
	file.close();

	//set font and scale for highscore label
	highScoreLable.setCharacterSize(30);
	highScoreLable.setFillColor(sf::Color::Red);
	highScoreLable.setFont(font);
	highScoreLable.setString("High Score: " + line);
	highScoreLable.setOrigin({ (float)highScoreLable.getGlobalBounds().size.x / 2, highScoreLable.getGlobalBounds().size.y / 2 });
	highScoreLable.setPosition({ (float)windowSize.x / 1.25f , (float)windowSize.y * .93f });

}