#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Ball.h"
#include "Paddle.h"
#include "Tiles.h"
#include "SoundEffect.h"
#include "HighScores.h"

using std::cout;
using std::endl;

// Global Variables
const sf::Vector2f GameWindowSize(730.0f,720.0f);
sf::Texture TileTexture;
const float WallWidth = 10.0f;
const std::string ResourcesPath = "../resources/";

std::string intro();

int main ()
{
    srand(static_cast<unsigned>(time(0)));
    std::string name = intro();

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1024, 720), "", sf::Style::Close);
    window.setTitle(sf::String(name + "'s JBreakout"));

    if (!sf::Joystick::isConnected(0))
    {
        cout << "\nGame controller is not connected ...\nYou can't play the game\n";
        return 1;
    }

    bool gameStarted = false;
    bool gameOver = false;
    bool gamePaused = false;
    const float WallWidth = 10.0f;
    sf::Event evnt;
    unsigned score = 0;
    int ballsUsed = 0;
    HighScores highScores;

    // Create walls for game
    sf::RectangleShape wall[3];
    for (auto i = 0; i < 3; i++)
    {
        wall[i].setFillColor(sf::Color(0, 255, 0, 255));
        wall[i].setOutlineColor(sf::Color(0, 0, 0, 255));
        wall[i].setSize(sf::Vector2f(WallWidth, GameWindowSize.x));
    }
    wall[0].setPosition(0.0f,WallWidth);
    wall[0].rotate(-90.0f);
    wall[1].setPosition(0.0f,0.0f);
    wall[2].setPosition(GameWindowSize.x,0.0f);

    // Create sprites
    Ball ball(ResourcesPath + "ball.png");
    Paddle paddle(ResourcesPath + "paddle.png");

    // tiles
    TileTexture.loadFromFile(ResourcesPath + "tile.png");
    Tiles tiles;

    // Fonts
    sf::Font font;
    font.loadFromFile(ResourcesPath + "Arial.ttf");
    sf::Font HSfont;
    HSfont.loadFromFile(ResourcesPath + "CourierNew.ttf");

    // Texts
    sf::Text message("Press the Start button to begin", font);
    message.setFillColor(sf::Color(10,220,250));
    message.setPosition(160.0f,460.0f);

    sf::Text scoreText("      " + name + "\n\n\nBalls Used " + std::to_string(ballsUsed) + "\n\n    Score " + std::to_string(score), font, 24);
    scoreText.setFillColor(sf::Color(200,220,250));
    scoreText.setPosition(790.0f,70.0f);

    std::ostringstream sout;
    sout << highScores << std::endl;
    sf::Text highScoresText(sout.str(), HSfont, 16);
    highScoresText.setFillColor(sf::Color(220,220, 0));
    highScoresText.setPosition(755.0f,350.0f);

    window.draw(highScoresText);

    // Sound effects
    SoundEffect soundEffect;

    // time
    sf::Clock tickClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

    sf::Vector2f joystick = sf::Vector2f(0.f,0.f);

    // the game loop
    while (!gameOver)
    {
        while (window.pollEvent(evnt))
        {
            if (sf::Joystick::isButtonPressed(0,7))   // Start button pressed
            {
                paddle.moveToStartPosition();
                ball.moveToStartPosition();
                gameStarted = true;
                ballsUsed++;
            }
            else if (sf::Joystick::isButtonPressed(0,2))   // 'X' button pressed (pause game)
            {
                gamePaused = true;
                message.setString("Game paused.  Press 'Y' button to continue");
            }
            else if (sf::Joystick::isButtonPressed(0,3))   // 'Y' button pressed (resume game)
            {
                gamePaused = false;
                message.setString("");
            }
            else if (sf::Joystick::isButtonPressed(0,0))   // 'A' button pressed (stop game)
                gameOver = true;

            else {};
            break;
        }

        if (gameStarted && !gamePaused)
        {
            //for paddle movement
            joystick = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));

            timeSinceLastUpdate += tickClock.restart();
            //std::cout << timeSinceLastUpdate.asMilliseconds () << "   " << TimePerFrame.asMilliseconds () << std::endl;
            while (timeSinceLastUpdate > TimePerFrame)
            {
                timeSinceLastUpdate -= TimePerFrame;

                //Ignore slight joystick movement (less than 15%)
                if (joystick.x > 15.f)
                    paddle.move(Paddle::Right);
                if (joystick.x < -15.f)
                    paddle.move(Paddle::Left);

                // Check for paddle hitting the right wall
                if (paddle.getPaddle().getPosition().x > (GameWindowSize.x - paddle.size().x/2.0f))
                {
                    soundEffect[SoundEffect::PaddleHitWall].play();
                    paddle.move(Paddle::Left);
                }

                // Check for paddle hitting the left wall
                if (paddle.getPaddle().getPosition().x < (paddle.size().x/2.0f + WallWidth))
                {
                    soundEffect[SoundEffect::PaddleHitWall].play();
                    paddle.move(Paddle::Right);
                }

                // Check for paddle hitting the ball
                if (paddle.hitTheBall(ball))
                {
                    // Check for all tiles removed
                    //std::cout << score << std::endl;
                    soundEffect[SoundEffect::PaddleHitBall].play();
                    break;
                }
            }

            // move the ball
            if (!gameOver && !gamePaused)
            {
                ball.move();

                // ball hits a tile
                if (ball.hitATile(tiles))
                {
                    soundEffect[SoundEffect::BallHitTile].play();
                    score++;
                    scoreText.setString("      " + name + "\n\n\nBalls Used " + std::to_string(ballsUsed) + "\n\n    Score " + std::to_string(score));
                }

                // ball hits the wall
                else if (ball.hitTheWall())
                    soundEffect[SoundEffect::BallHitWall].play();

                // paddle misses the ball
                else if (ball.getPosition().y > GameWindowSize.y)
                {
                    if (ballsUsed == 3)
                    {
                        message.setCharacterSize(48);
                        message.setFillColor(sf::Color(210,20,20));
                        message.setStyle(sf::Text::Bold);
                        soundEffect[SoundEffect::EndOfGame].play();
                        gameOver = true;
                        message.setString("      Game Over");
                    }
                    else
                    {
                        message.setCharacterSize(36);
                        message.setFillColor(sf::Color(10,220,50));
                        message.setString(std::to_string(3-ballsUsed) + " ball" + ((3-ballsUsed) > 1 ? "s" : "") + " to go.  Get ready ...");
                        ballsUsed++;
                        soundEffect[static_cast<SoundEffect::SoundType>(SoundEffect::PaddleMissBall + (rand()%4))].play();
                        paddle.moveToStartPosition();
                        ball.moveToStartPosition();
                        ball.move();
                    }
                    window.draw(message);
                    window.draw(scoreText);
                    window.display();
                    sf::sleep(sf::Time(sf::seconds(3.0f)));
                }
                else
                {

                }
            }      // if (!gameOver and !gamePaused)
        }          // if (gameStarted)

        // Update the window
        window.clear();
        window.draw(scoreText);
        window.draw(highScoresText);

        if (!gameStarted)
            window.draw(message);
        window.draw(highScoresText);

        for (auto i = 0; i < 3; i++)
            window.draw(wall[i]);                      // draw walls

        window.draw(paddle.getPaddle());               // draw paddle

        for (auto row = 0U; row < Tiles::Rows; row++)  // draw tiles
        {
            for (auto col = 0U; col < Tiles::Cols; col++)
            {
                if (tiles.getTile(row,col).isVisible())
                    window.draw(tiles.getTile(row,col).getSprite());
            }
        }
        window.draw(ball.getBall());                   // draw ball

        // Update the window
        window.display();

    }
    highScores.updateHighScores(Score(name.c_str(),score,time(0)));
    highScores.WriteHighScoresFile();
    sf::sleep(sf::Time(sf::seconds(3.0f)));
    window.close();
}

std::string intro()
{
    std::cout << "Welcome to Joe's Breakout Game\n\n";
    std::cout << "Make sure your game controller is plugged in.\n\n";
    std::cout << "Use the left stick to move the paddle\n\n";
    std::cout << "**********  Buttons  *********\n\n";
    std::cout << "   'Start' to start the game\n";
    std::cout << "   'A' to stop the game\n";
    std::cout << "   'X' to pause the game\n";
    std::cout << "   'Y' to resume the game\n";
    std::cout << "\n";

    std::string name;
    std::cout << "Type your name and press Enter => ";

    std::cin >> name;
//#ifdef _MSC_VER
//#pragma warning(disable : 4244)
//#endif
    name[0] = toupper(name[0]);
#ifdef _MSC_VER
#pragma warning(default : 4244)
#endif
    return name;
}

