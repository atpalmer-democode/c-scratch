P=main
OBJECTS=main.o hash_dict.o hash.o
CFLAGS=-Wall -Wextra -pedantic

$P: $(OBJECTS)
	$(CC) $(CFLAGS) -o $P $(OBJECTS)

.PHONY: clean
clean:
	rm $P $(OBJECTS)
