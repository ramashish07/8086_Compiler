src = g++
ccflags = -c

run : mainfile
	./main.exe

clean : memory.o registers.o operation.o immediate.o main.exe alu.o
	rm memory.o registers.o operation.o immediate.o main.exe alu.o

mainfile : main.cpp registers.o global_objects.h memory.o operation.o immediate.o memory_global_object.h alu.o
	$(src) main.cpp registers.o memory.o operation.o immediate.o alu.o -o main.exe

registers.o : registers.cpp registers.h
	$(src) $(ccflags) registers.cpp

memory.o : memory.cpp registers.o memory.h
	$(src) $(ccflags) memory.cpp

operation.o : operation.cpp operation.h
	$(src) $(ccflags) operation.cpp

immediate.o : immediate.cpp immediate.h registers.h
	$(src) $(ccflags) immediate.cpp

alu.o : global_objects.h memory_global_object.h alu.h alu.cpp
	$(src) $(ccflags) alu.cpp




