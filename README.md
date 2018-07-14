# HilbertCurve

Hilbert Curves generated with Cairo in C.

The program takes an integer n as an argument and draws the (n+1)th approximation to the limiting Hilbert Curve generated with in the file `Curve.pdf`. 


 Requisites
------------------
The Cairo library is required to compile this program. You will find [here](https://cairographics.org/download/) the instructions to download it.

Instructions
------------------
#### Build the project
Build the binaries by simply typing `make` in your terminal, in the project directory.

#### Run the program
You can run the program with the following command :

    $ ./main [n]

Where `[n]` is the level of approximation of the curve. If no argument is found, the default value of the level of approximation is 4. It is set in `main.c`, where it is defined with the macro `DEFAULT_DEPTH`.

Another way of testing the program is to make the pdf file :

    $ make Curve.pdf

This will simply call the main function with no argument.

License
------------------
This project is licensed under the GPL license, see the [LICENSE](LICENSE) file for details.
