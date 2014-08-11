.. _functions:

Functions
*********

All programming languages have built-in mechanisms for *structuring* and *modularizing* the code.  The main mechanism that C provides is the *subroutine*, or *function*.  In fact, C provides little beyond this basic technique [#f1]_.

Good program design in C (and many other programming languages) involves creating what are typically short functions that accomplish one task, and in which there is little or no duplication of functionality.  The main reasons for creating short, single-purpose functions is to make them more easily testable and to make them easier to read.  There are many benefits to having one place in the code where each major component is implemented, including making it easier to modify the code and making it easier to test.  These ideas are so important in programming that they are present in many different design principles, such as the Abstraction Principle [#f2]_, the Don't Repeat Yourself (DRY) principle [#f3]_, and structured programming [#f4]_.

.. sidebar:: The Abstraction Principle

    "Each significant piece of functionality in a program should be implemented in just one place in the source code. Where similar functions are carried out by distinct pieces of code, it is generally beneficial to combine them into one by abstracting out the varying parts."

    Benjamin C. Pierce, "Types and Programming Languages" (http://www.cis.upenn.edu/~bcpierce/tapl/).


Function syntax
===============

A function has a name, a comma-separated list of parameters, the block of code it executes when called, and, optionally, a return value.  The basic function definition syntax is:: 

    return-type function-name(parameters) { code-block }

For example, here is a function that computes and returns :math:`n!` (n factorial):

.. code-block:: c
   :linenos:

    /*
     * iteratively computes and returns n!
     * if n < 0, returns 0.
     */
    int factorial(int n) {
        if (n < 0) {
            return 0;
        } 
        int result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i; 
        } 
        return result;
    }

We have seen most of this syntax already (with the ``main`` function), but it is worth reviewing.  

1.  On line 5, the type declaration of the function is given, and shows that the data type of the return value of the function is ``int``, and that the function takes a single ``int`` parameter, named ``n``.  The function is, of course, named ``factorial``.  

2.  There can be any number of parameters to a function (though a small number of parameters is strongly preferable).  Each parameter is separated by a comma, and follows the basic syntax of a variable declaration, which is: ``type-name variable-name``.  

3.  Any parameters to the function are *passed by value*.  This means that the function receives *copies* of the arguments passed to it by the caller.  If any modifications are made to those copies, they have *zero effect* on the variables or values passed to the function --- any changes are confined (*local*) to the function.  We will have more to say about pass-by-value function call semantics, below.

4.  The ``return`` statement delivers a value from the function back to the caller of the function.  There are ``return`` statements on lines 7 and 13 of this function.  The first ``return`` handles the special case where ``n`` is less than 0, thus :math:`n!` is undefined.  

5.  Any variables declared inside the function are *local* to the function, just as with Java, Python, and many other programming languages.  Thus, the parameter variable ``n`` and the local variable ``result`` only exist while the function is being executed.

To call the ``factorial`` function, a programmer uses parentheses after the function name, passing any required arguments between the parens:

.. code-block:: c
    
    #include <stdio.h>
    #include <stdlib.h>

    // ...

    char buffer[8];
    printf("I'll compute n! for you.  What should n be? ");
    fgets(buffer, 8, stdin);
    n = atoi(buffer);
    int result = factorial(n);
    printf ("%d! is %d\n", n, result);


So far, none of this should be particularly surprising.  You may have already seen "public static methods" in Java (e.g., ``main``!), which are very similar to C functions, or you may have already seen function in Python (defined using the ``def`` keyword).  Both public static methods in Java and functions in Python behave very similarly to functions in C.  In fact, all of these languages use pass-by-value semantics for parameters. 

``main`` is where it all begins
-------------------------------

Every C program **must** have a ``main`` function.  An attempt to compile a program in C which does not have a ``main`` function defined somewhere will result in an error.  Unlike Java, where any number of class definitions can have a ``public static void main`` definition, it's a highlander situation in C: *there can be only one* [#f5]_.


Function naming restrictions and conventions
--------------------------------------------

The only *requirement* for naming C functions is similar to many programming languages: function names must either begin with a letter or underscore, and can only include numbers, letters, and underscores.  Conventionally, function names that start with an underscore typically mean that the function should be treated as a "private" library function, off limits to other programmers.

As far as naming conventions, there are a wide variety of practices in existence.  Some programmers like to name their functions using ``lowerCamelCase``, which is common in languages such as Java, or using ``snake_case``, which is common in Python and Ruby.  In the C standard library, a common practice is to use short, abbreviated names consisting of a single word (e.g., ``tolower``).  Still others like to use the abomination referred to as Hungarian Notation [#f6]_.  A fairly widely used convention in C is ``snake_case``, which is the practice followed in this book.

Data types for parameters and return values
===========================================

There are, technically speaking, no restrictions on the data types of parameters or return values for functions in C.  Functions in C can accept all the basic data types as parameters, as well as ``struct`` types, arrays, and as we will see soon, memory pointers to any data type.

Likewise, there are no syntactic restrictions on the data type of the return value from a function.  C does not permit *multiple* return values, unlike some other languages, but it is permissible to return a ``struct`` type that contains multiple fields (or, as we will later see, a pointer to a memory block containing multiple data items).

.. topic:: C-ing and Nothingness --- ``void``

    ``void`` is a type formalized in ANSI C which means "nothing". To indicate that a function does not return anything, use ``void`` as the return type.  If a function does not take any parameters, its parameter list may either be empty (i.e., ``()``), or it can contain the keyword ``void`` to indicate that the function does not take parameters.  It is more common and conventional in C to use an empty parameter list for functions that don't take parameters.
    

Parameters to functions are passed by value
-------------------------------------------

The key thing to remember for function parameters is that they are *passed by value*.  (Note that Java also uses pass-by-value semantics for function parameters.)  Passing by value means that the actual parameter values are *copied* into local storage (on the stack).  This scheme is fine for many purposes, but it has two disadvantages:

 1. Because the function invocation ("callee") has its own copy (or copies) of parameters, modifications to that memory are always *local to the function*.  Therefore, value parameters do not allow the callee to communicate back to the caller. The function's return value can communicate some information back to the caller, but not all problems can be solved with the single return value.

 2. Sometimes it is undesirable to copy the value from the caller to the callee because the value is large and copying is expensive, or because, for some reason, copying the value is undesirable.  Modern compilers have generally made this issue moot: the overhead of copying is very often not something that a programmer should be concerned with.  


Example 1: an anti-example of swapping two values
-------------------------------------------------

As mentioned above, a key implication of pass-by-value function parameters is that the function gets *local* copies of any parameter values.  Say that we want to exchange two values via a function.  For example, we want to swap the numerator and denominator in a fraction ``struct``.  This is some code that would *not* work:

.. code-block:: c

    typedef struct fraction {
        int numerator;
        int denominator;
    } fraction_t;

    void swap_numerator_denominator1(fraction_t frac) {
        int tmp = frac.numerator;
        frac.numerator = frac.denominator;
        frac.denominator = tmp;
    }

    void swap_numerator_denominator2(int num, int den) {
        int tmp = num;
        num = den;
        den = tmp;
    }

    int main() {
        fraction_t f1 = { 1, 3};
        swap_numerator_denominator1(f1);  // swap?  uh, no.
        swap_numerator_denominator2(f1.numerator, f1.denominator);  // no, again
        return 0;
    }

Epic fail, times 2.  For each of the swap functions, the exchange happens only *inside* the function; the caller will *never* see any swap of nancy and donkey.  The fact that the function takes a ``struct`` here is irrelevant for attempt 1; even if we wrote a ``swap`` to take two ``int`` parameters (i.e., attempt 2), there is no way this is going to happen.  Sorry.  In the next chapter on :ref:`pointers`, we will solve this problem.  

Example 2: passing a ``struct`` to and from a function
------------------------------------------------------

Ok, enough of the anti-examples.  Here is an example of passing *and* returning a ``struct``.  We'll write a function to add two fractions together and return a new fraction.

.. literalinclude:: code/addfrac.c
   :language: c
   :lines: 9-30


Example 3: passing an array to a function
-----------------------------------------

example with multiplying fractions, return a fraction.  take an array, return fraction


.. todo::

   different types of function params: ints, arrays, structs (it's all pass by value!)


.. topic:: No function overloading or default parameters in C

    In some languages, e.g., C++, it is permitted to have more than one function definition with the same name, as long as the two definitions differ in the number and data type(s) of parameters.  Other languages permit "default" parameters, which means that if a caller chooses *not* to pass a particular argument, the parameter gets a *default* value.  Unfortunately, C's syntax does not permit either of these fairly convenient techniques.


A longer example
----------------


.. literalinclude:: code/structparam.c
   :linenos:
   :language: c

..

::

    A. Student, Class of 2019, GPA: 3.00
    B. Smart, Class of 2018, GPA: 2.25


Stack allocation, recursion, and return types
=============================================




Swap Example
------------ 

The classic example of wanting to modify the caller's memory is a ``swap()`` function which exchanges two values. Because C uses call by value, the following version of Swap will not work ::

    void Swap(int x, int y) {        // NO does not work
        int temp;
        temp = x;
        x = y;      // these operations just change the local x,y,temp
        y = temp;   // -- nothing connects them back to the caller's a,b
    }

    // Some caller code which calls Swap()...
    int a = 1;
    int b = 2;
    Swap(a, b);

``Swap()`` does not affect the arguments a and b in the caller. The function above only operates on the copies of a and b local to Swap() itself. This is a good example of how "local" memory such as ( x, y, temp) behaves -- it exists independent of everything else only while its owning function is running. When the owning function exits, its local memory disappears.


.. sidebar:: The keyword ``static``

    FIXME: add text about static in two forms: internal linkage and static variables in functions

    The keyword "static" defines that the function will only be available to callers in the file where it is declared. If a function needs to be called from another file, the function cannot be static and will require a prototype -- see prototypes below. The static form is convenient for utility functions which will only be used in the file where they are declared.


.. rubric:: Exercises

1.  Refactor exercise 1 in the ``struct`` chapter.  Write functions to parse a single line into a struct, and to print out a struct.


.. [#f1] There are advanced techniques that build upon the basic mechanisms available in C to, for example, mimic capabilities found in object-oriented programming languages.  As a introductory text, this book will not go into any of those techniques.  One additional technique we cover in this book is found in the chapter on :ref:`compilation-and-program-structure`, in which we discuss a technique that provides a type of information hiding by enabling functions to remain "hidden" on a per-file basis.

.. [#f2] http://en.wikipedia.org/wiki/Abstraction_principle_(programming)

.. [#f3] http://en.wikipedia.org/wiki/Don%27t_repeat_yourself

.. [#f4] http://en.wikipedia.org/wiki/Structured_programming

.. [#f5] http://en.wikipedia.org/wiki/Highlander_(film)

.. [#f6] http://en.wikipedia.org/wiki/Hungarian_notation
