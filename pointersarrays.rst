.. _pointers:

Pointers and more arrays
************************

The C programming language has a somewhat split personality.  On the one hand, it is a *high-level programming language* [#f1]_ in that it provides basic control and data abstractions so that a programmer does not have to work in low-level assembly code.  On the other hand, it is often considered a fairly *low-level language* [#f2]_ due to the fact that it provides only simple data types that directly reflect standard hardware capabilities (e.g., integers, floating point numbers, and simple character strings) and that it allows programmers to directly manipulate memory and memory addresses.  In this chapter, we focus on C's capabilities for enabling a programmer to directly manipulate memory and memory addresses.  

.. wanted to say something about C not providing guardrails like array bounds checking, etc.  C assumes a programmer knows what he or she is doing.

.. index:: process, address space, stack, heap

.. topic:: Process address spaces

   As a bit of context for discussing memory addresses and pointers, consider the following depiction of the *address space* of a running program (a "process").

   .. image:: figures/addressspace.*
      :align: center

   A process in memory typically has (at least) 4 different portions of memory ("segments") dedicated to different purposes.  For example, the binary machine code for the program must reside in memory, and a segment is dedicated to storage for global ("static") variables.  These portions of memory typically remain *constant* in size, e.g., the amount of memory used for program code does not need to change.  There are two segments, however, that are designed to grow and shrink over the lifetime of a process: the *stack* and the *heap*.  The stack holds data for each function in progress, including space for local variables, space for parameters, and space for return values.  For each function call and return, the size of the stack will grow or shrink, respectively.  The heap contains storage for dynamic data structures, e.g., data objects in linked lists, which are managed by the programmer.


Pointers
========

A *pointer* is a variable that holds a memory address.  The C language allows a programmer to manipulate data *indirectly* through a pointer variable, as well as manipulate the memory address itself stored in the pointer variable.  

.. index:: 
   double: pointers; pointer declaration
   double: pointers; pointer initialization
   double: pointers; NULL
   single: \* (pointer declaration)

Declaration syntax
------------------

Declaring a new pointer variable is accomplished by using the syntax ``<data-type> *<variable-name>;``  The asterisk symbol between the data type and variable name indicates that the variable holds a memory address that refers to a location holding the given data type.  For example, the following declaration creates a variable ``p`` that contains a memory address referring to a location holding an ``int`` type.

.. code-block:: c

    int *p; // p points to ???

Recall that C does not do any automatic initialization of variables.  Thus, the variable ``p`` will hold an *undefined* memory address after the above declaration.  To initialize the pointer so that it points to "nothing", you use ``NULL`` in C, which is defined as the special address 0.

.. code-block:: c

    int *p = NULL; // p points to nothing

The figure below depicts the state of ``p`` after this assignment


.. figure:: figures/nullptr.*
   :align: center

   ``p`` is ``NULL``, or holds the special address 0.
   
.. index:: 
   double: &; address-of operator
   double: pointers; address-of operator

``&``: Address-of operator
--------------------------

It is often the case that we need to obtain the address of a variable in memory in order to indirectly manipulate its contents through a pointer variable.  The address-of operator --- ``&`` --- is used for this purpose.  For example, the following two lines of code create an integer variable ``i`` initialized with contents 42, and a *pointer to int* variable ``p`` which is initialized with *the address of i*.  Notice that the ``&`` goes before the variable for which we want to obtain the address.

.. code-block:: c

    int i = 42;  // i directly holds the integer 42 (on the stack)
    int *p = &i; // p holds the address of i 

Below is an example depiction of the contents of memory assuming that the variable ``i`` is stored at (hex) address 0x1004, and ``p`` is stored in the next four bytes.  (Note that this figure assumes 32 bit addressing, since ``p`` --- which holds a memory address --- occupies exactly 4 bytes, or 32 bits, in this diagram.)


.. figure:: figures/addrof.*
   :align: center

   ``i`` directly holds the value 42, and ``p`` holds the address
   of ``i``.


.. index::
   single: \* (pointer dereference)
   double: pointers; dereference operator (\*)

Dereferencing, or "following" a pointer
---------------------------------------

Now that ``p`` "points to" the contents of ``i``, we could indirectly modify ``i``\'s contents through ``p``.  Essentially what we want to do is to "follow" (or "dereference") the pointer ``p`` to get to the integer that its address refers to (i.e., ``i``), and modify those contents.  

The asterisk (``*``) is used as the dereference operator.  The basic syntax is: ``* <pointer-variable>``, which means "obtain the contents of the memory address to which ``<pointer-variable>`` refers.  (Notice that the asterisk goes to the left of the pointer variable that we wish to dereference.)  We could use this syntax to increment ``i`` by one, indirectly through ``p``, as follows:

.. code-block:: c

    int i = 42;  // i directly holds the integer 42
    int *p = &i; // p holds address of i
    *p = *p + 1; // dereference p (follow pointer), add one to int to which
                 // p points, then assign back to int to which p points
    printf("%d\n", i); // -> will print 43

.. index:: 
   double: pointers; swap function


A canonical example for why pointers can be useful is for implementing a function that successfully swaps two values.  Here is the code to do it:

 
.. code-block:: c

    #include <stdio.h>

    void swap(int *a, int *b) {
        int tmp = *a;
        *a = *b;
        *b = tmp;
    }

    int main() {
        int x = 42, y = 13;
        printf("x is %d, y is %d\n", x, y);
        swap(&x, &y);
        printf("x is %d, y is %d\n", x, y);
        return 0;
    }

.. figure:: figures/swap.*
   :align: center
   :scale: 75%

   Inside the ``swap`` function, ``a`` holds the address of ``x`` back on
   ``main``\'s stack and ``b`` holds the address of ``y`` also on ``main``\'s 
   stack.  With pass-by-value semantics, ``a`` gets a *copy* of the address
   of ``x`` (likewise, ``b`` gets a *copy* of the address of ``y``).

The key to this code is that we declare the ``swap`` function to take two *pointers to ints* as parameters (rather than the two integers themselves).  In ``main``, we pass *copies of the addresses of x and y*, as shown in the figure above.  Inside ``swap``, therefore, ``a`` holds the memory address of ``x`` (which is back on ``main``\'s stack) and ``b`` holds the memory address of ``y`` (which is also back on ``main``\'s stack).  Through the pointers, we indirectly modify the contents of ``x`` and ``y``.  


.. sidebar:: Pitfall: uninitialized pointers

    When using pointers, there are two entities to keep track of: the pointer itself, and the memory address to which the pointer points, sometimes called the "pointee".  There are three things that must be done for a pointer/pointee relationship to work correctly:

     1. The pointer must be declared and allocated
     2. The pointee must be declared and allocated
     3. The pointer (1) must be initialized so that it points to the pointee (2)

    A common error is to do (1), but not (2) or (3).  For example::

        int *p;  // p points to ???
        *p = 13; // follow p to some unknown memory location and put 13 there

    Since C does not do any initialization for the programmer, just declaring a pointer (i.e., step 1) isn't enough for *using* a pointer.  In the above code, ``p`` points to some undefined memory location and the act of writing the integer 13 to that location *may* result in a crash.  The crash will likely appear to be *random*, but is entirely due to the fact that ``p`` was never properly initialized.

    To fix this error, ``p`` must point to some actual ``int`` in memory, for example::

        int q = 99;
        int *p = &q; // p now is initialized to hold the address of q
        *p = 13;     

 
Pointers to ``struct``\'s
-------------------------


.. code-block:: c

    void flip_fraction(struct fraction *f) {
        int tmp = (*f).denominator;
        (*f).denominator = (*f).numerator;
        (*f).numerator = tmp;
    }

    struct fraction frac = { 1,2};
    flip_fraction(&frac);


.. code-block:: c

    void flip_fraction(struct fraction *f) {
      int tmp = f->denominator;
      f->denominator = f->numerator;
      f->numerator = tmp;
    }


Example operating system call with pointers: ``gettimeofday``
-------------------------------------------------------------

A standard function for getting the current system time in seconds and microseconds is to use the ``gettimeofday`` call.  This function is declared in the header file ``<sys/time.h>`` and has the following signature::

    int gettimeofday(struct timeval *, struct timezone *);

where the first argument is a pointer to a ``struct timeval``, and the second argument is a pointer to a ``struct timezone``.  A ``struct timeval`` has two fields: ``tv_sec`` and ``tv_usec``, which contain the seconds and microseconds after the UNIX epoch (Midnight, January 1, 1970), respectively.  This function *fills in* these fields in the ``struct timeval`` passed to the function (i.e., it modifies the two fields of this struct).  ``NULL`` is normally passed for the timezone argument. 

If a programmer wants to get the current system time, a standard way to use this function is to declare a ``struct timeval`` on the stack of the currently executing function (i.e., as a local variable), then pass the address of this struct to ``gettimeofday``, as follows:

.. code-block:: c

    struct timeval tv;
    gettimeofday(&tv, NULL);
    // tv.tv_sec and tv.tv_usec now have meaningful values filled in by the gettimeofday function

This pattern of passing the address of a stack-allocated struct is fairly common when making various system calls.


.. sidebar:: The ``const`` qualifier

  The keyword ``const`` can be added to the left of a variable or parameter type to declare that the code using the variable will not change the variable.  As a practical matter, use of ``const`` is very sporadic in the C programming community. It does have one very handy use, which is to clarify the role of a parameter in a function prototype.  For example, in:: 

      void foo(const struct fraction* fract);

  In the ``foo()`` function prototype, the ``const`` declares that ``foo()`` does not intend to change the struct fraction pointee which is passed to it.  Since the fraction is passed by pointer, we could not know otherwise if ``foo()`` intended to change our memory or not.  Using ``const``, ``foo()`` makes its intentions clear. Declaring this extra bit of information helps to clarify the role of the function to its implementor and caller.


Advanced C Arrays
=================


C strings revisited
-------------------

Recall that a string in C is just a series of ``char``\s which has a null character (``\0``) to signify the end of the string.  We have used arrays of ``char`` to create a contiguous block of ``char``\s 


Thus far, we have created character arrays to hold C strings


Because of the way C handles the types of arrays, the type of the variable localString above is essentially char*. C programs very often manipulate strings using variables of type char* which point to arrays of characters. Manipulating the actual chars in a string requires code which manipulates the underlying array, or the use of library functions such as strcpy() which manipulate the array for you. See Section 6 for more detail on pointers and arrays.


 
Array-pointer duality
---------------------
 
 array holds a mem addr; show same ptr syntac to get at elements of array; jntroduce pointer arithmetic

show p* refering to array, with ptr and arry syntax

The next subsection (on dereferencing) describes how that is done.  Before we get to that, here is one more address-of example.  The following code creates an array of 10 integers (which should occupy exactly 40 bytes if the size of an ``int`` is 4 bytes).  

.. code-block:: c
    
    int fibarr[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
    int *p = &fibarr[0];  // p holds address of the first element of fibarr
    int *q = fibarr;      // ditto for q ---
                          // fibarr by itself refers to the starting address
                          // of the array.





In C, an array is formed by laying out all the elements contiguously in memory. The square bracket syntax can be used to refer to the elements in the array. The array as a whole is referred to by the address of the first element which is also known as the "base address" of the whole array.

::

    {
        int array[6];
        int sum = 0;
        sum += array[0] + array[1];
        array
        // refer to elements using []
    }

The array name acts like a pointer to the first element; in this case an ``(int*)``.

..
         ￼
.. todo:: 

    Make a picture 


The programmer can refer to elements in the array with the simple [ ] syntax such as array[1]. This scheme works by combining the base address of the whole array with the index to compute the base address of the desired element in the array. It just requires a little arithmetic. Each element takes up a fixed number of bytes which is known at compile-time. So the address of element n in the array using 0 based indexing will be at an offset of (n * element_size) bytes from the base address of the whole array.

::

    address of nth element = address_of_0th_element + (n * element_size_in_bytes)

The square bracket syntax [ ] deals with this address arithmetic for you, but it's useful to know what it's doing. The [ ] takes the integer index, multiplies by the element size, adds the resulting offset to the array base address, and finally dereferences the resulting pointer to get to the desired element.

::

    {
        int intArray[6];
        intArray[3] = 13;
    }

.. todo:: 

    Make a picture of the above


'+' Syntax
----------

In a closely related piece of syntax, a + between a pointer and an integer does the same offset computation, but leaves the result as a pointer. The square bracket syntax gives the nth element while the + syntax gives a pointer to the nth element.

So the expression (intArray + 3) is a pointer to the integer intArray[3]. (intArray + 3) is of type (int*) while intArray[3] is of type int. The two expressions only differ by whether the pointer is dereferenced or not. So the expression (intArray + 3) is exactly equivalent to the expression (&(intArray[3])). In fact those two probably compile to exactly the same code. They both represent a pointer to the element at index 3.

Any ``[]`` expression can be written with the + syntax instead. We just need to add in the pointer dereference. So ``intArray[3]`` is exactly equivalent to ``*(intArray + 3)``. For most purposes, it's easiest and most readable to use the [] syntax. Every once in a while the + is convenient if you needed a pointer to the element instead of the element itself.



Pointer Type Effects
--------------------

Both ``[ ]`` and ``+`` implicitly use the compile time type of the pointer to compute the element_size which affects the offset arithmetic. When looking at code, it's easy to assume that everything is in the units of bytes.

::

    int *p;
    p = p + 12;    // at run-time, what does this add to p? 12?

The above code does not add the number 12 to the address in p-- that would increment p by 12 bytes. The code above increments p by 12 ints. Each int probably takes 4 bytes, so at run time the code will effectively increment the address in p by 48. The compiler figures all this out based on the type of the pointer.

Using casts, the following code really does just add 12 to the address in the pointer p. It works by telling the compiler that the pointer points to char instead of int. The size of char is defined to be exactly 1 byte (or whatever the smallest addressable unit is on the computer). In other words, sizeof(char) is always 1. We then cast the resulting ``(char*)`` back to an ``(int*)``. The programmer is allowed to cast any pointer type to any other pointer type like this to change the code the compiler generates.

::
    p = (int*) ( ((char*)p) + 12);


Array and pointer duality
-------------------------

One effect of the C array scheme is that the compiler does not distinguish meaningfully between arrays and pointers-- they both just look like pointers. In the following example, the value of intArray is a pointer to the first element in the array so it's an (int*). The value of the variable intPtr is also (int*) and it is set to point to a single integer i. So what's the difference between intArray and intPtr? Not much as far as the compiler is concerned. They are both just (int*) pointers, and the compiler is perfectly happy to apply the [] or + syntax to either. It's the programmer's responsibility to ensure that the elements referred to by a [] or + operation really are there. Really its' just the same old rule that C doesn't do any bounds checking. C thinks of the single integer i as just a sort of degenerate array of size 1.

::

    {
        int intArray[6];
        int *intPtr;
        int i;
        intPtr = &i;
        intArray[3] = 13; // ok
        intPtr[0] = 12; // odd, but ok.  changes i.
        intPtr[3] = 13; // BAD! no integer reserved here.
    }

.. todo::

   memory diagram/picture of the above

Array Names Are Const
---------------------

One subtle distinction between an array and a pointer, is that the pointer which represents the base address of an array cannot be changed in the code. The array base address behaves like a const pointer. The constraint applies to the name of the array where it is declared in the code --- the variable ints in the example below.

::

    {
        int ints[100]
        int *p;
        int i;

        ints = NULL;      // NO, cannot change the base addr ptr
        ints = &i;  // NO
        ints = ints + 1;    // NO
        ints++; // NO

        p = ints; // OK, p is a regular pointer which can be changed
        // here it is getting a copy of the ints pointer

        p++; // OK, p can still be changed (and ints cannot)
        p = NULL; // OK
        p = &i; // OK
        foo(ints); // OK (possible foo definitions are below)
    }

Array parameters are passed as pointers. The following two definitions of foo look different, but to the compiler they mean exactly the same thing. It's preferable to use whichever syntax is more accurate for readability. If the pointer coming in really is the base address of a whole array, then use ``[ ]``.

::

    void foo(int arrayParam[]) {
        arrayParam = NULL;      // Silly but valid. Just changes the local pointer
    }

    void foo(int *arrayParam) {
        arrayParam = NULL;      // ditto
    }

Dynamic memory allocation on the heap
=====================================

C gives programmers the standard sort of facilities to allocate and deallocate dynamic heap memory. A word of warning: writing programs which manage their heap memory is notoriously difficult. This partly explains the great popularity of languages such as Java and Perl which handle heap management automatically. These languages take over a task which has proven to be extremely difficult for the programmer. As a result Perl and Java programs run a little more slowly, but they contain far fewer bugs.

C provides access to the heap features through library functions which any C code can call. The prototypes for these functions are in the file <stdlib.h>, so any code which wants to call these must #include that header file. The three functions of interest are...

``void* malloc(size_t size)``
    Request a contiguous block of memory of the given size in the heap. malloc() returns a pointer to the heap block or NULL if the request could not be satisfied. The type size_t is essentially an unsigned long which indicates how large a block the caller would like measured in bytes. Because the block pointer returned by malloc() is a void* (i.e. it makes no claim about the type of its pointee), a cast will probably be required when storing the void* pointer into a regular typed pointer.

``void free(void* block)``
    The mirror image of malloc() -- free takes a pointer to a heap block earlier allocated by malloc() and returns that block to the heap for re-use. After the free(), the client should not access any part of the block or assume that the block is valid memory. The block should not be freed a second time.


.. sidebar:: Pointing into the ``void``

    Notice that the ``malloc`` function returns a "pointer to void" (``void *``), and ``free`` takes a ``void *`` as a parameter. By convention in C, a pointer which does not point to any particular type is declared as ``void*``. Sometimes ``void*`` is used to force two bodies of code not to depend on each other, since ``void*`` translates roughly to "this points to something, but I'm not telling you (the client) the type of the pointee exactly because you do not really need to know."  That's exactly the case with ``malloc`` and ``free``:  the ``malloc`` function cannot possibly know what the caller wants the new memory block allocated on the heap to contain, and neither can the ``free`` function know what data type some memory block points to.

    Note that a ``void *`` cannot be dereferenced --- the compiler prevents this.  The pointer must be cast to a pointer to some concrete type in order to be dereferenced.  

    Also, interestingly, ``NULL`` is usually defined as ``(void*)0``.

.. sidebar:: The valgrind tool

   Valgrind is awesome and you should always use it.





All of a program's memory is deallocated automatically when the it exits, so a program only needs to use free() during execution if it is important for the program to recycle its memory while it runs -- typically because it uses a lot of memory or because it runs for a long time. The pointer passed to free() must be exactly the pointer which was originally returned by malloc() or realloc(), not just a pointer into somewhere within the heap block.

Dynamic Arrays
--------------

Since arrays are just contiguous areas of bytes, you can allocate your own arrays in the heap using malloc(). The following code allocates two arrays of 1000 ints-- one in the stack the usual "local" way, and one in the heap using malloc(). Other than the different allocations, the two are syntactically similar in use.

::

    {
        int a[1000];
        int *b;
        b = (int*) malloc( sizeof(int) * 1000);
        assert(b != NULL);      // check that the allocation succeeded
        a[123] = 13;      // Just use good ol' [] to access elements
        b[123] = 13;      // in both arrays.
        free(b); 
    }

Although both arrays can be accessed with ``[ ]``, the rules for their maintenance are very different.

Advantages of being in the heap
-------------------------------

 * Size (in this case 1000) can be defined at run time. Not so for an array like "a".

 * The array will exist until it is explicitly deallocated with a call to free().

 * You can change the size of the array at will at run time using realloc(). The following changes the size of the array to 2000. Realloc() takes care of copying over the old elements.

::
    b = realloc(b, sizeof(int) * 2000);
    assert(b != NULL);

Disadvantages of being in the heap
----------------------------------

 * You have to remember to allocate the array, and you have to get it right.

 * You have to remember to deallocate it exactly once when you are done with it, and you have to get that right.

 * The above two disadvantages have the same basic profile: if you get them wrong, your code still looks right. It compiles fine. It even runs for small cases, but for some input cases it just crashes unexpectedly because random memory is getting overwritten somewhere like the smiley face. This sort of "random memory smasher" bug can be a real ordeal to track down.


.. _dynamic-strings:

Dynamic Strings
---------------


The dynamic allocation of arrays works very well for allocating strings in the heap. The advantage of heap allocating a string is that the heap block can be just big enough to store the actual number of characters in the string. The common local variable technique such as char string[1000]; allocates way too much space most of the time, wasting the unused bytes, and yet fails if the string ever gets bigger than the variable's fixed size.

::

    #include <string.h>
    /*
      Takes a c string as input, and makes a copy of that string
      in the heap. The caller takes over ownership of the new string
      and is responsible for freeing it.
     */
    char* MakeStringInHeap(const char* source) {
        char* newString;
        newString = (char*) malloc(strlen(source) + 1); // +1 for the '\0'
        assert(newString != NULL);
        strcpy(newString, source);
        return(newString);
    }


Linked lists
------------

.. code-block:: c

    struct node {
        int data;
        struct node* next;
    };



.. [#f1] http://en.wikipedia.org/wiki/High-level_programming_language

.. [#f2] http://en.wikipedia.org/wiki/Low-level_programming_language
