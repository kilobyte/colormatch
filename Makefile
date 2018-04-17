PROGS = ct nearest256 xterm256 243-256 riemersma-256 rate 16x16
all: $(PROGS)
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

xterm256: xterm256.o 256.o
	$(CC) $(CFLAGS) -o $@ $^

243-256: 243-256.o 256.o
	$(CC) $(CFLAGS) -o $@ $^

riemersma-256: riemersma-256.o 256.o
	$(CC) $(CFLAGS) -o $@ $^

rate: rate.o color_distance.o 256.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

16x16: 16x16.o color_distance.o 256.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

*.o: Makefile
