Control Structures
******************

In this chapter, we encounter the set of "control" structures in C, such as conditional statements and looping constructs.  As a preview, the control structures in C are nearly identical to those found in Java (since Java syntax is heavily based on C), and bear strong resemblance to control structures found in other programming languages.

.. index:: if statement

``if`` Statement
================

Both an ``if`` and an ``if-else`` are available in C.  The ``<expression>`` can be any valid C expression.  The parentheses around the expression are required, even if it is just a single variable:

.. code-block:: c

    if (<expression>) <statement>    // simple form with no {}'s or else clause

You should **always** use curly braces around the statement block associated with an ``if`` statement.  Although C allows a programmer *not* to include the curly braces in the case of ``if`` statements and other control structures when there is only a single statement in the statement block, you should **always use the curly braces**.  Why, you ask, should I type those additional characters?  Just ask the unfortunate engineering group at Apple that introduced the "goto fail" bug into their SSL (secure sockets layer) library: a bug that affected the macOS and iOS operating systems quite severely [#f1]_.  The upshot of this security failure is that it could have been prevented with the rigorous use of curly braces for all statement blocks.

So, for the simple form of the ``if`` statement shown above, you should *really* write:

.. code-block:: c

    if (<expression>) { <statement> }   // simple form with no {}'s or else clause
                                        // note the curly braces around the statement
                                        // block!

..   For ``if`` statements and loop constructs that only contain a single line of code in the body, 
..   Curly braces are only required in the case of multiple statements in the body of one of these constructs.  Using the curly braces, however, is very strongly recommended.  
   


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

This is an *expression*, not a *statement*, so it represents a value. The operator works by evaluating ``expression1``. If it is true (non-zero), it evaluates and returns ``expression2``. Otherwise, it evaluates and returns ``expression3``.

The classic example of the ternary operator is to return the smaller of two variables. Instead of writing:

.. code-block:: c

    if (x < y) {
        min = x;
    } else {
        min = y; 
    }

you can write:

.. code-block:: c

    min = (x < y) ? x : y;

The ternary operator is viewed by some programmers as "excessively tricky" since expressions with such operators can be hard to read.  Use your best judgment, and don't do something this [Horrific]_ example.

.. index:: switch statement

``switch`` statement
====================

The switch statement is a sort of specialized form of ``if`` with the goal of efficiently separating different blocks of code based on the value of an integer.  The ``switch`` expression is evaluated, and then the flow of control jumps to the matching const-expression ``case``. The ``case`` expressions are typically ``int`` or ``char`` constants (unfortunately, you cannot use strings as ``case`` expressions).  The ``switch`` statement is probably the single most syntactically awkward and error-prone feature of the C language:

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

Each constant needs its own ``case`` keyword and a trailing colon (:).  Once execution has jumped to a particular case, the program will keep running through all the cases from that point down --- this so called *fall through* operation is used in the above example so that ``expression-3`` and ``expression-4`` run the same statements. The explicit ``break`` statements are necessary to exit the ``switch``. Omitting the ``break`` statements is a common error --- it compiles, but leads to inadvertent, unexpected, and likely erroneous fall-through behavior.

Why does the ``switch`` statement fall-through behavior work the way it does? The best explanation might be that C was originally developed for an audience of assembly language programmers. The assembly language programmers were used to the idea of a "jump table" with fall-through behavior, so that's the way C does it (it's also relatively easy to implement it this way). Unfortunately, the audience for C is now quite different, and the fall-through behavior is widely regarded as an unfortunate part of the language.

.. index:: while loop

``while`` loop
==============

The ``while`` loop evaluates the test expression before every loop, so it can execute zero times if the condition is initially false.  The conditional expression requires parentheses like the if:

.. code-block:: c

    while (<expression>) {
        <statement>
    }

Although the curly braces are not technically required if there is only one statement in the body of the ``while`` loop, you should *always* use the curly braces.  Again, see [#f1]_ for why.

.. index:: do-while loop

``do-while`` loop
=================

Like a ``while`` loop, but with the test condition at the *bottom* of the loop. The loop body will always execute at least once. The ``do-while`` tends to be an unpopular area of the language.  Although many users of C use the straight ``while`` if possible, a ``do-while`` loop can be very useful in some situations:

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

The ``break`` statement causes execution to exit the current loop or switch statement.  Stylistically speaking, ``break`` has the potential to be a bit vulgar.  It is preferable to use a straight ``while`` with a single conditional expression at the top if possible, but sometimes you are forced to use a ``break`` because the test can occur only somewhere in the midst of the statements in the loop body.  To keep the code readable, be sure to make the ``break`` obvious --- forgetting to account for the action of a ``break`` is a traditional source of bugs in loop behavior:

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

Statement labels and ``goto``
-----------------------------

Continuing the theme of statements that have a tendency of being a bit vulgar, we come to the king of vulgarity, the infamous ``goto`` statement [Goto]_.  The structure of a ``goto`` statement in C is to *unconditionally* jump to a statement label, and continue execution from there.  The basic structure is:

.. code-block:: c

   label:
     <statement>
     ..
     <statement>
     goto label;

The ``goto`` statement is not uncommon to encounter in operating systems code when there is a legitimate need to handle complex errors that can happen.  A pattern that you might see is something like:

.. code-block:: c

    int complex_function(void) {
        if (initialize_1() != SUCCESS) { goto out1; }
        if (initialize_2() != SUCCESS) { goto out2; }
        if (initialize_3() != SUCCESS) { goto out3; }

        /* other statements */

        return SUCCESS;
      
      out3:
        deinitialize_3();
      out2:
        deinitialize_2();
      out1:
        deinitialize_1();
      return ERROR;
    }

Notice the structure above: there are multiple steps being performed to carry out some initialization for an operation [#f2]_.  If one of those initialization operations fails, code execution transfers to a statement to handle *deinitialization*, and those de-init operations happen in *reverse order of initialization*.  It is possible to rewrite the above code to use ``if``/``else`` structures, but the structure becomes much more complex (see an exercise below).  Although ``goto`` has the reputation of leading to "spaghetti code", judicious use of this statement in situations like the above makes for cleaner and clearer code.  


.. rubric:: Exercises

1. Rewrite the ``goto`` example code above (the last code example, above) to use ``if``/``else`` instead.  Which code do you think exhibits a more clear structure?  

2. Consider the following program snippet:

   .. code-block:: c

     char buffer[64];
     printf("Enter an integer: ");
     fgets(buffer, 64, stdin);
     int val = atoi(buffer); // convert the string to an integer
     if (val % 2 == 1)
       val *= 2;
       val += 1
     printf("%d\n", val);

  What is printed if the number 3 is entered?

3.  Say that you want to write a program that repeatedly asks for a snowfall amount, and that you want to keep asking for another value until the sum of all values exceeds a certain value.  What control structure would work best to facilitate entry of the snowfall values, and why?

4.  Say you want to write a program that computes the average of quiz scores.  You have a big stack of quizzes, so you do not know the number of quizzes up front.  What control structure would work best to facilitate entry of the scores, and why?  

5.  Say that you want to simulate rolling a die (singular of dice) a fixed number of times and to compute and print the average value for the die rolls.  What control structure would work best for this problem, and why?


.. rubric:: References

.. [Horrific] http://thedailywtf.com/articles/One-Bad-Ternary-Operator-Deserves-Another

.. [Goto] E. W. Dijkstra.  Letters to the editor: go to statement considered harmful.  Communications of the ACM, Volume 11, Issue 3, March 1968.  https://dl.acm.org/citation.cfm?id=362947

.. rubric:: Footnotes

.. [#f1] See https://gotofail.com for a variety of information about the bug, and https://www.imperialviolet.org/2014/02/22/applebug.html for detailed analysis of the source code that caused the problem.

.. [#f2] This example was adapted from https://blog.regehr.org/archives/894, where you can find additional discussion on tasteful use of ``goto`` in systems code.
