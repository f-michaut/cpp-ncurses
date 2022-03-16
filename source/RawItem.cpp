/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Thu Mar  3 18:13:46 2022 Francois Michaut
** Last update Wed Mar 16 13:01:08 2022 Francois Michaut
**
** RawItem.cpp : RawItem implementation
*/

#include "Exceptions.hpp"
#include "RawMenu.hpp"

#include <menu.h>

using ncurses::RawItem;

struct ItemDeleter {
    void operator()(void *item) {
        int ret = 0;

        if (item)
            ret = ::free_item((::ITEM *)item);
        (void)ret; // TODO
    }
};

const static ItemDeleter item_deleter;

RawItem::RawItem(std::string name, std::string description) :
    name(std::move(name)), description(std::move(description)),
    item(new_item(this->name.c_str(), this->description.c_str()), item_deleter)
{
    if (item == nullptr)
        throw MenuError("new_item", nullptr);
}

bool RawItem::value() const
{
    return ::item_value((::ITEM *)item.get());
}
