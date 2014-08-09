Introduction: The C Language
****************************

C is a professional programmer's language.  It is a *systems implementation language*, and was originally designed as a high-level language for creating the UNIX operating systems at Bell Labs back in the 1960s.  Prior to the development of C, all operating systems were developed in assembly language [#f1]_\ !  C's designers --- Brian Kernighan and Dennis Ritchie --- developed the language so that it could provide some level of portability of the operating system, while still allowing a programmer to manipulate low-level resources such as memory and I/O devices.  If you are interested in some the history behind the development of UNIX, see [Evolution]_ by Dennis Ritchie.

C was designed to get in one's way as little as possible.  It is a high-level programming language (i.e., not assembly), but is quite minimal in design and scope.  Its minimalism can make it feel difficult to work with.  Its syntax is somewhat terse, and it does not contain the sort of "guardrails" available in other programming languages such as Python or Java.  There are no classes, objects, or methods, only functions.  C's type system and error checks exist only at compile-time.  The compiled code runs in a stripped down run-time model with no safety checks for bad type casts, bad array indices, or bad memory accesses.  There is no garbage collector to manage memory as in Java and Python.  Instead, the programmer must manage heap memory manually.  All this makes C fast but fragile.

Some languages, like Python, are forgiving.  The programmer needs only a basic sense of how things work.  Errors in the code are flagged by the compile-time or run-time system, and a programmer can muddle through and eventually fix things up to work correctly.  The C language is not like that.  The C programming model assumes that the programmer knows exactly what he or she wants to do, and how to use the language constructs to achieve that goal. The language lets the expert programmer express what they want in the minimum time by staying out of their way.

As a result, C can be hard to work with at first.  A feature can work fine in one context, but crash in another. The programmer needs to understand how the features work and use them correctly.  On the other hand, the number of features is pretty small, and the simplicity of C is beautiful in its own way.  What seem like limitations at first can feel liberating as you gain more experience with the language.

As you start to learn and use C, a good piece of advice is to just *be careful*.  Don't write code that don't understand.  Debugging can be challenging in C, and the language is unforgiving.  Create a mental (or real) picture of how your program is using memory.  That's good advice for writing code in any language, but in C it is critical.

.. sidebar:: C's popularity

   Although introduced over 40 years ago, C is one of the most popular programming languages in use today [#f2]_\ .Moreover, C's syntax has highly influenced the design of other programming languages (Java syntax is largely based on C).  All modern operating systems today are written largely in C (or a combination of C and C++) due to the fact that low-level resources (memory and I/O devices) can be directly manipulated with a minimum of assembly.  


This book is intended to be a short, though mostly complete introduction to the C programming language.  For an in-depth treatment of the language and language features, there are two other books to recommend.  The mostly widely used C book is one simply referred to as "K&R", written by the designers of the language, Brian Kernighan and Dennis Ritchie [K&R]_\ .  It is an excellent (though somewhat dated) reference to the language.  An excellent, if lengthy, introduction to C can be found in Stephen Prata's *C Primer Plus* [CPP]_\ .  It contains a more modern treatment of C than K&R, with lots of detail and exercises.  


.. [K&R] B. Kernighan and D. Ritchie.  *The C Programming Language, 2nd ed.*.  Prentice-Hall, 1988.  http://cm.bell-labs.com/cm/cs/cbook/   

.. [CPP] S. Prata.  *C Primer Plus (5th ed.)*.  S. Prata.  SAMS Publishing (2005).

.. [Evolution] D. Ritchie. *The Evolution of the Unix Time-sharing System*, AT&T Bell Laboratories Technical Journal 63 No. 6 Part 2, October 1984, pp. 1577-93.  Available at: http://cm.bell-labs.com/who/dmr/hist.html


.. [#f1] Take a moment and consider how difficult it must have been to develop a new OS!

.. [#f2] See http://www.tiobe.com/index.php/content/paperinfo/tpci/index.html for one ranking of programming language popularity.
