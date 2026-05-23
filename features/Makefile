CC = gcc
CFLAGS = -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c

%.o: %.c
	$(CC) $(CFLAGS) -I ../include $< -o $@

all: fs.o quit.o ls.o cd.o mkdir.o pwd.o echo.o touch.o cat.o write.o rm.o vim.o art.o color.o

clean:
	rm -rf *.o *~
