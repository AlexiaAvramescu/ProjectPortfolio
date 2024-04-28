#include "NotStatePlayingException.h"

NotStatePlayingException::NotStatePlayingException()
	:MethodException("[CAN'T CALL THIS FUNCTION] Game State is not Playing.\n")
{}
