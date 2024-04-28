#include "NotStateDrawProposedException.h"

NotStateDrawProposedException::NotStateDrawProposedException()
	: MethodException("[CAN'T CALL THIS FUNCTION] Game State is not DrawProposed. \n")
{
}
