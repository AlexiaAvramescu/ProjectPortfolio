#include "CapturedPiecesCellDisplay.h"


CapturedPiecesCellDisplay::CapturedPiecesCellDisplay(PieceType pieceType /*= PieceType::none*/, PieceColor pieceColor /*= PieceColor::none*/, QWidget* parent /*= nullptr*/)
	: QLabel(parent)
{
	//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	Update(pieceType, pieceColor);
	setStyleSheet("background-color: #D2E7B8; border: none;");
}

void CapturedPiecesCellDisplay::Update(PieceType type, PieceColor color)
{
	std::string imagePath = "res/";

	switch (color)
	{
	case PieceColor::none:
		imagePath += "empty";
		break;
	case PieceColor::black:
		imagePath += "b";
		break;
	case PieceColor::white:
		imagePath += "w";
		break;
	}

	switch (type)
	{
	case PieceType::none:
		break;
	case PieceType::king:
		imagePath += "k";
		break;
	case PieceType::rook:
		imagePath += "r";
		break;
	case PieceType::bishop:
		imagePath += "b";
		break;
	case PieceType::queen:
		imagePath += "q";
		break;
	case PieceType::knight:
		imagePath += "h";
		break;
	case PieceType::pawn:
		imagePath += "p";
		break;
	default:
		break;
	}

	imagePath += ".png";

	QPixmap pixmap(QString::fromStdString(imagePath));
	QPixmap ceva = 	pixmap.scaled(60, 50);
	setPixmap(ceva);
}
