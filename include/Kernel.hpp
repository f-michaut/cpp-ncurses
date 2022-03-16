/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Wed Feb 23 23:09:40 2022 Francois Michaut
** Last update Wed Mar 16 17:47:26 2022 Francois Michaut
**
** Kernel.hpp : Class to control program-wide parameters and abstract `curs_kernel(3x)` functions
*/

#pragma once

namespace ncurses {
    class Kernel {
        public:
            Kernel();
            ~Kernel();

            Kernel(const Kernel &other) = delete;
            Kernel(Kernel &&other) = delete;

            Kernel &operator=(const Kernel &other) = delete;
            Kernel &operator=(Kernel &&other) = delete;

            void cbreak(bool value);
            void echo(bool value);
            void raw(bool value);
            void qiflush(bool value);
            void nl(bool value);
            int curs_set(int visibility); // TODO replace INT with enum

            static Kernel &get();
        private:
            static Kernel *kernel; // NOLINT
            // std::vector<Colors> colors;

    };
}
