#pragma once

#include "IPieceInfo.h"

class PieceInfo : public IPieceInfo
{
public:
	PieceInfo(EPieceType type, EPieceColor color);

	EPieceColor GetColor() const override;
	EPieceType GetType() const override;

private:
	EPieceType m_type;
	EPieceColor m_color;
};
