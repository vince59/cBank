AR=ar rc libCbank.a
LIB=libCbank.a
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(LIB)
	@echo "compile libCbank.a"

libCbank.a: $(OBJ)
	@$(AR) $@ $^ 

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o
	@rm -rf *.a

mrproper: clean
	@rm -rf $(LIB)