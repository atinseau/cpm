
CC=clang++
LIB_FLAGS=-ljsoncpp -lpthread
EXTRA_FLAGS=-Wall -Wextra -Werror -I./includes
FLAGS=-std=c++11 -g $(EXTRA_FLAGS)
BUILD_DIR=.build
NAME=cpm

SRC=	src/module.cpp \
		src/filesystem.cpp \
		src/app.cpp

OBJ=$(SRC:src/%.cpp=$(BUILD_DIR)/%.o)

MAIN=main.cpp

$(NAME): $(OBJ)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -c $< -o $@ 
	@echo "✅ $< is compiled"

all: $(NAME)
	@$(CC) $(FLAGS) $(OBJ) $(MAIN) -o $(NAME) $(LIB_FLAGS)
	@echo "✅ $(NAME) is ready to start"

clean:
	@rm -f $(OBJ)
	@echo "🗑 Cleaning library"

fclean: clean
	@rm -rf $(BUILD_DIR)
	@rm -f $(NAME)
	@echo "🗑 Cleaning build folder"

re: fclean all

.DEFAULT_GOAL:=all