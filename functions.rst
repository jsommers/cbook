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

.. code-block::
    
    #include <stdio.h>
    #include <stdlib.h>

    // ...

    char buffer[8];
    printf("I'll compute n! for you.  What should n be? ");
    fgets(buffer, 8, stdin);
    n = atoi(buffer);
    int result = factorial(n);
    printf ("%d! is %d\n", n, result);


So far, none of this should be particularly surprising.  You may have already seen "static methods" in Java, which are very similar to C functions, or you may have already seen function in Python (defined using the ``def`` keyword).  Both static methods in Java and functions in Python behave very similarly to functions in C.  In fact, all of these languages use pass-by-value semantics for parameters. 

Function naming restrictions and conventions
--------------------------------------------

The special function called ``main`` is where program execution begins. Some programmers like to name their functions using ``lowerCamelCase``, which is common in languages such as Java, or using ``snake_case``, which is common in Python and Ruby.  In the C standard library, a common practice is to use short, abbreviated names consisting of a single word (e.g., ``tolower``), but the only *real* requirement is that the name begin with either a letter or underscore, and that it not include any symbols besides underscores.  


.. topic:: C-ing and Nothingness -- void

    void is a type formalized in ANSI C which means "nothing". To indicate that a function does not return anything, use void as the return type. Also, by convention, a pointer which does not point to any particular type is declared as void*. Sometimes void* is used to force two bodies of code to not depend on each other where void* translates roughly to "this points to something, but I'm not telling you (the client) the type of the pointee exactly because you do not really need to know." If a function does not take any parameters, its parameter list is empty, or it can contain the keyword void but that style is now out of favor.


.. topic:: No function overloading allowed in C

    FIXME: mention that you can't have overloaded functions (same name, different parameters)


Function parameters are passed by value
---------------------------------------

C passes parameters "by value" which means that the actual parameter values are copied into local storage. The caller and callee functions do not share any memory -- they each have their own copy. This scheme is fine for many purposes, but it has two disadvantages.

 1. Because the callee has its own copy, modifications to that memory are not communicated back to the caller. Therefore, value parameters do not allow the callee to communicate back to the caller. The function's return value can communicate some information back to the caller, but not all problems can be solved with the single return value.

 2. Sometimes it is undesirable to copy the value from the caller to the callee because the value is large and so copying it is expensive, or because at a conceptual level copying the value is undesirable.


The alternative is to pass the arguments "by reference". Instead of passing a copy of a value from the caller to the callee, pass a pointer to the value. In this way there is only one copy of the value at any time, and the caller and callee both access that one value through pointers.

Some languages support reference parameters automatically. C does not do this -- the programmer must implement reference parameters manually using the existing pointer constructs in the language.



.. todo::

   different types of function params: ints, arrays, structs (it's all pass by value!)

.. todo::

   Forward reference to how this will work eventually (pointers; next chapter)

.. todo::
 
   Yes, recursion works.

Return values and restrictions
------------------------------

Can't return an array (it's allocated on the stack!)
Can return a struct, or any other primitive (non-pointer) type.


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

