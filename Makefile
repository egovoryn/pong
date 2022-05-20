CC = gcc -Wall -Werror -Wextra -lncurses
NAME = pong

all: clean
	$(CC) $(NAME).c -o $(NAME)
	./$(NAME)

clean: 
	rm -rf $(NAME) *.o *.out
	
leaks:
	CK_FORK=no leaks --atExit -- ./$(NAME)

check:
	cppcheck *.c *.h
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h
