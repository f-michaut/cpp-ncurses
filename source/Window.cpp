/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Thu Feb 24 11:37:30 2022 Francois Michaut
** Last update Wed Mar 16 13:00:18 2022 Francois Michaut
**
** Window.cpp : Window implementation
*/

#include "Exceptions.hpp"
#include "Window.hpp"

#include <ncurses.h>

using ncurses::Window;

struct NullDeleter {
    void operator()(void *) {}
};

const static NullDeleter null_deleter;

Window::Window(Size size, Position position) :
    size(size), pos(position)
{
    wptr = std::shared_ptr<WINDOW>(newwin(size.y, size.x, position.y, position.x), null_deleter);
    if (wptr == nullptr)
        throw RoutineError("newwin", nullptr);
}

Window::Window(WINDOW *ptr, const std::string &routine_name) :
    wptr(ptr, null_deleter), owns(false), size(0, 0), pos(0, 0)
{
    if (ptr == nullptr)
        throw InvalidState(routine_name + " returned NULL Window pointer, " +
                           "did you forget to initialize a Kernel ?");
    getmaxyx((::WINDOW *)ptr, size.y, size.x);
}

Window::Window(Window &&other) noexcept :
    wptr(std::move(other.wptr)), owns(other.owns), size(other.size), pos(other.pos)
{
    other.owns = false;
}

Window::~Window()
{
    int ret = 0;

    if (owns) {
        ret = delwin((::WINDOW *)wptr.get());
        (void)ret;
        // if (ret == ERR) // TODO
    }
}

Window Window::stdscr()
{
    return Window(::stdscr, "stdscr");
}

Window Window::newscr()
{
    return Window(::newscr, "newscr");
}

Window Window::curscr()
{
    return Window(::curscr, "curscr");
}

Window::Size::Size(int x, int y) :
    Position(x, y)
{}

Window::Position::Position(int x, int y) :
    x(x), y(y)
{}

void Window::intrflush(bool bf)
{
    int ret = ::intrflush((::WINDOW *)wptr.get(), bf);

    if (ret == ERR)
        throw RoutineError("intrflush", ret);
}

void Window::keypad(bool bf)
{
    int ret = ::keypad((::WINDOW *)wptr.get(), bf);

    if (ret == ERR)
        throw RoutineError("keypad", ret);
}

void Window::meta(bool bf)
{
    int ret = ::meta((::WINDOW *)wptr.get(), bf);

    if (ret == ERR)
        throw RoutineError("meta", ret);
}

void Window::leaveok(bool bf)
{
    int ret = ::leaveok((::WINDOW *)wptr.get(), bf);

    if (ret == ERR)
        throw RoutineError("leaveok", ret);
}
