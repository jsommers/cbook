C Standard Library Functions
****************************

Precedence and Associativity
function-call() [] -> .
! ~ ++ -- + - *(ptr deref) sizeof &(addr of)
   (all unary ops are the same)
*/ %
(the top tier arithmetic binary ops)
+-
(second tier arithmetic binary ops)
< <= > >= == !=
   in order: & ^ | && ||
   (note that bitwise comes before boolean)
   = and all its variants
   , (comma)  .
L to R
R to L
L to R
L to R
L to R
L to R
L to R
R to L
L to R
   A combinations which never works right without parens: *structptr.field
   You have to write it as (*structptr).field or structptr->field
Standard Library Functions
Many basic housekeeping funcions are available to a C program in form of standard library functions. To call these, a program must #include the appropriate .h file. Most
compilers link section are the here.
stdio.h ctype.h string.h math.h stdlib.h assert.h stdarg.h setjmp.h signal.h time.h
in the standard library code by default. The functions listed in the next most commonly used ones, but there are many more which are not listed
file input and output
character tests
string operations
mathematical functions such as sin() and cos() utility functions such as malloc() and rand() the assert() debugging macro
support for functions with variable numbers of arguments support for non-local flow control jumps
support for exceptional condition signals
date and time
limits.h, float.h constants which define type range values such as INT_MAX


stdio.h
Stdio.h is a very common file to #include -- it includes functions to print and read strings from files and to open and close files in the file system.
   FILE* fopen(const char* fname, const char* mode);
Open a file named in the filesystem and return a FILE* for it. Mode = "r" read,"w" write,"a"append, returns NULL on error. The standard files stdout, stdin, stderr are automatically opened and closed for you by the system.
  int fclose(FILE* file);
Close a previously opened file. Returns EOF on error. The operating system closes all of a program's files when it exits, but it's tidy to do it beforehand. Also, there is typically a limit to the number of files which a program may have open simultaneously.
  int fgetc(FILE* in);
Read and return the next unsigned char out of a file, or EOF if the file has been exhausted. (detail) This and other file functions return ints instead of a chars because the EOF constant they potentially is not a char, but is an int. getc() is an alternate, faster version implemented as a macro which may evaluate the FILE* expression more than once.
   char* fgets(char* dest, int n, FILE* in)
Reads the next line of text into a string supplied by the caller. Reads at most n-1 characters from the file, stopping at the first '\n' character. In any case, the string is '\0' terminated. The '\n' is included in the string. Returns NULL on EOF or error.
   int fputc(int ch, FILE* out);
Write the char to the file as an unsigned char. Returns ch, or EOF on err. putc() is an alternate, faster version implemented as a macro which may evaluate the FILE* expression more than once.
   int ungetc(int ch, FILE* in);
Push the most recent fgetc() char back onto the file. EOF may not be pushed back. Returns ch or EOF on error.
   int printf(const char* format_string, ...);
Prints a string with values possibly inserted into it to standard output. Takes a variable number of arguments -- first a format string followed by a number of matching arguments. The format string contains text mixed with % directives which mark things to be inserted in the output. %d = int, %Ld=long int, %s=string, %f=double, %c=char. Every % directive must have a matching argument of the correct type after the format string. Returns the number of characters written, or negative on error. If the percent directives do not match the number and type of arguments, printf() tends to crash or otherwise do the wrong thing at run time. fprintf() is a variant which takes an additional FILE* argument which specifies the file to print to. Examples...
printf("hello\n");
printf("hello %d there %d\n", 13, 1+1); printf("hello %c there %d %s\n", 'A', 42, "ok");
prints: hello
prints: hello 13 there 2 prints: hello A there 42 ok

   int scanf(const char* format, ...)
Opposite of printf() -- reads characters from standard input trying to match elements in the format string. Each percent directive in the format string must have a matching pointer in the argument list which scanf() uses to store the values it finds. scanf() skips whitespace as it tries to read in each percent directive. Returns the number of percent directives processed successfully, or EOF on error. scanf() is famously sensitive to programmer errors. If scanf() is called with anything but the correct pointers after the format string, it tends to crash or otherwise do the wrong thing at run time. sscanf() is a variant which takes an additional initial string from which it does its reading. fscanf() is a variant which takes an additional initial FILE* from which it does its reading. Example...
{
int num;
         char s1[1000];
         char s2[1000];
         scanf("hello %d %s %s", &num, s1, s2);
      }
Looks for the word "hello" followed by a number and two words (all separated by whitespace). scanf() uses the pointers &num, s1, and s2 to store what it finds into the local variables.
ctype.h
ctype.h includes macros for doing simple tests and operations on characters isalpha(ch) // ch is an upper or lower case letter
islower(ch), isupper(ch) // same as above, but upper/lower specific isspace(ch) // ch is a whitepace character such as tab, space, newline, etc. isdigit(ch) // digit such as '0'..'9'
toupper(ch), tolower(ch) // Return the lower or upper case version of a alphabetic character, otherwise pass it through unchanged.


string.h
None of these string routines allocate memory or check that the passed in memory is the right size. The caller is responsible for making sure there is "enough" memory for the operation. The type size_t is an unsigned integer wide enough for the computer's address space -- most likely an unsigned long.
   size_t strlen(const char* string);
Return the number of chars in a C string. EG strlen("abc")==3
   char* strcpy(char* dest, const char* source);
Copy the characters from the source string to the destination string.
   size_t strlcpy(char* dest, const char* source,
                    size_t dest_size);
Like strcpy(), but knows the size of the dest. Truncates if necessary. Use this to avoid memory errors and buffer-overflow security problems. This function is not as standard as strcpy(), but most sytems have it. Do not use the old strncpy() function -- it is difficult to use correctly.
   char *strcat(char* dest, const char* source);
Append the characters from the source string to the end of destination string. (There is a non-standard strlcat() variant that takes the size of the dest as third argument.)
   int strcmp(const char* a, const char* b);
Compare two strings and return an int which encodes their ordering. zero:a==b, negative:a<b, positive:a>b. It is a common error to think of the result of strcmp() as being boolean true if the strings are equal which is, unfortunately, exactly backwards.
   char* strchr(const char* searchIn, char ch);
Search the given string for the first occurence of the given character. Returns a pointer to the character, or NULL if none is found.
   char* strstr(const char* searchIn, const char* searchFor);
Similar to strchr(), but searches for an entire string instead of a single character. The search is case sensitive.
   void* memcpy(void* dest, const void* source, size_t n);
Copy the given number of bytes from the source to the destination. The source and destination must not overlap. This may be implemented in a specialized but highly optimized way for a particular computer.
   void* memmove(void* dest, const void* source, size_t n);
Similar to memcpy() but allows the areas to overlap. This probably runs slightly slower than memcpy().

stdlib.h
int rand();
Returns a pseudo random integer in the range 0..RAND_MAX (limits.h) which is at least 32767.
   void srand(unsigned int seed);
The sequence of random numbers returned by rand() is initially controlled by a global "seed" variable. srand() sets this seed which, by default, starts with the value 1. Pass the expression time(NULL) (time.h) to set the seed to a value based on the current time to ensure that the random sequence is different from one run to the next.
   void* malloc(size_t size);
Allocate a heap block of the given size in bytes. Returns a pointer to the block or NULL on failure. A cast may be required to store the void* pointer into a regular typed pointer. [ed: see the Heap Allocation section above for the longer discussion of malloc(), free(), and realloc()]
   void free(void* block);
Opposite of malloc(). Returns a previous malloc block to the system for reuse
   void* realloc(void* block, size_t size);
Resize an existing heap block to the new size. Takes care of copying bytes from the old block to the new. Returns the new base address of the heap block. It is a common error to forget to catch the return value from realloc(). Returns NULL if the resize operation was not possible.
   void exit(int status);
Halt and exit the program and pass a condition int back to the operating sytem. Pass 0 to signal normal program termination, non-zero otherwise.
void* bsearch(const void* key, const void* base, size_t len, size_t elem_size, <compare_function>);
Do a binary search in an array of elements. The last argument is a function which takes pointers to the two elements to compare. Its prototype should be:
int compare(const void* a, const void* b);, and it should return 0, -1, or 1 as strcmp() does. Returns a pointer to a found element, or NULL otherwise. Note that strcmp() itself cannot be used directly as a compare function for bsearch() on an array of char* strings because strcmp() takes char* arguments and bsearch() will need a comparator that takes pointers to the array elements -- char**.
void qsort(void* base, size_t len, size_t elem_size, <compare_function>);
Sort an array of elements. Takes a function pointer just like besearch().


Missing

atoi, strtol, strtoul
atof, strtod
fprintf
open 
close
write
read
seek

time functions: gettimeofday
