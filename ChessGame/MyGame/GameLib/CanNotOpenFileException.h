#pragma once
#include "FileException.h"
class CanNotOpenFileException :
    public FileException
{
public:
    CanNotOpenFileException();
};

