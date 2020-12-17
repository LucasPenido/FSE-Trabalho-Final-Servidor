CC = gcc
LDFLAGS = -g
LDLIBS = -lpaho-mqtt3a -lpthread -lncurses -lbcm2835 
BLDDIR = .
INCDIR = $(BLDDIR)/inc
SRCDIR = $(BLDDIR)/src
OBJDIR = $(BLDDIR)/obj
CFLAGS = -c -Wall -I$(INCDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = bin/bin

all: $(EXE) 
    
$(EXE): $(OBJ) 
	@mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJDIR)/*.o $(LDLIBS) -o $@ 

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJDIR)/*.o $(EXE)

run:
	bin/bin