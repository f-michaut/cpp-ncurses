##
## Project cpp-ncurses, 2022
##
## Author Francois Michaut
##
## Started on  Thu Feb 17 18:53:24 2022 Francois Michaut
## Last update Thu Mar 17 01:51:16 2022 Francois Michaut
##
## Makefile : Makefile of the project
##

CC			=	g++
CFLAGS		+=	-W -Wall -Werror -Wextra -std=c++17 -pedantic -fPIC
LDFLAGS 	+=	-lncurses -lmenu

BUILD_DIR	?=	build
SRC_DIR 	?=	source
INCS_DIR	?=	include

SRC_EXT		=	cpp
SUFFIXES	+=	.d

SRCS		:=	$(shell find $(SRC_DIR) -name *.$(SRC_EXT))
OBJS		:=	$(SRCS:$(SRC_DIR)/%.$(SRC_EXT)=$(BUILD_DIR)/%.o)
DEPS		:=	$(OBJS:%.o=%.d)

CFLAGS		+=	-I $(INCS_DIR)
ifdef DEBUG
  CFLAGS	+=	-DDEBUG -g3
endif

NAME		=	libcpp-ncurses

YELLOW		=	\033[0;103m
RED			=	\033[0;31m
GREEN		=	\033[0;32m
NC			=	\033[0m
GREY		=	\033[90m
BLUE		=	\033[0;94m
PURPLE		=	\033[0;95m
BG_COLOR	=	\033[46m####\033[m
IRED		=	\033[0;91m


all:	$(NAME)

$(NAME): shared static

shared:	$(OBJS)
	@echo -e '${NC}${BG_COLOR}Libs: $(LDFLAGS)${NC}'
	@echo -e '${BG_COLOR}Flags: $(CFLAGS)${NC}'
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME).so $(LDFLAGS) -shared \
		&& echo -e '${BLUE}Create${NC}: ${YELLOW}${GREY}./$(NAME).so${NC}'\
		|| (echo -e '${RED}[ FAILED ]${NC}: __Create__${GREY} ./$(NAME).so${NC}' && exit 1)

static:	$(OBJS)
	@ar rcs $(NAME).a $(OBJS) \
		&& echo -e '${BLUE}Create${NC}: ${YELLOW}${GREY}./$(NAME).a${NC}'\
		|| (echo -e '${RED}[ FAILED ]${NC}: __Create__${GREY} ./$(NAME).a${NC}' && exit 1)

${BUILD_DIR}/%.d: ${SRC_DIR}/%.${SRC_EXT}
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -MM -MT '$(patsubst ${SRC_DIR}/%.${SRC_EXT},${BUILD_DIR}/%.o,$<)' $< -MF $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.${SRC_EXT}  $(BUILD_DIR)/%.d
	@rm -f $@
	@mkdir -p $(shell dirname $@)
	@-$(CC) $(CFLAGS) -MMD -o $@ -c $< \
		&& echo -e '${GREEN} [ OK ]${NC} Build $<'\
		|| echo -e '${RED}[ FAILED ]${NC} __Build__ $<'
	@echo -ne '${NC}'

clean:
	@find . -name "*~" -delete -o -name "#*#" -delete
	@rm -rf ./build
	@rm -rf $(OBJS) $(DEPS)
	@rm -rf vgcore.*
	@rm -rf a.out
	@rm -rf $(NAME).so $(NAME).a
	@find . -name "*.d" -delete
	@echo -e '${BLUE}Clean${NC} : OK'

re:	clean	all

-include $(DEPS)

ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif

#A phony target should not be a prerequisite of a real target file;
#if it is, its recipe will be run every time make goes to update that file.
.PHONY:	all	$(NAME)	static	shared	clean	re
