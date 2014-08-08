Arrays and Strings
******************

Arrays
======

Arrays in C are declared and used much like they are in Java.  The syntax for using arrays in C is nearly identical to Java, and is very similar to the syntax used for Python lists.  As with both Python and Java, arrays in C are always indexed starting at 0.  Thus, in the following example, the first ``int`` in the ``scores`` array is ``scores[0]`` and the last is ``scores[99]``::

.. code-block:: c
   :linenos:

    int scores[100];
    scores[0]  = 13;           // set first element
    scores[99] = 42;           // set last element

The name of the array refers, in some sense, to the *whole array*. In actuality, the array name refers to the memory address at which the array storage begins. As in Java, elements of an array in C are stored *contiguously*.  Thus, for the above array, if the first element in the array is stored at memory address *x*, the next element is stored at *x+4* (since the ``int`` is 4 bytes on most machines today), as depicted in :ref:`array-figure`, below.

.. _array-figure:

.. figure:: figures/array.*
   :alt: A diagram of an array in C

   A memory diagram of the scores array.

Array initialization
--------------------

Note that because C does not do any automatic initialization of variables, the array has undefined contents at the point of declaration (line 1, above).  A common practice is to use either a simple ``for`` loop construct to set all values in the array to a specific value, e.g.,::

    int scores[100];
    for (int i = 0; i < 100; i++) {
        scores[i] = 0; 
    }

Another common practice is to use the ``memset`` function or ``bzero`` function to set everything in an array to zeroes.  The ``memset`` function is declared in ``strings.h`` (so you need to ``#include`` it), and takes three parameters: a memory address (which can just be the name of the array), the character that should be written into each byte of the memory address, and the number of bytes to set.  Thus, the above ``for`` loop could be replaced with the following::

    // at the top of your source code
    #include <string.h>

    int scores[100];
    memset(scores, 0, 100*sizeof(int));

Note that we need to specify the number of *bytes* we want to set to 0, thus we say ``sizeof(int)`` multiplied by the number of array elements.  It's always good practice to use ``sizeof``, even if you think you can assume that the size of an ``int`` is 4.  Don't make that assumption; use ``sizeof``.

One last way that array contents can be initialized is to use C initializer syntax. Say that we just want to create an array of 10 scores.  We could initialize the array as follows::

    int scores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

The initializer syntax in C is just a set of curly braces, within which are comma-separate values.  You can even leave off the array size if you give an initializer, and the compiler will figure out how large to make the array::

    int scores[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // equivalent to above

The initializer syntax is especially useful for small-ish arrays for which the initial values are not all identical.

``sizeof`` and arrays
---------------------

The built-in ``sizeof`` function works with arrays.  Specifically, it will return the number of bytes occupied by the array, which is quite helpful.  So, the ``memset`` code we wrote earlier could be replaced with::

    // at the top of your source code
    #include <string.h>

    int scores[100];
    memset(scores, 0, sizeof(scores)); 

Array bounds checking (not!)
----------------------------

It is a very common error to try to refer to non-existent array element. Unlike Java or Python, in which an out-of-bounds array (or list) reference will result in an exception, C will happily attempt to access the non-existent element.  The program behavior in such a case is *undefined*, which basically means that anything can happen.  Your program might crash, but it might not.  It might behave as you expect, but it might not.  Yuck.  Welcome to C.  

So what you can you do about this?  The best thing is to use good tools for detecting memory corruption and bad array accesses.  The :command:`valgrind` tool [#f1]_ is especially good in this regard, and is highly recommended.  Its output can be somewhat difficult to understand at first, but it is a hugely helpful tool when trying to debug seemingly random program behavior.


Variable length arrays
----------------------

At the point of declaration, the size of an array in C *can* be specified with a variable, which creates what is called a *variable length array*.  Variable length arrays were added to C in the C99 standard, so if you use a variable when specifying the size of an array and there is a compile-time error on that line, make sure that you are compiling in C99 mode (``-std=c99`` on :command:`clang` and :command:`gcc`).  Here is an example with using a variable-length array (notice that we're using the ``atoi`` function to convert a string to an integer):

.. literalinclude:: code/vararray.c
   :language: c
   :linenos:


C Strings
=========

C has minimal support of character strings. For the most part, strings operate as ordinary arrays of characters. Their maintenance is up to the programmer using the standard facilities available for arrays and pointers. C does include a standard library of functions which perform common string operations, but the programmer is responsible for the managing the string memory and calling the right functions. Unfortunately computations involving strings are very common, so becoming a good C programmer often requires becoming adept at writing code which manages strings which means managing pointers and arrays.

A C string is just an array of char with the one additional convention that a "null" character ('\0') is stored after the last real character in the array to mark the end of the string. The compiler represents string constants in the source code such as "binky" as arrays which follow this convention. The string library functions (see the appendix for a partial list) operate on strings stored in this way. The most useful library function is strcpy(char dest[], const char source[]); which copies the bytes of one string over to another. The order of the arguments to strcpy() mimics the arguments in of '=' -- the right is assigned to the left. Another useful string function is strlen(const char string[]); which returns the number of characters in C string not counting the trailing '\0'.

Note that the regular assignment operator (=) does not do string copying which is why strcpy() is necessary. See Section 6, Advanced Pointers and Arrays, for more detail on how arrays and pointers work.

The following code allocates a 10 char array and uses strcpy() to copy the bytes of the string constant "binky" into that local array.

::

    {
        char localString[10];
        strcpy(localString, "binky");
    }

.. todo::

   Make a memory drawing of the above

The memory drawing shows the local variable localString with the string "binky" copied into it. The letters take up the first 5 characters and the '\0' char marks the end of the string after the 'y'. The x's represent characters which have not been set to any particular value.

If the code instead tried to store the string "I enjoy languages which have good string support" into localString, the code would just crash at run time since the 10 character array can contain at most a 9 character string. The large string will be written passed the right hand side of localString, overwriting whatever was stored there.

String Code Example
-------------------

Here's a moderately complex for loop which reverses a string stored in a local array. It demonstrates calling the standard library functions strcpy() and strlen() and demonstrates that a string really is just an array of characters with a '\0' to mark the effective end of the string. Test your C knowledge of arrays and for loops by making a drawing of the

::

    {
        char string[1000];   // string is a local 1000 char array
        int len;
        strcpy(string, "binky");
        len = strlen(string);
        /*
            Reverse the chars in the string:
            i starts at the beginning and goes up
            j starts at the end and goes down
            i/j exchange their chars as they go until they meet
        */
        int i, j;
        char temp;
        for (i = 0, j = len - 1; i < j; i++, j--) {
            temp = string[i];
            string[i] = string[j];
            string[j] = temp;
        }
        // at this point the local string should be "yknib"
   }

"Large Enough" Strings
----------------------

The convention with C strings is that the owner of the string is responsible for allocating array space which is "large enough" to store whatever the string will need to store. Most routines do not check that size of the string memory they operate on, they just assume its big enough and blast away. Many, many programs contain declarations like the following ::

    {
        char localString[1000];
        // ...
    }

The program works fine so long as the strings stored are 999 characters or shorter. Someday when the program needs to store a string which is 1000 characters or longer, then it crashes. Such array-not-quite-big-enough problems are a common source of bugs, and are also the source of so called "buffer overflow" security problems. This scheme has the additional disadvantage that most of the time when the array is storing short strings, 95% of the memory reserved is actually being wasted. A better solution allocates the string dynamically in the heap, so it has just the right size.

To avoid buffer overflow attacks, production code should check the size of the data first, to make sure it fits in the destination string. See the strlcpy() function in Appendix A.

.. rubric:: Exercises

1.  Run the following program, which has a bad array index.  What is its behavior?  What if you change the ``for`` loop so that the second part of the for loop reads ``i <= max*100`` --- what happens then?

.. code-block:: c
   :linenos:
   
    #include <stdio.h>

    int main() {
        int max = 10;
        int array[max] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
        
        for (int i = 0; i <= max; i++) {
            printf("Array index %d contains %d\n", i, array[i]);
        }
        return 0;
    }

2.  Write a program to FIXME


.. rubric:: Footnotes

.. [#f1] http://valgrind.org
