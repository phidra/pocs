#include <iostream>
#include <chrono>
#include <thread>
#include <ncurses.h>

// see also "mazecast" project

using namespace std;

int main(int argc, char* argv[])
{
    // init ncurses :
    initscr();
    noecho();
    curs_set(FALSE);

    // window limits :
    int max_x = 0;
    int max_y = 0;
    getmaxyx(stdscr, max_y, max_x);

    int displayed_x = 0;
    int displayed_y = 0;
    
    while (true)
    {
        // background :
        for (int y = 0; y < max_y; ++y)
        {
            for (int x = 0; x < max_x; ++x)
            {
                string char_to_display = (x % 4 == 0 ? "|" : " ");
                mvprintw(y, x, char_to_display.c_str());
            }
        }

        // displaying :
        const string TO_DISPLAY = "...oooOOOooo...";
        mvprintw(displayed_y % max_y, displayed_x % max_x, TO_DISPLAY.c_str());

        refresh();

        this_thread::sleep_for(chrono::milliseconds(5));
        ++displayed_x;
        displayed_y = (displayed_x / max_x);
    }

    // restore normal terminal behaviour
    this_thread::sleep_for(chrono::seconds(2));
    endwin();

}
