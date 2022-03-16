/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Thu Mar  3 07:09:17 2022 Francois Michaut
** Last update Wed Mar 16 13:00:58 2022 Francois Michaut
**
** Menu.cpp : Menu implementation
*/

#include "Exceptions.hpp"
#include "Menu.hpp"

#include <algorithm>

#include <menu.h>

using ncurses::MenuItem;
using ncurses::Menu;
using ncurses::Window;
using ncurses::RawMenu;

Menu::Menu(Window window, std::string title, std::vector<std::shared_ptr<MenuItem>> items) :
    window(std::move(window)), title(std::move(title)), items(std::move(items)),
    raw_menu({})
{}

Menu::Menu(Window window, std::string title, const std::vector<MenuItem> &items) :
    window(std::move(window)), title(std::move(title)), raw_menu({})

{
    set_items(items);
}

Menu::Menu(std::string title, std::vector<std::shared_ptr<MenuItem>> items) :
    Menu(Window::stdscr(), std::move(title), std::move(items))
{}

Menu::Menu(std::string title, const std::vector<MenuItem> &items) :
    Menu(Window::stdscr(), std::move(title), items)
{}

void Menu::set_items(std::vector<std::shared_ptr<MenuItem>> items)
{
    this->items = std::move(items);
    items_dirty = true;
}

void Menu::set_items(const std::vector<MenuItem> &items)
{
    this->items.clear();
    this->items.reserve(items.size());
    for (const auto &item : items) {
        this->items.emplace_back(std::make_shared<MenuItem>(item));
    }
    items_dirty = true;
}

std::vector<std::shared_ptr<MenuItem>> Menu::get_items() const
{
    return items;
}

void Menu::add_item(std::shared_ptr<MenuItem> item)
{
    items.push_back(std::move(item));
    items_dirty = true;
}

void Menu::add_item(MenuItem item)
{
    items.emplace_back(std::make_shared<MenuItem>(std::move(item)));
    items_dirty = true;
}

bool Menu::remove_item(const std::shared_ptr<MenuItem> &item)
{
    auto iter = std::find(items.begin(), items.end(), item);

    if (iter == items.end()) {
        return false;
    }
    items.erase(iter);
    items_dirty = true;
    return true;
}

void Menu::run()
{
    int key = 0;

    running = true;
    while (running) {
        if (items_dirty) {
            raw_menu.set_items(std::vector<std::shared_ptr<RawItem>>(
                                   items.begin(), items.end()));
            items_dirty = false;
        }
        // TODO get screen size and handle resizes ?
        if (!raw_menu.is_posted())
            raw_menu.safe_post();
        refresh();
        key = getch();
        driver(key);
    }
}

void Menu::stop(bool stop)
{
    running = !stop;
}

bool Menu::is_running() const
{
    return running;
}

void Menu::driver(int key)
{
    int ret = 0;

    switch (key) {
        case KEY_UP:
            ret = raw_menu.driver(REQ_UP_ITEM);
            break;
        case KEY_DOWN:
            ret = raw_menu.driver(REQ_DOWN_ITEM);
            break;
        case KEY_LEFT:
            ret = raw_menu.driver(REQ_LEFT_ITEM);
            break;
        case KEY_RIGHT:
            ret = raw_menu.driver(REQ_RIGHT_ITEM);
            break;
        case KEY_BTAB:
            ret = raw_menu.driver(REQ_PREV_ITEM);
            break;
        case KEY_STAB:
        case '\t':
            ret = raw_menu.driver(REQ_NEXT_ITEM);
            break;
        case KEY_ENTER:
        case '\r':
        case '\n': {
            const auto &item = raw_menu.current_item();
            const auto &menu_item = std::dynamic_pointer_cast<MenuItem>(item);
            ret = raw_menu.driver(REQ_TOGGLE_ITEM);

            if (menu_item)
                menu_item->trigger_action(*this);
            break;
        }
        case KEY_BACKSPACE:
            ret = raw_menu.driver(REQ_BACK_PATTERN);
            break;
        case KEY_RESIZE:
        default:
            ret = raw_menu.driver(key);
            if (ret == E_NO_MATCH || ret == E_UNKNOWN_COMMAND)
                ret = E_OK; // Ignore the error
            break;
    }
    if (ret != E_OK && ret != E_REQUEST_DENIED)
        throw MenuError("menu_driver", ret);
}

RawMenu &Menu::get_raw_menu()
{
    return raw_menu;
}
