CC = gcc
CFLAGS = -Werror -Wall -pedantic 
OBJ = custom_find.o 

.PHONY : clean

%.o : %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

custom_find : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

main : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean : 
	@ echo "rimozione file compilati"
	@ rm *.o
	@ if [ -e custom_find ]; then rm custom_find; fi
	@ if [ -e main ]; then rm main; fi

setup :
	@ echo "Creazione file e cartelle per test"
	@ mkdir prova1 
	@ touch prova1/a.txt
	@ touch prova1/b.txt
	@ touch prova1/c.sh
	@ touch prova1/d.c 
	@ mkdir prova2
	@ touch prova2/e.pdf
	@ touch prova2/f.txt
	@ touch prova2/g.c 
	@ touch prova2/h.png
	@ touch i.txt
	@ touch l.sh
	@ touch m.c 
	@ ls -ahlR


clean_setup :
	@ echo "rimozioni file e cartelle di prova creati"
	@ rm -r prova1 
	@ rm -r prova2 
	@ rm i.txt
	@ rm l.sh
	@ rm m.c