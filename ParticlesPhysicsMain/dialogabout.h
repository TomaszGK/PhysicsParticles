#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <memory>
#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include "qtextanimation.h"
#include "qinteractivelabel.h"

/** @file
 * @brief Class @ref DialogAbout
 */

/**
 * @class DialogAbout
 * @brief Info dialog window QT form definition.
 *
 * @author Tomasz Gburek
 * @date 2019
 */

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT

public slots:

    void update();

public:

    explicit DialogAbout(QWidget *parent = nullptr);
    ~DialogAbout();

    void setFadeAnimation( int duration );

private:

    Ui::DialogAbout *ui;
    QTimer updateTimer;
    std::unique_ptr<QPropertyAnimation> propertyAnimation {nullptr};

    std::map<QString,std::unique_ptr<QTextAnimation>> animatedText;
    std::map<QString,QLabel*> labels;

    QInteractiveLabel labelPicture;

};

#endif // DIALOGABOUT_H
