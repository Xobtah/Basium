NAME    = DataBaseTest

SRC     = main.cpp \

OBJ     = $(SRC:.cpp=.o)

CXXFLAGS = -Wall -std=c++11

all: $(NAME)

$(NAME): $(OBJ)
	g++ $^ -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
