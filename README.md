# FinalProject
 Final Project in the course System Programming Lab by the Open University of Israel.
 The purpose of this project is to build an assembler for 16-instruction assembly language, 
 for an imaginary 14-bit CPU with 14-bit & 256 words memory size. 
 The author of this project is @BAxPI.

 <h1> Directory Structure (Modules) </h1>
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
