#pragma once

#include <QLabel>
#include <QEvent>

class QInteractiveLabel : public QLabel
{

    Q_OBJECT

public:

    QInteractiveLabel(QWidget* parent = nullptr);

    void setPicture(const QString& filename);

protected:

    void enterEvent(QEvent *ev) override;

    void leaveEvent(QEvent *ev) override;

};

