DEPS = include/common.h include/queue.h include/opprocesor.h include/op_code_buffer.h include/map.h
CFLAGS= -Wall -c -o

.PHONY: all debug sanitize clean

all: framework sensor test_framework test_sensor

framework: obj/framework.o obj/common.o obj/queue.o obj/op_code_buffer.o obj/opprocesor.o obj/map.o $(DEPS)
	gcc -o $@ obj/framework.o obj/common.o obj/queue.o obj/op_code_buffer.o obj/opprocesor.o obj/map.o -L. -lpthread $(DFLAGS)

sensor: obj/sensor.o obj/common.o $(DEPS)
	gcc -o $@ obj/sensor.o obj/common.o -L. -lpthread $(DFLAGS)

test_framework: testing_scripts/test_framework.o include/test.h
	gcc -o $@ testing_scripts/test_framework.o -L. -lpthread

test_sensor: testing_scripts/test_sensor.o include/test.h
	gcc -o $@ testing_scripts/test_sensor.o -L. -lpthread

obj/%.o: src/%.c $(DEPS)
	mkdir -p obj/
	gcc $(CFLAGS) $@ $< $(DFLAGS) 

# Compila usando la opción -g para facilitar la depuración con gdb.
debug: DFLAGS = -g
debug: clean all

# Compila habilitando la herramienta AddressSanitizer para
# facilitar la depuración en tiempo de ejecución.
sanitize: DFLAGS = -fsanitize=address,undefined
sanitize: clean all

clean:
	rm -rf sensor framework obj/*.o test_framework testing_scripts/*.o test_sensor