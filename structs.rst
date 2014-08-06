Aggregate Data Structures
*************************

The C ``struct``
================

C has the usual facilities for grouping things together to form composite types-- arrays and records (which are called "structures"). The following definition declares a type called "struct fraction" that has two integer sub fields named "numerator" and "denominator". If you forget the semicolon it tends to produce a syntax error in whatever thing follows the struct declaration::

    struct fraction {
        int numerator;
        int denominator;
    };          // Don't forget the semicolon!

This declaration introduces the type struct fraction (both words are required) as a new type. C uses the period (.) to access the fields in a record. You can copy two records of the same type using a single assignment statement, however == does not work on structs::

    struct fraction f1, f2;          // declare two fractions
    f1.numerator = 22;
    f1.denominator = 7;
    f2 = f1;    // this copies over the whole struct


Arrays of structs
-----------------

The following declares an array named "numbers" which holds 1000 struct fraction's.

::

    struct fraction numbers[1000];
    numbers[0].numerator = 22;       // set the 0th struct fraction 
    numbers[0].denominator = 7;

Here's a general trick for unraveling C variable declarations: look at the right hand side and imagine that it is an expression. The type of that expression is the left hand side. For the above declarations, an expression which looks like the right hand side (numbers[1000], or really anything of the form numbers[...]) will be the type on the left hand side (struct fraction).


Multidimensional Arrays
=======================

The following declares a two-dimensional 10 by 10 array of integers and sets the first and last elements to be 13::

    int board [10][10];
    board[0][0] = 13;
    board[9][9] = 13;

The implementation of the array stores all the elements in a single contiguous block of memory. The other possible implementation would be a combination of several distinct one dimensional arrays -- that's not how C does it. In memory, the array is arranged with the elements of the rightmost index next to each other. In other words, board[1][8] comes right before board[1][9] in memory.

(highly optional efficiency point) It's typically efficient to access memory which is near other recently accessed memory. This means that the most efficient way to read through a chunk of the array is to vary the rightmost index the most frequently since that will access elements that are near each other in memory.
