Control Structures
******************

Curly Braces ``{}``
-------------------

C uses curly braces ({}) to group multiple statements together. The statements execute in order. Some languages let you declare variables on any line (C++). Other languages insist that variables are declared only at the beginning of functions (Pascal). C takes the middle road -- variables may be declared within the body of a function, but they must follow a '{'. More modern languages like Java and C++ allow you to declare variables on any line, which is handy.

If Statement
------------

Both an ``if`` and an ``if-else`` are available in C. The <expression> can be any valid expression. The parentheses around the expression are required, even if it is just a single variable::

    if (<expression>) <statement>    // simple form with no {}'s or else clause

   ￼if (<expression>) { // simple form with {}'s to group statements
        <statement>
        <statement>
    }

    if (<expression>) { // full then/else form
        <statement>
    } else {
        <statement>
    }
    
Conditional Expression -or- The Ternary Operator
------------------------------------------------

The conditional expression can be used as a shorthand for some if-else statements. The general syntax of the conditional operator is::

    <expression1> ? <expression2> : <expression3>

This is an expression, not a statement, so it represents a value. The operator works by evaluating expression1. If it is true (non-zero), it evaluates and returns expression2 . Otherwise, it evaluates and returns expression3.

The classic example of the ternary operator is to return the smaller of two variables. Every once in a while, the following form is just what you needed. Instead of::

    if (x < y) {
        min = x;
    } else {
        min = y; 
    }

you can write::

    min = (x < y) ? x : y;

Switch Statement
----------------

The switch statement is a sort of specialized form of if used to efficiently separate different blocks of code based on the value of an integer. The switch expression is evaluated, and then the flow of control jumps to the matching const-expression case. The case expressions are typically int or char constants. The switch statement is probably the single most syntactically awkward and error-prone features of the C language::

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

Each constant needs its own case keyword and a trailing colon (:). Once execution has jumped to a particular case, the program will keep running through all the cases from that point down -- this so called "fall through" operation is used in the above example so that expression-3 and expression-4 run the same statements. The explicit break statements are necessary to exit the switch. Omitting the break statements is a common error -- it compiles, but leads to inadvertent fall-through behavior.

Why does the switch statement fall-through behavior work the way it does? The best explanation I can think of is that originally C was developed for an audience of assembly language programmers. The assembly language programmers were used to the idea of a jump table with fall-through behavior, so that's the way C does it (it's also relatively easy to implement it this way.) Unfortunately, the audience for C is now quite different, and the fall-through behavior is widely regarded as a terrible part of the language.

While Loop
----------

The while loop evaluates the test expression before every loop, so it can execute zero times if the condition is initially false. It requires the parenthesis like the if::

    while (<expression>) {
        <statement>
    }

Do-While Loop
-------------

Like a while, but with the test condition at the bottom of the loop. The loop body will always execute at least once. The do-while is an unpopular area of the language, most everyone tries to use the straight while if at all possible::

    do {
        <statement>
    } while (<expression>);


For Loop
--------

The for loop in C is the most general looping construct. The loop header contains three parts: an initialization, a continuation condition, and an action::

    for (<initialization>; <continuation>; <action>) {
        <statement>
    }

The initialization is executed once before the body of the loop is entered. The loop continues to run as long as the continuation condition remains true (like a while). After every execution of the loop, the action is executed. The following example executes 10 times by counting 0..9. Many loops look very much like the following::

    for (i = 0; i < 10; i++) {
        <statement>
    }

C programs often have series of the form 0..(some_number-1). It's idiomatic in C for the above type loop to start at 0 and use < in the test so the series runs up to but not equal to the upper bound. In other languages you might start at 1 and use <= in the test.

Each of the three parts of the for loop can be made up of multiple expressions separated by commas. Expressions separated by commas are executed in order, left to right, and represent the value of the last expression. (See the string-reverse example below for a demonstration of a complex for loop.)

Break
-----

The ``break`` statement will move control outside a loop or switch statement. Stylistically speaking, break has the potential to be a bit vulgar. It's preferable to use a straight while with a single test at the top if possible. Sometimes you are forced to use a break because the test can occur only somewhere in the midst of the statements in the loop body. To keep the code readable, be sure to make the break obvious -- forgetting to account for the action of a break is a traditional source of bugs in loop behavior::

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

The break does not work with if. It only works in loops and switches. Thinking that a break refers to an if when it really refers to the enclosing while has created some high quality bugs. When using a break, it's nice to write the enclosing loop to iterate in the most straightforward, obvious, normal way, and then use the break to explicitly catch the exceptional, weird cases.

Continue
--------

The continue statement causes control to jump to the bottom of the loop, effectively skipping over any code below the continue. As with break, this has a reputation as being vulgar, so use it sparingly. You can almost always get the effect more clearly using an if inside your loop::

    while (<expression>) {
        <statement>
        if (<condition>) {
            continue;
        }
        <statement>
        <statement>
        // control jumps here on the continue
    }


