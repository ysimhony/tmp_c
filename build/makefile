BINDIR = ../bin
INCDIR = ../inc
SRCDIR = ../src
OBJDIR = ../obj
LIBDIR = ../lib

CC=gcc
CFLAGS=-I$(INCDIR) -g -Wall -ansi -pedantic


LIBS=

SOURCES=$(shell find $(SRCDIR) -name *.c)
OBJECTS=$(subst ${SRCDIR}, ${OBJDIR}, $(SOURCES:%.c=%.o))

$(warning OBJECTS = ${OBJECTS})
$(warning SOURCES = ${SOURCES})

$(OBJDIR)/%.o: $(SOURCES)
	@mkdir -p ${OBJDIR}
	$(CC) -c -o $@ $< $(CFLAGS)
	$(CC) -MM $(CFLAGS) $(SRCDIR)/$*.c > $(OBJDIR)/$*.d

$(BINDIR)/assembler: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/* $(BINDIR)/* core $(INCDIR)/*~ 


-include $(OBJS:.o=.d)
