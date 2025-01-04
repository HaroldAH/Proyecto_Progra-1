#include "Field.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Field::Field()
{
    seatInN = 0;
    seatInM = 0;
    fieldId = 0;
}
void Field::fillField(int const sizeN,int const sizeM)
{
    char letter;
    seatInN = 0;
    seatInM = 0;

    printf("\n\t Digite la letra del asiento entre A y %c ", char(sizeN + 64));
    while (seatInN < 65 || seatInN > (sizeN + 64)) 
    {
        printf("  :=>  ");
        
        // Usamos un buffer para leer la línea completa
        char buffer[16];  
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            // Manejo de error o EOF, puedes salir o repetir
            // por ejemplo:
            continue;
        }
        
        // Si tenemos un salto de línea al final, lo quitamos
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }

        // Sólo esperamos un carácter
        // Si deseas, puedes validar que buffer tenga exactamente 1 carácter
        if (strlen(buffer) == 1)
        {
            letter = buffer[0];
            seatInN = int(letter);
        }
        else
        {
            // Si el usuario tecleó más de un caracter, ignoramos y repetimos
            seatInN = 0; 
        }
    }

    printf("\n\t Digite el numero del asiento :=> ");
    while (verifyAnswer(&seatInM, 1, sizeM))
    {
        printf("\n\t Porfavor introduzca un asiento valido entre 1 y %d :=> ", sizeM);
    }
}

void Field::setFieldId(int const m_fieldId)
{
    this->fieldId = m_fieldId;
    printf("\n\t Su tiquete asignado : %d ... RECORDAR \n",fieldId);
}
int Field::getFieldId()
{
    return this->fieldId;
}
int Field::getSeatInN()
{
    return seatInN-64;
}
int Field::getSeatInM()
{
    return seatInM;
}