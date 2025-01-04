#pragma once

#include "Utilities.h"

class Field
{
    int fieldId;
    int seatInN;
    int seatInM;
public:
    Field();
    void setFieldId(int const m_fieldId);
    int getSeatInN();
    int getSeatInM();
    int getFieldId();
    void fillField(int const sizeN,int const sizeM);
};