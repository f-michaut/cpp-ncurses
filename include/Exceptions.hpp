/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Thu Feb 24 11:43:24 2022 Francois Michaut
** Last update Wed Mar 16 17:47:12 2022 Francois Michaut
**
** Exceptions.hpp : Defines all exceptions that can occur during program execution
*/

#pragma once

#include <exception>
#include <string>

namespace ncurses {
    class Exception : public std::exception {
        protected:
            Exception() = default;

        public:
            const char* what() const noexcept override = 0;
    };

    class RoutineError : public Exception {
        public:
            RoutineError(std::string routine_name, int return_code);
            RoutineError(std::string routine_name, std::nullptr_t);

            const char* what() const noexcept override;

            const std::string &get_routine_name() const;
            int get_return_code() const;
        protected:
            RoutineError(const std::string &begin_msg, std::string routine_name, const std::string &end_msg, int return_code);
            RoutineError(const std::string &begin_msg, std::string routine_name, const std::string &end_msg, std::nullptr_t);

        private:
            std::string name;
            int ret_code;
            std::string msg;
    };

    class MenuError : public RoutineError {
        public:
            MenuError(const std::string &routine_name, int return_code);
            MenuError(const std::string &routine_name, std::nullptr_t);
    };

    class InvalidState : public Exception {
        public:
            InvalidState(std::string msg);

            const char* what() const noexcept override;
        private:
            std::string msg;

    };
}
