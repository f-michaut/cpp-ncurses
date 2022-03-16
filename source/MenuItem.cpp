/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Thu Mar  3 18:13:08 2022 Francois Michaut
** Last update Wed Mar 16 13:01:02 2022 Francois Michaut
**
** MenuItem.cpp : MenuItem implementation
*/

#include "Menu.hpp"

using ncurses::MenuItem;

MenuItem::MenuItem(MenuItem::ActionFun action, std::string name,
                       std::string description, void *user_ptr) :
    RawItem(std::move(name), std::move(description)),
    action(std::move(action)), user_ptr(user_ptr)
{}

void MenuItem::set_action(ActionFun action)
{
    this->action = std::move(action);
}

void MenuItem::remove_action()
{
    action = nullptr;
}

bool MenuItem::trigger_action(Menu &menu)
{
    if (action == nullptr)
        return false;
    action(menu, user_ptr);
    return true;
}
