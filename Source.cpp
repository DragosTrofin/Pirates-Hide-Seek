
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include "RoundedRectangle.h"

struct Piece
{
    sf::RectangleShape* rect;
    bool selected{ false };
    int index{ -1 };
};

int fr[9];
int frTarget[9];
float pieceSize{ 175.f };
int hovered{ -1 };
int theme{ 1 };
std::map<Piece*, int> PieceToCadran;
std::map<int, Piece*> CadranToPiece;
sf::Sprite compass;
sf::Sprite* icons[8]{};
int compassOrientation;
bool showBackScreen{ false };
bool youWon{ false };

sf::Texture shipreckTexture;
sf::Texture barrelTexture;
sf::Texture shipTexture;
sf::Texture pirateShipTexture;
sf::Texture treasureTexture;
sf::Texture deadIslandTexture;
sf::Texture towerTexture;
sf::Texture krackenTexture;
sf::Texture compassTexture;

enum class Interface
{
    MainScreen,
    SelectLevel,
    Game,
    Settings
};

Interface interface { Interface::MainScreen };

int m[4][3][3]
{
    {
        { 1, 1, 0 },
        { 1, 1, 1 },
        { 0, 1, 1 },
    },
    {
        { 1, 0, 1 },
        { 1, 1, 1 },
        { 1, 0, 1 },
    },
    {
        { 1, 0, 1 },
        { 1, 0, 1 },
        { 1, 1, 1 },
    },
    {
        { 1, 1, 0 },
        { 1, 1, 1 },
        { 1, 0, 1 },
    }
};


int mTable[4][3][3]
{
    {
        { 0, 2, 1 },
        { 3, 0, 5 },
        { 8, 0, 0 },
    },
    {
        { 2, 7, 0 },
        { 0, 3, 5 },
        { 8, 4, 0 },
    },
    {
        { 2, 1, 4 },
        { 3, 5, 0 },
        { 4, 7, 6 },
    },
    {
        { 0, 0, 0 },
        { 7, 2, 6 },
        { 3, 4, 5 },
    }
};

void rotate(int m[3][3])
{
    int aux[3][3]{};

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            aux[j][3 - i - 1] = m[i][j];

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = aux[i][j];
}

void CheckHover(Piece* piece, sf::Vector2i coords, sf::RectangleShape q[4])
{
    piece->rect->setScale(290.f / pieceSize, 290.f / pieceSize);

    for (int i{ 0 }; i < 4; i++)
    {
        if (q[i].getGlobalBounds().contains({(float)coords.x, (float)coords.y}) && !CadranToPiece[i])
        {
            piece->rect->setPosition(q[i].getPosition());
            hovered = i;
            return;
        }
    }

    piece->rect->setPosition(coords.x, coords.y);
    hovered = -1;
}

void LoadLevel(int lvl)
{
    std::ifstream in("lvl" + std::to_string(lvl) + ".txt");
    std::string type;

    for (int i{ 0 }; i < 8; i++)
    {
        delete icons[i];
        icons[i] = nullptr;
        frTarget[i + 1] = 0;
    }

    int i{ 0 };
    while (in >> type)
    {
        compassOrientation = std::atoi(type.c_str());
        icons[i] = new sf::Sprite;

        if (type == "Shipwreck")
        {
            icons[i]->setTexture(shipreckTexture);
            frTarget[1]++;
        }
        else if (type == "Barrel")
        {
            icons[i]->setTexture(barrelTexture);
            frTarget[2]++;
        }
        else if (type == "Ship")
        {
            icons[i]->setTexture(shipTexture);
            frTarget[3]++;
        }
        else if (type == "Pirate_Ship")
        {
            icons[i]->setTexture(pirateShipTexture);
            frTarget[4]++;
        }
        else if (type == "Treasure")
        {
            icons[i]->setTexture(treasureTexture);
            frTarget[5]++;
        }
        else if (type == "Dead_Island")
        {
            icons[i]->setTexture(deadIslandTexture);
            frTarget[6]++;
        }
        else if (type == "Tower")
        {
            icons[i]->setTexture(towerTexture);
            frTarget[7]++;
        }
        else if (type == "Kraken")
        {
            icons[i]->setTexture(krackenTexture);
            frTarget[8]++;
        }
        i++;
    }
    compass.rotate((compassOrientation - 1) * 90);

    float offset{ (900 - ((i - 1) * 80 + 20 * (i - 2))) / 2.f };
    for (int j{ 0 }; j < i; j++)
    {
        icons[j]->setPosition(200, offset + j * 100);
    }
}

int main()
{
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;;
    sf::RenderWindow window(sf::VideoMode(1500, 900), "Pirates Hide and Seek", sf::Style::Default, windowSettings);
    window.setFramerateLimit(60);

    sf::RectangleShape table({ 750.f, 750.f });
    table.setPosition(750.f, 450.f);
    table.setOrigin(table.getSize().x / 2.f, table.getSize().y / 2.f);

    float qSize{ 282.f };
    sf::RectangleShape q1({ qSize, qSize });
    q1.setOrigin(q1.getSize().x / 2.f, q1.getSize().y / 2.f);

    sf::RectangleShape q2{ q1 };
    sf::RectangleShape q3{ q1 };
    sf::RectangleShape q4{ q1 };

    q1.setPosition(450.f + qSize / 2.f, 150.f + qSize / 2.f);
    q2.setPosition(768.f + qSize / 2.f, 150.f + qSize / 2.f);
    q3.setPosition(450.f + qSize / 2.f, 468.f + qSize / 2.f);
    q4.setPosition(768.f + qSize / 2.f, 468.f + qSize / 2.f);

    sf::RectangleShape q[4]{ q1,q2,q3,q4 };

    sf::Texture texture;
    texture.loadFromFile("Theme1/tabla.png");
    table.setTexture(&texture);

    sf::RectangleShape piece1({ pieceSize, pieceSize });
    piece1.setOrigin(pieceSize / 2.f, pieceSize / 2.f);

    sf::RectangleShape piece2{ piece1 };
    sf::RectangleShape piece3{ piece1 };
    sf::RectangleShape piece4{ piece1 };

    piece1.setPosition(1320.f, 150.f);
    piece2.setPosition(1320.f, 350.f);
    piece3.setPosition(1320.f, 550.f);
    piece4.setPosition(1320.f, 750.f);

    sf::Texture texture1;
    texture1.loadFromFile("Theme1/Piesa1.png");

    sf::Texture texture2;
    texture2.loadFromFile("Theme1/Piesa2.png");

    sf::Texture texture3;
    texture3.loadFromFile("Theme1/Piesa3.png");

    sf::Texture texture4;
    texture4.loadFromFile("Theme1/Piesa4.png");

    piece1.setTexture(&texture1);
    piece2.setTexture(&texture2);
    piece3.setTexture(&texture3);
    piece4.setTexture(&texture4);

    Piece p1{ &piece1 };
    Piece p2{ &piece2 };
    Piece p3{ &piece3 };
    Piece p4{ &piece4 };

    Piece* p[4]{ &p1,&p2,&p3,&p4 };

    for (int i{ 0 }; i < 4; i++)
    {
        PieceToCadran[p[i]] = -1;
        p[i]->index = i;
    }

    sf::Texture outlineTexture;
    outlineTexture.loadFromFile("Theme2/Outline.png");
    sf::RectangleShape outline;
    outline.setTexture(&outlineTexture);
    outline.setSize((sf::Vector2f)window.getSize());

    shipreckTexture.loadFromFile("Theme1/Shipwreck.png");
    barrelTexture.loadFromFile("Theme1/Barrel.png");
    shipTexture.loadFromFile("Theme1/Ship.png");
    pirateShipTexture.loadFromFile("Theme1/Pirate_Ship.png");
    treasureTexture.loadFromFile("Theme1/Treasure.png");
    deadIslandTexture.loadFromFile("Theme1/Dead_Island.png");
    towerTexture.loadFromFile("Theme1/Tower.png");
    krackenTexture.loadFromFile("Theme1/Kraken.png");
    compassTexture.loadFromFile("Theme1/Compass.png");

    compass.setTexture(compassTexture);
    compass.setOrigin(compass.getGlobalBounds().width / 2.f, compass.getGlobalBounds().height / 2.f);
    compass.setPosition(100.f, 450.f);
    compass.setScale(1.4f, 1.4f);

    sf::SoundBuffer buffer;
    buffer.loadFromFile("click.wav");
    sf::Sound sound;
    sound.setBuffer(buffer);

    sf::SoundBuffer buffer2;
    buffer2.loadFromFile("rotire.wav");
    sf::Sound sound2;
    sound2.setBuffer(buffer2);

    sf::Music music;
    music.openFromFile("theme_song.wav");
    music.play();
    music.setLoop(true);

    sf::Music music2;
    music2.openFromFile("waves.wav");
    music2.play();
    music2.setLoop(true);

    sf::Texture backgroundGameTexture;
    backgroundGameTexture.loadFromFile("Theme1/GameB.png");
    sf::RectangleShape gameBackground;
    gameBackground.setTexture(&backgroundGameTexture);
    gameBackground.setSize((sf::Vector2f)window.getSize());

    sf::Font font;
    font.loadFromFile("Lora.ttf");


    //Main Screen
    sf::Texture mainScreenTexture;
    mainScreenTexture.loadFromFile("Theme1/Cover1B.png");
    sf::RectangleShape mainScreenBackground;
    mainScreenBackground.setTexture(&mainScreenTexture);
    mainScreenBackground.setSize((sf::Vector2f)window.getSize());

    RoundedRectangle play({ 400.f, 80.f }, 40.f);
    play.setOrigin(play.getSize().x / 2.f, play.getSize().y / 2.f);
    play.setPosition(750.f, 600.f);
    play.setFillColor(sf::Color(255, 197, 0, 230));
    sf::Text playText("Play", font, 50);
    playText.setOrigin(playText.getGlobalBounds().width / 2.f, playText.getGlobalBounds().height / 2.f);
    playText.setPosition(750.f, play.getPosition().y - 10.f);
    playText.setFillColor(sf::Color::Black);

    RoundedRectangle settings(play);
    settings.setPosition(750.f, 700.f);
    sf::Text settingsText(playText);
    settingsText.setString("Settings");
    settingsText.setOrigin(settingsText.getGlobalBounds().width / 2.f, settingsText.getGlobalBounds().height / 2.f);
    settingsText.setPosition(750.f, settings.getPosition().y - 10.f);

    RoundedRectangle exit(play);
    exit.setPosition(750.f, 800.f);
    sf::Text exitText(playText);
    exitText.setString("Exit");
    exitText.setOrigin(exitText.getGlobalBounds().width / 2.f, exitText.getGlobalBounds().height / 2.f);
    exitText.setPosition(750.f, exit.getPosition().y - 10.f);

    sf::Text backToLevelMenu("Back to Level Menu", font, 80);
    backToLevelMenu.setOrigin(backToLevelMenu.getGlobalBounds().width / 2.f, backToLevelMenu.getGlobalBounds().height / 2.f);
    backToLevelMenu.setPosition(750.f, 450.f);

    sf::Text youWonText("You Won!", font, 80);
    youWonText.setOrigin(youWonText.getGlobalBounds().width / 2.f, youWonText.getGlobalBounds().height / 2.f);
    youWonText.setPosition(750.f, 450.f);

    //Level Screen
    sf::Texture levelScreenTexture;
    levelScreenTexture.loadFromFile("Theme1/Cover2B.png");
    sf::RectangleShape levelScreenBackground;
    levelScreenBackground.setTexture(&levelScreenTexture);
    levelScreenBackground.setSize((sf::Vector2f)window.getSize());

    RoundedRectangle levels[10];
    sf::Text levelsText[10];
    for (int i{ 0 }; i < 10; i++)
    {
        levels[i].setSize({ 125.f, 100.f });
        levels[i].setOrigin(levels[i].getSize().x / 2.f, levels[i].getSize().y / 2.f);
        levels[i].setFillColor(sf::Color(255, 197, 0, 230));
        levels[i].setCornerRadius(30.f);

        levelsText[i].setFont(font);
        levelsText[i].setString(std::to_string(i+1));
        levelsText[i].setCharacterSize(70);
        levelsText[i].setStyle(sf::Text::Style::Bold);
        levelsText[i].setOrigin(levelsText[i].getGlobalBounds().width / 2.f, levelsText[i].getGlobalBounds().height / 2.f);
        levelsText[i].setFillColor(sf::Color::Black);
    }

    levels[0].setPosition(500.f, 250.f);
    levels[1].setPosition(750.f, 270.f);
    levels[2].setPosition(1020.f, 250.f);
    levels[3].setPosition(400.f, 450.f);
    levels[4].setPosition(650.f, 470.f);
    levels[5].setPosition(900.f, 470.f);
    levels[6].setPosition(1140.f, 450.f);
    levels[7].setPosition(490.f, 650.f);
    levels[8].setPosition(760.f, 700.f);
    levels[9].setPosition(1020.f, 650.f);

    for (int i{ 0 }; i < 10; i++)
    {
        levelsText[i].setPosition(levels[i].getPosition().x, levels[i].getPosition().y - 20.f);
    }

    //Settings Screen
    RoundedRectangle changeTheme(play);
    changeTheme.setPosition(750.f, 300.f);
    sf::Text changeThemeText(playText);
    changeThemeText.setString("Change Theme");
    changeThemeText.setOrigin(changeThemeText.getGlobalBounds().width / 2.f, changeThemeText.getGlobalBounds().height / 2.f);
    changeThemeText.setPosition(750.f, changeTheme.getPosition().y - 10.f);

    RoundedRectangle toggleMusic(play);
    toggleMusic.setPosition(750.f, 600.f);
    sf::Text toggleMusicText(playText);
    toggleMusicText.setString("Toggle Music");
    toggleMusicText.setOrigin(toggleMusicText.getGlobalBounds().width / 2.f, toggleMusicText.getGlobalBounds().height / 2.f);
    toggleMusicText.setPosition(750.f, toggleMusic.getPosition().y - 10.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (interface == Interface::Game)
                    {
                        for (int i{ 0 }; i < 4; i++)
                        {
                            if (p[i]->selected)
                            {
                                sound2.play();
                                p[i]->rect->rotate(90);
                                rotate(m[i]);
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    switch (interface)
                    {
                        case Interface::MainScreen:
                        {
                            if (play.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                            {
                                interface = Interface::SelectLevel;
                            }
                            if (settings.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                            {
                                interface = Interface::Settings;
                            }
                            if (exit.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                            {
                                window.close();
                            }
                            break;
                        }
                        case Interface::SelectLevel:
                        {
                            for (int i{ 0 }; i < 10; i++)
                            {
                                if (levels[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                {
                                    LoadLevel(i+1);
                                    interface = Interface::Game;
                                }
                            }
                            break;
                        }
                        case Interface::Game:
                        {
                            if (showBackScreen)
                            {
                                if (backToLevelMenu.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                {
                                    interface = Interface::SelectLevel;
                                    showBackScreen = false;

                                    p[0]->rect->setPosition(1320.f, 150.f);
                                    p[1]->rect->setPosition(1320.f, 350.f);
                                    p[2]->rect->setPosition(1320.f, 550.f);
                                    p[3]->rect->setPosition(1320.f, 750.f);

                                    for (int i{ 0 }; i < 4; i++)
                                    {
                                        p[i]->rect->setScale(1.f, 1.f);
                                        CadranToPiece[PieceToCadran[p[i]]] = nullptr;
                                        PieceToCadran[p[i]] = -1;
                                        int rotations = (360.f - p[i]->rect->getRotation()) / 90;
                                        p[i]->rect->setRotation(0);
                                        for (int j{ 0 }; j < rotations; j++)
                                        {
                                            rotate(m[i]);
                                        }
                                    }
                                }
                            }
                            else if (!youWon)
                            {
                                for (int i{ 0 }; i < 4; i++)
                                {
                                    if (!p[i]->selected)
                                    {
                                        if (p[i]->rect->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                                        {
                                            p[i]->selected = true;
                                            p[i]->rect->setFillColor(sf::Color(255, 255, 255, 128));
                                            CadranToPiece[PieceToCadran[p[i]]] = nullptr;
                                            PieceToCadran[p[i]] = -1;
                                            sound.play();
                                        }
                                    }
                                    else if (hovered != -1)
                                    {
                                        p[i]->selected = false;
                                        p[i]->rect->setFillColor(sf::Color(255, 255, 255, 255));
                                        CadranToPiece[hovered] = p[i];
                                        PieceToCadran[p[i]] = hovered;
                                        sound.play();
                                    }
                                    else
                                    {
                                        switch (i)
                                        {
                                            case 0:
                                            {
                                                p[i]->rect->setPosition(1320.f, 150.f);
                                                break;
                                            }
                                            case 1:
                                            {
                                                p[i]->rect->setPosition(1320.f, 350.f);
                                                break;
                                            }
                                            case 2:
                                            {
                                                p[i]->rect->setPosition(1320.f, 550.f);
                                                break;
                                            }
                                            case 3:
                                            {
                                                p[i]->rect->setPosition(1320.f, 750.f);
                                                break;
                                            }
                                        }
                                        sound.play();
                                        p[i]->rect->setScale(1.f, 1.f);
                                        p[i]->selected = false;
                                        p[i]->rect->setFillColor(sf::Color(255, 255, 255, 255));

                                        int rotations = (360.f - p[i]->rect->getRotation()) / 90;
                                        p[i]->rect->setRotation(0);
                                        for (int j{ 0 }; j < rotations; j++)
                                        {
                                            rotate(m[i]);
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case Interface::Settings:
                        {
                            if (toggleMusic.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                            {
                                if (music.getStatus() == sf::SoundSource::Paused)
                                {
                                    music.play();
                                    music2.play();
                                }
                                else
                                {
                                    music.pause();
                                    music2.pause();
                                }
                            }
                            if (changeTheme.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                            {
                                std::string themeString;
                                if (theme == 1)
                                {
                                    themeString = "Theme2";
                                    play.setFillColor(sf::Color(150, 106, 0, 150));
                                    settings.setFillColor(sf::Color(150, 106, 0, 150));
                                    exit.setFillColor(sf::Color(150, 106, 0, 150));
                                    changeTheme.setFillColor(sf::Color(150, 106, 0, 150));
                                    toggleMusic.setFillColor(sf::Color(150, 106, 0, 150));

                                    for (int i{ 0 }; i < 10; i++)
                                    {
                                        levels[i].setFillColor(sf::Color(150, 106, 0, 190));
                                    }
                                    theme = 2;
                                }
                                else
                                {
                                    themeString = "Theme1";
                                    play.setFillColor(sf::Color(255, 197, 0, 230));
                                    settings.setFillColor(sf::Color(255, 197, 0, 230));
                                    exit.setFillColor(sf::Color(255, 197, 0, 230));
                                    changeTheme.setFillColor(sf::Color(255, 197, 0, 230));
                                    toggleMusic.setFillColor(sf::Color(255, 197, 0, 230));

                                    for (int i{ 0 }; i < 10; i++)
                                    {
                                        levels[i].setFillColor(sf::Color(255, 197, 0, 230));
                                    }
                                    theme = 1;
                                }

                                backgroundGameTexture.loadFromFile(themeString + "/GameB.png");
                                mainScreenTexture.loadFromFile(themeString + "/Cover1B.png");
                                levelScreenTexture.loadFromFile(themeString + "/Cover2B.png");
                                shipreckTexture.loadFromFile(themeString + "/Shipwreck.png");
                                barrelTexture.loadFromFile(themeString + "/Barrel.png");
                                shipTexture.loadFromFile(themeString + "/Ship.png");
                                pirateShipTexture.loadFromFile(themeString + "/Pirate_Ship.png");
                                treasureTexture.loadFromFile(themeString + "/Treasure.png");
                                deadIslandTexture.loadFromFile(themeString + "/Dead_Island.png");
                                towerTexture.loadFromFile(themeString + "/Tower.png");
                                krackenTexture.loadFromFile(themeString + "/Kraken.png");
                                compassTexture.loadFromFile(themeString + "/Compass.png");
                                texture1.loadFromFile(themeString + "/Piesa1.png");
                                texture2.loadFromFile(themeString + "/Piesa2.png");
                                texture3.loadFromFile(themeString + "/Piesa3.png");
                                texture4.loadFromFile(themeString + "/Piesa4.png");
                            }
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::Resized)
            {
                sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
                window.setView(view);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (youWon)
                {
                    interface = Interface::MainScreen;
                    showBackScreen = false;

                    p[0]->rect->setPosition(1320.f, 150.f);
                    p[1]->rect->setPosition(1320.f, 350.f);
                    p[2]->rect->setPosition(1320.f, 550.f);
                    p[3]->rect->setPosition(1320.f, 750.f);

                    for (int i{ 0 }; i < 4; i++)
                    {
                        p[i]->rect->setScale(1.f, 1.f);
                        CadranToPiece[PieceToCadran[p[i]]] = nullptr;
                        PieceToCadran[p[i]] = -1;
                        int rotations = (360.f - p[i]->rect->getRotation()) / 90;
                        p[i]->rect->setRotation(0);
                        for (int j{ 0 }; j < rotations; j++)
                        {
                            rotate(m[i]);
                        }
                    }
                    youWon = false;
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (interface == Interface::SelectLevel) interface = Interface::MainScreen;
                    if (interface == Interface::Settings) interface = Interface::MainScreen;
                    if (interface == Interface::Game)
                    {
                        showBackScreen = !showBackScreen;
                    }
                }
            }
        }

        for (int k{ 0 }; k < 4; k++) //cele 4 cadrane
        {
            for (int i{ 0 }; i < 3; i++)
            {
                for (int j{ 0 }; j < 3; j++)
                {
                    if (CadranToPiece[k])
                    {
                        if (!m[CadranToPiece[k]->index][i][j])
                        {
                            fr[mTable[k][i][j]]++;
                        }
                    }
                    else fr[mTable[k][i][j]]++;
                }
            }
        }
        bool win{ true };
        for (int i{ 1 }; i < 9; i++)
        {
            if (frTarget[i] != fr[i]) win = false;
            fr[i] = 0;
        }
        if (win && interface == Interface::Game) youWon = true;

        window.clear(sf::Color::White);

        switch (interface)
        {
            case Interface::MainScreen:
            {
                window.draw(mainScreenBackground);
                window.draw(play);
                window.draw(playText);
                window.draw(settings);
                window.draw(settingsText);
                window.draw(exit);
                window.draw(exitText);
                break;
            }
            case Interface::Settings:
            {
                window.draw(levelScreenBackground);
                window.draw(changeTheme);
                window.draw(changeThemeText);
                window.draw(toggleMusic);
                window.draw(toggleMusicText);
                break;
            }
            case Interface::SelectLevel:
            {
                window.draw(levelScreenBackground);
                for (int i{ 0 }; i < 10; i++)
                {
                    window.draw(levels[i]);
                    window.draw(levelsText[i]);
                }
                break;
            }
            case Interface::Game:
            {
                for (int i{ 0 }; i < 4; i++)
                {
                    if (p[i]->selected) CheckHover(p[i], sf::Mouse::getPosition(window), q);
                }

                window.draw(gameBackground);
                if (theme == 1) window.draw(table);
                if (compassOrientation)
                {
                    window.draw(compass);
                }
                for (int i{ 0 }; i < 8; i++)
                {
                    if (icons[i]) window.draw(*icons[i]);
                }

                window.draw(piece1);
                window.draw(piece2);
                window.draw(piece3);
                window.draw(piece4);

                if (theme == 2) window.draw(outline);

                sf::RectangleShape rect(sf::Vector2f(1500, 900));
                rect.setFillColor(sf::Color(0, 0, 0, 128));

                if (showBackScreen)
                {
                    window.draw(rect);
                    window.draw(backToLevelMenu);
                }
                if (youWon)
                {
                    window.draw(rect);
                    window.draw(youWonText);
                }
                break;
            }
        }

        window.display();
    }

    return 0;
}