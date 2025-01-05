#pragma once

#include "Utilities.h"

class Field {
    int fieldId;
    int seatRow;
    int seatColumn;

public:
    Field();
    void setFieldId(int const m_fieldId);
    int getSeatRow();
    int getSeatColumn();
    int getFieldId();
    void fillField(int const sizeN, int const sizeM);
};
