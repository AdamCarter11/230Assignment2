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
#include <fstream>
#include <vector>
#include <algorithm>

//global variables
Sprite tileSprite;
const int spriteCount = 21;
Texture tileTexture[spriteCount];
Sprite spriteArr[spriteCount];
vector<Sprite> placedSprites;
vector<int> spriteTextures;
int whichSprite;
int tileSize = 70;
int gridWidth = 10, gridHeight = 10;
int windowX = tileSize * gridWidth;
int windowY = tileSize * gridHeight;

//prototypes
void handleInput(RenderWindow& window, Event& e);
void update(RenderWindow& window);
void render(RenderWindow& window);
string convertToString(Sprite spriteToConvert, int position);
string* splitSentence(string stringToSplit);

int main()
{
    //creating a render window to match the sprite size, can be made larger or smaller by changing the grid variables
    RenderWindow window(sf::VideoMode(tileSize*gridWidth, tileSize*gridHeight), "Tilemap Editor");

    //used to load in the sprites into an array
    whichSprite = 0;
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

    //used to place a sprite
    if (Mouse::isButtonPressed(Mouse::Left)) {
        bool canPlace = true;
        int i = 0;
        for (Sprite &X : placedSprites) {
            if (X.getPosition().x == spriteArr[whichSprite].getPosition().x && X.getPosition().y == spriteArr[whichSprite].getPosition().y) {
                X.setTexture(tileTexture[whichSprite]);
                spriteTextures[i] = whichSprite;
                canPlace = false;
            }
            i++;
        }
        //checks if you are trying to erase or not to make sure the value doesn't get overriden
        if (canPlace) {
            placedSprites.push_back(spriteArr[whichSprite]);
            spriteTextures.push_back(whichSprite);
        }
    }

    //used to erase a sprite
    if (Mouse::isButtonPressed(Mouse::Right)) {
        int i = 0;
        for (Sprite& X : placedSprites) {
            if (X.getPosition().x == spriteArr[whichSprite].getPosition().x && X.getPosition().y == spriteArr[whichSprite].getPosition().y) {
                placedSprites.erase(placedSprites.begin() + i);
                spriteTextures.erase(spriteTextures.begin() + i);
            }
            i++;
        }
    }

    //used to cycle through the sprites
    if (Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        whichSprite++;
        if (whichSprite >= spriteCount) {
            whichSprite = 0;
        }
        cout << whichSprite;
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        whichSprite--;
        if (whichSprite < 0) {
            whichSprite = spriteCount - 1;
        }
    }

    //saves a png
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        Texture texture;
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);
        if (texture.copyToImage().saveToFile("Homework2Screenshot.png"))
        {
            std::cout << "Screenshot saved to Homework 2 Screenshot.png" << std::endl;
        }
    }

    //save to a textfile
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        int pos = 0;
        ofstream fileToStreamTo;
        fileToStreamTo.open("level.txt");
        
        for (Sprite X : placedSprites) {
            fileToStreamTo << convertToString(X, pos) << endl;
            pos++;
        }

        fileToStreamTo.close();
        cout << "saved to text file" << endl;
    }

    //load sprites from a text file
    if (Keyboard::isKeyPressed(Keyboard::L)) {
        placedSprites.clear();
        //Creates a stream and opens text file
        ifstream readFile;
        readFile.open("level.txt");
        string line;
        //if the file is open, then continue to read in until end of file
        if (readFile.is_open()) {
            cout << "start" << endl;
            while (!readFile.eof()) {
                while (getline(readFile, line)) {
                    Sprite tempSprite;
                    auto tempString = splitSentence(line);
                    cout << " " << tempString[2] << endl;
                    tempSprite.setTexture(tileTexture[stoi(tempString[0])]);
                    tempSprite.setPosition(stoi(tempString[1]), stoi(tempString[2]));
                    placedSprites.push_back(tempSprite);
                }

            }
        }
        //if the file does not exist (or open), then you get this error message
        else {
            cout << "ERROR: This file doesn't exist!" << endl;
        }
    }
}

/*
*   Used to split a string into words (split based on spaces)
*       Parameters: 
*           stringToSplit - a string, in this case, a sentence that contains 3 seperate values
*       Returns:
*           an array of size 3, each value is a seperate word (in this case, number)
*/
string* splitSentence(string stringToSplit) {
    cout << stringToSplit;
    string* arrayOfWords = new string[3];
    int i = 0;
    string word = "";
    for (auto x : stringToSplit)
    {
        if (x == ' ')
        {
            arrayOfWords[i] = word;
            word = "";
            i++;
        }
        else {
            word = word + x;
            
        }
    }
    arrayOfWords[i] = word;
    word = "";
    return arrayOfWords;
}

//Used to combine values into a string, mainly just a function to keep my code a little cleaner
string convertToString(Sprite spriteToConvert, int position) {
    string stringToReturn;

    stringToReturn = to_string(spriteTextures[position]) + " " + to_string((int)spriteToConvert.getPosition().x) + " " + to_string((int)spriteToConvert.getPosition().y);

    return stringToReturn;
}

void update(RenderWindow& window) {
    //used to make the sprites move on a grid
    //X
    int xPos = Mouse::getPosition().x;
    xPos = xPos - (xPos % 70);
    //Y
    int yPos = Mouse::getPosition().y;
    yPos = yPos - (yPos % 70);
    //Increments of 70 to make it line up properly with the window as the sprites are 70x70
    spriteArr[whichSprite].setPosition(xPos-windowX + 140, yPos-windowY/2 + 140);
}

/*
    Whenever you click with the mouse, pushback a sprite onto a map with the current sprites
        make sure to draw both the current sprite on cursor AND each of the values in the vector
*/
void render(RenderWindow& window) {
    //used for updating the window
    window.clear();

    //the sprite that is being used by the mouse
    window.draw(spriteArr[whichSprite]);

    //the placed sprites
    for (Sprite X : placedSprites) {
        window.draw(X);
    }

    window.display();

}