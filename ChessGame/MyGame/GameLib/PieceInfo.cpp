
#include "PieceInfo.h"

PieceInfo::PieceInfo(EPieceType type, EPieceColor color)
	: m_type(type)
	, m_color(color)
{

}

EPieceColor PieceInfo::GetColor() const
{
	return m_color;
}

EPieceType PieceInfo::GetType() const
{
	return m_type;
}