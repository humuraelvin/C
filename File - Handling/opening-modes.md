* Opening Modes in Standard I/O

Mode	Meaning of Mode	                    During Inexistence of file
r	    Open for reading.	                If the file does not exist, fopen() returns NULL.
rb	    Open for reading in binary mode.	If the file does not exist, fopen() returns NULL.
w	    Open for writing.	                If the file exists, its contents are overwritten.

				                            If the file does not exist, it will be created.
wb	    Open for writing in binary mode.	If the file exists, its contents are overwritten.

				                            If the file does not exist, it will be created.
a	    Open for append.

				                            Data is added to the end of the file.	If the file does not exist, it will be created.
ab	    Open for append in binary mode.

				                            Data is added to the end of the file.	If the file does not exist, it will be created.
r+	    Open for both reading and writing.	If the file does not exist, fopen() returns NULL.
rb+	    Open for both reading and writing   If the file does not exist, fopen() returns NULL.
         in binary mode.	
w+	    Open for both reading and writing.	If the file exists, its contents are overwritten.

				                            If the file does not exist, it will be created.
wb+	    Open for both reading and           If the file exists, its contents are overwritte
        writing in binary mode.	n.

				                            If the file does not exist, it will be created.

a+	    Open for both reading and           If the file does not exist, it will be created.
        appending.	

ab+	    Open for both reading and           If the file does not exist, it will be created.
        appending in binary mode.	