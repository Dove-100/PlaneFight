#pragma once  
#include <SFML/graphics.hpp>  
#include <string>  
using std::string;  

class Background : public sf::Sprite  
{  
public:  
  Background(const sf::Texture& texture, sf::Vector2f position = { 0.f, 0.f });  

private:  
  sf::Texture texture;
  float scale;  
};  

Background::Background(const sf::Texture& texture, sf::Vector2f position)  
  : sf::Sprite(texture), texture(texture) //initialize  member texture  
{  
  setTexture(this->texture); 
  setPosition(position);  
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();  
  setScale({ (float)desktop.size.x/ texture.getSize().x , (float)desktop.size.y / texture.getSize().y });
}