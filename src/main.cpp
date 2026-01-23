#include "car.h"
int main() {
    Car car;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    /*Main Loop*/
    for (;;) {
        car.tick();
    }

    endwin();   
    return 0;
}
