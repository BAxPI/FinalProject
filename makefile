# Compilation macros
 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 GLOBAL_DEPS = globals.h # Dependencies for everything
 EXE_DEPS = assembler.o  utils.o tables.o preprocessor.o first_pass.o second_pass.o code.o directives.o write_files.o lexer.o # Deps for exe
 
 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@
	
assembler.o:  assembler.c $(GLOBAL_DEPS)
	$(CC) -c assembler.c $(CFLAGS) -o $@

preprocessor.o: preprocessor.c preprocessor.h $(GLOBAL_DEPS)
	$(CC) -c preprocessor.c $(CFLAGS) -o $@

first_pass.o: first_pass.c first_pass.h $(GLOBAL_DEPS)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

second_pass.o: second_pass.c second_pass.h $(GLOBAL_DEPS)
	$(CC) -c second_pass.c $(CFLAGS) -o $@

code.o: code.c code.h $(GLOBAL_DEPS)
	$(CC) -c code.c $(CFLAGS) -o $@

directives.o: directives.c directives.h $(GLOBAL_DEPS)
	$(CC) -c directives.c $(CFLAGS) -o $@

tables.o: tables.c tables.h $(GLOBAL_DEPS)
	$(CC) -c tables.c $(CFLAGS) -o $@

utils.o: utils.c utils.h $(GLOBAL_DEPS)
	$(CC) -c utils.c $(CFLAGS) -o $@

write_files.o: write_files.c write_files.h $(GLOBAL_DEPS)
	$(CC) -c write_files.c $(CFLAGS) -o $@

lexer.o: lexer.c lexer.h $(GLOBAL_DEPS)
	$(CC) -c lexer.c $(CFLAGS) -o $@

clean:
	rm -rf *.o




