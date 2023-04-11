<h1> FinalProject </h1>
 Final Project in the course System Programming Lab by the Open University of Israel.
 The purpose of this project is to build an assembler for 16-instruction assembly language, 
 for an imaginary 14-bit CPU with 14-bit & 256 words memory size. 
 The author of this project is @BAxPI.
 
<h2>Running the Program</h2>
To run the program, follow these steps:

1. Open a terminal window and navigate to the directory where the program files are located.
2. Run the `make` command to compile the program.
3. Run the `assembler` command followed by the name of the file(s) you want to assemble. 
   The file(s) should be in the .as format and the input should be provided without the .as extension.
Here's an example command for assembling a single file named program:


```console
$ assembler program
```

If the file(s) are syntaxly correct, the program will generate 4 output files in the same directory:
 <p>1. <code>.am</code> - Contains the macros from the original file spread, comment lines (starting with ;) deleted, and empty lines deleted. </p>
 <p>2. <code>.obj</code> - Contains the binary code of the source code.</p>
 <p>3. <code>.ext</code> - Contains the extern symbols in the source code.</p>
 <p>4. <code>.ent</code> - Contains the entry symbols in the source code.</p>
 That's it! You should now have successfully run the program and generated the expected output files.

 <h2> Directory Structure (Modules) </h2>
 <p>1. <code>assembler</code> - main function definition, argv & argc processing, single file processing.</p>
 <p>2. <code>preprocessor</code> - the preprocessor of the assembler. </p>
 <p>3. <code>first_pass</code> - The first pass of the assembler. </p>
 <p>4. <code>second_pass</code> - The second pass of the assembler. </p>
 <p>5. <code>globals.h</code> - Contains type and constant definitions. </p>
 <p>6. <code>lexer.h</code> - Contains the definition of the Abstract Syntax Tree of a line in a source code. </p>
 <p>7. <code>lexer</code> - Contains the implementation of the lexer of the assembler. </p>
 <p>8. <code>code</code> - Functions related to the encoding of command line. </p>
 <p>9. <code>directives</code> - Functions related to the encoding of directive line. </p>
 <p>10. <code>tables</code> - Contains the implementation of Macros Table & Symbols Table. </p>
 <p>11. <code>write_files</code> - Functions related to the creation of output files. </p>
 <p>12. <code>utils</code> - Contains function for general use throughout the entire project. </p>
