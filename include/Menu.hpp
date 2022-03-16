/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Thu Mar  3 06:34:09 2022 Francois Michaut
** Last update Wed Mar 16 13:02:00 2022 Francois Michaut
**
** Menu.hpp : High level menu abstraction
*/

#pragma once

#include "RawMenu.hpp"
#include "Window.hpp"

#include <functional>

namespace ncurses {
    class Menu;

    class MenuItem : public RawItem {
        public:
            using RawItem::RawItem;
            using ActionFun=std::function<void(Menu &menu, void *user_ptr)>;
            MenuItem(ActionFun action, std::string name,
                       std::string description = "", void *user_ptr = nullptr);
            ~MenuItem() override = default;

            MenuItem(const MenuItem &other) = default;
            MenuItem(MenuItem &&other) noexcept = default;

            // TODO protect against self assignement
            MenuItem &operator=(const MenuItem &other) =delete; // TODO
            MenuItem &operator=(MenuItem &&other) noexcept =delete; // TODO

            void set_action(ActionFun action);
            void remove_action();

            bool trigger_action(Menu &menu);
        private:
            ActionFun action;
            void *user_ptr;
    };

    class Menu {
        public:
            Menu(Window window, std::string title, std::vector<std::shared_ptr<MenuItem>> items);
            Menu(Window window, std::string title, const std::vector<MenuItem> &items);
            Menu(std::string title, std::vector<std::shared_ptr<MenuItem>> items);
            Menu(std::string title, const std::vector<MenuItem> &items);

            std::vector<std::shared_ptr<MenuItem>> get_items() const;
            void set_items(std::vector<std::shared_ptr<MenuItem>> items);
            void set_items(const std::vector<MenuItem> &items);

            void add_item(std::shared_ptr<MenuItem> item);
            void add_item(MenuItem item);
            bool remove_item(const std::shared_ptr<MenuItem> &item);

            const Window &get_window() const;
            Window &get_window();
            void set_window(Window window);

            std::string &get_title();
            const std::string &get_title() const;
            void set_title(std::string title);

            void run();
            void stop(bool stop = true);
            bool is_running() const;

            void driver(int key);

        protected:
            RawMenu &get_raw_menu();

        private:
            bool items_dirty = true;
            bool running = false;

            Window window;
            std::string title;
            std::vector<std::shared_ptr<MenuItem>> items;
            RawMenu raw_menu;
    };
}
