//#pragma once
//#include <qlistwidget.h>
//class SynchronizedListWidget :
//    public QListWidget
//{
//	Q_OBJECT
//
//public:
//	SynchronizedListWidget(QWidget* parent = nullptr) : QListWidget(parent) {}
//
//	void setSynchronizedLists(QList<SynchronizedListWidget*>& lists) {
//		synchronizedLists = &lists;
//	}
//
//protected:
//	void scrollContentsBy(int dx, int dy) override {
//		QListWidget::scrollContentsBy(dx, dy);
//
//		// Synchronize scrolling with other lists
//		if (synchronizedLists) {
//			for (SynchronizedListWidget* list : *synchronizedLists) {
//				if (list != this) {
//					list->verticalScrollBar()->setValue(verticalScrollBar()->value());
//				}
//			}
//		}
//	}
//
//	void addItem(const QString& label) {
//		QListWidget::addItem(label);
//
//		// Add the same item to other lists
//		if (synchronizedLists) {
//			for (SynchronizedListWidget* list : *synchronizedLists) {
//				if (list != this) {
//					list->addItem(label);
//				}
//			}
//		}
//	}
//
//private:
//	QList<SynchronizedListWidget*>* synchronizedLists = nullptr;
//};
//
