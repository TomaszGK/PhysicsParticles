#include "qinfodisplay.h"


QInfoDisplay::QInfoDisplay( QWidget *parentWidget )
: QBoxPainter { parentWidget }
{
    boxStyle.colors[BoxColors::BACKGROUND] = QColor(145, 215, 215);
    setAutoFillBackground(false);    
    boxStyle.values[BoxValues::MARGIN_LEFT] = boxStyle.values[BoxValues::MARGIN_RIGHT] = boxStyle.values[BoxValues::MARGIN_TOP] = boxStyle.values[BoxValues::MARGIN_BOTTOM] = 5;
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

    xcap = static_cast<int>((width()-boxStyle.values[BoxValues::MARGIN_LEFT]-boxStyle.values[BoxValues::MARGIN_RIGHT])*value[PlanePart::UP]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values[BoxValues::MARGIN_TOP]-boxStyle.values[BoxValues::MARGIN_BOTTOM])*value[PlanePart::UP]*0.01*0.5);

    poly << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT],boxStyle.values[BoxValues::MARGIN_TOP])
         << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT]+xcap, boxStyle.values[BoxValues::MARGIN_TOP]+ycap)
         << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT]-xcap, boxStyle.values[BoxValues::MARGIN_TOP]+ycap)
         << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT], boxStyle.values[BoxValues::MARGIN_TOP]);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.values[BoxValues::MARGIN_LEFT]-boxStyle.values[BoxValues::MARGIN_RIGHT])*value[PlanePart::DOWN]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values[BoxValues::MARGIN_TOP]-boxStyle.values[BoxValues::MARGIN_BOTTOM])*value[PlanePart::DOWN]*0.01*0.5);

    poly << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT],height()-boxStyle.values[BoxValues::MARGIN_TOP])
         << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT]+xcap, height()-boxStyle.values[BoxValues::MARGIN_TOP]-ycap)
         << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT]-xcap, height()-boxStyle.values[BoxValues::MARGIN_TOP]-ycap)
         << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT], height()-boxStyle.values[BoxValues::MARGIN_BOTTOM]);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.values[BoxValues::MARGIN_LEFT]-boxStyle.values[BoxValues::MARGIN_RIGHT])*value[PlanePart::LEFT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values[BoxValues::MARGIN_TOP]-boxStyle.values[BoxValues::MARGIN_BOTTOM])*value[PlanePart::LEFT]*0.01*0.5);

    poly << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT],boxStyle.values[BoxValues::MARGIN_TOP])
         << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT]+xcap, boxStyle.values[BoxValues::MARGIN_TOP]+ycap)
         << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT]+xcap, height()-boxStyle.values[BoxValues::MARGIN_TOP]-ycap)
         << QPoint(boxStyle.values[BoxValues::MARGIN_LEFT], height()-boxStyle.values[BoxValues::MARGIN_BOTTOM]);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.values[BoxValues::MARGIN_LEFT]-boxStyle.values[BoxValues::MARGIN_RIGHT])*value[PlanePart::RIGHT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values[BoxValues::MARGIN_TOP]-boxStyle.values[BoxValues::MARGIN_BOTTOM])*value[PlanePart::RIGHT]*0.01*0.5);

    poly << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT],boxStyle.values[BoxValues::MARGIN_TOP])
         << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT]-xcap, boxStyle.values[BoxValues::MARGIN_TOP]+ycap)
         << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT]-xcap, height()-boxStyle.values[BoxValues::MARGIN_TOP]-ycap)
         << QPoint(width()-boxStyle.values[BoxValues::MARGIN_RIGHT], height()-boxStyle.values[BoxValues::MARGIN_BOTTOM]);

    painter.drawPolygon(poly);
    poly.clear();

    painter.setPen(boxStyle.colors[BoxColors::LABEL]);
    painter.drawText(2*boxStyle.values[BoxValues::MARGIN_LEFT],height()/2+5,QString::number(value[PlanePart::LEFT])+" %");
    painter.drawText(width()-2*boxStyle.values[BoxValues::MARGIN_RIGHT]-textWidth[PlanePart::RIGHT],height()/2+5,QString::number(value[PlanePart::RIGHT])+" %");
    painter.drawText(boxStyle.values[BoxValues::MARGIN_LEFT] + (width()-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT])-textWidth[PlanePart::UP])/2,2*boxStyle.values[BoxValues::MARGIN_TOP]+20,QString::number(value[PlanePart::UP])+" %");
    painter.drawText(boxStyle.values[BoxValues::MARGIN_LEFT] + (width()-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT])-textWidth[PlanePart::DOWN])/2,height()-2*boxStyle.values[BoxValues::MARGIN_BOTTOM]-5,QString::number(value[PlanePart::DOWN])+" %");
}
