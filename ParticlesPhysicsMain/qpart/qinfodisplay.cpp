#include "qinfodisplay.h"


QInfoDisplay::QInfoDisplay( QWidget *parentWidget )
: QBoxPainter { parentWidget }
{
    boxStyle.cBackground = QColor(145, 215, 215);
    setAutoFillBackground(false);    
    boxStyle.marginLeft = boxStyle.marginRight = boxStyle.marginTop = boxStyle.marginBottom = 5;
}

void QInfoDisplay::setDisplay( PlanePart side, int newValue )
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

    painter.setPen(QColor(90, 141, 185));
    painter.setBrush(QColor(113, 161, 215));

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlanePart::UP]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlanePart::UP]*0.01*0.5);

    poly << QPoint(boxStyle.marginLeft,boxStyle.marginTop)
         << QPoint(boxStyle.marginLeft+xcap, boxStyle.marginTop+ycap)
         << QPoint(width()-boxStyle.marginRight-xcap, boxStyle.marginTop+ycap)
         << QPoint(width()-boxStyle.marginRight, boxStyle.marginTop);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlanePart::DOWN]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlanePart::DOWN]*0.01*0.5);

    poly << QPoint(boxStyle.marginLeft,height()-boxStyle.marginTop)
         << QPoint(boxStyle.marginLeft+xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(width()-boxStyle.marginRight-xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(width()-boxStyle.marginRight, height()-boxStyle.marginBottom);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlanePart::LEFT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlanePart::LEFT]*0.01*0.5);

    poly << QPoint(boxStyle.marginLeft,boxStyle.marginTop)
         << QPoint(boxStyle.marginLeft+xcap, boxStyle.marginTop+ycap)
         << QPoint(boxStyle.marginLeft+xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(boxStyle.marginLeft, height()-boxStyle.marginBottom);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.marginLeft-boxStyle.marginRight)*value[PlanePart::RIGHT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.marginTop-boxStyle.marginBottom)*value[PlanePart::RIGHT]*0.01*0.5);

    poly << QPoint(width()-boxStyle.marginRight,boxStyle.marginTop)
         << QPoint(width()-boxStyle.marginRight-xcap, boxStyle.marginTop+ycap)
         << QPoint(width()-boxStyle.marginRight-xcap, height()-boxStyle.marginTop-ycap)
         << QPoint(width()-boxStyle.marginRight, height()-boxStyle.marginBottom);

    painter.drawPolygon(poly);
    poly.clear();

    painter.setPen(boxStyle.cLabelColor);
    painter.drawText(2*boxStyle.marginLeft,height()/2+5,QString::number(value[PlanePart::LEFT])+" %");
    painter.drawText(width()-2*boxStyle.marginRight-textWidth[PlanePart::RIGHT],height()/2+5,QString::number(value[PlanePart::RIGHT])+" %");
    painter.drawText(boxStyle.marginLeft + (width()-(boxStyle.marginLeft+boxStyle.marginRight)-textWidth[PlanePart::UP])/2,2*boxStyle.marginTop+20,QString::number(value[PlanePart::UP])+" %");
    painter.drawText(boxStyle.marginLeft + (width()-(boxStyle.marginLeft+boxStyle.marginRight)-textWidth[PlanePart::DOWN])/2,height()-2*boxStyle.marginBottom-5,QString::number(value[PlanePart::DOWN])+" %");
}
