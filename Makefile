CC = g++
CFLAGS = -Wall -Wextra

INPUT = main.cpp
OUTPUT = ImageMaker
IMAGE = image.ppm

all:
	$(CC) $(CFLAGS) $(INPUT) -o $(OUTPUT)

run:
	./$(OUTPUT) > $(IMAGE)

validate:
	gimp $(IMAGE)

clean:
	rm $(OUTPUT)