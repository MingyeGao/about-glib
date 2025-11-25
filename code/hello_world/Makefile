GLIB_INCLUDE_FLAG := $(shell pkg-config --cflags glib-2.0)
GLIB_LIB_FLAG := $(shell pkg-config --libs glib-2.0)


all: main.c
	gcc -g ${GLIB_INCLUDE_FLAG} -o main main.c ${GLIB_LIB_FLAG}

clean:
	-rm ./main