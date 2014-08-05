Basic Types and Operators
*************************

C provides a standard, minimal set of basic data types.  Sometimes these are called "primitive" types.  More complex data structures can be built up from these basic types.

Integer Types
=============

The "integral" types in C form a family of integer types. They all behave like integers and can be mixed together and used in similar ways. The differences are due to the different number of bits ("widths") used to implement each type --- the wider types can store a greater ranges of values.

``char``: One ASCII character
    The size of a ``char`` is almost always 8 bits, or 1 byte.  Pronounced "car".  8 bits provides a signed range of -128..127 or an unsigned range is 0..255, which is enough to hold a single ASCII character. ``char`` is also required to be the "smallest addressable unit" for the machine --- each byte in memory has its own address.

``short``: A "small" integer
    A ``short`` is typically 16 bits, which provides a signed range of -32768..32767.  It is less common to use a ``short`` than a ``char``, ``int``, or something larger.

``int``: A "default" integer size
    An ``int`` is typically 32 bits (4 bytes), though it is only guaranteed to be at least 16 bits.  It is defined to be the "most comfortable" size for the computer architecture for which the compiler is targetted.  If you do not really care about the range for an integer variable, declare it ``int`` since that is likely to be an appropriate size which works well for that machine.

``long``: A large integer
    A least 32 bits.  On a 32-bit machine, it will usually be 32 bits, but on a 64 bit machine, it will usually be 64 bits.  
    
``long long``
    Most modern compilers support ``long long`` as an integer type.  It is usually 64 bits.

.. sidebar:: Types in Python compared with C

   For those who know Python, recall that there are four basic types:
   int, float, str, and bool.  A float in Python is identical to a double
   in C, but none of the other Python types have an exact analog in C.
   An int in Python can hold an integer of arbitrary size, but that's
   definitely not true in C!  

The integer types can be preceded by the qualifier ``unsigned`` which disallows representing negative numbers, but doubles the largest positive number representable. For example, a 16 bit implementation of short can store numbers in the range -32768..32767, while ``unsigned short`` can store 0..65535. You can think of pointers as being a form of unsigned long on a machine with 4 byte pointers. In my opinion, it's best to avoid using unsigned unless you really need to. It tends to cause more misunderstandings and problems than it is worth.

.. literalinclude:: code/sizes.c
   :linenos:
   :language: c


.. sidebar:: Extra: Portability Problems

    Instead of defining the exact sizes of the integer types, C defines lower bounds. This makes it easier to implement C compilers on a wide range of hardware. Unfortunately it occasionally leads to bugs where a program runs differently on a 16-bit-int machine than it runs on a 32-bit-int machine. In particular, if you are designing a function that will be implemented on several different machines, it is best to explicitly specify the sizes of integral types.  If you ``#include <stdint.h>``, you can use types that explicitly indicate their bit-widths: ``int8_t``, ``int16_t``, ``int32_t``, and ``int64_t``.  There are also ``unsigned`` variants of these types: ``uint8_t``, ``uint16_t``, ``uint32_t``, and ``uint64_t``.   

    For some operating systems-related functions it is extremely important to be sure that a variable is *exactly* of a given size. These types come in handy in those situations, too.


``char`` Constants
------------------

A ``char`` constant is written with single quotes (') like ``'A'`` or ``'z'``. The char constant ``'A'`` is really just a synonym for the ordinary integer value 65, which is the ASCII value for uppercase 'A'. There are special case char constants, such as '\t' for tab, for characters which are not convenient to type on a keyboard.

``'A'``
    uppercase 'A' character

``'\n'``
    newline character

``'\t'``
    tab character

``'\0'``
    the "null" character -- integer value 0 (different from the char digit '0')

``'\012'``
    the character with value 12 in octal, which is decimal 10

``int`` Constants
-----------------

Numbers in the source code such as 234 default to type int. They may be followed by an 'L' (upper or lower case) to designate that the constant should be a long such as 42L. An integer constant can be written with a leading 0x to indicate that it is expressed in hexadecimal -- 0x10 is way of expressing the number 16. Similarly, a constant may be written in octal by preceding it with "0" -- 012 is a way of expressing the number 10.

Type Combination and Promotion
------------------------------

The integral types may be mixed together in arithmetic expressions since they are all basically just integers with variation in their width. For example, char and int can be combined in arithmetic expressions such as (``'b' + 5``). How does the compiler deal with the different widths present in such an expression? In such a case, the compiler "promotes" the smaller type (char) to be the same size as the larger type (int) before combining the values. Promotions are determined at compile time based purely on the types of the values in the expressions. Promotions do not lose information --- they always convert from a type to compatible, larger type to avoid losing information.


.. sidebar:: Pitfall: ``int`` overflow

    I once had a piece of code which tried to compute the number of bytes in a buffer with the expression (k * 1024) where k was an int representing the number of kilobytes I wanted. Unfortunately this was on a machine where int happened to be 16 bits. Since k and 1024 were both int, there was no promotion. For values of k >= 32, the product was too big to fit in the 16 bit int resulting in an overflow. The compiler can do whatever it wants in overflow situations -- typically the high order bits just vanish. One way to fix the code was to rewrite it as (k * 1024L) -- the long constant forced the promotion of the int. This was not a fun bug to track down -- the expression sure looked reasonable in the source code. Only stepping past the key line in the debugger showed the overflow problem. "Professional Programmer's Language." This example also demonstrates the way that C only promotes based on the types in an expression. The compiler does not consider the values 32 or 1024 to realize that the operation will overflow (in general, the values don't exist until run time anyway). The compiler just looks at the compile time types, int and int in this case, and thinks everything is fine.

Floating point Types
--------------------

``float``
    Single precision floating point number typical size: 32 bits 

``double``
    Double precision floating point number typical size: 64 bits

``long double``
    Possibly even bigger floating point number (somewhat obscure)

Constants in the source code such as 3.14 default to type double unless the are suffixed with an 'f' (float) or 'l' (long double). Single precision equates to about 6 digits of precision and double is about 15 digits of precision. Most C programs use double for their computations. The main reason to use float is to save memory if many numbers need to be stored. The main thing to remember about floating point numbers is that they are inexact. For example, what is the value of the following double expression?


   (1.0/3.0 + 1.0/3.0 + 1.0/3.0)    // is this equal to 1.0 exactly?

The sum may or may not be 1.0 exactly, and it may vary from one type of machine to another. For this reason, you should never compare floating numbers to eachother for equality (``==``) --- use inequality (``<``) comparisons instead. Realize that a correct C program run on different computers may produce slightly different outputs in the rightmost digits of its floating point computations.

Comments
--------

Comments in C are enclosed by slash/star pairs::

    /* .. comments .. */ 

which may cross multiple lines. C++ introduced a form of comment started by two slashes and extending to the end of the line::
    // comment until the line end

The ``//`` comment form is so handy that many C compilers now also support it, although it is not technically part of the C language.

Along with well-chosen function names, comments are an important part of well written code. Comments should not just repeat what the code says. Comments should describe what the code accomplishes which is much more interesting than a translation of what each statement does. Comments should also narrate what is tricky or non-obvious about a section of code.

Variables
---------

As in most languages, a variable declaration reserves and names an area in memory at run time to hold a value of particular type. Syntactically, C puts the type first followed by the name of the variable. The following declares an int variable named "num" and the 2nd line stores the value 42 into num::

   int num;
   num = 42;

.. todo::

   Make a "number box" for the above
   
A variable corresponds to an area of memory which can store a value of the given type. Making a drawing is an excellent way to think about the variables in a program. Draw each variable as box with the current value inside the box. This may seem like a "beginner" technique, but when I'm buried in some horribly complex programming problem, I invariably resort to making a drawing to help think the problem through.

Variables, such asnum, do not have their memory cleared or set in any way when they are allocated at run time. Variables start with random values, and it is up to the program to set them to something sensible before depending on their values.

Names in C are case sensitive so "x" and "X" refer to different variables. Names can contain digits and underscores (_), but may not begin with a digit. Multiple variables can be declared after the type by separating them with commas. C is a classical "compile time" language -- the names of the variables, their types, and their implementations are all flushed out by the compiler at compile time (as opposed to figuring such details out at run time like an interpreter)::

    float x, y, z, X;

Assignment Operator =
---------------------

The assignment operator is the single equals sign (=)::

    i = 6;
    i = i + 1;

The assignment operator copies the value from its right hand side to the variable on its left hand side. The assignment also acts as an expression which returns the newly assigned value. Some programmers will use that feature to write things like the following::

    y = (x = 2 * x);     // double x, and also put x's new value in y

Truncation
----------

The opposite of promotion, truncation moves a value from a type to a smaller type. In that case, the compiler just drops the extra bits. It may or may not generate a compile time warning of the loss of information. Assigning from an integer to a smaller integer (e.g.. long to int, or int to char) drops the most significant bits. Assigning from a floating point type to an integer drops the fractional part of the number::

    char ch;
    int i;
    i = 321;
    ch = i;     // truncation of an int value to fit in a char
    // ch is now 65

The assignment will drop the upper bits of the int 321. The lower 8 bits of the number 321 represents the number 65 (321 - 256). So the value of ch will be (char)65 which happens to be 'A'.

The assignment of a floating point type to an integer type will drop the fractional part of the number. The following code will set i to the value 3. This happens when assigning a floating point number to an integer or passing a floating point number to a function which takes an integer::

    double pi;
    int i;
    pi = 3.14159;
    i = pi;     // truncation of a double to fit in an int
    // i is now 3

Pitfall -- int vs. float Arithmetic
-----------------------------------

Here's an example of the sort of code where int vs. float arithmetic can cause problems. Suppose the following code is supposed to scale a homework score in the range 0..20 to be in the range 0..100::

    {
        int score;
        ...  // suppose score gets set in the range 0..20 somehow
        score = (score / 20) * 100;         // NO -- score/20 truncates to 0
        ...
    }


Unfortunately, score will almost always be set to 0 for this code because the integer division in the expression (score/20) will be 0 for every value of score less than 20. The fix is to force the quotient to be computed as a floating point number::

    score = ((double)score / 20) * 100; // OK -- floating point division from cast
    score = (score / 20.0) * 100;  // OK -- floating point division from 20.0
    score = (int)(score / 20.0) * 100; // NO -- the (int) truncates the floating
                                       // quotient back to 0

No Boolean -- Use int
---------------------

C does not have a distinct boolean type-- int is used instead. The language treats integer 0 as false and all non-zero values as true. So the statement::

    i = 0;
    while (i - 10) {
        ...

will execute until the variable i takes on the value 10 at which time the expression (i - 10) will become false (i.e. 0). (we'll see the while() statement a bit later).

Mathematical Operators
----------------------

C includes the usual binary and unary arithmetic operators. See the appendix for the table of precedence. Personally, I just use parenthesis liberally to avoid any bugs due to a misunderstanding of precedence. The operators are sensitive to the type of the operands. So division (/) with two integer arguments will do integer division. If either argument is a float, it does floating point division. So (6/4) evaluates to 1 while (6/4.0) evaluates to 1.5 --- the 6 is promoted to 6.0 before the division.

``+``
    Addition
``-``
    Subtraction
``/``
    Division
``*``
    Multiplication
``%``
    Remainder (mod)

Unary Increment Operators: ``++`` and ``--``
--------------------------------------------

The unary ``++`` and ``--`` operators increment or decrement the value in a variable. There are "pre" and "post" variants for both operators which do slightly different things (explained below)

``var++``
    increment "post" variant 
``++var``
    increment "pre" variant
``var--``
    decrement "post" variant 
``--var``
    decrement "pre" variant

An example using post increment/decrement::

    int i = 42;
    i++;     // increment on i
    // i is now 43
    i--;     // decrement on i
    // i is now 42

Pre and Post Variations
-----------------------

The Pre/Post variation has to do with nesting a variable with the increment or decrement operator inside an expression -- should the entire expression represent the value of the variable before or after the change? I never use the operators in this way (see below), but an example looks like::

    int i = 42;
    int j;
    j = (i++ + 10);
    // i is now 43
    // j is now 52 (NOT 53)
    j = (++i + 10)
    // i is now 44
    // j is now 54

C Programming Cleverness and Ego Issues
---------------------------------------

Relying on the difference between the pre and post variations of these operators is a classic area of C programmer ego showmanship. The syntax is a little tricky. It makes the code a little shorter. These qualities drive some C programmers to show off how clever they are. C invites this sort of thing since the language has many areas (this is just one example) where the programmer can get a complex effect using a code which is short and dense.

If I want j to depend on i's value before the increment, I write::

    j = (i + 10);
    i++;

Or if I want to j to use the value after the increment, I write::

    i++;
    j = (i + 10);

Now then, isn't that nicer? (editorial) Build programs that do something cool rather than programs which flex the language's syntax.   Who cares about syntax, anyway?

Relational Operators
--------------------
These operate on integer or floating point values and return a 0 or 1 boolean value. 

``==``
    Equal
``!=``
    Not Equal
``>``
    Greater Than
``<``
    Less Than
``>=``
    Greater or Equal 
``<=``
    Less or Equal

To see if x equals three, write something like::

    if (x == 3) ...


Pitfall: ``= != ==``
--------------------

An absolutely classic pitfall is to write assignment (``=``) when you mean comparison (``==``). This would not be such a problem, except the incorrect assignment version compiles fine because the compiler assumes you mean to use the value returned by the assignment. This is rarely what you want ::

    if (x = 3) ...

This does not test if x is 3. This sets x to the value 3, and then returns the 3 to the if for testing. 3 is not 0, so it counts as "true" every time. This is probably the single most common error made by beginning C programmers. The problem is that the compiler is no help -- it thinks both forms are fine, so the only defense is extreme vigilance when coding. Or write "``=`` is not ``=``!" in big letters on the back of your hand before coding. This mistake is an absolute classic and it's a bear to debug. Watch Out! And need I say: "Professional Programmer's Language."

Logical Operators
-----------------

The value 0 is false, anything else is true. The operators evaluate left to right and stop as soon as the truth or falsity of the expression can be deduced. (Such operators are called "short circuiting") In ANSI C, these are furthermore guaranteed to use 1 to represent true, and not just some random non-zero bit pattern. However, there are many C programs out there which use values other than 1 for true (non-zero pointers for example), so when programming, do not assume that a true boolean is necessarily 1 exactly.

``!``
    Boolean not (unary) 
``&&``
    Boolean and
``||`` 
    Boolean or


Bitwise Operators
-----------------
C includes operators to manipulate memory at the bit level. This is useful for writing low-level hardware or operating system code where the ordinary abstractions of numbers, characters, pointers, etc... are insufficient -- an increasingly rare need. Bit manipulation code tends to be less "portable". Code is "portable" if with no programmer intervention it compiles and runs correctly on different types of computers. The bitwise operations are typically used with unsigned types. In particular, the shift operations are guaranteed to shift 0 bits into the newly vacated positions when used on unsigned values.

``~`` 
    Bitwise Negation (unary) – flip 0 to 1 and 1 to 0 throughout 
``&`` 
    Bitwise And
``|`` 
    Bitwise Or
``^`` 
    Bitwise Exclusive Or
``>>`` 
    Right Shift by right hand side (RHS) (divide by power of 2)
``<<`` 
    Left Shift by RHS (multiply by power of 2)

Do not confuse the Bitwise operators with the logical operators. The bitwise connectives are one character wide (``&``, ``|``) while the boolean connectives are two characters wide (``&&``, ``||``). The bitwise operators have higher precedence than the boolean operators. The compiler will never help you out with a type error if you use ``&`` when you meant ``&&``. As far as the type checker is concerned, they are identical-- they both take and produce integers since there is no distinct boolean type.

Other Assignment Operators
--------------------------

In addition to the plain ``=`` operator, C includes many shorthand operators which represents variations on the basic ``=``. For example ``+=`` adds the right hand side to the left hand side. ``x = x + 10`` can be reduced to ``x += 10``. This is most useful if ``x`` is a long expression such as the following, and in some cases it may run a little faster ::

   person->relatives.mom.numChildren += 2;      // increase children by 2

Here's the list of assignment shorthand operators:

``+=, -=``
    Increment or decrement by RHS
``*=, /=``
    Multiply or divide by RHS
``%=``
    Mod by RHS
``>>=``
    Bitwise right shift by RHS (divide by power of 2)
``<<=``
    Bitwise left shift by RHS (multiply by power of 2)
``&=, |=, ^=``
    Bitwise and, or, xor by RHS



