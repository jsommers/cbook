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

 2. Sometimes it is undesirable to copy the value from the caller to the callee because the value is large and copying is expensive, or because, for some reason, copying the value is undesirable.  


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

Ok, enough of the anti-examples.  Here is an example of passing *and* returning a ``struct``.  We'll write a function to add two fractions together and return a new fraction.  A few things to note about the code below:

 * The computation of the greatest common divisor is recursive.
 * The use of ``abs`` in the least common multiple function requires ``#include <stdlib.h>`` at the top of the source code. 
 * The ``add_fractions`` function separately computes the denominator and numerator of the result of the addition, then just constructs and returns a new ``fraction_t``.  For both the ``fraction_t`` parameters *and* the return value, entire copies are made to get the arguments "in" to the function and to get the result "out".  

.. literalinclude:: code/addfrac.c
   :language: c
   :lines: 9-30


Example 3: passing an array to a function
-----------------------------------------

Passing an array parameter to a function is somewhat different in nature than the other parameter data types we've seen:

 1. It is often the case that it is not possible to know the correct array length when declaring the *formal* parameter in the function declaration.  This is actually a *good* thing in disguise: it forces us to write a more general function instead of one that specifies an array of a certain size.  

    For example, say we want to write a function to multiple several fractions together, where each fraction is an element of an array.  We want to write the function so that it can handle *any* array size.  We write it as shown below.  Notice that we leave the array size blank in the formal parameter, and pass a second parameter that specifies the number of array elements we should examine.  Since an array in C does not know its own size, we are forced to pass its size separately.

.. code-block:: c

    fraction_t multiply_fractions(fraction_t fraclist[], int num_fractions) {
        fraction_t result = { 1, 1};
        for (int i = 0; i < num_fractions; i++) {
            result.numerator *= fraclist[i].numerator;
            result.denominator *= fraclist[i].denominator;
        }
        return result;
    }

..

 2. The second issue that makes array parameters somewhat different than any other data type we've seen thus far is that an array variable refers to the *memory address* of the first element in the array.  As a result, **it is possible to modify the contents of an array that is passed to a function**.  Here is an example of a function that modifies a C string by overwriting any *trailing* whitespace characters with the null-character.  Notice that for C strings we do *not* need to pass the size of the string, since, by convention, the null character marks the end of the string (and thus we can just use the built-in ``strlen`` function).

.. code-block:: c

    void strip_trailing_whitespace(char string[]) {
        int index = strlen(string)-1;
        while (index >= 0) {
            if (isspace(string[index])) {
                string[index] = '\0';
                index -= 1;
            } else {
                // as soon as we encounter first non-whitespace
                // character, get out of loop
                break;
            }
        }
    }

    int main() {
        char s[128] = "hello\n\n\t";
        strip_trailing_whitespace(s);
        printf("%s\n", s);
        return 0;
    }

How does this jive with pass-by-value?  What happens here is that ``s`` in ``main`` holds the memory address of the array, which is allocated on the stack of ``main``.  When the ``strip_trailing_whitespace`` function is called, the value of ``s`` is copied to the parameter ``string``, but *the value itself is a memory address*.  So the ``string`` array inside ``strip_trailing_whitespace`` holds the same memory address as ``s`` back in ``main``.  Thus, these two variables *refer to the same array in memory*, as depicted in the figure below.  As a result, when we modify the string inside the function, the changes can be observed when we return back to ``main``.

.. figure:: figures/arrayparam.*
   
    An array parameter gets a copy of the *memory* address of the array passed into the function, and thus "points" back to the same array contents as can be observed outside the function.


.. topic:: No function overloading or default parameters in C

    In some languages, e.g., C++, it is permitted to have more than one function definition with the same name, as long as the two definitions differ in the number and data type(s) of parameters.  Other languages permit "default" parameters, which means that if a caller chooses *not* to pass a particular argument, the parameter gets a *default* value.  Unfortunately, C's syntax does not permit either of these fairly convenient techniques.

A longer example
----------------

We'll wrap up this section with one more example.  A few things to note:

 * The ``struct student`` has an *embedded* struct field (``struct course_grade``).  Actually, an array of ``struct course_grade``.  One ``struct student`` would occupy a pretty large chunk of memory.  It is left to you to compute how many bytes, and where any padding is silently inserted by the compiler.

 * In ``struct student`` we need to keep the field ``num_courses_completed`` to know how many array elements in ``courses_completed`` are meaningful.

 * The ``typedef``\ s on lines 16-17 help to save a few keystrokes with the ``struct`` usage.

 * In ``compute_gpa``, we don't need to specify the size of the ``grade_t`` array, but we do need an additional parameter to tell us how many entries in the array we should consider.

 * The initialization syntax for the array of ``student_t`` just follows the rules we've discussed for array and struct initialization.  It is perfectly valid to nest the curly braces where necessary to achieve the correct field initializations.

.. literalinclude:: code/structparam.c
   :linenos:
   :language: c

..

Compiling and running this code gives the following output::

    A. Student, Class of 2019, GPA: 3.00
    B. Smart, Class of 2018, GPA: 2.25


Storage classes, the stack and the heap
=======================================


.. sidebar:: Static variables and the static storage class

    The keyword ``static`` has two, somewhat different, meanings and usages in C.  
    
    The first usage is that variables within functions can be prefixed with the ``static`` keyword to indicate that their value is retained across invocations of the function.  For example, consider the following function::

        void myfunction() {
            static int i = 0;
            i += 1;
            printf("i is now %d\n", i);
        }

    Because of the ``static`` keyword, the value stored in ``i`` is retained across multiple calls of ``myfunction``.  Without the ``static`` keyword, the output of the function would *always* be ``i is now 1``.

    The second meaning of ``static`` in C is to indicate that functions or variables defined outside any function should be *local* to the source file in which they are defined.  In :ref:`compilation-and-program-structure` we will discuss header files, compilation, and issues related to this usage of ``static`` in more detail.


There are two essential "storage classes" in C: *automatic* and *static*.  Static storage is somewhat of an advanced concept, and you can refer to the sidebar for a brief discussion.  

"Automatic" variables are what we have used exclusively thus far: they are variables that come into existence when a code block is entered, and which are discarded when a code block is exited.  This type of allocation and deallocation is done on the call stack of the running program.  Recall that all parameters and local variables are allocated on the stack in a last-in, first-out manner.  This is exactly the idea behind the "automatic" storage class --- memory is *automatically* assigned on the stack [#f7]_.

It's worth repeating that all variables in examples we've considered to this point are "automatic" and allocated on the stack.  That goes for strings, arrays of various sorts, structures, etc.  Most often, we've declared some local variables in ``main`` (which are allocated on the stack of ``main``), and passed parameters into functions (which results in the creation of copies of those parameters in the stack frame of the called function).  

.. rubric:: Exercises

1.  Refactor and modularize the code in exercise 1 in the ``struct`` chapter.  At the very least, write functions to parse a single line into a struct, and to print out a struct.  

2.  Write a text-based program to play hangperson.  Many of you have probably written this sort of program in Python.  Test your mettle by writing it in C.


.. [#f1] There are advanced techniques that build upon the basic mechanisms available in C to, for example, mimic capabilities found in object-oriented programming languages.  As a introductory text, this book will not go into any of those techniques.  One additional technique we cover in this book is found in the chapter on :ref:`compilation-and-program-structure`, in which we discuss a technique that provides a type of information hiding by enabling functions to remain "hidden" on a per-file basis.

.. [#f2] http://en.wikipedia.org/wiki/Abstraction_principle_(programming)

.. [#f3] http://en.wikipedia.org/wiki/Don%27t_repeat_yourself

.. [#f4] http://en.wikipedia.org/wiki/Structured_programming

.. [#f5] http://en.wikipedia.org/wiki/Highlander_(film)

.. [#f6] http://en.wikipedia.org/wiki/Hungarian_notation

.. [#f7] The compiler is responsible for this magic.  It must emit the right code so that the stack is managed correctly and variables come into existence and go away at exactly the right point in execution.
