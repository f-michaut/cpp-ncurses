/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Thu Feb 24 11:23:21 2022 Francois Michaut
** Last update Wed Mar 16 17:48:19 2022 Francois Michaut
**
** Window.hpp : Window manipulation
*/

#pragma once

#include <memory>

namespace ncurses {
    class Window {
        public:
            struct Position {
                Position(int x, int y);

                int x;
                int y;
            };
            struct Size : Position {
                Size(int x, int y);
            };

            Window(Size size, Position position);
            ~Window(); // delwin

            Window(const Window &other) = default;
            Window(Window &&other) noexcept;

            Window &operator=(const Window &other);
            Window &operator=(Window &&other) noexcept;

            Window dupwin() const;
            void intrflush(bool bf);
            void keypad(bool bf);
            void meta(bool bf);
            void nodelay(bool bf);
            void notimeout(bool bf);
            void wtimeout(int delay);

            void clearok(bool bf);
            void idlok(bool bf);
            void idcok(bool bf);
            void immedok(bool bf);
            void leaveok(bool bf);
            void setscrreg(int bot);
            void wsetscrreg(int top, int bot);
            void scrollok(bool bf);

            static Window stdscr();
            static Window newscr();
            static Window curscr();

            std::shared_ptr<Window> subwin();
            std::shared_ptr<Window> derwin();

            void box();
            void clearok();

            void copywin();
            void overlay();
            void overwrite();

            void mvwin();
            void mvderwin();

            void wsyncup();
            void syncok();
            void wcursyncup();
            void wsyncdown();

        private:
            using WINDOW = void;

            std::shared_ptr<WINDOW> wptr;
            bool owns = true;
            Size size;
            Position pos;

            Window(WINDOW *ptr, const std::string &routine_name);
    };
}
