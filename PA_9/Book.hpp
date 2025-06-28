#pragma once  
#include <SFML/graphics.hpp>  
#include <cmath>  
#define MAX_SPEED 7
#define BOUNCE_RNDM .1
#define BASE_SPEED 5

class Book : public sf::Sprite {
public:
    // 构造函数
    // 参数: tex - 书籍纹理
    Book(const sf::Texture& tex) : sf::Sprite(tex), velocity(sf::Vector2f{ -BASE_SPEED, -BASE_SPEED })
    {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();//获取屏幕分辨率
        setOrigin(sf::Vector2f{ static_cast<float>(tex.getSize().x / 2) , static_cast<float>(tex.getSize().y) / 2.f });//设置Book的原点位置
        setPosition(sf::Vector2f{ static_cast<float> (std::rand() % desktop.size.x) , -getGlobalBounds().size.y });//设置出现位置  x坐标随机
        scale = (float)desktop.size.x / 1920;//缩放系数
        float scales[2] = { scale, -scale };
        velocity.x *= (float)scales[rand() % 30]; // random x axis direction at start 随机速度方向（对x轴分量）
        velocity.y *= scale;
        setScale({ scale, scale });//设置精灵缩放
        originalHealth = health = rand() % 10 + 5;//随机生命值

    }

    sf::Vector2f getVelocity() const { return velocity; }//获取速度矢量

    void update(sf::Vector2u windowSize);  //更新书籍位置、速度、边界检测

    void moderateSpeed();//限制速度不超过最大速度

    void bounce(sf::Vector2u& windowSize);//边界检测

    void setTexture(const sf::Texture& tex) { texture = tex; }//设置纹理

    void setHealth(int newHealth) { health = newHealth; }//设置生命值

    int getHealth() const { return health; }//获取当前生命值

    int getOriginalHealth() const { return originalHealth; }//获取初始生命值

private:
    sf::Vector2f velocity;  //速度矢量
    float scale; //缩放系数
    sf::Texture texture;//纹理
    int health;//生命值
    int originalHealth;//初始生命值
};

void Book::update(sf::Vector2u windowSize) {

    moderateSpeed(); // keep speed in check

    move(velocity); // move the ball

    bounce(windowSize); // check for bounces

}

void Book::moderateSpeed()
{
    if (velocity.x > MAX_SPEED * scale)
        velocity.x = MAX_SPEED * scale;
    else if (velocity.x < -MAX_SPEED * scale)
        velocity.x = MAX_SPEED * scale;
    if (velocity.y > MAX_SPEED * scale)
        velocity.y = MAX_SPEED * scale;
    else if (velocity.y < -MAX_SPEED * scale)
        velocity.y = MAX_SPEED * scale;
}

void Book::bounce(sf::Vector2u& windowSize)
{
    const float bounceFactor = 1.0f;
    sf::Vector2f pos = getPosition();
    float radius = getGlobalBounds().size.x / 2.f;

    //bottom bounce
    if (pos.y + radius >= static_cast<float>(windowSize.y) * (float).88) {
        setPosition({ pos.x, static_cast<float>(windowSize.y) * (float).88 - radius });
        velocity.y = -velocity.y * bounceFactor; //bounce the ball
        velocity.x -= BOUNCE_RNDM; //slow down the ball slightly on bounce
    }

    //top bounce
    if (pos.y - radius <= 0.f) {
        setPosition({ pos.x, radius });
        velocity.y = -velocity.y * bounceFactor;
        velocity.x -= BOUNCE_RNDM;
    }

    //left bounce
    if (pos.x - radius <= 0.f) {
        setPosition({ radius, pos.y });
        velocity.x = -velocity.x * bounceFactor;
        velocity.y -= BOUNCE_RNDM;
    }

    //right bounce
    if (pos.x + radius >= static_cast<float>(windowSize.x)) {
        setPosition({ static_cast<float>(windowSize.x) - radius, pos.y });
        velocity.x = -velocity.x * bounceFactor;
        velocity.y -= BOUNCE_RNDM;
    }
}