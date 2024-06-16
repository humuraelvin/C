* Opening a file - for creation and edit
* Opening a file is performed using the fopen() function defined in the stdio.h header file.

- The syntax for opening a file in standard I/O is:

+ ptr = fopen("fileopen","mode");
For example,

- fopen("E:\\cprogram\\newprogram.txt","w");

- fopen("E:\\cprogram\\oldprogram.bin","rb");
Let's suppose the file newprogram.txt doesn't exist in the location E:\cprogram. The first function creates a new file named newprogram.txt and opens it for writing as per the mode 'w'.
The writing mode allows you to create and edit (overwrite) the contents of the file.
Now let's suppose the second binary file oldprogram.bin exists in the location E:\cprogram. The second function opens the existing file for reading in binary mode 'rb'.
The reading mode only allows you to read the file, you cannot write into the file.

functions used --

** fopen()
** fwrite()
** fread()
** fclose()
** fscanf()
** fprintf()