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

bool QInfoDisplay::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        // to do some needed recalculations
        return true;
    }
    else return false;
}

void QInfoDisplay::paint()
{

    QPolygon poly;
    int xcap {0} , ycap {0};

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlaneSide::UP]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlaneSide::UP]*0.01*0.5);

    painter.setBrush(QBrush(QColor(113, 161, 215)));
    poly << QPoint(boxStyle.marginLeft,boxStyle.marginTop)
         << QPoint(boxStyle.marginLeft+xcap, boxStyle.marginTop+ycap)
         << QPoint(width()-boxStyle.marginRight-xcap, boxStyle.marginTop+ycap)
         << QPoint(width()-boxStyle.marginRight, boxStyle.marginTop);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlaneSide::DOWN]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlaneSide::DOWN]*0.01*0.5);

    painter.setBrush(QBrush(QColor(113, 161, 215)));
    poly << QPoint(boxStyle.marginLeft,height()-boxStyle.marginTop)
         << QPoint(boxStyle.marginLeft+xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(width()-boxStyle.marginRight-xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(width()-boxStyle.marginRight, height()-boxStyle.marginBottom);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlaneSide::LEFT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlaneSide::LEFT]*0.01*0.5);

    painter.setBrush(QBrush(QColor(113, 161, 215)));
    poly << QPoint(boxStyle.marginLeft,boxStyle.marginTop)
         << QPoint(boxStyle.marginLeft+xcap, boxStyle.marginTop+ycap)
         << QPoint(boxStyle.marginLeft+xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(boxStyle.marginLeft, height()-boxStyle.marginBottom);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlaneSide::RIGHT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlaneSide::RIGHT]*0.01*0.5);

    painter.setBrush(QBrush(QColor(113, 161, 215)));
    poly << QPoint(width()-boxStyle.marginRight,boxStyle.marginTop)
         << QPoint(width()-boxStyle.marginRight-xcap, boxStyle.marginTop+ycap)
         << QPoint(width()-boxStyle.marginRight-xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(width()-boxStyle.marginRight, height()-boxStyle.marginBottom);

    painter.drawPolygon(poly);
    poly.clear();

    painter.setBrush(QBrush(QColor(130, 115, 215)));
    painter.setPen(QPen(boxStyle.cLabelColor));
    painter.drawText(2*boxStyle.marginLeft,height()/2+5,QString::number(value[PlaneSide::LEFT])+" %");
    painter.drawText(width()-2*boxStyle.marginRight-textWidth[PlaneSide::RIGHT],height()/2+5,QString::number(value[PlaneSide::RIGHT])+" %");
    painter.drawText(boxStyle.marginLeft + (width()-(boxStyle.marginLeft+boxStyle.marginRight)-textWidth[PlaneSide::UP])/2,2*boxStyle.marginTop+20,QString::number(value[PlaneSide::UP])+" %");
    painter.drawText(boxStyle.marginLeft + (width()-(boxStyle.marginLeft+boxStyle.marginRight)-textWidth[PlaneSide::DOWN])/2,height()-2*boxStyle.marginBottom-5,QString::number(value[PlaneSide::DOWN])+" %");
}
