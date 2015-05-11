Getting your feet wet in the C
******************************

In this first chapter, we'll get wet in the C by way of a tutorial that examines a variety of topics that are discussed in depth in later chapters.  Where appropriate, pointers to later sections are given if you want to read a bit more as you work through the following examples.  

Hello, somebody
---------------

We first start out by examining a simple "hello, world"-style program, but with a twist to make it somewhat more interesting.  The following program asks a user for his or her name, then prints ``Hello, <name>!``.  Here is the code:

.. index:: #include, main, printf, arrays, fgets

.. literalinclude:: code/hello.c
   :language: c
   :linenos:

Since this may be the first C program you've seen, we'll walk through it line by line:

  1.  The first line (``#include <stdio.h>``) tells the C compiler that we intend to use functions that are in the *C standard library* and declared in a *header file* called ``stdio.h``.  This line is (somewhat) analogous to saying ``import`` in Python or Java.  The file ``stdio.h`` contains declarations for input/output functions in C, such as ``printf`` and ``fgets``, which we will use to print text to the console and get input from a user, respectively.  Including those declarations in our program lets the compiler know the type signatures of the functions we want to use (i.e., the data types of the function parameters, and the return type). See :ref:`preprocessing <preprocessing>` for more discussion of ``#include`` files.

  2.  The second line is another ``#include`` line to tell the C compiler that we want to use some string-related functions.  In particular, we use ``#include <string.h>`` because we want to use a function called ``strlen``, which is used to compute the length of a string.

  3.  Whitespace is generally ignored in C, just as it is ignored in Java.

  4.  Line 4 starts a function called ``main``, which is the function in which every C program **must** start.  If you don't define a ``main`` function, the compiler will complain loudly.  Notice that the return value of ``main`` is of type ``int``.  In UNIX-based systems (and C), it is common for functions to return an integer value indicating whether the function "succeeded" or not.  Weirdly enough 0 usually means "success" and some non-zero value (usually -1, or some other negative number) means "failure".  If you look at line 10, you'll see that we unconditionally return 0 to indicate that the program successfully completes.  (The ``main`` function can also take two parameters; see :ref:`the main function <the-main-function>` for more about ``main``.)

  5.  We print a prompt for a user using the ``printf`` function.  There shouldn't be anything particularly surprising about this function call: as in Java, the string we print out is delimited with double quotes (``"``).  In C, all strings **must** be delimited with double quotes, unlike Python which has three different ways of enclosing text (single, double, and triple quotes).  (Note that, like Java, single characters must be enclosed in single quotes in C.)  You can also see on line 5 that the ``printf`` statement is terminated with a semicolon.  As with Java, all statements in C must end with a semicolon.   The ``printf`` function can take any number of parameters, and a "format" string can define how to display different data types.  See :ref:`stdio reference <stdio>` for more.

  6.  On line 6, we declare a variable called ``name``, which is an array of length 32 of ``char``.  A string in C is represented as an array of characters, where character after the last valid character of the string contains the special character ``'\0'``.  This character is referred to as the "null" character and is used to delimit strings.  The string termination character must fit within the array we've declared here, so the array we've declared can hold a maximum of 31 characters plus the null termination character.  (Note: don't confuse the null character with the ``NULL`` pointer, which we'll encounter later --- they're totally different.)  Unlike any other language you have probably encountered so far, the *representation* of a string in C is completely transparent: there is no "information hiding" or abstraction here --- it is all visible.  Thus, unlike strings in Python and Java which are *immutable*, strings in C are completely mutable.  Changing a string in C boils down to modifying individual characters of the string.  You just need to be sure that the string is terminated with the special ``'\0'`` (null) character.  See :ref:`character literals <character-literals>` for more information on character literals and the null character.

      One thing that may not be obvious on line 6 is that this statement only declares the variable ``name``.  The C compiler and runtime environment do not do any automatic initialization of the string.  So at the point of declaration, the programmer cannot assume that there is anything known about the contents of ``name`` --- it is currently what ever random characters that may have resided in the memory location occupied by ``name`` prior to its being brought to life.  

  7.  The next statement calls the ``fgets`` C library function to get input from the keyboard.  There are three parameters we pass: the string variable (i.e., the array of characters) we created on line 6 (``name``), the maximum number of characters that ``name`` can hold (32), and the "input stream" that we wish to read from.  The variable name ``stdin`` is predefined in the ``<stdio.h>`` header file, so we can just use it directly as the third parameter.  ``stdin`` means "standard input", and (usually) refers to input that can be collected from the keyboard.  As you'll see later on in this book, you can also use ``fgets`` to read from files that you open.  See :ref:`stdio reference <stdio>` for more.

      One note about the way ``fgets`` works: it will read up to *one less* than the number of characters you specify as the second parameter.  It must reserve space for the final null termination character (since all C strings must end with ``'\0'``).  If a user types more than 31 characters, we'll just get the first 31.  

      And one more note about C strings: they have a fixed maximum size.  Unlike Python or Java strings, C strings cannot automatically grow or shrink as needed --- this is left entirely to the programmer to deal with.  As a result, a programmer must assume some reasonable maximum length for string input.  This issue is discussed further in :ref:`dynamic strings <dynamic-strings>`.


  8.  One more important thing about the way ``fgets`` works is that if a user types a name followed by return, the character emitted when the return key is pressed is also included in the string.  This character happens to be the lowly newline (``'\n'``).  The task of line 8 is to remove this character so that we only have a person's name stored in ``name``, not including the newline character.  To get rid of the newline, we simply overwrite the last character of the string with a ``'\0'`` (remember: in C, strings are totally mutable!)  

      This line is not foolproof.  If the user types in more than 31 characters, the newline won't be included in ``name``.  As a result, line 8 will squash a "legitimate" character of the name.  It is left as an exercise to fix this potential problem.

  9.  On line 9 the "Hello" statement is printed using ``printf``.  Two things to note about this line of code: the first parameter defines a *format template*, including the special character sequence ``%s`` that tells ``printf`` that a C string should be substituted at that point in the template.  The string to be substituted is the next parameter supplied to ``printf``, which is just the variable ``name``.  Secondly, we have included an explicit newline character in the format string.  ``printf`` never automatically emits a newline for you: if you want one, you need to remember to include ``'\n'``.  We did not include one in the first ``printf`` statement (line 5) because we wanted the user to be able to type the name on the same line as the prompt.
  Note that two other common ``printf`` formatting placeholders are ``%d`` for printing a decimal integer, and ``%f`` for printing a floating point number.  

  10.  See the description of line 4, above.

.. index:: clang; compiling

Hello, :command:`clang`
-----------------------

Now that we've got our program done and explained in gory detail, let's compile and run it.  Just like Java, C requires an explicit compilation step to produce an executable program.  Unlike Java, however, the program produced by the compiler does not contain "byte code", it contains *real* executable instructions for the processor on which you're running the compiler.  So, if you happen to be compiling on a 64-bit Linux system using an Intel-based processor, the program produced by the C compiler will contain raw Intel x86_64 instructions [#f1]_.

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

    $ clang -g -Wall -std=c99 -o hello hello.c
    $ ./hello
    Enter your name: fred
    Hello, fred!

When invoked like this, :command:`clang` will perform all three basic phases of compilation (see :ref:`compilation phases <compilation-phases>` for details): preprocessing, compiling, and linking.  The final result is a binary executable program that can be executed on a particular processor architecture (e.g., Intel x86 or x86_64).  

.. index:: 
   double: clang; error messages

When :command:`clang` goes "bang!"
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Of course, not all our wonderful source code will compile and run correctly on the first go.  Let's modify our "hello, somebody" program to introduce a bug and see what happens.  Specifically, we will modify two lines: on line 6 we will remove the semicolon at the end of the line, and on line 9, we will remove the second argument to the ``printf`` function call (the line should then read: ``printf("Hello, %s!\n");``).  Here's what :command:`clang`\ 's output looks like::

    hello.c:6:18: error: expected ';' at end of declaration
        char name[32]
                     ^
                     ;
    hello.c:9:21: warning: more '%' conversions than data arguments [-Wformat]
        printf("Hello, %s!\n");
                       ~^
    1 warning and 1 error generated.    

:command:`Clang` helpfully tells us that we're missing the semicolon (the 6:18 means the sixth line and 18th character on that line), and that there was an unequal number of arguments to ``printf`` and ``%``-placeholders in the format string [#f5]_.


.. index::
   single: gcc; compiling
   double: clang; gcc, versus

:command:`clang` versus :command:`gcc`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you use :command:`gcc` instead of :command:`clang` (perhaps because :command:`clang` is not available for some reason), the command-line options are *exactly* the same.  Nothing needs to change there.  The key difference you will notice between :command:`gcc` and :command:`clang` is in the error and warning messages.  If you thought the error messages above weren't very good, ponder the following output of :command:`gcc` for the same example::

    hello.c: In function ‘main’:
    hello.c:7:5: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘fgets’
         fgets(name, 32, stdin);
         ^
    hello.c:7:11: error: ‘name’ undeclared (first use in this function)
         fgets(name, 32, stdin);
               ^
    hello.c:7:11: note: each undeclared identifier is reported only once for each function it appears in
    hello.c:9:5: warning: format ‘%s’ expects a matching ‘char *’ argument [-Wformat=]
         printf("Hello, %s!\n");
     ^

Ugh.  Not only are there *more* errors reported than actually exist, the output is simply more cluttered and confusing.  Advice: use :command:`clang` instead of :command:`gcc` if at all possible.  


.. rubric:: Exercises

1.  A common error is to fail to include a null-termination character in a C string.  Modify the code example above to remove the ``'\0'``.  What does ``printf`` do?  What do you think is happening?

2.  Line 8 in our example program above can squash a non-newline character if the user types more than 31 characters for a name.  Fix the program so that the last character of the string is *only* overwritten if it is a newline (``\n``).  Note that, as with Java and Python, characters can be compared using ``==`` (double-equals), and that ``if`` statements work very similarly in C as they do in Java.

3.  Revisit your fabulous time in COSC 101 by writing a program that asks for a dog's name, its age in human years, then prints its age in dog years (human years multiplied by 7).  A couple hints to help accomplish this:
 
    * You can use ``fgets`` to collect each input, but note that you'll need to convert the dog age to an integer (you can just do the computations as integers).  You can use the ``atoi`` function to convert a string to an integer; ``atoi`` takes a C string as a parameter and returns an ``int``.  You'll need to ``#include <stdlib.h>`` to use the ``atoi`` function.

    * Arithmetic operators in C are virtual identical to those available in Java (and Python).  There is no ``**`` operator as in Python to do exponentiation, but you certainly shouldn't need that to compute the dog's age.

    * Try to make the output look nicely formatted using ``printf``.  To format a decimal integer for output, you can use the ``%d`` placeholder in the ``printf`` format string (i.e., the first parameter).

4.  Write a simple "race-pace" calculator.  Ask a user to type the race distance (in miles), and a string representing the time they want to finish the race in, using a format like "HH:MM:SS".  Compute and return the pace per-mile required to achieve the finish time.  A few notes and hints about this problem:

    * You should accept the miles value as a floating point value.  You can use the standard library function ``atof`` to convert a string to a floating point value.  Any floating point variables can be declared as either ``float`` or ``double`` (just like Java).

    * You can assume that the string entered by the user for finish time is *exactly* in the format "HH:MM:SS", for simplicity.  Assume that if the user wants to finish in 31 minutes and 19 seconds, they type "00:31:19".

.. [#f1] The file containing the instructions is in a format known as ELF: http://en.wikipedia.org/wiki/Executable_and_Linkable_Format

.. [#f2] Don't look for any examples related to Windows or Visual C in this book: they don't exist.

.. [#f3] By default, recent versions of :command:`clang` operate in a "modern" mode (C99 or C11).  To be safe, however, you should specify the C standard to which the code is written.  

.. [#f4] http://en.wikipedia.org/wiki/A.out

.. [#f5] If the terminal in which you invoke :command:`clang` is capable, it will even color-highlight the output to help draw your attention to various errors and warnings.  
