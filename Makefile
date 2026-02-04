NAME = expert_system

TEST_NAME = expert_system_test

CPP = g++ -std=c++20

CFLAGS = -Wall -Werror -Wextra -Wno-shadow -Wconversion

INC = -I ./include/.

SRC_DIR = ./srcs/
	  
OBJ_DIR = .objs/

SRC_FILES = main.cpp utils.cpp print.cpp 

TEST_FILES = test.cpp utils.cpp print.cpp 

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ = $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

TEST_SRC = $(addprefix $(SRC_DIR), $(TEST_FILES))

TEST_OBJ = $(TEST_SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.test.o)

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	if [ ! -f $(NAME) ] || [ `find $(OBJ) -newer $(NAME) | wc -l` -ne 0 ]; then \
	$(CPP) $(CFLAGS) $(INC) $(OBJ) -o $(NAME); \
		printf "$(ERASE)$(GREEN)👷 Program$(RESET) $(CYAN)$(BOLD)$(NAME)$(RESET) $(GREEN)created! 👷\n$(RESET)"; \
	else \
		printf "$(ERASE)$(YELLOW)No relink needed for$(RESET) $(CYAN)$(BOLD)$(NAME)\n$(RESET)"; \
	fi \

$(TEST_NAME): $(TEST_OBJ)
	$(CPP) $(CFLAGS) $(INC) $(TEST_OBJ) -o $(TEST_NAME)
	printf "$(GREEN)🧪 Test binary built!$(RESET)\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	${CPP} $(CFLAGS) -c $< -o $@
	printf "$(ERASE)$(BLUE) > Compilation: $(RESET) $<"

$(OBJ_DIR)%.test.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(CPP) $(CFLAGS) -c $< -o $@
	printf "$(ERASE)$(BLUE) > Test Compilation: $(RESET) $<"

test: $(TEST_NAME)
	./run_tests.sh

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	printf "$(ERASE)$(GREEN)Fcleaning up...$(RESET)"
	$(RM) ${NAME}
	$(RM) ${TEST_NAME}
	printf "$(ERASE)🧼 $(GREEN)Fclean finished! 🧼\n$(RESET)"

re: fclean all

.PHONY: all clean fclean re test
.SILENT:

# COLORS
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
BOLD = \033[1m
ERASE = \033[2K\r
RESET = \033[0m
