#include "DrawingAreaWidget.h"
#include <QPalette>

DrawingAreaWidget::DrawingAreaWidget(QWidget* parent)
	: QWidget{ parent }
	, m_isMousePressed{ false }
	, m_isPlayerDrawing{ false }
{
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
}

void DrawingAreaWidget::UndoLastLine()
{
	if (m_isPlayerDrawing && !m_drawing.empty()) {
		m_drawing.pop_back();
		update();
	}
}

void DrawingAreaWidget::ResetDrawing()
{
	if (m_isPlayerDrawing) {
		m_drawing.clear();
		update();
	}
}

const DrawingConfig& DrawingAreaWidget::GetDrawing() const
{
	return m_drawing;
}

void DrawingAreaWidget::SetDrawing(const DrawingConfig& drawing)
{
	m_drawing = drawing;
	update();
}

void DrawingAreaWidget::SetIsPlayerDrawing(bool val)
{
	if (m_isPlayerDrawing != val) {
		ResetDrawing();
		m_isPlayerDrawing = val;
	}
}

void DrawingAreaWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton && m_isPlayerDrawing) {
		std::vector<QPoint> newLine;
		newLine.emplace_back(event->pos());
		m_drawing.emplace_back(std::move(newLine));
		m_isMousePressed = true;
		update();
	}
}

void DrawingAreaWidget::mouseMoveEvent(QMouseEvent* event) {
	if ((event->buttons() & Qt::LeftButton) && m_isMousePressed && m_isPlayerDrawing) {
		m_drawing.back().push_back(event->pos());
		update();
	}
}

void DrawingAreaWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton && m_isMousePressed && m_isPlayerDrawing) {
		m_drawing.back().push_back(event->pos());
		m_isMousePressed = false;
	}
}

void DrawingAreaWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);

	QPainter painter(this);

	if (!m_drawing.empty()) {
		for (const auto& line : m_drawing)
		{
			painter.setPen(Qt::black);
			painter.drawPolyline(line.data(), line.size());
		}
	}
}