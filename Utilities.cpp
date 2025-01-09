#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool verifyAnswer(int *number, int lowestOption, int highestOption)
{
    char buffer[128]; 

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return true;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
        buffer[len-1] = '\0';

    if (sscanf(buffer, "%d", number) != 1)
    {
        return true;
    }

    if (*number < lowestOption || *number > highestOption)
        return true;
    return false;
}

bool verifyAnswerFloat(float *number, int lowestOption, int highestOption)
{
    char buffer[128];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return true;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
        buffer[len-1] = '\0';

    if (sscanf(buffer, "%f", number) != 1)
    {
        return true;
    }

    if (*number < lowestOption || *number > highestOption)
        return true;
    return false;
}

void fillString(char *string, int size)
{

    if (fgets(string, size, stdin))
    {
        size_t len = strlen(string);
        if (len > 0 && string[len-1] == '\n')
            string[len-1] = '\0';
    }
    else
    {
        string[0] = '\0'; 
    }
}
