CC=gcc -Wall
DIRS=bin obj
ODIR=obj
BDIR=bin
DEPS=commonlib.h

.PHONY: clean

%.o: %.c $(DEPS)
	$(CC) -c -o $(ODIR)/$@ $<

multiprocess_server: multiprocess_server.o commonlib.o
	$(CC) -o $(BDIR)/multiprocess_server $(ODIR)/multiprocess_server.o $(ODIR)/commonlib.o

$(shell mkdir -p $(DIRS))

clean:
	rm -f $(BDIR)/*
	rm -f $(ODIR)/*.o