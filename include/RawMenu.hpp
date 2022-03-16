/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Sat Feb 26 02:14:08 2022 Francois Michaut
** Last update Wed Mar 16 13:01:37 2022 Francois Michaut
**
** RawMenu.hpp : Classes for the ncurses Menu extention
*/

#pragma once

#include "Window.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ncurses {
    class RawMenu;

    class RawItem {
        public:
            RawItem(std::string name, std::string description = "");
            virtual ~RawItem() = default;

            RawItem(const RawItem &other) = default;
            RawItem(RawItem &&other) noexcept = default;

            // TODO protect against self assignement
            RawItem &operator=(const RawItem &other);
            RawItem &operator=(RawItem &&other) noexcept;

            bool value() const;

        private:
            friend RawMenu;
            using ITEM = void;

            std::string name;
            std::string description;
            std::shared_ptr<ITEM> item;
    };

    class RawMenu {
        public:
            RawMenu(std::vector<std::shared_ptr<RawItem>> items);
            RawMenu(Window window, std::vector<std::shared_ptr<RawItem>> items);
            ~RawMenu();

            RawMenu(const RawMenu &other);
            RawMenu(RawMenu &&other) noexcept;

            // TODO protect against self assignement
            RawMenu &operator=(const RawMenu &other);
            RawMenu &operator=(RawMenu &&other) noexcept;

            void post(bool val = true);
            bool safe_post(bool val = true);
            bool is_posted() const;

            int driver(int action);

            std::vector<std::shared_ptr<RawItem>> get_items() const;
            void set_items(std::vector<std::shared_ptr<RawItem>> items);

            void set_current_item(const std::shared_ptr<RawItem> &item);
            std::shared_ptr<RawItem> current_item() const;

            void opts_on(int opts);
            void opts_off(int opts);
            void set_opts(int opts);
            void get_opts() const;

            Window get_window() const;

            // Hook menu_init (const MENU *);
            // Hook menu_term (const MENU *);
            // WINDOW* menu_sub (const MENU *);
            // const char * menu_mark (const MENU *);
            // const char * menu_request_name (int);
            // char menu_pattern (const MENU *);
            // void menu_userptr (const MENU *);
            // chtype menu_back (const MENU *);
            // chtype menu_fore (const MENU *);
            // chtype menu_grey (const MENU *);
            // int menu_pad (const MENU *);
            // int pos_menu_cursor (const MENU *);
            // int scale_menu (const MENU *,int *,int *);
            // int menu_spacing (const MENU *,int *,int *,int *);
            // void menu_format (const MENU *,int *,int *);
            // int menu_request_by_name (const char *);

            // int set_menu_back (MENU *,chtype);
            // int set_menu_fore (MENU *,chtype);
            // int set_menu_format (MENU *,int,int);
            // int set_menu_grey (MENU *,chtype);
            // int set_menu_init (MENU *, Menu_Hook);
            // int set_menu_mark (MENU *, const char *);
            // int set_menu_pad (MENU *,int);
            // int set_menu_pattern (MENU *,const char *);
            // int set_menu_sub (MENU *,WINDOW *);
            // int set_menu_term (MENU *, Menu_Hook);
            // int set_menu_userptr (MENU *,void *);
            // int set_menu_win (MENU *,WINDOW *);
            // int set_menu_spacing (MENU *,int,int,int);


        private:
            using MENU = void;

            bool isposted = false;
            Window window;
            std::vector<std::shared_ptr<RawItem>> items;
            std::unique_ptr<RawItem::ITEM*[]> raw_items; // NOLINT
            MENU *menu;

            void internal_set_raw_items();
    };
}
