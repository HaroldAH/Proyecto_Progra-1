#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool verifyAnswer(int *number, int lowestOption, int highestOption)
{
    char buffer[128]; // buffer temporal para leer la entrada

    // Leemos la línea completa con fgets
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        // Error o fin de archivo
        return true;
    }

    // Quitamos el salto de línea final si existe
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
        buffer[len-1] = '\0';

    // Convertimos usando sscanf (o strtol)
    if (sscanf(buffer, "%d", number) != 1)
    {
        // No se pudo leer un entero
        return true;
    }

    // Validamos el rango
    if (*number < lowestOption || *number > highestOption)
        return true;
    return false;
}

bool verifyAnswerFloat(float *number, int lowestOption, int highestOption)
{
    char buffer[128];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        // Error o fin de archivo
        return true;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
        buffer[len-1] = '\0';

    if (sscanf(buffer, "%f", number) != 1)
    {
        // No se pudo leer un float
        return true;
    }

    if (*number < lowestOption || *number > highestOption)
        return true;
    return false;
}

void fillString(char *string, int size)
{
    // Leemos la línea completa en 'string'
    if (fgets(string, size, stdin))
    {
        // Eliminamos salto de línea si existe
        size_t len = strlen(string);
        if (len > 0 && string[len-1] == '\n')
            string[len-1] = '\0';
    }
    else
    {
        // Manejar error o EOF
        string[0] = '\0'; 
    }
}
