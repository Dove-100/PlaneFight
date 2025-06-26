#pragma once

#include <SFML/Audio.hpp>
#include <SFML/graphics.hpp>
#include <string>
using std::string;
#define PLAYER_SCALE 0.15f
#define PLAYER_SPEED 13.f

class Player : public sf::Sprite
{
public:
    Player(sf::Vector2f position = { 0.f, 0.f }, const sf::Texture& playerTexture = sf::Texture());

    void movePlayer(sf::Vector2u windowSize);

	void decrementPlayerHealth();

	int getHealth() const { return health; }

	void setHealth(int newHealth) { health = newHealth; }

private:
    sf::Texture texture;
    float speed;
    int health;
};

Player::Player(sf::Vector2f position, const sf::Texture& playerTexture)
    : sf::Sprite(playerTexture), texture(playerTexture)
{
    setTexture(texture);
    setPosition({ position.x / 2, position.y / 1.5f });
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    setScale({ PLAYER_SCALE * desktop.size.x / 1920, PLAYER_SCALE * desktop.size.x / 1920 });
    speed = PLAYER_SPEED * (static_cast<float>(desktop.size.x) / 1920);
    health = 5;
}


void Player::movePlayer(sf::Vector2u windowSize)
{
	
    sf::Vector2u spriteSize = (getTexture().getSize());

    sf::Vector2f position = getPosition();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        if (position.x > 0)
            move({ -speed, 0 });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) 
        if (position.x + PLAYER_SCALE*spriteSize.x < windowSize.x) // *.1 to match scale
            move({ speed, 0 });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        if (position.y > 0)
            move({ 0,-speed});

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        if (position.y + PLAYER_SCALE * spriteSize.y < windowSize.y) // *.1 to match scale
            move({0,speed});



}


void Player::decrementPlayerHealth()
{
    if (health > 0)
    {
        health -= 1;

    }
}