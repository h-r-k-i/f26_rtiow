CC = g++
CFLAGS = -Wall -Wextra -ffast-math -O3 -march=native

INPUT = main.cpp
OUTPUT = ImageMaker
IMAGE = image2.ppm

all:
	$(CC) $(CFLAGS) $(INPUT) -o $(OUTPUT)

run:
	./$(OUTPUT)

validate:
	gimp $(IMAGE)

clean:
	rm $(OUTPUT)
