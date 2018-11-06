//
//  main.cpp
//  snake
//
//  Created by Jingxian Shi on 12/13/17.
//  Copyright © 2017 Jingxian Shi. All rights reserved.
//

//Users/jingxianshi/Library/Developer/Xcode/DerivedData/snake-delqcoczmmteykbavmsdwcfcibyf/Build/Products/Debug

#include <iostream>
#include "snake.h"
#include <ncurses.h>
#include <ctime>
using namespace std;


int main(int argc, const char * argv[])
{
    srand(time(0));
    World w;
    
    
    while(!w.gameOver)
    {
        w.print_world();
        w.get_console_input();
        w.play_one_step();
    }
    
    getch();
    endwin();
    
    return 0;
}
