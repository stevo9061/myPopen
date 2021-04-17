#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;
    char line[130];

    fp = popen("ls -l", "r");

    while (fgets(line, sizeof(line), fp))
    {
        printf("line: %s", line);
    }

    pclose(fp);


    printf("-------------------------------------------------------------------------");
    exit(0);
}

/*

fp = mypopen("ls -l", "r");

while (fgets(line, sizeof(line), fp))
{
printf("line: %s", line);
}

mypclose(fp);
exit(0);
}

//Erster Test "Good Case" Ohne Fehlerabwicklung

 */