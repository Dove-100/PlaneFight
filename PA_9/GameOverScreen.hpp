#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "menu.hpp"
#include "player.hpp"
#include "background.hpp"

class GameOverScreen : public Menu {
public:
    GameOverScreen(const sf::Texture& backgroundTexture, const sf::Texture& playerTexture, const sf::Font& font, sf::Vector2u windowSize)
        : Menu(backgroundTexture, playerTexture, font, windowSize), // Initialize the base class (Menu)
        background(backgroundTexture),
        player({ (float)windowSize.x, (float)windowSize.y }, playerTexture),
        gameOverText(font, "GAME OVER", 100),
        escapeCont(font, "Press ESCAPE To Play Again", 50),
        menuCont(font, "Or Press E to Exit", 50),
        credits(font, "Credits: \nDylan Hale \nIsaiah Foster \nAaron Sanchez \nJustin Scofield", 20)
    {
        // Set up the background
        background.setPosition({ 0, 0 });
        background.setScale({
            (float)windowSize.x / background.getTextureRect().size.x,
            (float)windowSize.y / background.getTextureRect().size.y
        });

        // Set up the player
        player.setPosition({ (float)windowSize.x / 2, (float)windowSize.y / 2 });
        player.setScale({ 0.5f, 0.5f }); // Adjust scale as needed
        player.setOrigin({ player.getGlobalBounds().size.x / 1, player.getGlobalBounds().size.y / 2 });

        // Set up the "Game Over" text
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setOrigin({ gameOverText.getGlobalBounds().size.x / 2, gameOverText.getGlobalBounds().size.y / 2 });
        gameOverText.setPosition({ (float)windowSize.x / 2, (float)windowSize.y / 5.5f });

        // Set up escape text
        escapeCont.setFillColor(sf::Color::Red);
        escapeCont.setOrigin({ escapeCont.getGlobalBounds().size.x / 2, escapeCont.getGlobalBounds().size.y / 2 });
        escapeCont.setPosition({ (float)windowSize.x / 2, (float)windowSize.y / 3.45f });

        // Set up go-to-menu option text
        menuCont.setFillColor(sf::Color::Red);
        menuCont.setOrigin({ escapeCont.getGlobalBounds().size.x / 2, escapeCont.getGlobalBounds().size.y / 2 });
        menuCont.setPosition({ (float)windowSize.x / 1.7f, (float)windowSize.y / 2.8f });

        // Set up credits
        credits.setFillColor(sf::Color::Red);
        credits.setOrigin({ credits.getGlobalBounds().size.x / 2, credits.getGlobalBounds().size.y / 2 });
        credits.setPosition({ (float)windowSize.x / 4.0f, (float)windowSize.y / 2.0f });
    }

    // Function to run the game over screen
    void run(sf::RenderWindow& window) {
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }

            // Restart the game if Escape is pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                return;
            }
            // Exit the game if 'E' is pressed
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
                exit(0);
            }

            // Draw the game over screen
            window.clear();
            window.draw(background);
            window.draw(player);
            window.draw(gameOverText);
            window.draw(escapeCont);
            window.draw(menuCont);
            window.draw(credits);
            window.display();
        }
        return; // If the window is closed, do not restart
    }

private:
    Background background;
    Player player;
    sf::Text gameOverText;
    sf::Text escapeCont;
    sf::Text menuCont;
    sf::Text credits;
};
