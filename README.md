# example-of-system-call-in-c
Create custom_find command that recursively searches for files with a certain extension in the subtree rooted in the directory possibly passed as an argument (see below). For each file found, print the absolute path of the directory in which it was found and the date of the last modification.

The program receives at most two input arguments:
1) extension string, e.g., ".txt" [first argument is mandatory]
2) string path [second argument, optional]: if the argument is there, search in the subtree rooted in the "path" directory, if it is not there, search starting from "."

Example:
bash: ~ $ ./custom_find ".txt" "directory"
/home/cc/directory/file1.txt Fri May 5 19:25:09 2020
/home/cc/directory/subdir/file2.txt Fri May 4 19:25:09 2020
bash: ~ $ ./custom_find ".txt"
/home/cc/file3.txt Fri May 5 19:25:09 2020
/home/cc/directory/file1.txt Fri May 5 19:25:09 2020
/home/cc/directory/subdir/file2.txt Fri May 4 19:25:09 2020

Information to compile:
Move to directory with the c file and write te command "make" from terminal. 
You can also use "make clean" to delete de compilated file after using it.

