 * Getting data using fseek()

- If you have many records inside a file and need to access a record at a specific position, you need to loop through all the records before it to get the record.

- This will waste a lot of memory and operation time. An easier way to get to the required data can be achieved using fseek().

- As the name suggests, fseek() seeks the cursor to the given record in the file.

* Syntax of fseek()
* fseek(FILE * stream, long int offset, int whence);

- The first parameter stream is the pointer to the file. The second parameter is the position of the record to be found, and the third parameter specifies the location where the offset starts.

-------------------- Different whence in fseek() -----------------------

            Whence	    Meaning
            SEEK_SET	Starts the offset from the beginning of the file.
            SEEK_END	Starts the offset from the end of the file.
            SEEK_CUR	Starts the offset from the current location of the cursor in the file.