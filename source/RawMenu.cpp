/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Sat Feb 26 15:49:38 2022 Francois Michaut
** Last update Wed Mar 16 13:00:29 2022 Francois Michaut
**
** RawMenu.cpp : RawMenu classes implementation
*/

#include "Exceptions.hpp"
#include "RawMenu.hpp"

#include <algorithm>

#include <menu.h>

using ncurses::RawItem;
using ncurses::RawMenu;

RawMenu::RawMenu(RawMenu &&other) noexcept :
    window(std::move(other.window)), items(std::move(other.items)),
    raw_items(std::move(other.raw_items)), menu(other.menu)
{
    other.menu = nullptr;
}

RawMenu::RawMenu(Window window, std::vector<std::shared_ptr<RawItem>> items) :
    window(std::move(window)), items(std::move(items)), raw_items(nullptr)
{
    internal_set_raw_items();
    menu = new_menu((::ITEM**)raw_items.get());
    if (menu == nullptr)
        throw MenuError("new_menu", nullptr);
}

RawMenu::RawMenu(std::vector<std::shared_ptr<RawItem>> items) :
    RawMenu(Window::stdscr(), std::move(items))
{}

RawMenu::~RawMenu()
{
    int ret = 0;

    if (menu) {
        ret = free_menu((::MENU *)menu);
    }
    (void)ret; // TODO
}

bool RawMenu::is_posted() const
{
    return this->isposted;
}

void RawMenu::post(bool val)
{
    int ret = 0;

    if (val) {
        ret = post_menu((::MENU *)menu);
    } else {
        ret = unpost_menu((::MENU *)menu);
    }
    if (ret != E_OK)
        throw MenuError(val ? "post_menu" : "unpost_menu", ret);
    this->isposted = val;
}

bool RawMenu::safe_post(bool val)
{
    if (val == this->isposted)
        return false; // Already in requested state
    this->post(val);
    return true;
}

int RawMenu::driver(int action)
{
    return menu_driver((::MENU *)menu, action);
}

std::vector<std::shared_ptr<RawItem>> RawMenu::get_items() const
{
    return this->items;
}

void RawMenu::set_items(std::vector<std::shared_ptr<RawItem>> items)
{
    int ret = 0;

    this->items = std::move(items);
    internal_set_raw_items();
    ret = set_menu_items((::MENU*)menu, (::ITEM**)raw_items.get());
    if (ret != E_OK)
        throw MenuError("set_menu_items", ret);
}

void RawMenu::internal_set_raw_items()
{
    raw_items = std::unique_ptr<RawItem::ITEM*[]>(new void*[this->items.size() + 1]); //NOLINT
    for (std::size_t i = 0; i < this->items.size(); i++) {
        raw_items[i] = this->items[i]->item.get();
    }
    raw_items[this->items.size()] = nullptr;
}

void RawMenu::set_current_item(const std::shared_ptr<RawItem> &item)
{
    int ret = ::set_current_item((::MENU *)menu, (::ITEM *)item->item.get());

    if (ret != E_OK)
        throw MenuError("set_current_item", ret);
}

std::shared_ptr<RawItem> RawMenu::current_item() const
{
    ITEM *ptr = ::current_item((::MENU *)menu);
    std::shared_ptr<RawItem> res;
    const auto &iter = std::find_if(items.begin(), items.end(), [&ptr](auto &item){
        return item->item.get() == ptr;
    });

    if (iter != items.end())
        res = *iter;
    return res;
}

void RawMenu::opts_on(int opts)
{
    int ret = ::menu_opts_on((::MENU *)menu, opts);

    if (ret != E_OK)
        throw MenuError("menu_opts_on", ret);
}

void RawMenu::opts_off(int opts)
{
    int ret = ::menu_opts_off((::MENU *)menu, opts);

    if (ret != E_OK)
        throw MenuError("menu_opts_off", ret);
}
