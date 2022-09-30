//Always include these headers *******
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
// ************************************
using namespace sf;
using namespace std;
#include <string>
#include <iostream>

//global variables

Sprite tileSprite;
const int spriteCount = 21;
Texture tileTexture[spriteCount];
Sprite spriteArr[spriteCount];

//prototypes
void handleInput(RenderWindow& window, Event& e);
void update(RenderWindow& window);
void render(RenderWindow& window);

int main()
{
    //creating a render window
    RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

    //create and fill circle
    CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    //Sprite
    /*
    if (!tileTexture.loadFromFile("Assets/sprites/Platformer-0.png")) {
        //file didn't load properly
        return -1;
    }
    tileSprite.setTexture(tileTexture);
    tileSprite.setPosition(365, 530);
    */
    
    for (int i = 0; i < spriteCount; i++) {
        string tempFileName = ("Assets/sprites/Platformer-" + to_string(i) + ".png");
        cout << tempFileName << endl;
        if (!tileTexture[i].loadFromFile(tempFileName)) {
            //if the sprite is loaded incorrectly
            return -1;
        }
        tileSprite.setTexture(tileTexture[i]);
        spriteArr[i] = tileSprite;     
    }
    

    //copy paste into every project (it's used for quitting)
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            handleInput(window, event);

        }
        update(window);
        render(window);
    }

    return 0;
}

void handleInput(RenderWindow& window, Event& e) {
    if (e.type == Event::Closed)
        window.close();

    //play sfx on mouse click
    //e.type == Event::MouseButtonPressed
    if (Mouse::isButtonPressed(Mouse::Left)) {
        
    }
}

void update(RenderWindow& window) {
    spriteArr[1].setPosition(Mouse::getPosition().x - 600, Mouse::getPosition().y - 310);
}

/*
    Whenever you click with the mouse, pushback a vector with the current sprite
        make sure to draw both the current sprite on cursor AND each of the values in the vector
*/
void render(RenderWindow& window) {
    //used for updating the window
    window.clear();

    window.draw(spriteArr[1]);
    window.draw(spriteArr[2]);

    window.display();

}