#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

const int EDGE = 80;
const int PADDING = 3;
const int BLOCK = EDGE + 2 * PADDING;
const int SIZE = 6;
const int HEIGHT = BLOCK * SIZE;
const int WIDTH = BLOCK * SIZE;

// g++ -Isrc/include -c unblockMe.cpp
// g++ unblockMe.o -o unblockMe -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

struct Wood {
    int x;
    int y;
    int id;
    int length;
    int direct[2];
    sf::RectangleShape rect;
    Wood() {
        x = 0; y = 0; length = 0; direct[0] = direct[1] = 0; id = 0;
    }
    Wood(int _x, int _y, int _length, int _dir_x, int _dir_y, int _id) : x(_x), y(_y), length(_length) {
        x *= BLOCK;
        y *= BLOCK;
        id = _id;
        length = (length * BLOCK) - 2 * PADDING;
        direct[0] = _dir_x * BLOCK;
        direct[1] = _dir_y * BLOCK;
        rect.setPosition(sf::Vector2f(1.f * x, 1.f * y));
        rect.setFillColor(sf::Color::Yellow);
        rect.setOutlineThickness(1.f*PADDING);
        rect.setOutlineColor(sf::Color::Cyan);
        rect.setOrigin(sf::Vector2f(-1.f*PADDING, -1.f*PADDING));
        if (direct[0] == 0) rect.setSize(sf::Vector2f(1.f * EDGE,1.f* length));
        else rect.setSize(sf::Vector2f(1.f * length, 1.f* EDGE));
    }
    void setPosition(int _X, int _Y) {
        x = _X;
        y = _Y;
        rect.setPosition(sf::Vector2f(1.f * x, 1.f * y));
    }
};

class Game
{
public:
    Game();
    void run();
private:
    void keyPress();
    void mouseClick();
    void processEvents();

    void update();
    void render();
private:    
    sf::RenderWindow mWindow;
    float deltaTime;
    int Map[SIZE][SIZE] = {
        {2,0,3,4,0,9},
        {2,0,3,4,0,9},
        {1,1,0,4,5,0},
        {0,0,0,0,5,8},
        {0,0,7,0,5,8},
        {0,0,7,6,6,0}
    };
    std::vector<Wood> listWood;
    Wood* selecting;
    bool change = false;
    bool running = true;
};
int main()
{
    Game game;
    game.run();
}

Game::Game() : mWindow(sf::VideoMode(WIDTH, HEIGHT), "Unblock Me", sf::Style::Default) {
    deltaTime = 0.f;
    selecting = NULL;
    listWood = std::vector<Wood>(100);
    int cnt = 0;
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (Map[r][c] != 0 && listWood[Map[r][c]].id == 0) {
                int x = c;
                int y = r;
                int leng = 0;
                int direct[2] = { 0,0 };
                //horizon
                int c_ = c;
                while (c_ + 1 < SIZE && Map[r][c_ + 1] == Map[r][c]) c_++;
                if (c_ != c) {
                    direct[0] = 1;
                    leng = c_ - c + 1;
                }
                //vertical
                int r_ = r;
                while (r_ + 1 < SIZE && Map[r_ + 1][c] == Map[r][c]) r_++;
                if (r_ != r) {
                    direct[1] = 1;
                    leng = r_ - r + 1;
                }
                Wood wood(x, y, leng, direct[0], direct[1], Map[r][c]);
                listWood[Map[r][c]] = wood;
                cnt = std::max(cnt, Map[r][c]);
            }
        }
    }
    listWood.push_back(Wood());
    listWood.resize(cnt + 1);
    /*listWood.push_back(Wood(2, 2, 2, 1, 0, listWood.size()));
    listWood.push_back(Wood(0, 0, 3, 0, 1, listWood.size()));
    listWood.push_back(Wood(1, 1, 2, 1, 0, listWood.size()));
    listWood.push_back(Wood(3, 0, 2, 0, 1, listWood.size()));
    listWood.push_back(Wood(4, 2, 3, 0, 1, listWood.size()));
    listWood.push_back(Wood(3, 5, 2, 1, 0, listWood.size()));
    listWood.push_back(Wood(2, 4, 2, 0, 1, listWood.size()));*/
    listWood[1].rect.setFillColor(sf::Color::Red);
}




void Game::run() {
    
    sf::Clock clock_;    
    while (mWindow.isOpen()) {
        deltaTime = clock_.restart().asSeconds();
        render();
        processEvents();
        //keyPress();
        mouseClick();
        update();
        mWindow.display();       
    }
}

void Game::mouseClick() {
    
}
void Game::keyPress() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (selecting->x - selecting->direct[0] * BLOCK >= 0) {
            std::cout << listWood[1].x << ' ';
            selecting->x -= (selecting->direct[0] * BLOCK);
            std::cout << listWood[1].x << '\n';
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (selecting->x + selecting->direct[0] * BLOCK < SIZE) {
            selecting->x += selecting->direct[0] * BLOCK;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (selecting->y + selecting->direct[1] * BLOCK < SIZE) {
            selecting->y += selecting->direct[1] * BLOCK;
        }
    }
}

void Game::processEvents() {
    sf::Event ev;
    while (mWindow.pollEvent(ev)) {        
        switch (ev.type)
        {
        case sf::Event::KeyPressed:
            if (ev.key.code == sf::Keyboard::Escape) {
                mWindow.close();
            }
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        default:
            break;
        }
        if (ev.type == sf::Event::MouseButtonPressed)
        {
            change = true;
            if (ev.mouseButton.button == sf::Mouse::Left)
            {
                int id_row = (int)ev.mouseButton.y / BLOCK;
                int id_col = (int)ev.mouseButton.x / BLOCK;
                //std::cout << id_row << ' ' << id_col << '\n';
                selecting = &listWood[Map[id_row][id_col]];
                //std::cout << selecting;
            }
        }
        if (ev.type == sf::Event::KeyPressed && selecting) {
            if (ev.key.code == sf::Keyboard::Left) {
                if (selecting->x - selecting->direct[0] >= 0 && Map[selecting->y / BLOCK][(selecting->x - selecting->direct[0]) / BLOCK] == 0) {
                    Map[selecting->y / BLOCK][(selecting->x + selecting->length) / BLOCK] = 0;
                    Map[selecting->y / BLOCK][(selecting->x - selecting->direct[0]) / BLOCK] = selecting->id;
                    selecting->x -= (selecting->direct[0]);
                }
            }
            if (ev.key.code == sf::Keyboard::Right) {
                if (selecting->x + selecting->direct[0] < SIZE * BLOCK && Map[selecting->y / BLOCK][(selecting->x + selecting->length * selecting->direct[0] / BLOCK) / BLOCK + 1] == 0) {
                    Map[selecting->y / BLOCK][selecting->x / BLOCK] = 0;
                    Map[selecting->y / BLOCK][(selecting->x + selecting->length * selecting->direct[0] / BLOCK) / BLOCK + 1] = selecting->id;
                    selecting->x += selecting->direct[0];
                }
            }
            if (ev.key.code == sf::Keyboard::Up) {
                if (selecting->y - selecting->direct[1] >= 0 && Map[(selecting->y - selecting->direct[1]) / BLOCK][selecting->x / BLOCK] == 0) {
                    Map[(selecting->y + selecting->length) / BLOCK][selecting->x / BLOCK] = 0;
                    Map[(selecting->y - selecting->direct[1]) / BLOCK][selecting->x / BLOCK] = selecting->id;
                    selecting->y -= selecting->direct[1];
                }
            }
            if (ev.key.code == sf::Keyboard::Down) {
                if (selecting->y + selecting->direct[1] < SIZE * BLOCK && Map[(selecting->y + selecting->length * (selecting->direct[1] / BLOCK)) / BLOCK + 1][selecting->x / BLOCK] == 0) {
                    Map[(selecting->y) / BLOCK][selecting->x / BLOCK] = 0;
                    Map[(selecting->y + selecting->length * (selecting->direct[1] / BLOCK)) / BLOCK + 1][selecting->x / BLOCK] = selecting->id;
                    selecting->y += selecting->direct[1];
                }
            }
            selecting->rect.setPosition(selecting->x, selecting->y);
            /*std::cout << selecting->x << ' ' << selecting->y << '\n';
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    std::cout << Map[i][j] << ' ';
                }std::cout << '\n';
            }*/
        }
    }
}

void Game::update() {
    //std::cout << BLOCK * SIZE - listWood[1].length << ' ' << BLOCK * 2 << '\n';
    if (listWood[1].x == BLOCK * SIZE - listWood[1].length - 2 * PADDING && listWood[1].y == BLOCK * 2) {
        running = false;
    }
}

void Game::render() {
    mWindow.clear(sf::Color::Black);
    if (running) {
        for (int i = 1; i < (int)listWood.size(); i++) {
            mWindow.draw(listWood[i].rect);
            //std::cout << listWood[i].x << ' ' << listWood[i].y << '\n';
           // std::cout << &listWood[i] << '\n';
        }
    }
    else {
        // set font
        sf::Font font;
        if (!font.loadFromFile("./BLKCHCRY.TTF")) return;
        //init text
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::Blue);
        text.setCharacterSize(50);
        text.setString("NEXT\nROUND");
        text.setPosition(WIDTH / 2 - 100, HEIGHT / 2 - 100);
        mWindow.draw(text);
    }
}