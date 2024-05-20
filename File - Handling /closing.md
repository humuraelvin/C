* Closing a File
The file (both text and binary) should be closed after reading/writing.


* Closing a file is performed using the fclose() function.

- fclose(fptr);
Here, fptr is a file pointer associated with the file to be closed.

* Reading and writing to a text file
For reading and writing to a text file, we use the functions fprintf() and fscanf().

They are just the file versions of printf() and scanf(). The only difference is that fprintf() and fscanf() expects a pointer to the structure FILE.