#pragma once  
#include <SFML/graphics.hpp>  
#include <cmath>  
#define MAX_SPEED 7
#define BOUNCE_RNDM .1
#define BASE_SPEED 5

class Book : public sf::Sprite {  
public:  
   Book(const sf::Texture& tex) : sf::Sprite(tex), velocity(sf::Vector2f{ -BASE_SPEED, -BASE_SPEED })
   {
       sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
       setOrigin(sf::Vector2f{static_cast<float>(tex.getSize().x/2) , static_cast<float>(tex.getSize().y) / 2.f});
       setPosition(sf::Vector2f{ static_cast<float> (std::rand() % desktop.size.x) , -getGlobalBounds().size.y});
	   scale = (float)desktop.size.x / 1920;
       float scales[2] = { scale, -scale };
	   velocity.x *= (float)scales[rand()%30]; // random x axis direction at start
	   velocity.y *= scale;
       setScale({ scale, scale });
       originalHealth = health = rand()%10 +5;
     
   }  

   sf::Vector2f getVelocity() const { return velocity; }

   void update(sf::Vector2u windowSize);  

   void moderateSpeed();

   void bounce(sf::Vector2u &windowSize);

   void setTexture(const sf::Texture& tex) { texture = tex; }

   void setHealth(int newHealth) { health = newHealth; }

   int getHealth() const { return health; }

   int getOriginalHealth() const { return originalHealth; }

private:  
   sf::Vector2f velocity;  
   float scale; 
   sf::Texture texture;
   int health;
   int originalHealth;
};

void Book::update(sf::Vector2u windowSize) {  

   moderateSpeed(); // keep speed in check

   move(velocity); // move the ball

   bounce(windowSize); // check for bounces

}

void Book::moderateSpeed() 
{
    if (velocity.x > MAX_SPEED*scale)
        velocity.x = MAX_SPEED * scale;
    else if (velocity.x < -MAX_SPEED * scale)
        velocity.x = MAX_SPEED * scale;
    if (velocity.y > MAX_SPEED * scale)
        velocity.y = MAX_SPEED * scale;
    else if (velocity.y < -MAX_SPEED * scale)
        velocity.y = MAX_SPEED * scale;
}

void Book::bounce(sf::Vector2u &windowSize)
{
    const float bounceFactor = 1.0f;
    sf::Vector2f pos = getPosition();
    float radius = getGlobalBounds().size.x / 2.f;

    //bottom bounce
    if (pos.y + radius >= static_cast<float>(windowSize.y)*(float).88) {
        setPosition({ pos.x, static_cast<float>(windowSize.y)*(float).88 - radius});
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
    if (pos.x + radius >= static_cast<float>(windowSize.x)){
        setPosition({ static_cast<float>(windowSize.x) - radius, pos.y });
        velocity.x = -velocity.x * bounceFactor;
        velocity.y -= BOUNCE_RNDM;
    }
}