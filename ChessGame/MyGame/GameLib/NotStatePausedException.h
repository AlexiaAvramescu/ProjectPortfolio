#pragma once
#include "MethodException.h"
class NotStatePausedException :
    public MethodException
{
public:
    NotStatePausedException();
};

