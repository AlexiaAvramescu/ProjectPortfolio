#pragma once

#include <QPainter>
#include <QMouseEvent>
#include <QWidget>

using DrawingConfig = std::vector<std::vector<QPoint>>;

class DrawingAreaWidget : public QWidget {
	Q_OBJECT

public:
	DrawingAreaWidget(QWidget* parent = nullptr);

	void UndoLastLine();
	void ResetDrawing();

	const DrawingConfig& GetDrawing() const;
	void SetDrawing(const DrawingConfig& drawing);

	void SetIsPlayerDrawing(bool val);

private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

private:
	DrawingConfig m_drawing;
	bool m_isMousePressed;
	bool m_isPlayerDrawing;
};

