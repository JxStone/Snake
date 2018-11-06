//
//  snake.h
//  snake
//
//  Created by Jingxian Shi on 12/19/17.
//  Copyright © 2017 Jingxian Shi. All rights reserved.
//
#include <iostream>
#include <vector>
#include <ncurses.h>
using namespace std;

enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
enum Snake_type {HEAD, TAIL, FOOD, EMPTY};

struct Position
{
    Position(int a, int b) : x(a), y(b) {}
    int x;
    int y;
};

class Snake;
typedef Snake* Snake_ptr;

class Snake
{
public:
    Snake() : position(0,0) {}
    virtual ~Snake() {}
    Snake(int x, int y) : position(x,y) {}
    virtual Position get_position() { return position; }
    void set_position(Position pos) { position = pos; }
    virtual void draw() = 0;
    virtual Snake_type who() = 0;
private:
    Position position;
};

class Head : public Snake
{
public:
    Head() {}
    Head(int x, int y) : Snake(x,y) {}
    void draw();
    Snake_type who() {return HEAD;}
private:
};

class Tail : public Snake
{
public:
    Tail() {}
    Tail(int x, int y) : Snake(x,y) {}
    void draw();
    Snake_type who() {return TAIL;}
private:
};

class Empty : public Snake
{
public:
    Empty() {}
    Empty(int x, int y) : Snake(x, y) {}
    void draw();
    Snake_type who() {return EMPTY;}
private:
};

class Food : public Snake
{
public:
    Food() {}
    Food(int x, int y) : Snake(x,y) {}
    void draw();
    Snake_type who() {return FOOD;}
private:
};


class World
{
public:
    World();
    int get_width() {return width;}
    int get_height() {return height;}
    void print_world();
    void get_console_input();
    void play_one_step();
    bool gameOver;
private:
    int width;
    int height;
    int headX;
    int headY;
    int foodX;
    int foodY;
    Direction direction;
    int score;
    vector<Snake_ptr> tail;
public:
    Snake_ptr** grid;
};

void Head::draw()
{
    mvprintw(get_position().y, get_position().x, "O");
}

void Tail::draw()
{
    mvprintw(get_position().y, get_position().x, "0");
}

void Food::draw()
{
    mvprintw(get_position().y, get_position().x, "$");
}

void Empty::draw()
{
    //mvprintw(get_position().y, get_position().x, "");
}


World::World()
{
    initscr();
    clear();
    noecho();
    //cbreak();
    curs_set(false);
    
    width = 20;
    height = 20;
    gameOver = false;
    direction = STOP;
    headX = width/2;
    headY = height/2;
    foodX = (rand()%(width-1))+ 1;
    foodY = (rand()%(height-1)) + 1;
    score = 0;
    grid = new Snake_ptr*[height];

    for (int i = 0; i < height; i++)
    {
        grid[i] = new Snake_ptr[width];
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(headX == j && headY == i)
            {
                grid[i][j] = new Head(j, i);
            }
            else if(foodX == j && foodY == i)
            {
                grid[i][j] = new Food(j, i);
            }
            else if(headX == j && headY + 1 == i)
            {
                grid[i][j] = new Tail(j,i);
                tail.push_back(grid[i][j]);
            }
            else
            {
                grid[i][j] = new Empty(j, i);
            }
        }
    }
}

void World::print_world()
{
    clear();
    
    for(int i = 0; i <= get_height(); i++)
    {
        for(int j = 0; j <= get_width(); j++)
        {
            if(i == 0 || i == get_height())
            {
                mvprintw(i, j, "-");
            }
            else if(j == 0 || j == get_width())
            {
                mvprintw(i, j, "|");
            }
            else
            {
                grid[i][j]->draw();
            }
        }
    }
    mvprintw(23, 0, "Score: %d", score);
    
    refresh();
}

void World::get_console_input()
{
    keypad(stdscr, TRUE);
    halfdelay(3);
    
    int c = getch();
    
    switch (c)
    {
        case KEY_LEFT:
            direction = LEFT;
            break;
        case KEY_RIGHT:
            direction = RIGHT;
            break;
        case KEY_UP:
            direction = UP;
            break;
        case KEY_DOWN:
            direction = DOWN;
            break;
        case 101:
            gameOver =  TRUE;
            break;
    }
}

void World::play_one_step()
{
    int x1 = headX;
    int y1 = headY;

    switch (direction)
    {
        case LEFT:
            x1--;
            break;
        case RIGHT:
            x1++;
            break;
        case UP:
            y1--;
            break;
        case DOWN:
            y1++;
            break;
        default:
            break;
    }
    
    if(x1 < 1 || x1 >= width || y1 < 1 || y1 >= height)
    {
        gameOver = TRUE;
    }
    else
    {
        if(grid[y1][x1]->who() == FOOD)
        {
            score++;
            
            delete grid[headY][headX];
            grid[headY][headX] = new Tail(headX, headY);
            tail.push_back(grid[headY][headX]);

            delete grid[y1][x1];
            grid[y1][x1] = new Head(headX, headY);
            headX = x1;
            headY = y1;
            
            do
            {
                foodX = (rand()%(width-1))+ 1;
                foodY = (rand()%(height-1)) + 1;
            }while(grid[foodY][foodX]->who() != EMPTY);

            delete grid[foodY][foodX];
            grid[foodY][foodX] = new Food(foodX, foodY);
        }
        else if(grid[y1][x1]->who() == EMPTY)
        {
            int x = tail[0]->get_position().x;
            int y = tail[0]->get_position().y;
            delete grid[y][x];
            grid[y][x] = new Empty(y, x);
            tail.erase(tail.begin());

            delete grid[headY][headX];
            grid[headY][headX] = new Tail(headX, headY);
            tail.push_back(grid[headY][headX]);

            delete grid[y1][x1];
            grid[y1][x1] = new Head(x1, y1);
            headX = x1;
            headY = y1;
        }
        else if(grid[y1][x1]->who() == TAIL)
        {
            gameOver = true;
        }
    }
    
}
