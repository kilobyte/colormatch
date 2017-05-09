PROGS = ct nearest256
all: ct nearest256
#export CFLAGS = -Wall -Wextra -Werror -Og \
#	-fdiagnostics-color=always -fsanitize=undefined -ggdb
export CFLAGS = -Wall -Wextra -Werror -Ofast -march=native -mtune=native \
	-flto -fdiagnostics-color=always

clean:
	rm -f $(PROGS) *.o

ct: ct.o color_distance.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

nearest256: nearest256.o color_distance.o 256.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

*.o: Makefile
