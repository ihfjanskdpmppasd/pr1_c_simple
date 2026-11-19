CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

ifeq ($(OS),Windows_NT)
EXE = .exe
endif

cmdcalc$(EXE): main.o calc.o
	$(CC) $(CFLAGS) main.o calc.o -o $@

main.o: main.c calc.h
	$(CC) $(CFLAGS) -c main.c

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c calc.c

.PHONY: run clean
run: cmdcalc$(EXE)
	./cmdcalc$(EXE) 41 + 33 111 - 8 22 '*' 5 221 % 111 100 + 13 -k 2

clean:
	rm -f *.o cmdcalc cmdcalc.exe
