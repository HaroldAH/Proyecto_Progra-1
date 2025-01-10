#include "Field.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Field::Field() {
    seatRow = 0;
    seatColumn = 0;
    fieldId = 0;
}

void Field::fillField(int const sizeN, int const sizeM) {

    char letter;
    seatRow = 0;
    seatColumn = 0;

    printf("\n\t Digite la letra del asiento entre A y %c ", char(sizeN + 64));
    while (seatRow < 65 || seatRow > (sizeN + 64)) {
        printf("  :=>  ");
        
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (strlen(buffer) == 1) {
            letter = buffer[0];
            seatRow = int(letter);
        } else {
            seatRow = 0;
        }
    }

    printf("\n\t Digite el numero del asiento :=> ");
    while (verifyAnswer(&seatColumn, 1, sizeM)) {
        printf("\n\t Porfavor introduzca un asiento valido entre 1 y %d :=> ", sizeM);
    }
}

int Field::getFieldId() {
    return this->fieldId;
}

int Field::getSeatRow() {
    return seatRow - 64;
}

int Field::getSeatColumn() {
    return seatColumn;
}
