CC := gcc
CFLAGS := -g -Wall
OBJECTS := main.o prompt.o game_data.o start_state.o exit_state.o save_state.o load_state.o game_state.o error.o

.PHONY: all
all: main

.PHONY: clean
clean:
	rm -f *.o

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJECTS): %.o : %.c 
	$(CC) $(CFLAGS) -I ./ -o $@ -c $<

