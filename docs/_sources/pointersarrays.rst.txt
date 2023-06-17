.. _pointers:

Pointers and more arrays
************************

The C programming language has a somewhat split personality.  On the one hand, it is a *high-level programming language* [#f1]_ in that it provides basic control and data abstractions so that a programmer does not have to work in low-level assembly code.  On the other hand, it is often considered a fairly *low-level language* [#f2]_ due to the fact that it provides only simple data types that directly reflect standard hardware capabilities (e.g., integers, floating point numbers, and simple character strings) and that it allows programmers to directly manipulate memory and memory addresses.  In this chapter, we focus on C's capabilities for enabling a programmer to directly manipulate memory and memory addresses.  

.. wanted to say something about C not providing guardrails like array bounds checking, etc.  C assumes a programmer knows what he or she is doing.

.. index:: process, address space, stack, heap

.. _addr-space:

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
    #include <stdlib.h>

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
        return EXIT_SUCCESS;
    }

.. figure:: figures/swap.*
   :align: center
   :scale: 75%

   Inside the ``swap`` function, ``a`` holds the address of ``x`` back on
   ``main``\'s stack and ``b`` holds the address of ``y`` also on ``main``\'s 
   stack.  With pass-by-value semantics, ``a`` gets a *copy* of the address
   of ``x`` (likewise, ``b`` gets a *copy* of the address of ``y``).

The key to this code is that we declare the ``swap`` function to take two *pointers to ints* as parameters (rather than the two integers themselves).  In ``main``, we pass *copies of the addresses of x and y*, as shown in the figure above.  Inside ``swap``, therefore, ``a`` holds the memory address of ``x`` (which is back on ``main``\'s stack) and ``b`` holds the memory address of ``y`` (which is also back on ``main``\'s stack).  Through the pointers, we indirectly modify the contents of ``x`` and ``y``.  


.. topic::  Uninitialized pointers

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

    It is worth noting that the :command:`scan-build` (see :ref:`an earlier discussion on scan-build <scan-build>`) static analysis tool can detect problems with uninitialized pointers.  

 
Pointers to ``struct``\'s
-------------------------

Pointer variables can refer to *any* data type, including ``struct`` variables.  For a ``struct``, the syntax for handling pointers can be a bit tricky.  To illustrate the trickiness, here is a function that exchanges (swaps) the numerator and denominator of a ``struct fraction`` (along with a bit of code to call the function):

.. code-block:: c

    void flip_fraction(struct fraction *f) {
        int tmp = (*f).denominator;
        (*f).denominator = (*f).numerator;
        (*f).numerator = tmp;
    }

    struct fraction frac = { 1,2};
    flip_fraction(&frac);

Why do we need to use parentheses around the ``(*f)``?  The reason is that the field selection operator (``.``) has higher operator precedence than the dereference operator.  Thus, a statement like ``*f.numerator`` simply does not work: it gets treated by the compiler as ``*(f.numerator)``.  If ``f`` is a pointer, ``f.numerator`` just doesn't make any sense.  As a result, it is necessary to first dereference the struct pointer, *then* access the numerator field.

Because of the awkwardness of requiring the parens for ``(*f).numerator`` to work right, C provides an operator to access a struct field through a pointer: the *arrow* operator (``->``):

.. code-block:: c

    void flip_fraction(struct fraction *f) {
      int tmp = f->denominator;
      f->denominator = f->numerator;
      f->numerator = tmp;
    }

The above function using the arrow operator has *exactly* the same effect as the more unwieldy version of the ``flip_fraction`` function above.


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


Advanced C Arrays and Pointer Arithmetic
========================================

Array/pointer duality
---------------------

Interestingly, C compilers do not meaningfully distinguish between arrays and pointers --- a C array variable actually just holds the memory address of the beginning of the array (also referred to as the *base address* of the array).  In the following code, we illustrate the *duality* of arrays and pointers by creating 10-element ``int`` array (``fibarray``) and a *pointer* to an ``int`` (``fibptr1``).  Notice that we directly assign the array variable to an ``int *``, which is perfectly legal in C and nicely illustrates the duality between pointers and arrays:

.. code-block:: c

    int fibarray[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
    int *fibptr1 = fibarray;

An alternative (and somewhat more explicit) syntax for obtaining the base address of the array is to use the address-of operator with the first element of the array.  The following declaration creates yet another pointer variable that refers to the beginning of the array:

.. code-block:: c
    
    int *fibptr2 = &fibarray[0]; // get the memory address of the first element of the array


.. sidebar:: Array names are constant pointers

  One subtle distinction between an array and a pointer is that the array name where it is declared in the code cannot be modified.  In other words, an array name cannot be made to refer to a *different* array or pointer in memory.  For example::

        int ints[100]
        int *p;
        int i;

        ints = NULL;      // NO: cannot change the base address pointer
        ints = &i;        // NO 
        ints = ints + 1;  // NO
        ints++;           // NO


Pointer arithmetic
------------------

The ``+`` operator can be used with pointers to access memory locations that reside at some *offset* from a pointer.   For example, say that we have the following variable: ``int *i``.  ``i+j`` (where j is an integer, *not* a pointer) is interpreted by the compiler as ``i + j * sizeof(int)``.  Thus, ``i+j`` yields the memory address of the jth ``int`` after the address ``i`` (where we start counting at 0, as you should expect).  

A somewhat longer example of adding a pointer and integer together is shown below:

.. code-block:: c

    int fibarray[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
    int *fibptr1 = fibarray;

    int a = *(fibptr + 0);  // add 0*sizeof(int) to fibptr address, then dereference (yields the value 1)
    int b = *(fibptr + 2);  // add 2*sizeof(int) to fibptr address, then dereference (yields the value 2)

Again, the syntax ``fibptr + 2`` is interpreted by the compiler as "get the address of the 2nd integer following the address fibptr".  

In fact, array indexing syntax works identically to pointer arithmetic.  As a result, square-brace indexing can be used with pointer variables.  Moreover, the nice thing about this syntax is that *dereferencing is automatic*. Continuing the code above:

.. code-block:: c

    int c = fibptr1[5]  // add 5*sizeof(int) to fibptr1 address, 
                        // then dereference (automatically!) (yields 8)


A totally bizarre implication of the way that C handles array indexing and pointers is that the array name and index value can be inverted!  

.. code-block:: c

    int array[] = { 1, 2, 3};
    printf("%d\n", array[1]);   // "normal" indexing
    printf("%d\n", 1[array]);   // bizarro inverted indexing, but legal and identical to previous line!
    printf("%d\n", *(array+1)); // pointer arithmetic syntax
    printf("%d\n", *(1+array)); // pointer arithmetic syntax, with operands reversed

The above code is purely an illustration --- don't write code with inverted indexing!  Although it is legal, it is a "feature" that makes the code harder to read since nobody writes indexes like that.  


Dynamic memory allocation
=========================

We started this chapter by outlining how memory is organized within a single running program, or process (see :ref:`Process address spaces <addr-space>`, above).  So far, we have just used local and parameter variables, which result in *stack-allocated* memory.  In this section, we discuss how to *dynamically* allocate and deallocate blocks of memory on the heap.  C requires that a program *manually* manage heap-allocated memory through explicit allocation and deallocation.  In contrast, a language like Java only requires that a programmer explicitly allocate memory, but the language runtime handles automatic deallocation through a process called *garbage collection*.  

``malloc`` and ``free``
-----------------------

The built-in functions ``malloc`` and ``free`` are used to manually allocate and deallocate blocks of heap memory.  These functions are declared in the header file ``<stdlib.h>`` (i.e., you must ``#include`` this file) and work as follows:

.. sidebar:: Pointing into the ``void``

    Notice that the ``malloc`` function returns a "pointer to void" (``void *``), and ``free`` takes a ``void *`` as a parameter. By convention in C, a pointer which does not point to any particular type is declared as ``void*``. Sometimes ``void*`` is used to force two bodies of code not to depend on each other, since ``void*`` translates roughly to "this points to something, but I'm not telling you (the client) the type of the pointee exactly because you do not really need to know."  That's exactly the case with ``malloc`` and ``free``:  the ``malloc`` function cannot possibly know what the caller wants the new memory block allocated on the heap to contain, and neither can the ``free`` function know what data type some memory block points to.

    Note that a ``void *`` cannot be dereferenced --- the compiler prevents this.  The pointer must be cast to a pointer to some concrete type in order to be dereferenced.  

    Also, interestingly, ``NULL`` is usually defined as ``(void*)0``.

``void* malloc(size_t size)``
    ``malloc`` takes one parameter: the number of bytes to allocate on the heap.  It returns a "generic pointer" (i.e., ``void *``) that refers to a newly allocated block of memory on the heap.  If there is not enough memory on the heap to satisfy the request, ``malloc`` returns ``NULL``.

``void free(void* block)``
    The mirror image of ``malloc``, ``free`` takes a pointer to a heap block previously returned by a call to ``malloc`` and returns it to the heap for re-use.  After calling ``free``, the caller should not access any part of the memory block that has been returned to the heap.

Note that all of a program's memory is deallocated automatically when it exits, so a program *technically* only needs to use ``free`` during execution if it is important for the program to recycle its memory while it runs --- typically because it uses a lot of memory or because it runs for a long time.  However, **it is always good practice to free what ever you malloc**.  You should not rely on the fact that a program does not run long or that you *think* it does not use a lot of memory.  

Here is some example code that uses ``malloc`` and ``free`` to allocate a block of ``struct fraction`` records (basically an array, but not declared as an array), fill each one in with user input, invert each one, then print them all out.  Notice that each of the functions ``get_fractions``, ``invert_fractions``, and ``print_fractions`` accesses each ``struct fraction`` in different ways: by index, and by pointer arithmetic.  Note specifically that the ``invert_fractions`` function modifies the ``fracblock`` pointer (by "incrementing it by 1, which makes the pointer advance to the next ``struct fraction``), but since that function just gets a *copy* of the pointer to the ``struct fraction`` this is totally ok.  

.. literalinclude:: code/fracheap.c
   :language: c
   :linenos:


Memory leaks and dangling pointers
----------------------------------

Note that in the above example code, we have exactly 1 call to ``malloc`` and exactly 1 matching call to ``free``.  If you do not have a matching ``free`` call for each malloc, your program has a *memory leak*.  Memory leaks are especially problematic for long-running programs (e.g., web browsers are often implicated in memory leak problems [#f3]_).  The following program is one example of a pretty horrible leak: there is a ``malloc`` call in a loop, but no matching ``free``.  Even worse, we completely lose the ability to access the memory block in the previous iteration of the loop by re-assigning to ``memory_block`` each time through the loop.  Note also that assigning ``NULL`` doesn't free a block; it simply makes a block inaccessible to the program.  

.. code-block:: c

    for (int i = 0; i < BIGNUMBER; i++) {
        char *memory_block = malloc(1024*1024);  // allocate a chunk on the heap
        // do nothing else!  
    }
    memory_block = NULL;  // doesn't free anything!  we just lost our access
                          // to the memory block most recently allocated, so
                          // we've created a hopeless memory leak!


A *dangling pointer* is a pointer that refers to a invalid block of memory, either to an undefined memory address or to a memory block that has already been freeed, and should thus be considered inaccessible.  For example:

.. code-block:: c

    int *p = malloc(sizeof(int));
    *p = 42; 
    int *q = p;  // q is a pointer; now it just holds the same address as p
    printf("q is %d\n", *q); // 42
    printf("p is %d\n", *p); // 42
    free(p); // free p.
    printf("p is %d\n", *p); // NO!  p is invalid because we just free'd it!
    printf("q is %d\n", *q); // Double NO!  since we free'd p, q is a "dangling pointer"
                            // since it pointed to the same memory block!



.. topic:: :command:`scan-build` and :command:`valgrind`

   Valgrind is a pretty excellent tool for helping to ferret out memory leaks, memory trashing, and any other type of memory corruption error that can happen in C programs.  To run a program with valgrind, you can just type :command:`valgrind <program>`.  
   
   There are *many* command-line options to change the behavior or output of valgrind.  Type :command:`valgrind -h` for help (or :command:`man valgrind`).  See http://valgrind.org for more information on this great tool.

   Besides :command:`valgrind`, the :command:`scan-build` tool is also incredibly helpful.  It is also usually faster and with better (easier to understand) output.  See a :ref:`previous description of scan-build <scan-build>` as well as https://clang-analyzer.llvm.org/scan-build.html for more information.


Advantages and disadvantages of heap-allocated memory
-----------------------------------------------------

Heap-allocated memory makes it possible to create linked lists, dynamically-sized arrays and strings, and more exotic data structures such as trees, heaps, and hashtables.  Manually allocating and deallocating memory can be a pain, though.  As a result, you probably want to be strategic about whether to use stack-allocated memory (e.g., local arrays and variables) or heap-allocated memory in a program.  Here are some key advantages and disadvantages to help you consider what is right for a given situation:

Advantages to heap allocation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 * The size of an array, string, or some other data structure can be defined at run time.  With stack-allocated arrays, for example, you typically need to specify a "reasonable upper bound" for the size of the array, and somehow deal with the consequences if the size of the array is exceeded.

 * A block of memory will exist until it is explicitly deallocated with a call to ``free``.  For stack-allocated memory, the memory is automatically deallocated when a function is exited, which is totally inappropriate for data structures such as linked lists.  

 * You can dynamically *change* the size of the array, string, or some other data structure at run time.  There is a built-in ``realloc`` function that can help with this (see :command:`man realloc`), 

Disadvantages to heap allocation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

 * You have to remember to allocate and deallocate a data structure, and you have to get it right.  This is harder than it sounds, and when things go wrong the program will either exhibit unexpected (buggy) behavior, or crash in a ball of flames.  Debugging can be hard.

 * You have to remember to deallocate a memory block exactly once when you are done with it, and you have to get that right.  Also, harder than it looks.  For example, calling ``free`` on the same memory block *twice* is an error, and typically causes a crash.


Dynamic Arrays
--------------

Since arrays are just contiguous areas of bytes, you can allocate your own arrays in the heap using ``malloc``.  It is also fairly straightforward to resize an array as necessary (i.e., to grow it to accommodate more data items).  The following code allocates two arrays of 1000 ints --- one in the stack the usual "local" way, and one in the heap using ``malloc``.  Other than the different allocations, the two are syntactically similar in use.  

.. code-block:: c

    int a[1000]; // allocate 1000 ints in the stack
    int *b = malloc(sizeof(int) * 1000);  // allocate 1000 ints on the heap
    a[123] = 13;      // just use good ol' [] to access elements
    b[123] = 13;      // in both arrays
    free(b);          // must call free on the heap-allocated array


To grow the heap-allocated array, we could do something like the following.  (Note that the following code uses ``memcpy``, which accepts three parameters: a destination address, a source address, and the number of bytes to copy):

.. code-block:: c

    int *arr = malloc(sizeof(int) * 1000); // 1000 ints on the heap

    // assume we need to grow the array

    int *newarr = malloc(sizeof(int) * 2000); // double your integer pleasure
    memcpy(newarr, arr, 1000*sizeof(int)); // copy over contents of old array
    free(arr);    // free old array
    arr = newarr; // arr now points to new, larger block

.. _dynamic-strings:

C strings revisited
-------------------

Although we have used arrays of ``char`` to hold C strings thus far, a much more common way to declare the type of a C string is ``char *``.  This shouldn't be particularly surprising, since arrays and pointers are treated nearly synonymously in C.  That's not to say that stack-allocated C strings as arrays aren't useful.  Indeed, they are very commonly used.  It is, however, often necessary to copy and manipulate strings in memory, and using stack or statically allocated arrays becomes quite difficult.

As an example, say that we need to "escape" an HTML string to replace any occurrence of ``<`` with ``&lt;`` (lt: "less-than") and any occurrence of ``>`` with ``&gt;`` (gt: "greater-than").  (There are other characters that are replaced when "properly" escaping an HTML string; we're just focusing on these two characters in this example.)  Since the string will "grow" as we escape it, dynamic memory allocation has obvious benefits.  Here is the code:

.. literalinclude:: code/escapehtml.c
   :language: c
   :linenos:

Linked lists
------------

One of the most commonly used dynamic data structures is the *linked list*.  A standard definition of a linked list node in C, in which each node contains an integer, is as follows:

.. code-block:: c

    struct node {
        int value;
        struct node* next;
    };


Notice that there's something of a circular definition and usage here (i.e., inside the definition of ``struct node``, we declare a ``struct node`` as a field).  C is perfectly happy with that circularity.

Manipulating nodes in a linked list generally involves allocating new nodes on the heap, linking in new nodes to the list, and/or modifying node pointers in other ways.  Here is a bit of code for adding a new node to a list by inserting in the front:

.. code-block:: c

    struct node *insert(struct node *head, int new_value) {

        struct node *new_node = malloc(sizeof(struct node));
        new_node->value = new_value;
        new_node->next = head;  // next ptr of new node refers to head of old list
        return new_node;
    }

A function to traverse a list and print each value out might look like the following.  Notice that since the ``print_list`` function gets a *copy* of the head of the list, it is safe to modify that pointer within the ``print_list`` function.

.. code-block:: c

    void print_list(struct node *head) {
        int i = 0;
        while (head != NULL) {
            printf ("Node %d has the value %d\n", i+1, head->value); 
            head = head->next;  // advance the list pointer
            i += 1;
        }
    }

.. index:: pointers to pointers

Pointers to pointers, etc.
--------------------------

Some functions in the C standard library take pointers-to-pointers ("double pointers"), and you will likely encounter situations in which it is *useful* to make pointers-to-pointers.  One example of such a situation occurs when a function needs to allocate and initialize heap memory for a caller.  Here is an example in code:

.. code-block:: c

  int copy_string(char **dest, const char *source) {
      int buffer_size = strlen(source) + 1;
      *dest = malloc(sizeof(char) * buffer_size);
      if (!*dest) {
          return -1;  // failure!
      }
      strlcpy(*dest, source, buffer_size);
      return 0; // success
  }

  char *ptr;
  copy_string(&ptr, "here's a string!");
  printf("%s\n", ptr); 
  // don't forget: need to eventually free(ptr)!

In the above code, the ``copy_string`` function takes a "pointer to a pointer to a char" as the first parameter, and a constant C string as the second parameter.  The function allocates a new block of memory using ``malloc`` and copies the ``source`` string into a ``dest`` string.  

Why can't the first parameter be ``char *``?  The reason has to do with pass-by-value function parameters: since we want to *modify* what ``dest`` points to, we need to access that pointer indirectly or the modification only happens to a local variable.  (This is exactly the same situation we encountered with the failed ``swap`` function.)  Since we can indirectly access the ``dest`` pointer (i.e., via the pointer to the pointer), the change we make is observable to the caller of the function.

Outside the function, we declare a normal ``char *`` string variable (``ptr``), then pass *the address of ptr* to the function, which creates the pointer-to-a-pointer.  When we return from the function, ``ptr`` has been modified (specifically, the address held in the variable ``ptr`` has been modified).  As a side-effect of the way this function is implemented (i.e., it uses ``malloc``), we must remember to eventually call ``free`` on ``ptr`` in order to avoid a memory leak.

.. rubric:: Exercises

#.  Consider the following code.  Identify exactly what is allocated on the stack and what is allocated on the heap::

        int main() {
            int *p = NULL;
            p = malloc(sizeof(int)*42);
            char arr[1000];
            char *p = &arr[10];
            char *q = malloc(64);
            char *r = &q[10];
        }


#.  Write a function that mimics the built-in ``strdup`` function.

#.  Write a ``clear_list`` function that takes a pointer to a linked list (``struct node *``) and calls free for each element in the list (i.e., to completely deallocate the list).

#.  Create a ``clone_list`` function that takes a pointer to a linked list (``struct node *``) and returns a completely cloned copy of the list (i.e., there are exactly the same number of nodes in the new list, with the exactly the same values in the same order, but the old list is left totally unmodified).

#.  Create a ``reverse_list`` function that accepts a pointer to a ``struct node`` and returns a pointer to the list with all elements now in reversed order.

#.  Write a function that appends a new value to the end of a linked list.  The function should return a pointer to the head of the list.

#.  Write new implementations of the various linked list functions we've seen and written so far but instead of having an int as the value, use a ``char *`` (dynamically allocated C string).

#.  Write a function that accepts the name of a text file, and allocates and returns a C string that contains the *entire* contents of the file.  (If you're familiar with the basic file I/O API of Python, this function should work like the ``read()`` method of a file object.)

#.  Rewrite the ``escapehtml`` function so that it accepts a *pointer to a pointer to a char* (``char **``), allocates a new string that contains the escaped string and *assigns* the newly allocated string to the C string pointer to by the pointer argument to the function.  For example, if the parameter is ``char **str``, the variable ``*str`` refers to the C string containing the HTML text to be escaped.  When the function concludes, ``*str`` should *now* refer to the C string containing the escaped HTML text.  You should be sure to ``free`` the original unescaped C string.

#.  Rewrite the ``escapehtml`` function so that it accepts a *pointer to a char* (not a *const* pointer), and modifies the string *in place* to escape each ``<`` and ``>``.  You should use the built-in C library function ``realloc`` to dynamically reallocate heap memory allocated to the string passed into the function.  You'll need to read the ``man`` page for ``realloc`` to understand how this function works.

#.  Create a couple C structs to implement a dynamically growable/shrinkable stack data structure (a stack of integers) like the following::

      struct intnode {
        int value;
        struct intnode *next;
      };
    
      typedef struct {
        struct intnode *top;
      } stack_t;

    Write five functions to create, destroy/deallocate, and perform standard operations on a stack with these definitions.  Notice that the ``stack_t`` structure just contains a pointer to the top of the stack, and that stack is just implemented as a linked list of ``struct intnode``.  For the ``pop`` function, you can assume in your implementation that the stack is non-empty (i.e., it would be an error on the part of the user of the stack if ``pop`` was called on an empty stack).

      * ``stack_t* allocate_stack(void);``
      * ``void deallocate_stack(stack_t *);``
      * ``int empty(stack_t *);``
      * ``int pop(stack_t *);``
      * ``void push(stack_t *, int);``


.. rubric:: Footnotes

.. [#f1] http://en.wikipedia.org/wiki/High-level_programming_language

.. [#f2] http://en.wikipedia.org/wiki/Low-level_programming_language

.. [#f3] Just search for "Firefox memory leak" and you'll find plenty of posts not unlike the following: https://support.mozilla.org/en-US/questions/1006397  
