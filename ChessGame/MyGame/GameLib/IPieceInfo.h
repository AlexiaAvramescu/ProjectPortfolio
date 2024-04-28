#pragma once

#include "EPieceColor.h"
#include "EPieceType.h"

#include <memory>
#include <vector>

class IPieceInfo
{
public:
	virtual EPieceColor GetColor() const = 0;
	virtual EPieceType GetType() const = 0;

	virtual ~IPieceInfo() = default;
};

using IPieceInfoPtr = std::shared_ptr<IPieceInfo>;
using IPieceInfoPtrList = std::vector<IPieceInfoPtr>;
