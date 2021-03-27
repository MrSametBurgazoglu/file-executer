#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <sys/wait.h>
#define MAX_LINE_WIDTH 128

void free_array(char** array)
{
    int i;

    for ( i = 0; array[i]; i++ )
        free( array[i] );
    free(array);
}

int main(void){
    int fd = open("commands.txt", O_RDONLY);
    char character;
    char newline[MAX_LINE_WIDTH];
    int newline_current_index = 0;
    int line_counter = 0;
    char **parameters = malloc(line_counter * sizeof(char*));

    while(read(fd, &character, 1) != 0) {
        newline[newline_current_index] = character;
        if(character == '\n'){
            line_counter++;
            parameters = realloc(parameters, line_counter * sizeof(char*));
            parameters[line_counter-1] = malloc(newline_current_index*sizeof(char));
            memcpy(parameters[line_counter-1], newline, newline_current_index);
            newline_current_index = -1;
            memset(newline, 0, 128);
            }
        newline_current_index++;
        }
    for(int i = 0; i < line_counter; ++i){
        printf("%s\n", parameters[i]);
        int a = fork();
        if(fork() == 0){
            char *args[] = {"/bin/sh", "-c", parameters[i], NULL};
            execve("/bin/sh", args, NULL);
            exit(0);
        }
    }
    free_array(parameters);
    return 0;
}
