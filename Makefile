all: ct
export CFLAGS = -Wall -Wextra -Werror -Og \
	-fdiagnostics-color=always -fsanitize=undefined -ggdb

clean:
	rm -f ct *.o

ct: ct.o color_distance.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

*.o: Makefile
