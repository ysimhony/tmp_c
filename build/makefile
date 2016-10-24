BINDIR = ../bin
INCDIR = ../inc
SRCDIR = ../src
OBJDIR = ../obj
LIBDIR = ../lib

CC=gcc
CFLAGS=-I$(INCDIR)


LIBS=

SOURCES=$(shell find $(SRCDIR) -name *.c)
OBJECTS=$(SOURCES:%.c=%.o)

$(OBJDIR)/%.o: $(SOURCES)
	$(CC) -c -o $@ $< $(CFLAGS)
	$(CC) -MM $(CFLAGS) $(SRC)/$*.c > $(OBJ)/$*.d

$(BINDIR)/assembler: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*  core $(INCDIR)/*~ 


-include $(OBJS:.o=.d)
