#include "car.h"
int main() {
    Car car;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    /*Main Loop*/
    Porsche ps;
    ps.update();
    
    endwin();   
    return 0;
}
