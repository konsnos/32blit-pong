#include "game.hpp"

using namespace blit;

uint32_t lastTime = 0;

const Pen Black = Pen(0, 0, 0);
const Pen White = Pen(255, 255, 255);

const int paddleWith = 4;
const int paddleHeight = 40;
const float paddleSpeed = 150.0f;
const blit::Pen playerColour(0, 255, 0);
blit::Vec2 playerPosition;
blit::Rect playerRect;
uint8_t playerScore;

const blit::Pen enemyColour(255, 20, 147);
blit::Vec2 enemyPosition;
blit::Rect enemyRect;
uint8_t enemyScore;

const int ballSize = 4;
blit::Vec2 ballPosition;
blit::Rect ballRect;
const blit::Pen ballColour(0, 0, 255);
const float ballInitialSpeed = 80.0f;
const float ballSpeedIncrease = 1.1f;
blit::Vec2 ballVelocity;

void resetSession()
{
    playerPosition = Vec2(6.0f, (240.0f / 2.0f) - (paddleHeight / 2.0f));
    playerRect = Rect(playerPosition.x, playerPosition.y, paddleWith, paddleHeight);

    enemyPosition = Vec2(310.0f, (240.0f / 2.0f) - (paddleHeight / 2.0f));
    enemyRect = Rect(enemyPosition.x, enemyPosition.y, paddleWith, paddleHeight);
    
    ballPosition = Vec2(160.0f - ballSize, (240.0f / 2.0f) - (ballSize / 2.0f));
    ballRect = Rect(ballPosition.x, ballPosition.y, ballSize, ballSize);
    ballVelocity = Vec2(-ballInitialSpeed, 0.0f);
}

void resetGame()
{
    playerScore = 0;
    enemyScore = 0;

    resetSession();
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init()
{
    set_screen_mode(ScreenMode::hires);
    resetGame();
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time)
{
    // reset the screen alpha and clipping mask
    screen.alpha = 255;
    screen.mask = nullptr;

    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.pen = Black;
    screen.clear();

    // draw player
    screen.pen = playerColour;
    screen.rectangle(playerRect);

    // draw enemy
    screen.pen = enemyColour;
    screen.rectangle(enemyRect);

    // draw ball
    screen.pen = ballColour;
    screen.rectangle(ballRect);

    // draw some black text
    screen.pen = White;
    screen.text(std::to_string(playerScore), minimal_font, Point(5, 4));
    screen.text(std::to_string(enemyScore), minimal_font, Point(310, 4));
}

void handlePlayerInput(float dt)
{
    if(buttons.state)
    {
        if(buttons.state & Button::DPAD_UP)
        {
            playerPosition.y -= paddleSpeed * dt;
        }
        else if(buttons.state & Button::DPAD_DOWN)
        {
            playerPosition.y += paddleSpeed * dt;
        }

        if(playerPosition.y < 0)
        {
            playerPosition.y = 0;
        }
        else if(playerPosition.y > 240 - paddleHeight)
        {
            playerPosition.y = 240 - paddleHeight;
        }
        playerRect = Rect(playerPosition.x, playerPosition.y, paddleWith, paddleHeight);
    }
}

void handleBall(float dt)
{
    ballPosition += ballVelocity * dt;
    ballRect = Rect(ballPosition.x, ballPosition.y, ballSize, ballSize);

    if(ballPosition.x < 0)
    {
        enemyScore++;
        blit::debugf("Enemy scored!\n");
        resetSession();
    }
    else if(ballPosition.x > 320 - ballSize)
    {
        playerScore++;
        blit::debugf("Player scored!\n");
        resetSession();
    }
}

void updateEnemy(float dt)
{
    enemyRect = Rect(enemyPosition.x, enemyPosition.y, paddleWith, paddleHeight);
}

bool isColliding(blit::Rect rect1, blit::Rect rect2)
{
    return rect1.x < rect2.x + rect2.w &&
        rect1.x + rect1.w > rect2.x &&
        rect1.y < rect2.y + rect2.h &&
        rect1.y + rect1.h > rect2.y;
}

void checkCollisions(float dt)
{
    if(isColliding(ballRect, playerRect))
    {
        ballVelocity.x = -ballVelocity.x * ballSpeedIncrease;
        ballVelocity.y = ((ballPosition.y + ballSize / 2.0f) - (playerPosition.y + paddleHeight / 2.0f)) * 2.0f;
    }
    else if(isColliding(ballRect, enemyRect))
    {
        ballVelocity.x = -ballVelocity.x * ballSpeedIncrease;
        ballVelocity.y = ((ballPosition.y + ballSize / 2.0f) - (enemyPosition.y + paddleHeight / 2.0f)) * 2.0f;
    }

    if(ballPosition.y < 0)
    {
        ballVelocity.y = -ballVelocity.y;
    }
    else if(ballPosition.y > 240 - ballSize)
    {
        ballVelocity.y = -ballVelocity.y;
    }
}

///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time)
{
    auto dt = (time - lastTime) / 1000.0f;
    handleBall(dt);
    handlePlayerInput(dt);
    updateEnemy(dt);
    checkCollisions(dt);
    lastTime = time;
}