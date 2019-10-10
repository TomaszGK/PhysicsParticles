#include "qinfodisplay.h"


QInfoDisplay::QInfoDisplay( QWidget *parentWidget )
: QBoxPainter { parentWidget }
{
    boxStyle.cBackground = QColor(145, 215, 215);
    setAutoFillBackground(false);    
    boxStyle.marginLeft = boxStyle.marginRight = boxStyle.marginTop = boxStyle.marginBottom = 5;
}

void QInfoDisplay::setDisplay( PlaneSide side, int newValue )
{   
   value[side] = newValue;
   textWidth[side] = getStringWidthInPixels(QString::number(value[side])+" %");
}

bool QInfoDisplay::loadStyle(BoxStyles style)
{
    return false;
}

void QInfoDisplay::paint()
{
    painter.setBrush(QBrush(QColor(130, 115, 215)));
    painter.setPen(QPen(QColor(130, 115, 215)));

    //painter.drawText(marginLeft+50,height()/2,"Temperature");
    painter.drawText(boxStyle.marginLeft,height()/2,QString::number(value[PlaneSide::LEFT])+" %");
    painter.drawText(width()-boxStyle.marginRight-textWidth[PlaneSide::RIGHT],height()/2,QString::number(value[PlaneSide::RIGHT])+" %");
    painter.drawText(boxStyle.marginLeft + (width()-(boxStyle.marginLeft+boxStyle.marginRight)-textWidth[PlaneSide::UP])/2,boxStyle.marginTop+20,QString::number(value[PlaneSide::UP])+" %");
    painter.drawText(boxStyle.marginLeft + (width()-(boxStyle.marginLeft+boxStyle.marginRight)-textWidth[PlaneSide::DOWN])/2,height()-boxStyle.marginBottom-5,QString::number(value[PlaneSide::DOWN])+" %");
}
