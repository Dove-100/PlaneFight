#pragma once
#include <SFML/Audio.hpp>
#include "player.hpp"

class Pencil : public sf::Sprite
{
public:
    //constructor creates a pencil
    // ���캯��������һ��Ǧ�ʶ���
    // ������
    //   texture - Ǧ�ʵ�����
    //   position - ��ʼλ�ã�+50ƫ��ʹǦ�ʳ�ʼλ�ڷɴ��·���
    Pencil(const sf::Texture& texture, sf::Vector2f position) ://the plus 50 offsets the starting position of the pencils to be located right underneath the spaceship
        sf::Sprite(texture)
    {
        setPosition(position);//����Ǧ�ʳ�ʼλ��
        setScale(sf::Vector2f(0.1f, 0.1f));//adjust to change pencil size//�������Ŵ�С
        isShooting = false;//��ʼ״̬��δ���
        isHit = false;//��ʼ״̬��δ����
        speed = 30;//�ٶȣ�30
    }
    //����Ǧ��״̬
    // ������
     //   window - ��Ϸ��������
     //   laserSound - ������Ч����
     //   player - ��Ҷ���
    void update(const sf::RenderWindow& window, sf::Music& laserSound, Player player)
    {

        sf::Vector2u windowSize = window.getSize(); //get the window size ��ȡ����ֱ���

        //δ�����Ǧ�ʸ������
        if (!isShooting)
        {
            setPosition({ player.getPosition().x + 30, player.getPosition().y - 10 });
        }
        //�����Ǧ���ƶ�
        //movement for already been shot
        if (isShooting)
        {
            move({ 0.f, -speed * windowSize.x / 1920 });
            return;
        }
        //�ո����
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))//shoot pencil
        {
            isShooting = true;
            laserSound.play();
        }
    }
    //��ȡ���״̬
    bool getIsShooting()const//get value of isShooting
    {
        return isShooting;
    }
    //�������״̬
    void setIsShooting(bool isShootingNewValue)
    {
        isShooting = isShootingNewValue;
    }
    //���û���״̬
    void setIsHit()
    {
        isHit = true;
    }
    //��ȡ�Ƿ����״̬
    const bool getIsHit() const
    {
        return isHit;
    }

private:
    float speed;//�ٶ�
    bool isShooting;//�Ƿ���
    bool isHit;//�Ƿ����
};