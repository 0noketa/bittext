CC = cl /MD

main.exe: bittext.obj
	$(CC) main.c bittext.obj