EXEC=load
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)
	@echo "compile loader"

load: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@mv ./load ../

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)