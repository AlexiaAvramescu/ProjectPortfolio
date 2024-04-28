#pragma once
#include "MethodException.h"
class NotStateDrawProposedException :
    public MethodException
{
public:
    NotStateDrawProposedException();
};

