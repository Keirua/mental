mental
======

During the summer 2012, I've found [this TED video](http://www.ted.com/talks/arthur_benjamin_does_mathemagic.html) on mental calculation.

I've especially been pretty impressed by the ease with which Arthur Benjamin can compute squares. Since I've always found mental calculation to be funny, I decided to learn how to do that as well.

Reverse engineering some easy tricks was possible, but I've been quite happy to find a copy of it's book (The secrets of mental maths) in order to learn some tricks for harder tasks (multiplication with large numbers).

Anyway, I needed a tool to assess my progresses. I wanted to easily validate that my result is correct, and to measure how long it takes me to perform some computation. A few hours later, this program was there.

So, this small program allows you to run a serie where you are asked to compute either squares, cubes or 2-number multiplications. You can choose how many numbers, what kind of operations and the range of values used. It's pretty intuitive, just have a look at the command line.

The code could be greatly improved and a lot of features could be added, but this is not necessary : this tool does what it is meant for !

Compilation:
------------

Compilation ok with gcc 4.6.3
	
   gcc main.c -o mental -lm -std=c99

Usage :
-------

Usage : ./mental [nb mode a b]
   
   nb : number of figures that will be asked. Default 10.
   mode : game mode. Can be "square", "cube", "product". Default to square
   a : minimum value. Default 10.
   b : maximum value. Default 100.

If the game mode is "square" or "cube", min and max indicate the range of the values in which the number will be asked.
If the game mode is "product", the values indicate the number of figures composing the 2 numbers. Ex: a=2 and b=3 mean that the product will be in the form (number with 2 figures)*(number with 3 figures)
