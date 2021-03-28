/*
*GNU GENERAL PUBLIC LICENSE V3.0
*Developer:Samet Burgazoğlu
*Date:27/03/2021
*This software read lines from programs.txt. For every line of file create a child process in shell and execute line in shell.
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <sys/wait.h>
#include <limits.h>
#define MAX_LINE_WIDTH 128

void free_array(char** array)//free a array taken from malloc
{
    int i;

    for ( i = 0; array[i]; i++ )
        free( array[i] );
    free(array);
}

int main(void){
    int fd = open("programs.txt", O_RDONLY);//get file for reading
    char character;//current character in file
    char newline[MAX_LINE_WIDTH];//char array for every line
    int newline_current_index = 0;//current seek in line
    int line_counter = 0;//how many line read
    char **parameters = malloc(line_counter * sizeof(char*));//lines from file

    while(read(fd, &character, 1) != 0) {
        newline[newline_current_index] = character;//get a character from file
        if(character == '\n'){//if we are at end of the line
            line_counter++;//there is a new line
            parameters = realloc(parameters, line_counter * sizeof(char*));//take space for new line
            parameters[line_counter-1] = malloc(newline_current_index*sizeof(char));//take space for last line in parameters
            memcpy(parameters[line_counter-1], newline, newline_current_index);//copy current line to parameters last value
            newline_current_index = -1;//this will be 0 in start of line
            memset(newline, 0, 128);//reset char array
            }
        newline_current_index++;//new character
        }
    for(int i = 0; i < line_counter; ++i){//every line of file
        int a = fork();//create child process
        if(fork() == 0){//if current process is child process
            char *args[] = {"/bin/sh", "-c", parameters[i], NULL};//args for shell
            execve("/bin/sh", args, NULL);//give line to shell for execute
            exit(0);//exit
        }
    }
    free_array(parameters);//give space back
    close(fd);//close file
    return 0;
}
