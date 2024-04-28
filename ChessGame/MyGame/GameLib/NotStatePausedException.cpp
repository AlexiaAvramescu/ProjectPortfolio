#include "NotStatePausedException.h"

NotStatePausedException::NotStatePausedException()
	:MethodException("[CAN'T CALL THIS FUNCTION] Game State is not Paused.\n")
{}
