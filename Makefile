DEPS = common.h
CFLAGS= -Wall -c

.PHONY: all debug sanitize clean

all: framework sensor

framework: opprocessors.o common.o threadpool.o $(DEPS) threadpool.h
	gcc -o $@ opprocessors.o common.o threadpool.o -L. -lpthread $(DFLAGS) threadpool.h

sensor: sensor.o common.o $(DEPS)
	gcc -o $@ sensor.o common.o $(DFLAGS)


%.o: %.c $(DEPS) threadpool.h
	gcc $(CFLAGS) $< $(DFLAGS)

# Compila usando la opción -g para facilitar la depuración con gdb.
debug: DFLAGS = -g
debug: clean all

# Compila habilitando la herramienta AddressSanitizer para
# facilitar la depuración en tiempo de ejecución.
sanitize: DFLAGS = -fsanitize=address,undefined
sanitize: clean all

clean:
	rm -rf sensor framework *.o