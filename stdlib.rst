C Standard Library Functions
****************************

.. index:: operator precedence, operator associativity, operators

Precedence and Associativity
----------------------------

======= ======================================================= ===============
 Order   Operation                                               Associativity
======= ======================================================= ===============
  1.     function-call(), ``[]  ->  .``                          L to R
  2.     ``! ~ ++ -- + - *``(ptr deref) ``sizeof &`` (addr of)   *R to L*
         (all unary operations are the same precedence)
  3.     ``* / %`` (the top tier arithmetic binary ops)          L to R
  4.     ``+ -`` (second tier arithmetic binary ops)             L to R
  5.     ``< <= > >= == !=``                                     L to R
  6.     in order: ``& ^ | && ||`` (bitwise before Boolean)      L to R 
  7.     ``=`` and all its variants                              *R to L*
  8.     ``,`` (comma)                                           L to R
======= ======================================================= ===============

One common pitfall with the above precedence fields is that::
    
    *structptr.field

never works.  The field selection operator (``.``) has higher precedence
than the dereference operator (``*``), thus the expression is evaluated
as ``*(structptr.field)`` instead of the (usually) desired ``(*structptr).field``.  A way to avoid this problem altogether is to write ``structptr->field``.

.. index:: C standard library

.. _C-standard-library-functions:

Standard Library Functions
--------------------------

Many basic housekeeping funcions are available to a C program in form of standard library functions. To call these, a program must ``#include`` the appropriate header file. All modern compilers link in the standard library code by default, so all that is needed is to include the correct header file.  The functions listed in the next table are the most commonly used ones, but there are many more which are not listed here.

==============  ===================================================
 header file      types of functions available
==============  ===================================================
  stdio.h       file input and output, e.g., ``printf``
  ctype.h       character tests, e.g., ``isspace``
  string.h      string operations
  stdlib.h      utility functions, e.g., ``atoi``, ``rand``, ``abs``
  math.h        mathematical functions, e.g., ``sin``, ``pow``
  assert.h      the ``assert`` debugging macro
  stdarg.h      support to create functions that take a variable
                number of parameters
  signal.h      support for exceptional condition signals
  time.h        date and time, e.g., ``time``
  sys/time.h    other date/time functions, e.g., ``gettimeofday``
  limits.h      constants which define type range values such
  float.h            as ``INT_MAX``, ``FLOAT_MAX``
  stdbool.h     ``bool`` type (requires ``-std=c99`` compiler flag)
==============  ===================================================

.. _stdio:

.. index:: printf, fgets, snprintf, scanf, fopen, fclose

stdio.h
-------
 ``stdio.h`` is a very common file to include.  It includes functions to print and read strings from files and to open and close files in the file system.

``FILE* fopen(const char* fname, const char* mode);``
    Open a file named in the filesystem and return a FILE* for it. Mode = "r" read, "w" write, "a" append, returns NULL on error. The standard files stdout, stdin, stderr are automatically opened and closed for you by the system.

``int fclose(FILE* file);``
    Close a previously opened file. Returns EOF on error. The operating system closes all of a program's files when it exits, but it's tidy to do it beforehand. Also, there is typically a limit to the number of files which a program may have open simultaneously.

``int fgetc(FILE* in);``
    Read and return the next unsigned char out of a file, or EOF if the file has been exhausted. (detail) This and other file functions return ints instead of a chars because the EOF constant they potentially is not a char, but is an int. getc() is an alternate, faster version implemented as a macro which may evaluate the FILE* expression more than once.

``char* fgets(char* dest, int n, FILE* in)``
    Reads the next line of text into a string supplied by the caller. Reads at most n-1 characters from the file, stopping at the first '\n' character. In any case, the string is '\0' terminated. The '\n' is included in the string. Returns NULL on EOF or error.  There's also a ``gets`` function, but *you should never use it!* (read the ``man`` page for why).

``int fputc(int ch, FILE* out);``
    Write the char to the file as an unsigned char. Returns ch, or EOF on err. putc() is an alternate, faster version implemented as a macro which may evaluate the FILE* expression more than once.

``int ungetc(int ch, FILE* in);``
    Push the most recent fgetc() char back onto the file. EOF may not be pushed back. Returns ch or EOF on error.

``int printf(const char* format_string, ...);``
    Prints a string with values possibly inserted into it to standard output. Takes a variable number of arguments -- first a format string followed by a number of matching arguments. The format string contains text mixed with % directives which mark things to be inserted in the output. %d = int, %Ld=long int, %s=string, %f=double, %c=char. Every % directive must have a matching argument of the correct type after the format string. Returns the number of characters written, or negative on error. If the percent directives do not match the number and type of arguments, printf() tends to crash or otherwise do the wrong thing at run time. fprintf() is a variant which takes an additional FILE* argument which specifies the file to print to. Examples::

        printf("hello\n");
                // prints: hello
        printf("hello %d there %d\n", 13, 1+1); 
                // prints: hello 13 there 2 
        printf("hello %c there %d %s\n", 'A', 42, "ok");
                // prints: hello A there 42 ok
        
``int scanf(const char* format, ...)``
    Opposite of printf() -- reads characters from standard input trying to match elements in the format string. Each percent directive in the format string must have a matching pointer in the argument list which scanf() uses to store the values it finds. scanf() skips whitespace as it tries to read in each percent directive. Returns the number of percent directives processed successfully, or EOF on error. scanf() is famously sensitive to programmer errors. If scanf() is called with anything but the correct pointers after the format string, it tends to crash or otherwise do the wrong thing at run time. sscanf() is a variant which takes an additional initial string from which it does its reading. ``fscanf()`` is a variant which takes an additional initial FILE* from which it does its reading. Example::

        {
            int num;
            char s1[1000];
            char s2[1000];
            scanf("hello %d %s %s", &num, s1, s2);
        }

The above code looks for the word "hello" followed by a number and two words (all separated by whitespace).  ``scanf()`` uses the pointers &num, s1, and s2 to store what it finds into the local variables.

``int snprintf(char* buffer, size_t size, const char *format, ...)``
    A version of ``printf`` that fills a char buffer with the resulting formatted string.  The first two arguments of ``snprintf`` are the buffer to file and the size of the buffer.  The remaining arguments are exactly like ``printf``: a format string followed by any arguments to be formatted in the resulting string.  There is also a ``sprintf`` function, but it is not "safe" since it does not include the buffer size in the set of parameters, which makes buffer overflows [#f1]_ easily possible.

``int fprintf(FILE *stream, const char *format, ...)``
    A version of ``printf`` that causes output to be sent to a file instead of to the default standard output.  ``printf`` works exactly like ``fprintf(stdout, ...)`` since ``stdout`` is predefined in ``stdio.h`` as a ``FILE *`` that results in console output.



..

.. index:: isalpha, islower, isupper, isspace, isdigit, toupper, tolower

ctype.h
-------

ctype.h includes macros for doing simple tests and operations on characters 

``isalpha(ch)``
    Check whether ch is an upper or lower case letter

``islower(ch), isupper(ch)``
    Same as above, but upper/lower specific 

``isspace(ch)``
    Check whether ch is a whitepace character such as tab, space, newline, etc. 

``isdigit(ch)``
    Check whether ch is a digit such as '0'..'9'

``toupper(ch), tolower(ch)``
    Return the lower or upper case version of a alphabetic character, otherwise pass it through unchanged.


.. index:: strlen, strcpy, strlcpy, strlcat, strcmp, strncmp, strcasecmp, strncasecmp, strchr, strstr, memcpy, memmove

string.h
--------

None of these string routines allocate memory or check that the passed in memory is the right size. The caller is responsible for making sure there is "enough" memory for the operation. The type ``size_t`` is an unsigned integer wide enough for the computer's address space (most likely an ``unsigned long``).

``size_t strlen(const char* string);``
    Return the number of chars in a C string. EG strlen("abc")==3

``char* strcpy(char* dest, const char* source);``
    Copy the characters from the source string to the destination string.

``size_t strlcpy(char* dest, const char* source, size_t dest_size);``
    Like strcpy(), but knows the size of the dest. Truncates if necessary. Use this to avoid memory errors and buffer-overflow security problems. This function is not as standard as strcpy(), but most sytems have it.  Do not use the old strncpy() function -- it is difficult to use correctly.

``char *strlcat(char* dest, const char* source, size_t dest_size);``
    Append the characters from the source string to the end of destination string.

``int strcmp(const char* a, const char* b);``
    Compare two strings and return an int which encodes their ordering. zero:a==b, negative:a<b, positive:a>b. It is a common error to think of the result of strcmp() as being boolean true if the strings are equal which is, unfortunately, exactly backwards.

``int strncmp(const char *a, const char *b, size_t n);``
    Just like ``strcmp``, except only the minimum of the lengths of ``a`` and ``b``, and the value ``n`` characters are compared.  There's also ``strncasecmp`` and ``strcasecmp`` which compare strings in a case-insensitive manner.

``char* strchr(const char* searchIn, char ch);``
    Search the given string for the first occurence of the given character. Returns a pointer to the character, or NULL if none is found.

``char* strstr(const char* searchIn, const char* searchFor);``
    Similar to strchr(), but searches for an entire string instead of a single character. The search is case sensitive.

``void* memcpy(void* dest, const void* source, size_t n);``
    Copy the given number of bytes from the source to the destination. The source and destination must not overlap. This may be implemented in a specialized but highly optimized way for a particular computer.

``void* memmove(void* dest, const void* source, size_t n);``
    Similar to memcpy() but allows the areas to overlap. This probably runs slightly slower than memcpy().

.. index:: rand, srand, abs, malloc, free, exit, bsearch, qsort, atoi, atof, strtol, realloc, strtod

stdlib.h
--------
``int rand();``
    Returns a pseudo random integer in the range 0..RAND_MAX (limits.h) which is at least 32767.

``void srand(unsigned int seed);``
    The sequence of random numbers returned by rand() is initially controlled by a global "seed" variable. srand() sets this seed which, by default, starts with the value 1. Pass the expression time(NULL) (time.h) to set the seed to a value based on the current time to ensure that the random sequence is different from one run to the next.

``int abs(int i);``
    Return the absolute value of ``i``.

``void* malloc(size_t size);``
    Allocate a heap block of the given size in bytes. Returns a pointer to the block or NULL on failure. A cast may be required to store the void* pointer into a regular typed pointer.  There is also a ``realloc`` function which can *change* the size of a heap-allocated block of memory.  See the ``man`` page for details.

``void free(void* block);``
    Opposite of malloc(). Returns a previous malloc block to the system for reuse

``void exit(int status);``
    Halt and exit the program and pass a condition int back to the operating sytem. Pass 0 to signal normal program termination, non-zero otherwise.

``void* bsearch(const void* key, const void* base, size_t len, size_t elem_size, <compare_function>);``
    Do a binary search in an array of elements. The last argument is a function which takes pointers to the two elements to compare. Its prototype should be: ``int compare(const void* a, const void* b);``, and it should return 0, -1, or 1 as strcmp() does. Returns a pointer to a found element, or NULL otherwise. Note that strcmp() itself cannot be used directly as a compare function for bsearch() on an array of char* strings because strcmp() takes char* arguments and bsearch() will need a comparator that takes pointers to the array elements -- char**.

``void qsort(void* base, size_t len, size_t elem_size, <compare_function>);``
    Sort an array of elements. Takes a function pointer just like bsearch().

``int atoi(const char *s)``
    Return an integer parsed from the string s.  This function is somewhat problematic since it cannot return errors if the string does not contain a parseable integer.  You should generally use ``strtol`` (and related functions) which can return errors.  See the ``man`` page on ``strtol`` for more.

``double atof(const char *)``
    Return a floating point number in ``double`` format parsed from the string s.  Like ``atoi`` this function is somewhat problematic since it cannot return errors if the string does not contain a parseable floating point number.  You should generally use ``strtod`` (and related functions) instead.

.. rubric:: Footnotes

.. [#f1]  http://en.wikipedia.org/wiki/Buffer_overflow
