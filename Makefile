DEPS = common.h queue.h opprocesor.h op_code_buffer.h
CFLAGS= -Wall -c -g

.PHONY: all debug sanitize clean

all: framework sensor

framework: framework.o common.o queue.o op_code_buffer.o opprocesor.o  $(DEPS)
	gcc -o $@ framework.o common.o queue.o op_code_buffer.o opprocesor.o -L. -lpthread $(DFLAGS)

sensor: sensor.o common.o $(DEPS)
	gcc -o $@ sensor.o common.o $(DFLAGS)


%.o: %.c $(DEPS)
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