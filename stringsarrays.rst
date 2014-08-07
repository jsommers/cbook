Arrays and Strings
******************

Arrays
======

The simplest type of array in C is one which is declared and used in one place. There are more complex uses of arrays which I will address later along with pointers. The following declares an array called scores to hold 100 integers and sets the first and last elements. C arrays are always indexed from 0. So the first int in scores array is scores[0] and the last is scores[99].

::
    int scores[100];
    scores[0]  = 13;           // set first element
    scores[99] = 42;           // set last element

The name of the array refers to the whole array. (implementation) it works by representing a pointer to the start of the array.  

.. todo::

   Add a picture of an array.

.. 

 * There is space for each int element in the scores array — this element is referred to as scores[0].
 * These elements have random values because the code has not yet initialized them to anything.
 * Someone else’s memory off either end of the array — do not read or write this memory.

It is a very common error to try to refer to non-existent scores[100] element. C does not do any run time or compile time bounds checking in arrays. At run time the code will just access or mangle whatever memory it happens to hit and crash or misbehave in some unpredictable way thereafter. "Professional programmer's language." The convention of numbering things 0..(number of things - 1) pervades the language. To best integrate with C and other C programmers, you should use that sort of numbering in your own data structures as well.

C Strings
=========

C has minimal support of character strings. For the most part, strings operate as ordinary arrays of characters. Their maintenance is up to the programmer using the standard facilities available for arrays and pointers. C does include a standard library of functions which perform common string operations, but the programmer is responsible for the managing the string memory and calling the right functions. Unfortunately computations involving strings are very common, so becoming a good C programmer often requires becoming adept at writing code which manages strings which means managing pointers and arrays.

A C string is just an array of char with the one additional convention that a "null" character ('\0') is stored after the last real character in the array to mark the end of the string. The compiler represents string constants in the source code such as "binky" as arrays which follow this convention. The string library functions (see the appendix for a partial list) operate on strings stored in this way. The most useful library function is strcpy(char dest[], const char source[]); which copies the bytes of one string over to another. The order of the arguments to strcpy() mimics the arguments in of '=' -- the right is assigned to the left. Another useful string function is strlen(const char string[]); which returns the number of characters in C string not counting the trailing '\0'.

Note that the regular assignment operator (=) does not do string copying which is why strcpy() is necessary. See Section 6, Advanced Pointers and Arrays, for more detail on how arrays and pointers work.

The following code allocates a 10 char array and uses strcpy() to copy the bytes of the string constant "binky" into that local array.

::

    {
        char localString[10];
        strcpy(localString, "binky");
    }

.. todo::

   Make a memory drawing of the above

The memory drawing shows the local variable localString with the string "binky" copied into it. The letters take up the first 5 characters and the '\0' char marks the end of the string after the 'y'. The x's represent characters which have not been set to any particular value.

If the code instead tried to store the string "I enjoy languages which have good string support" into localString, the code would just crash at run time since the 10 character array can contain at most a 9 character string. The large string will be written passed the right hand side of localString, overwriting whatever was stored there.

String Code Example
-------------------

Here's a moderately complex for loop which reverses a string stored in a local array. It demonstrates calling the standard library functions strcpy() and strlen() and demonstrates that a string really is just an array of characters with a '\0' to mark the effective end of the string. Test your C knowledge of arrays and for loops by making a drawing of the

::

    {
        char string[1000];   // string is a local 1000 char array
        int len;
        strcpy(string, "binky");
        len = strlen(string);
        /*
            Reverse the chars in the string:
            i starts at the beginning and goes up
            j starts at the end and goes down
            i/j exchange their chars as they go until they meet
        */
        int i, j;
        char temp;
        for (i = 0, j = len - 1; i < j; i++, j--) {
            temp = string[i];
            string[i] = string[j];
            string[j] = temp;
        }
        // at this point the local string should be "yknib"
   }

"Large Enough" Strings
----------------------

The convention with C strings is that the owner of the string is responsible for allocating array space which is "large enough" to store whatever the string will need to store. Most routines do not check that size of the string memory they operate on, they just assume its big enough and blast away. Many, many programs contain declarations like the following ::

    {
        char localString[1000];
        // ...
    }

The program works fine so long as the strings stored are 999 characters or shorter. Someday when the program needs to store a string which is 1000 characters or longer, then it crashes. Such array-not-quite-big-enough problems are a common source of bugs, and are also the source of so called "buffer overflow" security problems. This scheme has the additional disadvantage that most of the time when the array is storing short strings, 95% of the memory reserved is actually being wasted. A better solution allocates the string dynamically in the heap, so it has just the right size.

To avoid buffer overflow attacks, production code should check the size of the data first, to make sure it fits in the destination string. See the strlcpy() function in Appendix A.

.. rubric:: Exercises

.. todo:: add exercises for basic types
