CC=gcc
LIBS="-lm"

run: .build/out
	@./.build/out < input.txt > output.txt

.build/out: prepare_build\
main.c
	@$(CC) main.c -o .build/out $(LIBS)

# .build/out: prepare_build\
# main.c\
# .build/modules/module.obj
# 	@$(CC) main.c .build/modules/module.obj -o .build/out $(LIBS)

# .build/out: prepare_build\
# modules/module/main.c
# 	@$(CC) -c modules/module/main.c -o .build/modules/module.obj $(LIBS)

prepare_build:
	@mkdir -p .build
	@mkdir -p .build/modules