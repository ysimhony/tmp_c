BINDIR = ../bin
INCDIR = ../inc
SRCDIR = ../src
OBJDIR = ../obj
LIBDIR = ../lib

CC=gcc
CFLAGS=-I$(INCDIR) -g -Wall -std=c99 -pedantic


LIBS=

SOURCES=$(shell find $(SRCDIR) -name *.c)
OBJECTS=$(subst ${SRCDIR}, ${OBJDIR}, $(SOURCES:%.c=%.o))

#$(warning OBJECTS = ${OBJECTS})
#$(warning SOURCES = ${SOURCES})

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p ${OBJDIR}
	$(CC) -c -o $@ $< $(CFLAGS)
	$(CC) -MM $(CFLAGS) $(SRCDIR)/$*.c > $(OBJDIR)/$*.Td
	sed 's,\($*\)\.o[ :]*,$@ : ,g' < $(OBJDIR)/$*.Td > $(OBJDIR)/$*.d;
	rm -fr $(OBJDIR)/*.Td
	
$(BINDIR)/assembler: $(OBJECTS)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/* $(BINDIR)/* core $(INCDIR)/*~ 

all: $(BINDIR)/assembler

-include $(OBJECTS:.o=.d)
