#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include "player.hpp"

class Pencil : public sf::Sprite
{
public:
    //constructor creates a pencil
    // 构造函数：创建一个铅笔对象
    // 参数：
    //   texture - 铅笔的纹理
    //   position - 初始位置（+50偏移使铅笔初始位于飞船下方）
    Pencil(const sf::Texture& texture, sf::Vector2f position) ://the plus 50 offsets the starting position of the pencils to be located right underneath the spaceship
        sf::Sprite(texture)
    {
        setPosition(position);//设置铅笔初始位置
        setScale(sf::Vector2f(0.1f, 0.1f));//adjust to change pencil size//设置缩放大小
        velocity_bullet = { 0,0 };
        hadShooted = false;
        isShooting = false;//初始状态：未射击
        isHit = false;//初始状态：未射中
        speed = 20;//速度：30
    }
    //更新铅笔状态
    // 参数：
     //   window - 游戏窗口引用
     //   laserSound - 激光音效引用
     //   player - 玩家对象
    void update(const sf::RenderWindow& window, sf::Music& laserSound, Player player)
    {

        sf::Vector2u windowSize = window.getSize(); //get the window size 获取桌面分辨率

        //未射击，铅笔跟随玩家
        if (!isShooting)
        {
            setPosition({ player.getPosition().x, player.getPosition().y});
        }
        //射击，铅笔移动
        //movement for already been shot
        if (isShooting && !hadShooted)
        {
            
            float x = sf::Mouse::getPosition().x;
            float y = sf::Mouse::getPosition().y;

            float xy = sqrt((x - player.getPosition().x) * (x - player.getPosition().x) + (y - player.getPosition().y) * (y - player.getPosition().y));
            velocity_bullet = { (x - player.getPosition().x) / xy * speed, (y - player.getPosition().y) / xy * speed };
            hadShooted = true;
            return;
        }
        if (hadShooted) {
            move(velocity_bullet);
        }
        //左键射击
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            isShooting = true;
            laserSound.play();
        }
    }
    //获取射击状态
    bool getIsShooting()const//get value of isShooting
    {
        return isShooting;
    }
    //设置射击状态
    void setIsShooting(bool isShootingNewValue)
    {
        isShooting = isShootingNewValue;
    }
    //设置击中状态
    void setIsHit()
    {
        isHit = true;
    }
    //获取是否击中状态
    const bool getIsHit() const
    {
        return isHit;
    }

private:
    float speed;//速度
    bool isShooting;//是否发射
    bool isHit;//是否击中
    bool hadShooted;
    sf::Vector2f velocity_bullet;  //速度矢量
};