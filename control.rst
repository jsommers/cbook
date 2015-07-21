Control Structures
******************

In this chapter, we encounter the set of "control" structures in C, such as conditional statements and looping constructs.  As a preview, the control structures in C are nearly identical to those found in Java (since Java syntax is heavily based on C), and bear strong resemblance to control structures found in other programming languages.

.. index:: if statement

``if`` Statement
================

Both an ``if`` and an ``if-else`` are available in C.  The ``<expression>`` can be any valid expression. The parentheses around the expression are required, even if it is just a single variable:

.. code-block:: c

    if (<expression>) <statement>    // simple form with no {}'s or else clause

I **strongly** recommend against using the above form of curly brace-free ``if`` statement.  **Always use the curly braces**, even if you only have a single statement as part of an ``if`` or some other control structure.  Why, you ask, should I type those additional characters?  See the sidebar on the Apple SSL bug for why.


.. sidebar:: Why you should always use curly braces, even when they're optional
    
   For ``if`` statements and loop constructs that only contain a single line of code in the body, C allows the programmer *not* to include the curly braces.  Curly braces are only required in the case of multiple statements in the body of one of these constructs.  Using the curly braces, however, is very strongly recommended.  Just ask an unfortunate engineering group at Apple who introduced the "goto fail" bug into their SSL (secure sockets layer) library: a bug that affected MacOS X and iOS operating systems quite severely [#f1]_.


.. index:: curly braces

.. topic:: Code blocks use curly braces (``{}``)

    C uses curly braces (``{}``) to group multiple statements together, very much like Java.  Whitespace is
    generally insignificant, very much *unlike* Python.  Not surprisingly, within a code block the statements are executed in order.  

    Note that older versions of C (pre-C99) required that all variables be declared at the beginning of a code block.  Since the C99 standard, however, variables can be declared on any line of code, as in Java and C++.


As in Java, the ``else`` keyword can be used to provide alternative execution for a conditional expression.  Also similar to Java, multiple ``if`` ... ``else if`` statements can be chained together.  There is no ``elif`` as in Python (or ``elsif`` as in Ruby).

.. code-block:: c

    if (<expression>) { // simple form with {}'s to group statements
        <statements>
    }

    if (<expression>) { // full then/else form
        <statements>
    } else {
        <statements>
    }

    if (<expression1>) { // chained if/else if
        <statements>
    } else if (<expression2>) {
        <statements>
    } ...

.. index:: ternary operator

The conditional expression (ternary operator)
=============================================

The conditional expression can be used as a shorthand for some if-else statements. The general syntax of the conditional operator is:

.. code-block:: c

    <expression1> ? <expression2> : <expression3>

This is an expression, not a statement, so it represents a value. The operator works by evaluating expression1. If it is true (non-zero), it evaluates and returns expression2 . Otherwise, it evaluates and returns expression3.

The classic example of the ternary operator is to return the smaller of two variables. Every once in a while, the following form is just what you needed. Instead of:

.. code-block:: c

    if (x < y) {
        min = x;
    } else {
        min = y; 
    }

you can write:

.. code-block:: c

    min = (x < y) ? x : y;

The ternary operator is viewed by some programmers as "excessively tricky" since expressions with such operators can be hard to read.  Use your best judgment, and don't do something this horrific_.

.. index:: switch statement

``switch`` statement
====================

The switch statement is a sort of specialized form of ``if`` used to efficiently separate different blocks of code based on the value of an integer.  The ``switch`` expression is evaluated, and then the flow of control jumps to the matching const-expression ``case``. The ``case`` expressions are typically ``int`` or ``char`` constants (unfortunately, you cannot use strings as ``case`` expressions).  The ``switch`` statement is probably the single most syntactically awkward and error-prone features of the C language:

.. code-block:: c

    switch (<expression>) {
        case <const-expression-1>:
            <statement>
            break;
        case <const-expression-2>:
            <statement>
            break;
        case <const-expression-3>:
        case <const-expression-4>:  // here we combine case 3 and 4
            <statement>
            break;
        default:    // optional
            <statement>
    }

Each constant needs its own ``case`` keyword and a trailing colon (:).  Once execution has jumped to a particular case, the program will keep running through all the cases from that point down --- this so called *fall through* operation is used in the above example so that ``expression-3`` and ``expression-4`` run the same statements. The explicit ``break`` statements are necessary to exit the ``switch``. Omitting the ``break`` statements is a common error --- it compiles, but leads to inadvertent fall-through behavior.

Why does the ``switch`` statement fall-through behavior work the way it does? The best explanation might be that C was originally developed for an audience of assembly language programmers. The assembly language programmers were used to the idea of a jump table with fall-through behavior, so that's the way C does it (it's also relatively easy to implement it this way). Unfortunately, the audience for C is now quite different, and the fall-through behavior is widely regarded as an unfortunate part of the language.

.. index:: while loop

``while`` loop
==============

The ``while`` loop evaluates the test expression before every loop, so it can execute zero times if the condition is initially false.  The conditional expression requires parenthesis like the if:

.. code-block:: c

    while (<expression>) {
        <statement>
    }

Although the curly braces are not technically required if there is only one statement in the body of the ``while`` loop, you should *always* use the curly braces.  Again, see [#f1]_ for why.

.. index:: do-while loop

``do-while`` loop
=================

Like a ``while``, but with the test condition at the bottom of the loop. The loop body will always execute at least once. The ``do-while`` tends to be an unpopular area of the language, most everyone tries to use the straight ``while`` if possible:

.. code-block:: c

    do {
        <statement>
    } while (<expression>);

.. index:: for loop

``for`` loop
============

.. fixme: C99 declaration of variable; show what error shows up if you don't use -std=c99 and declare in the for loop.  for is *not* the most general looping statement.  ridiculous, dude.


The ``for`` loop in C contains three components that are often used in looping constructs, making it a fairly convenient statement to use.  The three parts are an initializer, a continuation condition, and an action, as in:

.. code-block:: c

    for (<initializer>; <continuation>; <action>) {
        <statement>
    }

The initializer is executed once before the body of the loop is entered.  The loop continues to run as long as the continuation condition remains true.  After every execution of the loop, the action is executed. The following example executes 10 times by counting 0..9. Many loops look very much like the following:

.. literalinclude:: code/forloop.c
   :language: c
   :linenos:

C programs often have series of the form 0..(some_number-1). It's idiomatic in C for loops like the example above to start at 0 and use ``<`` in the test so the series runs up to but not equal to the upper bound. In other languages you might start at 1 and use ``<=`` in the test.

Each of the three parts of the for loop can be made up of multiple expressions separated by commas. Expressions separated by commas are executed in order, left to right, and represent the value of the last expression. 

Note that in the C standard prior to C99, it was illegal to declare a variable in the initializer part of a ``for`` loop.  In C99, however, it is perfectly legal.  If you compile the above code using :command:`gcc` without the ``-std=c99`` flag, you will get the following error::

    forloop.c: In function ‘main’:
    forloop.c:4:5: error: ‘for’ loop initial declarations are only allowed in C99 mode
    forloop.c:4:5: note: use option -std=c99 or -std=gnu99 to compile your code

Once the ``-std=c99`` flag is added, the code compiles correctly, as expected.


``break``
---------

The ``break`` statement causes to execution to exit the current loop or switch statement.  Stylistically speaking, ``break`` has the potential to be a bit vulgar.  It is preferable to use a straight ``while`` with a single conditional expression at the top if possible, but sometimes you are forced to use a ``break`` because the test can occur only somewhere in the midst of the statements in the loop body.  To keep the code readable, be sure to make the ``break`` obvious --- forgetting to account for the action of a ``break`` is a traditional source of bugs in loop behavior:

.. code-block:: c

    while (<expression>) {
        <statement>
        statement>
        if (<condition which can only be evaluated here>) {
            break;
        }
        <statement>
        <statement>
    }
    // control jumps down here on the break

The ``break`` does not work with ``if``; it only works in loops and switches. Thinking that a ``break`` refers to an ``if`` when it really refers to the enclosing ``while`` has created some high-quality bugs. When using a ``break``, it is nice to write the enclosing loop to iterate in the most straightforward, obvious, normal way, and then use the ``break`` to explicitly catch the exceptional, weird cases.

``continue``
------------

The ``continue`` statement causes control to jump to the bottom of the loop, effectively skipping over any code below the ``continue``. As with ``break``, this has a reputation as being vulgar, so use it sparingly. You can almost always get the effect more clearly using an ``if`` inside your loop:

.. code-block:: c

    while (<expression>) {
        <statement>
        if (<condition>) {
            continue;
        }
        <statement>
        <statement>
        // control jumps here on the continue
    }


.. rubric:: Footnotes

.. _horrific: http://thedailywtf.com/articles/One-Bad-Ternary-Operator-Deserves-Another

.. [#f1] See https://gotofail.com for a variety of information about the bug, and https://www.imperialviolet.org/2014/02/22/applebug.html for detailed analysis of the source code that caused the problem.
