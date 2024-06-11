* Reading and writing to a binary file
- Functions fread() and fwrite() are used for reading from and writing to a file on the disk respectively in case of binary files.

* Writing to a binary file
To write into a binary file, you need to use the fwrite() function. The functions take four arguments:

- address of data to be written in the disk
- size of data to be written in the disk
- number of such type of data
- pointer to the file where you want to write.
- fwrite(addressData, sizeData, numbersData, pointerToFile);