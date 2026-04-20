SRC = err.c main.c dialog.c inp.c
TAB_SRC = tab.c
HEADERS = $(wildcard *.h)
OBJ = $(SRC:.c=.o)
TAB_OBJ = $(TAB_SRC:.c=.o)
LIB = libprog.so
TARGET1 = prog1

all: $(TARGET1)

$(TARGET1): $(OBJ) $(LIB)
	gcc -o $(TARGET1) $(OBJ) -L. -lprog -lreadline -Wl,-rpath,. -g

$(LIB): $(TAB_OBJ)
	gcc -shared -o $@ $^

%.o: %.c $(HEADERS)
	gcc -c $< -o $@ -g
	
$(TAB_OBJ): $(TAB_SRC) $(HEADERS)
	gcc -c -fPIC $(TAB_SRC) -o $@ -g

clean:
	rm -f $(TARGET1) $(OBJ) $(LIB) $(TAB_OBJ)
