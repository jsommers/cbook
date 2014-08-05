Arrays and Pointers
*******************

Advanced C Arrays
-----------------

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

Pointer++ Style -- strcpy()
---------------------------

If p is a pointer to an element in an array, then (p+1) points to the next element in the array. Code can exploit this using the construct p++ to step a pointer over the elements in an array. It doesn't help readability any, so I can't recommend the technique, but you may see it in code written by others.

(This example was originally inspired by Mike Cleron) There's a library function called strcpy(char* destination, char* source) which copies the bytes of a C string from one place to another. Below are four different implementations of strcpy() written in order: from most verbose to most cryptic. In the first one, the normally straightforward while loop is actually sortof tricky to ensure that the terminating null character is copied over. The second removes that trickiness by moving assignment into the test. The last two are cute (and they demonstrate using ++ on pointers), but not really the sort of code you want to maintain. Among the four, I think strcpy2() is the best stylistically. With a smart compiler, all four will compile to basically the same code with the same efficiency.

::

    // Unfortunately, a straight while or for loop won't work.
    // The best we can do is use a while (1) with the test
    // in the middle of the loop.
    void strcpy1(char dest[], const char source[]) {
        int i = 0;
        while (1) {
            dest[i] = source[i];
            if (dest[i] == '\0') break; // we're done
            i++;
        } 
    }

    // Move the assignment into the test
    void strcpy2(char dest[], const char source[]) {
        int i = 0;
        while ((dest[i] = source[i]) != '\0') {
            i++;
        }
    }

    // Get rid of i and just move the pointers.
    // Relies on the precedence of * and ++.
    void strcpy3(char dest[], const char source[]) {
        while ((*dest++ = *source++) != '\0') ;
    }

    // Rely on the fact that '\0' is equivalent to FALSE
    void strcpy4(char dest[], const char source[]) {
        while (*dest++ = *source++) ;
    }

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

Arrays and Pointers
-------------------

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

Heap Memory
-----------

C gives programmers the standard sort of facilities to allocate and deallocate dynamic heap memory. A word of warning: writing programs which manage their heap memory is notoriously difficult. This partly explains the great popularity of languages such as Java and Perl which handle heap management automatically. These languages take over a task which has proven to be extremely difficult for the programmer. As a result Perl and Java programs run a little more slowly, but they contain far fewer bugs. (For a detailed discussion of heap memory see http://cslibrary.stanford.edu/102/, Pointers and Memory.)

C provides access to the heap features through library functions which any C code can call. The prototypes for these functions are in the file <stdlib.h>, so any code which wants to call these must #include that header file. The three functions of interest are...

void* malloc(size_t size)
    Request a contiguous block of memory of the given size in the heap. malloc() returns a pointer to the heap block or NULL if the request could not be satisfied. The type size_t is essentially an unsigned long which indicates how large a block the caller would like measured in bytes. Because the block pointer returned by malloc() is a void* (i.e. it makes no claim about the type of its pointee), a cast will probably be required when storing the void* pointer into a regular typed pointer.

void free(void* block)
    The mirror image of malloc() -- free takes a pointer to a heap block earlier allocated by malloc() and returns that block to the heap for re-use. After the free(), the client should not access any part of the block or assume that the block is valid memory. The block should not be freed a second time.

void* realloc(void* block, size_t size)
    Take an existing heap block and try to relocate it to a heap block of the given size which may be larger or smaller than the original size of the block. Returns a pointer to the new block, or NULL if the relocation was unsuccessful. Remember to catch and examine the return value of realloc() -- it is a common error to continue to use the old block pointer. Realloc() takes care of moving the bytes from the old block to the new block. Realloc() exists because it can be implemented using low-level features which make it more efficient than C code the client could write.

Memory Management
-----------------

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

