Functions
*********

All languages have a construct to separate and package blocks of code. C uses the "function" to package blocks of code. This article concentrates on the syntax and peculiarities of C functions. The motivation and design for dividing a computation into separate blocks is an entire discipline in its own.

A function has a name, a list of arguments which it takes when called, and the block of code it executes when called. C functions are defined in a text file and the names of all the functions in a C program are lumped together in a single, flat namespace. The special function called "main" is where program execution begins. Some programmers like to begin their function names with Upper case, using lower case for variables and parameters, Here is a simple C function declaration. This declares a function named Twice which takes a single int argument named num. The body of the function computes the value which is twice the num argument and returns that value to the caller.


::
    /*
      Computes double of a number.
      Works by tripling the number, and then subtracting to get back to double.
     */
    static int Twice(int num) {
        int result = num * 3;
        result = result - num;
        return(result);
    }

Syntax
------

The keyword "static" defines that the function will only be available to callers in the file where it is declared. If a function needs to be called from another file, the function cannot be static and will require a prototype -- see prototypes below. The static form is convenient for utility functions which will only be used in the file where they are declared. Next , the "int" in the function above is the type of its return value. Next comes name of the function and its list of parameters. When referring to a function by name in documentation or other prose, it's a convention to keep the parenthesis () suffix, so in this case I refer to the function as "Twice()". The parameters are listed with their types and names, just like variables.

Inside the function, the parameter num and the local variable result are "local" to the function -- they get their own memory and exist only so long as the function is executing. This independence of "local" memory is a standard feature of most languages (See CSLibrary/102 for the detailed discussion of local memory).

The "caller" code which calls Twice() looks like ::

    int num = 13;
    int a = 1;
    int b = 2;
    a = Twice(a);        // call Twice() passing the value of a
    b = Twice(b + num);  // call Twice() passing the value b+num
    // a == 2
    // b == 30
    // num == 13 (this num is totally independent of the "num" local to Twice()

Things to notice:

 * (vocabulary) The expression passed to a function by its caller is called the "actual parameter" -- such as "a" and "b + num" above. The parameter storage local to the function is called the "formal parameter" such as the "num" in "static int Twice(int num)".

 * Parameters are passed "by value" that means there is a single copying assignment operation (=) from each actual parameter to set each formal parameter. The actual parameter is evaluated in the caller's context, and then the value is copied into the function's formal parameter just before the function begins executing. The alternative parameter mechanism is "by reference" which C does not implement directly, but which the programmer can implement manually when needed (see below). When a parameter is a struct, it is copied.

 * The variables local to Twice(), num and result, only exist temporarily while Twice() is executing. This is the standard definition for "local" storage for functions.
 
 * The return at the end of Twice() computes the return value and exits the function. Execution resumes with the caller. There can be multiple return statements within a function, but it's good style to at least have one at the end if a return value needs to be specified. Forgetting to account of a return somewhere in the middle of a function is a traditional source of bugs.

C-ing and Nothingness -- void
-----------------------------

void is a type formalized in ANSI C which means "nothing". To indicate that a function does not return anything, use void as the return type. Also, by convention, a pointer which does not point to any particular type is declared as void*. Sometimes void* is used to force two bodies of code to not depend on each other where void* translates roughly to "this points to something, but I'm not telling you (the client) the type of the pointee exactly because you do not really need to know." If a function does not take any parameters, its parameter list is empty, or it can contain the keyword void but that style is now out of favor.

::

    void TakesAnIntAndReturnsNothing(int anInt);
    int TakesNothingAndReturnsAnInt();
    int TakesNothingAndReturnsAnInt(void); // equivalent syntax for above


Call by Value vs. Call by Reference
-----------------------------------

C passes parameters "by value" which means that the actual parameter values are copied into local storage. The caller and callee functions do not share any memory -- they each have their own copy. This scheme is fine for many purposes, but it has two disadvantages.

 1. Because the callee has its own copy, modifications to that memory are not communicated back to the caller. Therefore, value parameters do not allow the callee to communicate back to the caller. The function's return value can communicate some information back to the caller, but not all problems can be solved with the single return value.

 2. Sometimes it is undesirable to copy the value from the caller to the callee because the value is large and so copying it is expensive, or because at a conceptual level copying the value is undesirable.


The alternative is to pass the arguments "by reference". Instead of passing a copy of a value from the caller to the callee, pass a pointer to the value. In this way there is only one copy of the value at any time, and the caller and callee both access that one value through pointers.

Some languages support reference parameters automatically. C does not do this -- the programmer must implement reference parameters manually using the existing pointer constructs in the language.

Swap Example
------------ 

The classic example of wanting to modify the caller's memory is a ``swap()`` function which exchanges two values. Because C uses call by value, the following version of Swap will not work ::

    void Swap(int x, int y) {        // NO does not work
        int temp;
        temp = x;
        x = y;      // these operations just change the local x,y,temp
        y = temp;   // -- nothing connects them back to the caller's a,b
    }

    // Some caller code which calls Swap()...
    int a = 1;
    int b = 2;
    Swap(a, b);

``Swap()`` does not affect the arguments a and b in the caller. The function above only operates on the copies of a and b local to Swap() itself. This is a good example of how "local" memory such as ( x, y, temp) behaves -- it exists independent of everything else only while its owning function is running. When the owning function exits, its local memory disappears.

Reference Parameter Technique
-----------------------------

To pass an object X as a reference parameter, the programmer must pass a pointer to X instead of X itself. The formal parameter will be a pointer to the value of interest. The caller will need to use & or other operators to compute the correct pointer actual parameter. The callee will need to dereference the pointer with * where appropriate to access the value of interest. Here is an example of a correct Swap() function.

::

    static void Swap(int* x, int* y) {     // params are int* instead of int
        int temp;
        temp = *x;        // use * to follow the pointer back to the caller's memory
        *x = *y;
        *y = temp;
    }

    // Some caller code which calls Swap()...
    int a = 1;
    int b = 2;
    Swap(&a, &b);

Things to notice:
 
 * The formal parameters are int* instead of int.
 * The caller uses & to compute pointers to its local memory (a,b).
 * The callee uses * to dereference the formal parameter pointers back to get the caller's memory.

Since the operator & produces the address of a variable -- &a is a pointer to a. In Swap() itself, the formal parameters are declared to be pointers, and the values of interest (a,b) are accessed through them. There is no special relationship between the names used for the actual and formal parameters. The function call matches up the actual and formal parameters by their order -- the first actual parameter is assigned to the first formal parameter, and so on. I deliberately used different names (a,b vs x,y) to emphasize that the names do not matter.

const
-----

The qualifier const can be added to the left of a variable or parameter type to declare that the code using the variable will not change the variable. As a practical matter, use of const is very sporadic in the C programming community. It does have one very handy use, which is to clarify the role of a parameter in a function prototype ::

    void foo(const struct fraction* fract);

In the foo() prototype, the const declares that foo() does not intend to change the struct fraction pointee which is passed to it. Since the fraction is passed by pointer, we could not know otherwise if foo() intended to change our memory or not. Using the const, foo() makes its intentions clear. Declaring this extra bit of information helps to clarify the role of the function to its implementor and caller.

Bigger Pointer Example
----------------------

The following code is a large example of using reference parameters. There are several common features of C programs in this example...Reference parameters are used to allow the functions Swap() and IncrementAndSwap() to affect the memory of their callers. There's a tricky case inside of IncrementAndSwap() where it calls Swap() -- no additional use of & is necessary in this case since the parameters x, y inside InrementAndSwap() are already pointers to the values of interest. The names of the variables through the program(a, b, x, y, alice, bob) do not need to match up in any particular way for the parameters to work. The parameter mechanism only depends on the types of the parameters and their order in the parameter list -- not their names. Finally this is an example of what multiple functions look like in a file and how they are called from the main() function.

::

    static void Swap(int* a, int* b) {
        int temp;
        temp = *a;
        *a = *b;
        *b = temp;
    }

    static void IncrementAndSwap(int* x, int* y) {
        (*x)++;
        (*y)++;
        Swap(x, y); // don't need & here since a and b are already
                    // int*'s.
    }

    int main(int argc, char **argv) {
        int alice = 10;
        int bob = 20;

        Swap(&alice, &bob);
        // at this point alice==20 and bob==10

        IncrementAndSwap(&alice, &bob);
        // at this point alice==11 and bob==21

        return 0; 
    }

