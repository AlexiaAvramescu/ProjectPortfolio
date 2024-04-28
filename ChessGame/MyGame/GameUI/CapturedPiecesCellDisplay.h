#pragma once

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include "Enums.h"


class CapturedPiecesCellDisplay : public QLabel {

	Q_OBJECT

public:
	CapturedPiecesCellDisplay(PieceType pieceType = PieceType::none, PieceColor pieceColor = PieceColor::none, QWidget* parent = nullptr);
	~CapturedPiecesCellDisplay() = default;

	void Update(PieceType type, PieceColor color);
};

