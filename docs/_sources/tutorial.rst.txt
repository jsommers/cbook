.. _tutorial:

Getting your feet wet in the C
******************************

In this first chapter, we'll get wet in the C by way of a tutorial that examines a variety of topics that are discussed in depth in later chapters.  Where appropriate, pointers to later sections are given if you want to read a bit more as you work through the following examples.

Hello, somebody
---------------

We first start out by examining a simple "hello, world"-style program: a program that simply echoes (prints) what ever is typed on the keyboard, until end-of-file is reached.
Here is the code:

.. index:: #include, main, getchar, putchar

.. literalinclude:: code/echo.c
   :language: c
   :linenos:

.. sidebar:: C and stdio when targeting microcontrollers

    When writing C to compile and run on embedded devices (e.g., microcontrollers) there are generally no I/O functions like ``printf``, ``fgets``, and the like.  Any I/O is typically either bit- or byte-/character-oriented.  In this first example program, we are using the built-in ``getchar`` and ``putchar`` functions, which are character-oriented.  These functions are also not typically available as-is in microcontroller programming environments, but they at least have some similarity to how I/O works in these constrained settings.

Since this may be the first C program you've seen, we'll walk through it line by line:

  1.  The first line (``#include <stdio.h>``) tells the C compiler that we intend to use functions that are in the *C standard library* and declared in a *header file* called ``stdio.h``.  This line is (somewhat) analogous to saying ``import`` in Python or Java.  The file ``stdio.h`` contains declarations for input/output functions in C, such as ``getchar`` and ``putchar``, which are used to print a character to the console or read a character from the keyboard, respectively.  The ``stdio.h`` header also has declarations for the more commonly used ``printf`` and ``fgets`` which are used in generic C programs, as well as many other related functions.  Including those declarations in our program lets the compiler know the type signatures of the functions we want to use (i.e., the data types of the function parameters, and the return type). See :ref:`preprocessing <preprocessing>` for more discussion of ``#include`` files.

  2.  The second line includes another source of standard function declarations and definitions (``#include <stdlib.h>``).  We include this file specifically for the ``EXIT_SUCCESS`` value returned on the last line of the ``main`` function (as discussed below).

  3.  Whitespace is generally ignored in C, just as it is ignored in Java.

  4.  Line 4 starts a function called ``main``, which is the function in which every C program **must** start.  If you don't define a ``main`` function, the compiler will complain loudly.  Notice that the return value of ``main`` is of type ``int``.  In UNIX-based systems (and C), it is common for functions to return an integer value indicating whether the function "succeeded" or not.  Weirdly enough 0 usually means "success" and some non-zero value (sometimes 1, or -1, or some other value) means "failure".  The symbol ``EXIT_SUCCESS`` is defined (inside ``<stdlib.h>``) to be 0; there is also a symbol ``EXIT_FAILURE`` that is defined to be 1.  If you look at line 11, you'll see that we unconditionally return ``EXIT_SUCCESS`` to indicate that the program successfully completes.  (The ``main`` function can also take two parameters; see :ref:`the main function <the-main-function>` for more about ``main``.)

  5.  On line 5, we declare a variable called ``character`` of type ``int``.  This might look like an ordinary declaration you'd find in Java, but what may not be obvious is that this line *only declares the variable*: **there is no automatic initialization of variables in C**.  There is no way to know what would be stored in ``character`` following that line of code since we did not explicitly assign it to anything --- it will hold whatever random character that may have resided in the memory location occupied by ``character`` prior to its being brought to life.

  6.  On line 6, we call the built-in function ``getchar`` to read a character from the keyboard (technically, from the input stream ``stdin``).  The program will pause until a character is typed [#f0]_.  Once something is typed, the *integer representation of the character* will be assigned to the variable ``character``.

  7.  Line 7 starts a ``while`` loop, in which we will continue to loop until the program reaches "end of file" (EOF) on the input stream.  If running this program interactively, you can give an EOF on the keyboard by typing Ctrl+D.  The ``EOF`` constant comes from the ``<stdio.h>`` header file (see description for line 1, above).

  8. Since we did not get ``EOF``, we can write the character most recently read out to the console (``stdout``) using the ``putchar`` function.

  9.  At the end of the loop, we read the next character from the keyboard for the next time the ``while`` loop condition is evaluated.  

  11. See the description of line 5, above.


An aside on C strings and string-based I/O
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In ordinary C programs, you'll often used string-based I/O functions instead of character-based ones.  Strings in C are very much unlike strings in Java or Python: they are simply arrays of characters that are terminated with a "null termination character", which is written ``'\0'``.  Notice single quotes are used for literal characters in C and double quotes are used for literal strings.  You can read more later in this book about :ref:`C strings <cstrings>`.
  
Printing strings is typically handled using the ``printf`` function, and there are a variety of functions used for reading strings (e.g., ``fgets``, ``scanf``).  An example ``printf`` line that could replace line 8 in the program above is ``printf("%c", character);``.  Notice that the first argument to ``printf`` is a string (double quotes), which defines a *format template*.  The special character sequence ``%c`` indicates to ``printf`` that a character should be substituted at that point in the template; the character to be substituted is just the second argument to the function.  Note that two other common ``printf`` formatting placeholders are ``%s`` for printing a string, ``%d`` for printing a decimal integer, and ``%f`` for printing a floating point number.  Read more about printf in the :ref:`stdio reference <stdio>`.

.. index:: clang; compiling

Hello, :command:`clang`
-----------------------

Now that we've gone through the ``hello`` program in some gory detail, let's compile and run it.  Just like Java, C requires an explicit compilation step to produce an executable program.  Unlike Java, however, the program produced by the compiler does not contain "byte code", it contains *real* executable instructions for the processor on which you're running the compiler.  So, if you happen to be compiling on a 64-bit Linux system using an Intel-based processor, the program produced by the C compiler will contain raw Intel x86_64 instructions [#f1]_.

All the examples in this book will use a compiler called :command:`clang`, and all examples will use the command line (in particular, the :command:`bash` shell). :command:`clang` is available on most recent Linux systems and on MacOS X [#f2]_.  If :command:`clang` is unavailable, you can also use the :command:`gcc` compiler.  The reason we favor :command:`clang` is that the error messages produced by :command:`clang` are far, far superior to the cryptic nonsense spewed by :command:`gcc`.

The basic recipe for using :command:`clang` is::

    clang -g -Wall -std=c99 -o <executablename> <sourcefiles>

Notice that there are a few command-line flags/options given:

 * ``-g`` tells the compiler to include debugging symbols in the compiled program.  This is a good thing to do because it will enable you to use a symbolic debugger like :command:`gdb`, if necessary.

 * ``-Wall`` tells the compiler to turn on all warnings.  If :command:`clang` detects something odd or suspicious about your code, it will say so.  Turning on this flag is a ridiculously good idea.  If you enjoy the feeling of someone yelling at you, you can even turn on the ``-pedantic`` flag.

 * ``-std=c99`` tells the compiler to turn on "C99" features, or language features that were introduced in a 1999 revision of the C programming language standard.  We will use various C99 constructs in this book, so you should always turn on this flag [#f3]_.

 * ``-o <executablename>`` tells the compiler how to name the file that is produced as an executable program.  You should replace ``<executablename>`` with something more meaningful (as shown in the example below).

 * ``<sourcefiles>`` comes last, and can be one or more ``.c`` files containing C source code.

.. sidebar:: a.out is the default executable file name produced by clang

   If you do not supply an output executable name using the ``-o`` flag, :command:`clang` will create a file named ``a.out``.  Which is very, very weird, right?  There is, of course, a history behind this file name [#f4]_.


For example, with the "hello, someone" example above, we might compile and run the program as follows::

    $ clang -g -Wall -std=c99 -o echo echo.c
    $ ./echo
    hello,
    hello,
    world!
    world!
    e   
    e
    c
    c
    h
    h
    o
    o

When invoked like this, :command:`clang` will perform all three basic phases of compilation (see :ref:`compilation phases <compilation-phases>` for details): preprocessing, compiling, and linking.  The final result is a binary executable program that can be executed on a particular processor architecture (e.g., Intel x86 or x86_64).

.. index::
   double: clang; error messages

When :command:`clang` goes "bang!"
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Of course, not all our wonderful source code will compile and run correctly on the first go.  Let's modify our "echo" program to introduce a bug and see what happens.  Specifically, we will modify two lines: on line 5 we will remove the semicolon at the end of the line, and on line 8, we will remove the argument to ``putchar``.  Here's what :command:`clang`\ 's output looks like::

    echo.c:5:18: error: expected ';' at end of declaration
        int character
                    ^
                    ;
    echo.c:8:17: error: too few arguments to function call, expected 1, have 0
            putchar();
            ~~~~~~~ ^

    2 errors generated.

:command:`Clang` helpfully tells us that we're missing the semicolon (the 5:18 means the fifth line and 18th character on that line), and that there was an incorrect number of arguments to ``putchar`` [#f5]_.


.. index::
   single: gcc; compiling
   double: clang; gcc, versus

:command:`clang` versus :command:`gcc`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you use :command:`gcc` instead of :command:`clang` (perhaps because :command:`clang` is not available for some reason), the command-line options are *exactly* the same.  Nothing needs to change there.  The key difference you will notice between :command:`gcc` and :command:`clang` is in the error and warning messages.  If you thought the error messages above weren't very good, ponder the following output of :command:`gcc` for the same example::

    echo.c: In function 'main':
    echo.c:6:5: error: expected '=', ',', ';', 'asm' or '__attribute__' before 'character'
        6 |     character = getchar();
          |     ^~~~~~~~~
    echo.c:6:5: error: 'character' undeclared (first use in this function)
    echo.c:6:5: note: each undeclared identifier is reported only once for each function it appears in
    In file included from code/echo.c:1:
    echo.c:8:9: error: expected expression before ',' token
        8 |         putchar();
          |         ^~~~~~~

It's just not quite as nice as what ``clang`` prints.  If possible, a recommendation is to use ``clang``, but ``gcc`` is very widely used and you're likely to use it at some point.  Advice: read the error messages very carefully!


.. rubric:: Exercises

1.  Modify the above program to use string-based I/O using ``fgets`` and ``printf``.  To use ``fgets``, you'll need to declare an array of ``char`` with some reasonable size.  If you name the array ``string``, the call to ``fgets`` should be ``fgets(string, 64, stdin);``.  

2.  Modify the ``while`` loop in the character-oriented version of the echo program to convert any letters to upper case.  ``if`` statements work similarly as they do in Java, and you can compare variables storing characters to literal characters using standard operators (e.g., ``==``, ``<=``, etc.)

3.  Revisit your fabulous time in COSC 101 by writing a program that asks for a dog's name, its age in human years, then prints its age in dog years (human years multiplied by 7).  A couple hints to help accomplish this:

    * If you use string-based I/O, you can use ``fgets`` to collect each input, but note that you'll need to convert the dog age to an integer (you can just do the computations as integers).  You can use the ``atoi`` function to convert a string to an integer; ``atoi`` takes a C string as a parameter and returns an ``int``.  You'll need to ``#include <stdlib.h>`` to use the ``atoi`` function.  To format a decimal integer for output with ``printf``, you can use the ``%d`` placeholder in the ``printf`` format string (i.e., the first parameter).

    * If you use character-based I/O, you could collect input using ``getchar`` *until* receiving a newline character.  As you collect digits, you could convert them directly to an integer, or populate a string and use ``atoi`` as noted above.  Once you do the relevant computation (multiply by 7), you can consider how to take the integer dog years value and print it character by character using ``putchar``.  Hint: use integer division and modulus (mod - ``%``) to extract individual digits, starting with the left-most (largest placeholder).

    * Note that arithmetic operators in C are virtually identical to those available in Java (and Python).  There is no ``**`` operator as in Python to do exponentiation, but you certainly shouldn't need that to compute the dog's age.


.. [#f0] This is not strictly true in practice.  ``stdin`` and ``stdout`` are both *buffered* I/O streams, so if you just type a single character (like ``'a'``) you will *not* see any output.  If you type a character *plus* return/enter, then you will see some output because the newline character (``\n``, emitted from typing return) implicitly flushes the input and output streams.  On UNIX-based computer systems, you can use the ``setbuf`` or ``fflush`` built-in functions (among other possibilities) to either force I/O to be unbuffered, or to flush input and/or output streams.

.. [#f1] The file containing the instructions is in a format known as ELF: http://en.wikipedia.org/wiki/Executable_and_Linkable_Format

.. 5.  Write a simple "race-pace" calculator.  Ask a user to type the race distance (in miles), and a string representing the time they want to finish the race in, using a format like "HH:MM:SS".  Compute and return the pace per-mile required to achieve the finish time.  A few notes and hints about this problem:

..     * You should accept the miles value as a floating point value.  You can use the standard library function ``atof`` to convert a string to a floating point value.  Any floating point variables can be declared as either ``float`` or ``double`` (just like Java).

..     * You can assume that the string entered by the user for finish time is *exactly* in the format "HH:MM:SS", for simplicity.  Assume that if the user wants to finish in 31 minutes and 19 seconds, they type "00:31:19".

.. [#f2] Don't look for any examples related to Windows or Visual C in this book: they don't exist.

.. [#f3] By default, recent versions of :command:`clang` operate in a "modern" mode (C99 or C11).  To be safe, however, you should specify the C standard to which the code is written.

.. [#f4] http://en.wikipedia.org/wiki/A.out

.. [#f5] If the terminal in which you invoke :command:`clang` is capable, it will even color-highlight the output to help draw your attention to various errors and warnings.
