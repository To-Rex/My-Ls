CC := gcc
CC := ${CC}
OUTPUT := my_ls

all: my_ls 

my_ls: 
	@echo Compiling
	@${CC} my_ls.c -Wall -Wextra -Werror -o ${OUTPUT};
	@echo "Compiled to ${OUTPUT}"

clean: 
	# rm *.o

fclean: clean 
	rm ${OUTPUT}

re: fclean all