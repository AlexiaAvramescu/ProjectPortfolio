#pragma once
#include <QPushButton>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QPushButton>
#include "Enums.h"

#include "Position.h"

class GridButton : public QPushButton
{
	Q_OBJECT

public:
	GridButton(const Position& boardPosition, PieceType pieceType = PieceType::none, PieceColor pieceColor = PieceColor::none, QWidget* parent = nullptr);
	~GridButton() = default;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void updatePiece();
	void updateBackgroundColor();

	void setPiece(std::pair<PieceType, PieceColor> newPiece);
	void setHighlighted(bool highlighted);
	void setSelected(bool selected);

	PieceColor GetPieceColor() const;
	PieceType GetPieceType() const;

protected:
	void paintEvent(QPaintEvent* event) override;

signals:
	void Clicked(const Position& position);

private:
	Position m_Position;
	PieceType m_PieceType;
	PieceColor m_PieceColor;
	bool m_Highlighted;
	bool m_Selected;
};
