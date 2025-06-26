#pragma once
#include <SFML/Audio.hpp>
#include "player.hpp"

class Pencil : public sf::Sprite
{
public:
    //constructor creates a pencil
   Pencil(const sf::Texture& texture, sf::Vector2f position) ://the plus 50 offsets the starting position of the pencils to be located right underneath the spaceship
        sf::Sprite(texture)
   {
        setPosition(position);
        setScale(sf::Vector2f(0.1f, 0.1f));//adjust to change pencil size
		isShooting = false;
		isHit = false;
        speed = 30;
   }

   void update(const sf::RenderWindow& window, sf::Music& laserSound, Player player)
   {

       sf::Vector2u windowSize = window.getSize(); //get the window size

	   if (!isShooting)
	   {
           setPosition({ player.getPosition().x + 30, player.getPosition().y-10});
	   }

       //movement for already been shot
       if (isShooting)
       {
           move({ 0.f, -speed*windowSize.x/1920 });
           return;
       }

       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))//shoot pencil
       {
		   isShooting = true;
           laserSound.play();
       }
   }

   bool getIsShooting()const//get value of isShooting
   {
       return isShooting;
   }

   void setIsShooting(bool isShootingNewValue)
   {
       isShooting = isShootingNewValue;
   }

   void setIsHit()
   {
	   isHit = true;
   }
   
   const bool getIsHit() const
   {
	   return isHit;
   }

private:
    float speed;
    bool isShooting;
    bool isHit;
};