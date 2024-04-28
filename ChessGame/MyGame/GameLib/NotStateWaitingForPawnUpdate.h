#pragma once
#include "MethodException.h"
class NotStateWaitingForPawnUpdate :
    public MethodException
{
public:
    NotStateWaitingForPawnUpdate();
};

