#pragma once

#include "MethodException.h"

class NotStatePlayingException :
    public MethodException
{
public:
    NotStatePlayingException();
};

