/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Wed Feb 23 23:15:23 2022 Francois Michaut
** Last update Wed Mar 16 13:00:53 2022 Francois Michaut
**
** Kernel.cpp : Implementation of Kernel class
*/

#include "Exceptions.hpp"
#include "Kernel.hpp"

#include <ncurses.h>

ncurses::Kernel *ncurses::Kernel::kernel = nullptr; // NOLINT

ncurses::Kernel::Kernel()
{
    if (kernel != nullptr)
        throw InvalidState("There is already a Kernel initialized");
    kernel = this;
    // TODO ret check
    initscr();
    start_color();
}

ncurses::Kernel::~Kernel()
{
    kernel = nullptr;
    endwin();
}

ncurses::Kernel &ncurses::Kernel::get()
{
    if (kernel == nullptr)
        throw InvalidState("There is no Kernel initialized");
    return *kernel;
}

void ncurses::Kernel::echo(bool val)
{
    int ret = 0;

    if (val) {
        ret = ::echo();
    } else {
        ret = noecho();
    }
    if (ret == ERR)
        throw RoutineError(val ? "echo" : "noecho", ret);
}

void ncurses::Kernel::nl(bool val)
{
    int ret = 0;

    if (val) {
        ret = ::nl();
    } else {
        ret = nonl();
    }
    if (ret == ERR)
        throw RoutineError(val ? "nl" : "nonl", ret);
}

void ncurses::Kernel::cbreak(bool val)
{
    int ret = 0;

    if (val) {
        ret = ::cbreak();
    } else {
        ret = nocbreak();
    }
    if (ret == ERR)
        throw RoutineError(val ? "nl" : "nonl", ret);
}

int ncurses::Kernel::curs_set(int visibility)
{
    int ret = ::curs_set(visibility);

    if (ret == ERR)
        throw RoutineError("curs_set", ret);
    return ret;
}
