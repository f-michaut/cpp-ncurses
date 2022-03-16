/*
** Project cpp-ncurses, 2022
**
** Author Francois Michaut
**
** Started on  Sat Feb 26 00:16:31 2022 Francois Michaut
** Last update Wed Mar 16 17:59:50 2022 Francois Michaut
**
** Exceptions.cpp : Exceptions implementation
*/

#include "Exceptions.hpp"

#include <cstring>
#include <menu.h>
#include <ncurses.h>

static std::string error_code_to_str(int err_code)
{
    if (err_code == ERR) {
        return " (ERR)";
    } else {
        return "";
    }
}

static std::string menu_error_code_to_str(int err_code)
{
    switch (err_code) {
        case E_OK:
            return " (E_OK)";
        case E_BAD_ARGUMENT:
            return " (E_BAD_ARGUMENT)";
        case E_BAD_STATE:
            return " (E_BAD_STATE)";
        case E_NOT_CONNECTED:
            return " (E_NOT_CONNECTED)";
        case E_POSTED:
            return " (E_POSTED)";
        case E_NOT_POSTED:
            return " (E_NOT_POSTED)";
        case E_UNKNOWN_COMMAND:
            return " (E_UNKNOWN_COMMAND)";
        case E_NO_ROOM:
            return " (E_NO_ROOM)";
        case E_NO_MATCH:
            return " (E_NO_MATCH)";
        case E_REQUEST_DENIED:
            return " (E_REQUEST_DENIED)";
        case E_SYSTEM_ERROR:
            return std::string(" (E_SYSTEM_ERROR: ") + strerror(errno) + ")";
        default:
            return "UNKNOWN_MENU_ERROR: " + std::to_string(err_code);
    }
}

ncurses::RoutineError::RoutineError(std::string routine_name, int return_code) :
    RoutineError("Ncurses routine '", std::move(routine_name),
                 "' exited with error status '" + std::to_string(return_code) +
                 "'" + error_code_to_str(return_code), return_code)
{}

ncurses::RoutineError::RoutineError(std::string routine_name, std::nullptr_t) :
    RoutineError("Ncurses routine '", std::move(routine_name),
                 "' returned a NULL pointer", 0)
{}

ncurses::RoutineError::RoutineError(const std::string &begin_msg,
                                    std::string routine_name,
                                    const std::string &end_msg,
                                    int return_code) :
    name(std::move(routine_name)), ret_code(return_code),
    msg(begin_msg + name + end_msg)
{}

ncurses::RoutineError::RoutineError(const std::string &begin_msg,
                                    std::string routine_name,
                                    const std::string &end_msg,
                                    std::nullptr_t) :
    name(std::move(routine_name)), ret_code(0),
    msg(begin_msg + name + end_msg)
{}

const char *ncurses::RoutineError::what() const noexcept
{
    return msg.c_str();
}

ncurses::InvalidState::InvalidState(std::string msg) :
    msg(std::move(msg))
{}

const char *ncurses::InvalidState::what() const noexcept
{
    return msg.c_str();
}

ncurses::MenuError::MenuError(const std::string &routine_name, int ret_code) :
    RoutineError("Ncurses Menu routine '", routine_name,
                 "' exited with error status '" + std::to_string(ret_code) +
                 "'" + menu_error_code_to_str(ret_code), ret_code)
{}

ncurses::MenuError::MenuError(const std::string &routine_name, std::nullptr_t) :
    RoutineError("Ncurses Menu routine '", routine_name,
                 "' returned a NULL pointer", nullptr)
{}
