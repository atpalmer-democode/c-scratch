P=main
OBJECTS=main.o hash_dict.o hash.o

$P: $(OBJECTS)
	$(CC) -o $P $(OBJECTS)
