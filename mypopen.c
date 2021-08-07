/**
 * @file mypopen.c
 * @author Stefan Bittgen
 * @brief mypopen,mypclose - Pipe implementation
 * @version 1.0
 * @date 17.05.21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "mypopen.h"

//Global Variables
pid_t pid = -1;
FILE *Stream = NULL;

/**
 * @brief Creates a pipe to a child process and executes a program in parent
 * @param command is the command to execute
 * @param type operation type ( r or w)
 * @return FILE* File pointer to the stream, is NULL if an error occured.
 */
FILE *mypopen(const char *command, const char *type) {

    //initial vars
    int fd[2];
    // fd[0] - read
    // fd[1] - write
    int fd2; //Filedeskriptor will be not used - I use now instead of "fd2" "STDOUT_FILENO" in line 65. Reason is readability


    char buffer[PIPE_BUF];

    //Create a pipe, if < 0, e.g. -1 then error
    if (pipe(fd) == -1) {
        printf("An error occurred with opening the pipe\n");
        //pipe creation failed
        exit(EXIT_FAILURE);
    }
/*fork(): Create a new child process with fork():
*    I can find out at the end of the fork function if i have a parent or child process
*    by returning 0 or not equal to 0.
*    < 0 means, that the setting of a child process was not successful
*    0 means that the return value of the new child process was successful
*    > 0 Return value is returned to the parent process or caller. The value contains the process ID of the newly created child process.
*/
    pid = fork();
    if (pid < 0) {
//        if ((pid = fork ()) < 0) {
        printf("An error occurred with fork()");
        exit(EXIT_FAILURE);
    }

    //Child writes in pipe
    if (pid == 0) {
        close(fd[0]); //Close unused read end
        write(fd[1], command, sizeof(command)); //writes the output of the command e.g. "ls" in fd[1]

/*Dup2: The filedescriptor is duplicated to clear the close-on-exec flag. If dup2 is not used, then the child/parent process is overlaid
 *      by a new process with an exec*() function. Without dup() or dup(2) otherwise the close-on-exec flag would be set. And then all filedescriptors
 *      become invalid (because overlapped by the new process) and thus no longer available.
 */
        //Simply explained, with dup2 we connect the pipe(fd[1]) to the parent process(STDOUT)
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
    } else { //Parent process reads from pipe
        close(fd[1]); //Close unused write end
        //I read now from the fd[0] pipe in the buffer
        read(fd[0], &buffer, sizeof(buffer));

//      execl("/bin/ls", "ls", "-l", NULL);
//        execlp(command, command, NULL);
    if (execl("/bin/sh", "sh", "-c", command, (char *) NULL) == -1) {
            _exit(EXIT_FAILURE);
    }
/*Notice: You cannot specify any code after calling execl, because the function is overwriting
 *        the rest of our code.
*/
        close(fd[0]);
        mypclose(Stream);
    }

    return Stream;

}

/**
 * @brief Closes the pipe from mypopen
 *
 * @param stream stream that tries to close
 * @return returns 0 on success, -1 on error
 */

int *mypclose(FILE *stream){


/*    With this function we are waiting of closing the child
 *    process, after this process the parent process quits his job.
*/
    //wait() should also operate
    if (waitpid (pid, NULL, 0) != pid) {
        perror ("Waitpid()");
        return EXIT_FAILURE;
    }

    if (pid == -1) {
        printf("No process was open");
    }


}

int main(int argc, char **argv) {

    char line[50];
    char typ;


    if (argc != 1) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit (EXIT_FAILURE);
    }

    printf("Please enter a command, (currently we only support read):");
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = 0;
    printf("The command entered is: %s\n", line);

    printf("Please enter a type r or w:\n");

    scanf("%c", &typ);
    printf("Entered typ is %c\n ", typ);


    if (typ == 'r') {
        mypopen(line, "r"); //"r" is for STDOUT
    }

    if (typ == 'w') {

        mypopen(line, "w");

    }

    return 0;

}

/*printf("-------------------------------------------------------------------------");

    FILE *ep;
    char line2[130];

    ep = popen("ls -l", "r");

    while (fgets(line2, sizeof(line2), ep))
    {
        printf("line: %s", line2);
    }

    pclose(ep);
    exit(0);
*/




/*
fp = mypopen("ls -l", "r");

while (fgets(line, sizeof(line), fp))
{
printf("line: %s", line);
}

mypclose(fp);
exit(0);
}

*/
