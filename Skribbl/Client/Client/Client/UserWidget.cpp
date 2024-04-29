#include "UserWidget.h"

UserWidget::UserWidget(const QString& user, QWidget* parent) :
    QWidget{ parent }, 
    m_user{ user }
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* imageLabel = new QLabel(this);
    QLabel* nameLabel = new QLabel(m_user, this);
    layout->addWidget(nameLabel);
}
