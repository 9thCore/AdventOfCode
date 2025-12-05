CC=gcc
LIBS="-lm"

run: .build/out
	@time .build/out < input.txt > output.txt

.build/out: prepare_build\
main.c
	@$(CC) main.c -o .build/out $(LIBS)

prepare_build:
	@mkdir -p .build
	@mkdir -p .build/modules