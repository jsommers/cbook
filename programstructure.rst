Program structure
*****************

The main function
=================

The execution of a C program begins with function named main(). All of the files and libraries for the C program are compiled together to build a single program file. That file must contain exactly one main() function which the operating system uses as the starting point for the program. Main() returns an int which, by convention, is 0 if the program completed successfully and non-zero if the program exited due to some error condition. This is just a convention which makes sense in shell oriented environments such as Unix or DOS.

.. todo::

   Need to include stuff about arguments to main and return value from main

Multiple Files
--------------

For a program of any size, it's convenient to separate the functions into several separate files. To allow the functions in separate files to cooperate, and yet allow the compiler to work on the files independently, C programs typically depend on two features...

.. todo::
   
   * C compiler makes a single pass
   * 3 Steps: preprocessing, compiling, linking

Prototypes
----------

A "prototype" for a function gives its name and arguments but not its body. In order for a caller, in any file, to use a function, the caller must have seen the prototype for that function. For example, here's what the prototypes would look like for Twice() and Swap(). The function body is absent and there's a semicolon (;) to terminate the prototype ::

    int Twice(int num);
    void Swap(int* a, int* b);

In pre-ANSI C, the rules for prototypes where very sloppy -- callers were not required to see prototypes before calling functions, and as a result it was possible to get in situations where the compiler generated code which would crash horribly.

In ANSI C, I'll oversimplify a little to say that:

 1. a function may be declared static in which case it can only be used in the same file where it is used below the point of its declaration. Static functions do not require a separate prototype so long as they are defined before or above where they are called which saves some work.

 2. A non-static function needs a prototype. When the compiler compiles a function definition, it must have previously seen a prototype so that it can verify that the two are in agreement ("prototype before definition" rule). The prototype must also be seen by any client code which wants to call the function ("clients must see prototypes" rule).(The require-prototypes behavior is actually somewhat of a compiler option, but it's smart to leave it on.)

The preprocessor
================

The preprocessing step happens to the C source before it is fed to the compiler. The two most common preprocessor directives are #define and #include.
￼
#define
-------

The #define directive can be used to set up symbolic replacements in the source. As with all preprocessor operations, #define is extremely unintelligent -- it just does textual replacement without understanding. #define statements are used as a crude way of establishing symbolic constants.

::

    #define MAX 100
    #define SEVEN_WORDS that_symbol_expands_to_all_these_words

Later code can use the symbols MAX or SEVEN_WORDS which will be replaced by the text to the right of each symbol in its #define.

#include
--------

The "#include" directive brings in text from different files during compilation. #include is a very unintelligent and unstructured -- it just pastes in the text from the given file and continues compiling. The #include directive is used in the .h/.c file convention below which is used to satisfy the various constraints necessary to get prototypes correct.

::

    #include "foo.h"    // refers to a "user" foo.h file --
                        //    in the originating directory for the compile
    #include <foo.h>    // refers to a "system" foo.h file --
                        //    in the compiler's directory somewhere


foo.h vs. foo.c
---------------

The universally followed convention for C is that for a file named "foo.c" containing a bunch of functions:

 *  A separate file named foo.h will contain the prototypes for the functions in foo.c which clients may want to call. Functions in foo.c which are for "internal use only" and should never be called by clients should be declared static.
 * Near the top of foo.c will be the following line which ensures that the function definitions in foo.c see the prototypes in foo.h which ensures the "prototype before definition" rule above.
 
..

::

    #include "foo.h" // show the contents of "foo.h"
                     // to the compiler at this point

 *  Any xxx.c file which wishes to call a function defined in foo.c must include the following line to see the prototypes, ensuring the "clients must see prototypes" rule above.

::

    #include "foo.h"


#if
---

At compile time, there is some space of names defined by the #defines. The #if test can be used at compile-time to look at those symbols and turn on and off which lines the compiler uses. The following example depends on the value of the FOO #define symbol. If it is true, then the "aaa" lines (whatever they are) are compiled, and the "bbb" lines are ignored. If FOO were 0, then the reverse would be true.

::

    #define FOO 1

    ...

    #if FOO 
        aaa
        aaa 
    #else
        bbb
        bbb 
    #endif

You can use ``#if 0 ...#endif`` to effectively comment out areas of code you don't want to compile, but which you want to keeep in the source file.


Multiple #includes -- #pragma once
----------------------------------

There's a problem sometimes where a .h file is #included into a file more than one time resulting in compile errors. This can be a serious problem. Because of this, you want to avoid #including .h files in other .h files if at all possible. On the other hand, #including .h files in .c files is fine. If you are lucky, your compiler will support the #pragma once feature which automatically prevents a single file from being #included more than once in any one file. This largely solves multiple #include problems.

::

    // foo.h
    // The following line prevents problems in files which #include "foo.h"
    #pragma once
    <rest of foo.h ...>

.. todo::

   remove this pragma garbage

Assert
======

Array out of bounds references are an extremely common form of C run-time error. You can use the assert() function to sprinkle your code with your own bounds checks. A few seconds putting in assert statements can save you hours of debugging.

Getting out all the bugs is the hardest and scariest part of writing a large piece of software. Assert statements are one of the easiest and most effective helpers for that difficult phase.

::

    #include <assert.h>
    #define MAX_INTS 100
    {
        int ints[MAX_INTS];
        i = foo(<something complicated>);
        // i should be in bounds,
        // but is it really?
        assert(i>=0);           // safety assertions
        assert(i<MAX_INTS);
        ints[i] = 0;

Depending on the options specified at compile time, the assert() expressions will be left in the code for testing, or may be ignored. For that reason, it is important to only put expressions in assert() tests which do not need to be evaluated for the proper functioning of the program.

::

    int errCode = foo();      // yes
    assert(errCode == 0);
    assertfoo() == 0);        // NO, foo() will not be called if
                              // the compiler removes the assert()
