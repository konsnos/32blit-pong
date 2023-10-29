#include "game.hpp"

using namespace blit;

const int paddleWith = 4;
const int paddleHeight = 40;

blit::Point playerPosition(6, 240 / 2 - paddleHeight / 2);
blit::Point enemyPosition(310, 240 / 2 - paddleHeight / 2);

const int ballSize = 4;

blit::Point ballPosition(160-ballSize, 120-ballSize);

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init()
{
    set_screen_mode(ScreenMode::hires);
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
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    // draw player
    screen.pen = Pen(0, 255, 0);
    screen.rectangle(Rect(playerPosition.x, playerPosition.y, paddleWith, paddleHeight));

    // draw enemy
    screen.pen = Pen(255, 0, 0);
    screen.rectangle(Rect(enemyPosition.x, enemyPosition.y, paddleWith, paddleHeight));

    // draw ball
    screen.pen = Pen(0, 0, 255);
    screen.rectangle(Rect(ballPosition.x, ballPosition.y, ballSize, ballSize));

    // draw some black text
    // screen.pen = Pen(0, 0, 0);
    // screen.text("Hello PicoSystem!", minimal_font, Point(5, 4));
}

void handlePlayerInput()
{
    if(buttons.state)
    {
        if(buttons.state & Button::DPAD_UP)
        {
            playerPosition.y -= 1;
        }
        else if(buttons.state & Button::DPAD_DOWN)
        {
            playerPosition.y += 1;
        }

        if(playerPosition.y < 0)
        {
            playerPosition.y = 0;
        }
        else if(playerPosition.y > 240 - paddleHeight)
        {
            playerPosition.y = 240 - paddleHeight;
        }
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
    handlePlayerInput();
}