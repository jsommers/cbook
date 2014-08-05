Structs
*******

Advanced Data Types
*******************

Structs
-------

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

Arrays
------

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

Multidimensional Arrays
-----------------------

The following declares a two-dimensional 10 by 10 array of integers and sets the first and last elements to be 13::

    int board [10][10];
    board[0][0] = 13;
    board[9][9] = 13;

The implementation of the array stores all the elements in a single contiguous block of memory. The other possible implementation would be a combination of several distinct one dimensional arrays -- that's not how C does it. In memory, the array is arranged with the elements of the rightmost index next to each other. In other words, board[1][8] comes right before board[1][9] in memory.

(highly optional efficiency point) It's typically efficient to access memory which is near other recently accessed memory. This means that the most efficient way to read through a chunk of the array is to vary the rightmost index the most frequently since that will access elements that are near each other in memory.

Array of Structs
----------------

The following declares an array named "numbers" which holds 1000 struct fraction's.

::

    struct fraction numbers[1000];
    numbers[0].numerator = 22;       // set the 0th struct fraction 
    numbers[0].denominator = 7;

Here's a general trick for unraveling C variable declarations: look at the right hand side and imagine that it is an expression. The type of that expression is the left hand side. For the above declarations, an expression which looks like the right hand side (numbers[1000], or really anything of the form numbers[...]) will be the type on the left hand side (struct fraction).

Pointers
--------

A pointer is a value which represents a reference to another value sometimes known as the pointer's "pointee". Hopefully you have learned about pointers somewhere else, since the preceding sentence is probably inadequate explanation. This discussion will concentrate on the syntax of pointers in C --- for a much more complete discussion of pointers and their use see http://cslibrary.stanford.edu/102/, Pointers and Memory.

Syntax
------

Syntactically C uses the asterisk or "star" (*) to indicate a pointer. C defines pointer types based on the type pointee. A char* is type of pointer which refers to a single char. a struct fraction* is type of pointer which refers to a struct fraction.

::

    int* intPtr;   // declare an integer pointer variable intPtr
    char* charPtr; // declares a character pointer --
                   // a very common type of pointer
    // Declare two struct fraction pointers
    // (when declaring multiple variables on one line, the *
    //  should go on the right with the variable)
    struct fraction *f1, *f2;

The Floating "*"
----------------

In the syntax, the star is allowed to be anywhere between the base type and the variable name. Programmer's have their own conventions-- I generally stick the * on the left with the type. So the above declaration of intPtr could be written equivalently ::

    int  *intPtr;        // these are all the same
    int * intPtr;
    int*  intPtr;

Pointer Dereferencing
---------------------

We'll see shortly how a pointer is set to point to something -- for now just assume the pointer points to memory of the appropriate type. In an expression, the unary * to the left of a pointer dereferences it to retrieve the value it points to. The following drawing shows the types involved with a single pointer pointing to a struct fraction.

::
    struct fraction* f1;

 ================ ==================
 Expression       Type
 ================ ==================
  f1              struct fraction*
  * f1            struct fraction
 (* f1).numerator  int
 ================ ==================


There's an alternate, more readable syntax available for dereferencing a pointer to a struct. A "->" at the right of the pointer can access any of the fields in the struct. So the reference to the numerator field could be written f1->numerator.

Here are some more complex declarations ::

    struct fraction** fp;      // a pointer to a pointer to a struct fraction
    struct fraction fract_array[20];       // an array of 20 struct fractions
    struct fraction* fract_ptr_array[20];  // an array of 20 pointers to
                                           // struct fractions

One nice thing about the C type syntax is that it avoids the circular definition problems which come up when a pointer structure needs to refer to itself. The following definition defines a node in a linked list. Note that no preparatory declaration of the node pointer type is necessary.

::

    struct node {
        int data;
        struct node* next;
    };

The & Operator
--------------

The & operator is one of the ways that pointers are set to point to things. The & operator computes a pointer to the argument to its right. The argument can be any variable which takes up space in the stack or heap (known as an "LValue" technically). So ``&i`` and ``&(f1->numerator)`` are ok, but ``&6`` is not. Use & when you have some memory, and you want a pointer to that memory.

::

    void foo() {
        int* p;  // p is a pointer to an integer
        int i;   // i is an integer
        p = &i;  // Set p to point to i
        *p = 13; // Change what p points to -- in this case i -- to 13
        // At this point i is 13. So is *p. In fact *p is i. 
    }

When using a pointer to an object created with &, it is important to only use the pointer so long as the object exists. A local variable exists only as long as the function where it is declared is still executing (we'll see functions shortly). In the above example, i exists only as long as foo() is executing. Therefore any pointers which were initialized with &i are valid only as long as foo() is executing. This "lifetime" constraint of local memory is standard in many languages, and is something you need to take into account when using the & operator.

NULL
----

A pointer can be assigned the value 0 to explicitly represent that it does not currently have a pointee. Having a standard representation for "no current pointee" turns out to be very handy when using pointers. The constant NULL is defined to be 0 and is typically used when setting a pointer to NULL. Since it is just 0, a NULL pointer will behave like a boolean false when used in a boolean context. Dereferencing a NULL pointer is an error which, if you are lucky, the computer will detect at runtime -- whether the computer detects this depends on the operating system.

Pitfall -- Uninitialized Pointers
---------------------------------

When using pointers, there are two entities to keep track of. The pointer and the memory it is pointing to, sometimes called the "pointee". There are three things which must be done for a pointer/pointee relationship to work:

 1. The pointer must be declared and allocated
 2. The pointee must be declared and allocated
 3. The pointer (1) must be initialized so that it points to the pointee (2)

The most common pointer related error of all time is the following: Declare and allocate the pointer (step 1). Forget step 2 and/or 3. Start using the pointer as if it has been setup to point to something. Code with this error frequently compiles fine, but the runtime results are disastrous. Unfortunately the pointer does not point anywhere good unless (2) and (3) are done, so the run time dereference operations on the pointer with * will misuse and trample memory leading to a random crash at some point.

::

    {
        int* p;
        *p = 13;
        // NO NO NO p does not point to an int yet
        // this just overwrites a random area in memory
    }

Of course your code won't be so trivial, but the bug has the same basic form: declare a pointer, but forget to set it up to point to a particular pointee.

Using Pointers
--------------

Declaring a pointer allocates space for the pointer itself, but it does not allocate space for the pointee. The pointer must be set to point to something before you can dereference it.

Here's some code which doesn't do anything useful, but which does demonstrate (1) (2) (3) for pointer use correctly ::

    int* p;     // (1) allocate the pointer
    int i;      // (2) allocate pointee
    struct fraction f1;  // (2) allocate pointee
    p = &i;     // (3) setup p to point to i
    *p = 42;    // ok to use p since it's setup
    p = &(f1.numerator);       // (3) setup p to point to a different int
    *p = 22;
    p = &(f1.denominator);     // (3)
    *p = 7;

So far we have just used the & operator to create pointers to simple variables such as i. Later, we'll see other ways of getting pointers with arrays and other techniques.

C Strings
---------

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

``char*``
---------

Because of the way C handles the types of arrays, the type of the variable localString above is essentially char*. C programs very often manipulate strings using variables of type char* which point to arrays of characters. Manipulating the actual chars in a string requires code which manipulates the underlying array, or the use of library functions such as strcpy() which manipulate the array for you. See Section 6 for more detail on pointers and arrays.

Type aliases
------------

A typedef statement introduces an alias, or shorthand, for a type. The syntax is::

    typedef <type> <name>;

The following defines Fraction type to be the type (struct fraction). C is case sensitive, so fraction is different from Fraction. It's convenient to use typedef to create types with upper case names and use the lower-case version of the same word as a variable.

::

    typedef struct fraction Fraction;
    Fraction fraction;   // Declare the variable "fraction" of type "Fraction"
                         //  which is really just a synonym for "struct fraction".

The following typedef defines the name Tree as a standard pointer to a binary tree node where each node contains some data and "smaller" and "larger" subtree pointers.

::

    typedef struct treenode* Tree;
    struct treenode {
        int data;
        Tree smaller, larger;   // equivalently, this line could say
                                // "struct treenode *smaller, *larger"
    };

