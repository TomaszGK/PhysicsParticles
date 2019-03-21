#include "qinfodisplay.h"


QInfoDisplay::QInfoDisplay( QWidget *parentWidget )
: QBoxPainter { parentWidget }
{
    background = QBrush(QColor(145, 215, 215));
    setAutoFillBackground(false);    
    marginLeft = marginRight = marginTop = marginBottom = 5;
    init();
}

void QInfoDisplay::init()
{

}

void QInfoDisplay::setDisplay( PlaneSide side, int _value )
{   
   value[side] = _value;
   textWidth[side] = getStringWidthInPixels(QString::number(value[side])+" %");
}

void QInfoDisplay::paint()
{
    painter.setBrush(QBrush(QColor(130, 115, 215)));
    painter.setPen(QPen(QColor(130, 115, 215)));

    //painter.drawText(marginLeft+50,height()/2,"Temperature");
    painter.drawText(marginLeft,height()/2,QString::number(value[PlaneSide::LEFT])+" %");
    painter.drawText(width()-marginRight-textWidth[PlaneSide::RIGHT],height()/2,QString::number(value[PlaneSide::RIGHT])+" %");
    painter.drawText(marginLeft + (width()-(marginLeft+marginRight)-textWidth[PlaneSide::UP])/2,marginTop+20,QString::number(value[PlaneSide::UP])+" %");
    painter.drawText(marginLeft + (width()-(marginLeft+marginRight)-textWidth[PlaneSide::DOWN])/2,height()-marginBottom-5,QString::number(value[PlaneSide::DOWN])+" %");
}
