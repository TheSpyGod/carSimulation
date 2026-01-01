#include "car_module.h"

class Porsche {
 
    public:
    Car car;

    void update() {

        while (true) {
            car.tick();
        }
    }
};

int main() {
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
