Arrays and Strings
******************

.. index:: arrays, array indexing

Arrays
======

Arrays in C are declared and used much like they are in Java.  The syntax for using arrays in C is nearly identical to Java, and is very similar to the syntax used for Python lists.  As with both Python and Java, arrays in C are always indexed starting at 0.  Thus, in the following example, the first ``int`` in the ``scores`` array is ``scores[0]`` and the last is ``scores[99]``:

.. code-block:: c
   :linenos:

    int scores[100];
    scores[0]  = 13;           // set first element
    scores[99] = 42;           // set last element

The name of the array refers, in some sense, to the *whole array* but in actuality, the array name refers to the *memory address* at which the array storage begins.  As in Java, elements of an array in C are stored *contiguously*.  Thus, for the above array, if the first element in the array is stored at memory address *x*, the next element is stored at *x+4* (since the ``int`` is 4 bytes on most machines today), as depicted in :ref:`array-figure`, below.

.. _array-figure:

.. figure:: figures/array.*
   :alt: A diagram of an array in C
   :align: center

   A memory diagram of the scores array.

.. index:: array initialization, memset, bzero, initialization syntax

Array initialization
--------------------

Note that because C does not do any automatic initialization of variables, the array has *undefined* contents at the point of declaration (line 1, above).  A common practice is to use either a simple ``for`` loop construct to set all values in the array to a specific value, e.g.,:

.. code-block:: c

    int scores[100];
    for (int i = 0; i < 100; i++) {
        scores[i] = 0; 
    }

Another common practice is to use the ``memset`` function or ``bzero`` function to set everything in an array to zeroes.  The ``memset`` function is declared in ``strings.h`` (so you need to ``#include`` it), and takes three parameters: a memory address (which can just be the name of the array), the character that should be written into each byte of the memory address, and the number of bytes to set.  Thus, the above ``for`` loop could be replaced with the following:

.. code-block:: c

    // at the top of your source code
    #include <string.h>

    int scores[100];
    memset(scores, 0, 100*sizeof(int));

Note that we need to specify the number of *bytes* we want to set to 0, thus we say ``sizeof(int)`` multiplied by the number of array elements.  It's always good practice to use ``sizeof``, even if you think you can assume that the size of an ``int`` is 4.  Don't make that assumption; use ``sizeof``.

One last way that array contents can be initialized is to use C initializer syntax. Say that we just want to create an array of 10 scores.  We could initialize the array as follows:

.. code-block:: c

    int scores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

The initializer syntax in C is just a set of curly braces, within which are comma-separate values.  You can even leave off the array size if you give an initializer, and the compiler will figure out how large to make the array:

.. code-block:: c

    int scores[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // equivalent to above

The initializer syntax is especially useful for small-ish arrays for which the initial values are not all identical.

.. index:: sizeof, sizeof and arrays

``sizeof`` and arrays
---------------------

The built-in ``sizeof`` function works with arrays.  Specifically, it will return the number of bytes occupied by the array, which is quite helpful.  So, the ``memset`` code we wrote earlier could be replaced with:

.. code-block:: c

    // at the top of your source code
    #include <string.h>

    int scores[100];
    memset(scores, 0, sizeof(scores)); 

.. index:: array bounds checking (lack of)

No array bounds checking!
-------------------------

It is a very common error to try to refer to a non-existent array element. Unlike Java or Python, in which an out-of-bounds array (or list) reference will result in an exception, C will happily attempt to access the non-existent element.  The program behavior in such a case is *undefined*, which basically means that anything can happen.  Your program might crash, but it might not.  It might behave as you expect, but it might not.  It might cause your computer to levitate or to spontaneously combust.  Who knows?  Yuck.  Welcome to C.  

So what you can you do about this?  The best thing is to use good tools for detecting memory corruption and bad array accesses.  The :command:`valgrind` tool [#f1a]_ is especially good in this regard, and is highly recommended.  Its output can be somewhat difficult to understand at first, but it is a hugely helpful tool when trying to debug seemingly random program behavior.

Besides :command:`valgrind`, you can use the :command:`clang` *static analyzer*.  This tool analyzes your code to find potential bugs, too, but it is pretty fast (it doesn't actually execute your code) and the output is a little easier to grasp than :command:`valgrind`.  The tool to invoke is called :command:`scan-build` [#f1b]_, and can be used on the command line *before* any compiler tols that you invoke.  For example, consider the following program that increments an uninitialized variable (thus leading to undefined behavior):

.. code-block:: c

    #include <stdio.h>
    #include <stdlib.h> 
    
    int main() {
        int x;
        x += 10;
        printf("%d\n", x);
        return EXIT_SUCCESS; 
    }

Running :command:`scan-build` on this code results in the following::

    $ scan-build clang uninit.c 
    scan-build: Using '/usr/lib/llvm-3.8/bin/clang' for static analysis
    uninit.c:6:7: warning: The left expression of the compound assignment is an uninitialized value. The computed value will also be garbage
        x += 10;
        ~ ^ 
    1 warning generated.
    scan-build: 1 bug found.

This is helpful!  It's something that ordinary compilation will *not* uncover (:command:`clang` compiles this program without warning or error, ordinarily), and errors like this are pretty easy to make for inexperienced C programmers, especially when it comes to arrays and pointers (a topic coming soon).  Advice: run :command:`scan-build` as part of your C regimen.


.. index:: variable length arrays

Variable length arrays
----------------------

At the point of declaration, the size of an array in C *can* be specified with a variable, which creates what is called a *variable length array*.  Variable length arrays were added to C in the C99 standard, so if you use a variable when specifying the size of an array and there is a compile-time error on that line, make sure that you are compiling in C99 mode (``-std=c99`` on :command:`clang` and :command:`gcc`).  Here is an example with using a variable-length array (notice that we're using the ``atoi`` function to convert a string to an integer):

.. literalinclude:: code/vararray.c
   :language: c
   :linenos:

..

.. index::
   double: arrays; multi-dimensional arrays


Multidimensional Arrays
=======================

Just as in Java, C allows a programmer to declare "multi-dimensional" arrays by using multiple pairs of square braces in the array variable declaration.  For example, a 2-dimensional array of integers with ``r`` rows and ``c`` columns would be declared as ``int array[r][c]``.  Thus, if we wanted to declare a 3x3 array to hold the contents of a tic-tac-toe board, we might do something like this:

.. code-block:: c

    char board[3][3];

You can use array initialization syntax with multi-dimensional arrays, too.  For example, the ``board`` declaration could set each element as follows:

.. code-block:: c

    char board[3][3] = {{'O', 'O', ' '},
                        {'X', 'X', 'O'},
                        {' ', 'O', 'X'}};


Note that each nested set of curly braces in the initializer refers to a *row* in the array.

The underlying implementation of a multi-dimensional array stores all the elements in a *single contiguous block of memory*.  The array is arranged with the elements of the rightmost index next to each other. In other words, board[1][8] comes right before board[1][9] in memory.  (This arrangement is called "row-major order" [#f2]_.)

.. sidebar:: Memory access efficiency.

   If you know about CPU caches and cache lines, you'll know that it's more efficient to access memory which is near other recently accessed memory.  This means that the most efficient way to read through a chunk of the array is to vary the rightmost index the most frequently since that will access elements that are near each other in memory.

.. index:: strings, string initialization

C Strings
=========

C has minimal support of character strings.  A string in C is, in essence, an array of ``char``\ s.  C includes a standard library of functions for performing a variety of string operations, but the programmer is ultimately responsible for managing the underlying array (and memory) used by the string.  Computations involving strings is very common, so becoming a competent C programmer requires a level of adeptness at understanding and manipulating C strings.

A C string is just an array of ``char`` with the one additional convention that a "null" character (``'\0'``) is stored after the last character in the array, as an end-of-string marker.  For example, the following code creates and prints the C string ``"go 'gate"`` (using some array initialization syntax introduced above):

.. code-block:: c

    char string[] = {'g', 'o', ' ', '\'', 'g', 'a', 't', 'e', '\0' };
    printf("%s\n", string);

Notice a few things about the above line of code: (1) we don't need to specify the size of the array (the compiler can figure that out), (2) we need to "escape" the apostrophe (the 4th character), since we need to distinguish it from the character delimiters, and (3) we need to explicitly specify the end-of-string marker (null character).  

Another way to initialize a C string is to use double-quotes.  The following code is identical to above:

.. code-block:: c

    char string[] = "go 'gate";
    printf("%s\n", string);

The compiler *automatically* adds the null termination character as the last character in ``string``, giving an identical in-memory representation as the previous code example.

Since a C string is just an array of ``char``, it is totally *mutable* (which should be, hopefully, an obvious point).  As a result, we can tamper directly with the contents of the array to change the string.  For example, building on the last example, we could write:

.. code-block:: c
  
    string[3] = 's';
    string[4] = 'k';
    printf("%s\n", string);

to change the string to ``"go skate"`` and print it.

.. index:: strlen, string length

Getting the length of a string
------------------------------

It is often necessary in programs to obtain the length of a string.  There is a built-in ``strlen`` function just for this purpose.  ``strlen`` takes a single C string as a parameter, and returns an ``size_t`` type, which is typically the same size as a ``long`` (either 4 or 8 bytes, depending whether you're on a 32-bit or 64-bit machine, respectively).  ``strlen`` is declared in the ``string.h`` header file, so don't forget to include that file when using any built-in string functions like ``strlen``.

Here's a brief example:

.. code-block:: c

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    int main() {
        char name[128];
        printf("Please type your name: ");
        fgets(name, 128, stdin);
        printf("Your name is %d characters long.\n", strlen(name)-1);
            // why strlen(name) - 1?  
            // fgets includes the \n (newline) character that the user types in
            // the string filled in name, and we don't want to include that 
            // character as part of the length of the name.
        return EXIT_SUCCESS;
    }

.. index:: header files, man pages

.. topic:: Which header file do I need to include?

    For pretty much *all* C programs you write you will need to ``#include`` some header files (headers are discussed in more detail in :ref:`compilation-and-program-structure`).  Which header file will you need?  One of the easiest ways to find out is to use the ``man`` program in a Linux (or \*NIX) shell to read the manual page for a particular C library function.  For example, if you need to find out what include file to use for the function ``atoi``, you could simply type ``man atoi`` at the command line.  At the top of the man page the appropriate ``#include`` line will be listed.  You can also use a search engine and search for ``atoi man page`` and *usually* you'll get the same results, but different C library versions and compilers may use slightly different header files so its best just to use the ``man`` pages on your system.

    Manual pages can be a little bit difficult to wade through, but they are almost always divided into useful sections so you can (sort of) quickly find what you're looking for.  For finding out what header file to include, look in a section at the top of the man page called "SYNOPSIS".  That section also contains the function "prototypes" (which we'll discuss in a later chapter on functions), which provides the data types of any parameters and return values.

    To navigate a man page, you can usually type ``'d'`` to go down a page, ``'u'`` to go up a page, and ``'q'`` to quit (type ``'h'`` or ``'?'`` for help on navigating).  
    One confusing aspect of looking up a ``man`` page is that the same *name* can appear in multiple *sections* of the manual pages system.  For example, there's a ``printf`` C library function, and there is also a ``printf`` function available for writing shell scripts.  If you just type ``man printf``, you'll get the shell command reference, which may not be what you want.  To get the right man page, you can type ``man <sectionnumber> <symbol>``, as in ``man 3 printf`` (the C library function ``printf`` is in manual section 3).  To find out the manual section number, you can search the manual pages by typing ``man -k printf``, which will give a list of all man pages that contain the string ``printf``.  The section number is shown in parens after the function name.

.. index:: copying strings, strlcpy, strcpy

Copying strings
---------------

Recall that an array variable really just holds the memory address of the beginning of the array.  Thus, ``=`` (direct assignment) cannot be used to copy strings.  Instead, the characters must be copied one-by-one from one string to another.  Fortunately, the pain of doing this is (somewhat) alleviated by a number of built-in C library functions to do the work for us.  The best function to use for copying strings is called ``strlcpy``, which takes three parameters: the destination string buffer, the source string, and the size (number of bytes) in the destination string buffer.  For example, if we wanted to make a copy of a string that a user typed in, we could do the following:

.. _strlcpy-example:

.. code-block:: c

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    int main() {
        char buffer[128];
        printf("Gimme a string: ");
        fgets(buffer, 128, stdin);
        size_t size = strlen(buffer)+1; // add 1 for the null termination character!
        char copy[size];
        strlcpy(copy, buffer, size);
        return EXIT_SUCCESS;
    }

.. index:: buffer overflows and strcpy

Why, you may ask, do we need to pass the size of the destination string buffer as the third argument?  Can't the compiler figure it out?  Sadly, it cannot, especially in the case of variable length arrays and pointers (which we will encounter in a later chapter).  There is an "older" C library function called ``strcpy`` which only takes two parameters: the destination and the source strings.  One seriously bad thing that can happen with ``strcpy`` is exemplified by the following code:

.. code-block:: c

    char source[] = "this is a fairly long string, isn't it?";
    char dest[8];  // this is a rather small buffer, isn't it?
    strcpy(dest, source);

The ``strcpy`` function will happily copy the string referred to by ``source`` into the string referred to by ``dest``.  That's bad.  The length of ``source`` is *way* longer than ``dest``, so what happens is a *buffer overflow*.  That is, the ``strcpy`` function ends up blowing past the end of the 8 bytes we allocated to ``dest``, and starts writing data into what ever comes next (which happens to be on the stack of what ever function is executing).  Again, clearly bad stuff.  Even worse, the program may crash ... or it might not.  It's impossible to tell from the source code, because the behavior (according to the C language specification) is *undefined* [#f3]_.  The moral of the story: always use ``strlcpy``.  Also, it may be useful to note that :command:`scan-build`, described above, detects and prints a warning about this buffer overflow.

.. index:: comparing strings, strcmp, strncmp, strcasecmp, strncasecmp

Comparing strings
-----------------

Just as ``=`` cannot be used to copy strings, ``==`` cannot be used to compare strings.  The reason is very similar to why ``==`` cannot be used in the Java language to compare strings: the comparison for equality will just compare string *references* (or "pointers", which we will encounter soon) instead of comparing the *contents* of the strings.  

There are four C library functions that are commonly used to compare two strings.

``strcmp(s1, s2)``
    Compare C strings referred to by parameters s1 and s2. Return 0 if the string contents are equivalent, -1 if s1 lexicographically precedes s2, and 1 if s2 lexicographically precedes s1.

``strcasecmp(s1, s2)``
    Same as ``strcmp``, but compare the strings in a case-insensitive manner.

``strncmp(s1, s2, n)``
    Same as ``strcmp``, but only compare the first ``n`` characters of the two strings.  (Technically ``strncmp`` only compares the first min(n, strlen(s1), strlen(s2)) characters).

``strncasecmp(s1, s2, n)``
    Same as ``strncmp``, but compare the strings in a case-insensitive manner.

.. index:: islower, isupper, ispunct, isdigit, tolower, toupper, isspace, ctype, isalpha, isalnum

Another example
---------------

Let's look at one more example of a string manipulation program.  In this program, we ask the user for a string, then convert all characters in the string to lowercase.  

.. literalinclude:: code/tolower.c
   :language: c
   :linenos:

An example run of the program might look like this::

    Gimme a string: AbCDERX!!! whY?!
    Here's your string, lower-cased: abcderx!!! why?!

The core of the function is a for loop that iterates through all indexes of the string, checking whether each character should be lower-cased. The code above also demonstrates a couple functions from the ``#include <ctype.h>`` header file (``isupper`` and ``tolower``).  The ``isupper`` test (line 10) is, strictly speaking, unnecessary; calling ``tolower`` on an already-lowercased letter still results in a lowercase letter.  Otherwise, those two new functions behave as one might expect: given a character, they return either a new character, or a (quasi-)Boolean value [#f4]_.

There are quite a few functions defined in ``ctype.h``.  On MacOS X you can type :command:`man ctype` to get a list of those functions, and on Linux, you can type :command:`man islower` (or :command:`man <any ctype function>`) to get a catalog of all the various functions in ``ctype.h``.   The following is an incomplete list; see :command:`man` pages for gory details:

.. hlist::
   :columns: 4

   * ``isalnum``
   * ``isalpha``
   * ``isdigit``
   * ``ishexnumber``
   * ``islower``
   * ``isnumber``
   * ``isprint``
   * ``ispunct``
   * ``isspace``
   * ``isupper``
   * ``tolower``
   * ``toupper``

.. rubric:: Exercises

1.  Run the following program, which has a bad array index.  What is its behavior?  What if you change the ``for`` loop so that the second part of the for loop reads ``i <= max*100`` --- what happens then?

.. code-block:: c
   :linenos:
   
    #include <stdio.h>
    #include <stdlib.h>

    int main() {
        int max = 10;
        int array[max] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
        
        for (int i = 0; i <= max; i++) {
            printf("Array index %d contains %d\n", i, array[i]);
        }
        return EXIT_SUCCESS;
    }

2.  Write some code that computes the length of a string without using the built-in ``strlen`` function.  (Defining new functions is described in a later chapter, but with some Java and/or Python knowledge, you can probably make a good guess at how to define a new function in C.)

3.  Implement your own version of ``strlcpy``.   Instead of calling ``strlcpy`` on the second to last line of the :ref:`strlcpy example above <strlcpy-example>`, write your own ``for`` loop (or some other kind of loop) to accomplish the same thing.

4.  Write a program that asks for a string from a user and "strips" all whitespace characters from the end of the string (spaces, tabs and newlines).  To do that, you can simply assign a null character to the character array index that follows the last non-whitespace character of the string.

5.  Write a program that asks for a string from a user and prints the string in reverse.  
6.  Write a program that asks for a string from a user and prints whether the string is a palindrome.  Don't implement this problem recursively; check the characters within the string in some type of loop structure.  In your implementation, ignore non-letters and treating the string in a case-insensitive manner.  For example, "A man, a plan, a canal, Panama!" should be considered a valid palindrome.

7.  Write a program that asks for two strings and prints whether the two strings are anagrams of each other.  This is somewhat challenging to do given what has been covered in C thus far, but good practice!


.. rubric:: Footnotes

.. [#f1a] http://valgrind.org

.. [#f1b] https://clang-analyzer.llvm.org/scan-build.html

.. [#f2] http://en.wikipedia.org/wiki/Row-major_order

.. [#f3] http://en.wikipedia.org/wiki/Undefined_behavior

.. [#f4] The ``isupper`` function returns an ``int``, not ``bool``, which is fairly common in C. Since the ``bool`` type didn't get added to the language until fairly recently, most predicate functions return an integer representing True (1) or False (0).
