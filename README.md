<h1> FinalProject </h1>
 Final Project in the course System Programming Lab by the Open University of Israel.
 The purpose of this project is to build an assembler for 16-instruction assembly language, 
 for an imaginary 14-bit CPU with 14-bit & 256 words memory size. 
 The author of this project is @BAxPI.
 
<h2>Running the Program</h2>
To run the program, follow these steps:

<p>1. Open a terminal window and navigate to the directory where the program files are located.</p>
<p>2. Run the `make` command to compile the program.</p>
<p>3. Run the `assembler` command followed by the name of the file(s) you want to assemble.</p>
   The file(s) should be in the .as format and the input should be provided without the .as extension.</p>
Here's an example command for assembling a single file named program:

program.as

![program.as](https://github.com/BAxPI/FinalProject/blob/main/images/input_example.png?raw=true)

```console
$ assembler program
```

If the file(s) are syntaxly correct, the program will generate 4 output files in the same directory:
 <p>1. <code>.am</code> - Contains the macros from the original file spread, comment lines (starting with ;) deleted, and empty lines deleted.</p>
 
 ![program.am](https://github.com/BAxPI/FinalProject/blob/main/images/am_output.png?raw=true)
 
 <p>2. <code>.ob</code> - Contains the binary code of the source code.</p>
 
 ![program.ob](https://github.com/BAxPI/FinalProject/blob/main/images/ob_output.png?raw=true)
 
 <p>3. <code>.ext</code> - Contains the extern symbols in the source code.</p>
 
 ![program.ext](https://github.com/BAxPI/FinalProject/blob/main/images/ext_output.png?raw=true)
 
 <p>4. <code>.ent</code> - Contains the entry symbols in the source code.</p>
 
 ![program.ent](https://github.com/BAxPI/FinalProject/blob/main/images/ent_output.png?raw=true)

If the file(s) has errors the assembler won't produce any output file except the `.am file` and will print to the `stdout` all the errors it found.
Here's an example of file that contains errors: 

errors.as contains errors

![errors.as](https://github.com/BAxPI/FinalProject/blob/main/images/errors_example.png?raw=true)

here are the error messages you should excpect to get:

![errors_output](https://github.com/BAxPI/FinalProject/blob/main/images/errors_output.png?raw=true)
 
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
