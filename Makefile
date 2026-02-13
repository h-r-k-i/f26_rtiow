CC = g++
CFLAGS = -Wall -Wextra -O3 -march=native -ffast-math

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