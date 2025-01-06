#ifndef FIELD_H
#define FIELD_H

#include "Utilities.h"

class Field {
private:
    int fieldId;
    int seatRow;
    int seatColumn;

public:
    Field();

    void setFieldId(int const m_fieldId);
    int getFieldId();

    int getSeatRow();
    int getSeatColumn();

    void fillField(int const sizeN, int const sizeM);
};

#endif 
