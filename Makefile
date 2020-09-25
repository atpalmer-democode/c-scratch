P=main
OBJECTS=main.o hash_dict.o

$P: $(OBJECTS)
	$(CC) -o $P $(OBJECTS)
