#pragma once

#include <SFML/Audio.hpp>
#include <SFML/graphics.hpp>
#include <string>
using std::string;
#define PLAYER_SCALE 0.15f
#define PLAYER_SPEED 13.f
//�൱��.h�ļ�
class Player : public sf::Sprite
{
public:
    Player(sf::Vector2f position = { 0.f, 0.f }, const sf::Texture& playerTexture = sf::Texture());//��ʼ������ҵ�λ�ú�ͼƬ

    void movePlayer(sf::Vector2u windowSize);//�ƶ�

    void decrementPlayerHealth();//���ٽ���ֵ

    int getHealth() const { return health; }//��ȡ����ֵ

    void setHealth(int newHealth) { health = newHealth; }//���ý���ֵ

private:
    sf::Texture texture;//ͼƬ
    float speed;//�ٶ�
    int health;//����ֵ
};
//�൱��.cpp�ļ�
Player::Player(sf::Vector2f position, const sf::Texture& playerTexture)
    : sf::Sprite(playerTexture), texture(playerTexture)
{
    setTexture(texture);//������ҽ�ɫͼƬ
    setPosition({ position.x / 2, position.y / 1.5f });//����λ��
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();// ��ȡ����ֱ���
    setScale({ PLAYER_SCALE * desktop.size.x / 1920, PLAYER_SCALE * desktop.size.x / 1920 });//���÷ֱ���
    speed = PLAYER_SPEED * (static_cast<float>(desktop.size.x) / 1920);//�����ٶ�
    health = 5;//���ý���ֵ
}

//��Ա����
void Player::movePlayer(sf::Vector2u windowSize)
{

    sf::Vector2u spriteSize = (getTexture().getSize());//���ô�С

    sf::Vector2f position = getPosition();//λ��
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        if (position.x > 0)//�����߽�
            move({ -speed, 0 });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        if (position.x + PLAYER_SCALE * spriteSize.x < windowSize.x) //����ұ߽�
            move({ speed, 0 });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        if (position.y > 0)//����ϱ߽�
            move({ 0,-speed });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        if (position.y + PLAYER_SCALE * spriteSize.y < windowSize.y) //����±߽�
            move({ 0,speed });



}

//����ֵ����
void Player::decrementPlayerHealth()
{
    if (health > 0)
    {
        health -= 1;

    }
}