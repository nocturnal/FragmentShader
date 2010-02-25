TARGET = test.elf

SPU_C_SOURCES   = main.c
SPU_ASM_SOURCES = test.s

SPU_OBJECTS = $(SPU_C_SOURCES:.s=.o) $(SPU_ASM_SOURCES:.c=.o)

SPU_CPP_FLAGS = -g -O0 -I/usr/lib/gcc/spu/4.1.1/include -I/usr/spu/include/ 
SPU_LN_FLAGS  = -lm 
CC = spu-gcc

$(TARGET): $(SPU_OBJECTS)
	spu-gcc $(SPU_OBJECTS) -o $(TARGET) $(SPU_LN_FLAGS)

.cpp.o :
	spu-gcc $(SPU_CPP_FLAGS) -c $< -o $@

.s.o :
	spu-gcc $(SPU_S_FLAGS) -c $< -o $@

depend:
	makedepend -- $(SPU_CPP_FLAGS) -- $(SPU_SRCS)

