#include <iostream>
#include <SFML/Audio.hpp>
#include "pencil.hpp"
#include "player.hpp"
#include <vector>

class TestCases
{
public:

    //run all test cases
    bool testAll()
    {
        if (testAudioFilesLoaded() &&
            testTextureLoading() &&
            testProjectileCleanupWithoutImage() &&
            testCheckHighScore() && 
            testPencilDelete()
            ) 
        {
            return true;
        }

        else return false;
    }

    bool testPencilDelete()
    {
        bool test = true;

        std::vector<Pencil> pencils;
        sf::Texture pencilTex("pencil.png");
        Player temp;

        //make vector of pencils
        for (int i = 0 ; i < 10; i++)
        {
            Pencil newPencil(pencilTex, temp.getPosition());
            pencils.push_back(newPencil);
        }

        //remove one by one
        for (int j = 0; j < 10; j++)
        {
            pencils.erase(pencils.begin());
        }
        
        if (!pencils.empty())
        {
            test = false; 
            std::cout << "Pencil delete failed" << std::endl;
        }

        else
        {
            std::cout << "Pencil deletion passed" << std::endl;
        }

        return test;
    }

	//test case for loading audio files
    bool testAudioFilesLoaded()
    {
        std::vector<std::string> audioFiles = {
            "danceWeapon4.mp3",
            "track1.mp3",
            "LaserGunPew.wav",
            "pencil_hit.mp3",
            "pencil_hit.mp3"
        };

        bool allLoaded = true;
        for (const auto& file : audioFiles)
        {
            sf::Music testMusic;
            if (!testMusic.openFromFile(file))
            {
                std::cout << "Error: Failed to load audio file: " << file << std::endl;
                allLoaded = false;
            }
            else
            {
                std::cout << "Audio test loaded successfully: " << file << std::endl;
            }
        }
        return allLoaded;
    }


    bool testTextureLoading() 
    {
        sf::Texture book1, book2, book3, pencil, playerTex, backgroundTex;

        bool allLoaded =
            book1.loadFromFile("Book.png") &&
            book2.loadFromFile("Book2.png") &&
            book3.loadFromFile("Book3.png") &&
            pencil.loadFromFile("pencil.png") &&
            playerTex.loadFromFile("player.png") &&
            backgroundTex.loadFromFile("BackDrop.png");

        if (allLoaded) 
        {
            std::cout << "Texture loading test passed\n";
            return true;
        }
        else 
        {
            std::cout << "Texture loading test failed\n";
            return false;
        }

        return allLoaded;
    }


    bool testProjectileCleanupWithoutImage()
        {
        sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Test Window");
        sf::Music dummyMusic;
        Player player;

        // Declare a dummy texture — no need to load from image
        sf::Texture dummyTexture;

        //makes some pencils
        std::vector<Pencil> projectiles;
        for (int i = 0; i < 5; ++i) {
            projectiles.emplace_back(dummyTexture, sf::Vector2f(100.f, 500.f));
            projectiles.back().setIsShooting(true);
        }

        //simulation
        for (int frame = 0; frame < 100; ++frame) {
            for (auto& pencil : projectiles) {
                pencil.update(window, dummyMusic, player);
            }

            //remove pencils that have moved off screen
            projectiles.erase(
                std::remove_if(projectiles.begin(), projectiles.end(),
                    [&](const Pencil& pencil) {
                        return pencil.getPosition().y < 0;
                    }),
                projectiles.end()
            );
        }

        //test results
        if (projectiles.empty()) 
        {
            std::cout << "Projectile Cleanup Test Passed\n";
            return true;
        }
        else
        {
            std::cout << "Projectile Cleanup Test Failed: " << projectiles.size() << " projectiles still exist.\n";
            return false;
        }
    }


    bool testCheckHighScore() {


        std::ofstream file("HighScore.txt");

        file << 40;

        file.close();

        bool test = false;

        int result = checkHighScore(75);



        std::ifstream verify("HighScore.txt");

        int fileScore;

        verify >> fileScore;

        verify.close();



        if (result == 75 && fileScore == 75) {

            std::cout << "Check high score test passed: high score was updated correctly.\n";

            test = true;

        }

        else {

            std::cout << "Check high score test failed!\n";

            std::cout << "Returned: " << result << ", File Score: " << fileScore << std::endl;

        }
        return test;

    }

};