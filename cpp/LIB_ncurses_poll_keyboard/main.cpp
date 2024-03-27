#include <iostream>
#include <chrono>
#include <thread>
#include <ncurses.h>
#include <sys/poll.h>
#include <unistd.h>

// this is a POC on poll usage to get key pressed.

int main(int argc, char* argv[]) {
    struct pollfd pfdArray[1];
    pfdArray[0].fd = 0;
    pfdArray[0].events = POLLIN;

    // init ncurses :
    initscr();
    noecho();
    curs_set(FALSE);

    // window limits :
    int max_x = 0;
    int max_y = 0;
    getmaxyx(stdscr, max_y, max_x);

    unsigned int counter = 0;
    const std::string WHEN_KEYPRESSED(max_x, '=');
    const std::string BLANK(max_x, ' ');
    bool exit_asked = false;
    bool keypressed = false;
    char buffer[16];

    while (!exit_asked) {
        // is there a key pressed ?
        poll(pfdArray, 1, 5);
        if (pfdArray[0].revents & POLLIN) {
            keypressed = true;
            read(0, buffer, 16);
            exit_asked = (buffer[0] == 'q');
        }

        // default display = blank
        for (int y = max_y / 2 - 1; y <= max_y / 2 + 1; ++y) {
            mvprintw(y, 0, BLANK.c_str());
        }

        // key pressed display :
        if (keypressed) {
            for (int y = max_y / 2 - 1; y <= max_y / 2 + 1; ++y) {
                mvprintw(y, 0, WHEN_KEYPRESSED.c_str());
            }
            keypressed = false;
        }

        // spinning chars :
        for (int x = max_x / 2 - 1; x <= max_x / 2 + 1; ++x) {
            unsigned int modulo = counter % 4;
            char spin = '.';
            if (modulo == 0)
                spin = '|';
            else if (modulo == 1)
                spin = '/';
            else if (modulo == 2)
                spin = '-';
            else
                spin = '\\';
            mvaddch(max_y / 2, x, spin);
        }

        mvprintw(0, 0, "Press q to exit");
        refresh();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        ++counter;
    }

    endwin();
}
