#include "qinfodisplay.h"


QInfoDisplay::QInfoDisplay( QWidget *parentWidget )
: QBoxPainter { parentWidget }
{
    boxStyle.colors[BoxColors::BACKGROUND] = QColor(145, 215, 215);
    setAutoFillBackground(false);    
    boxStyle.values["marginLeft"] = boxStyle.values["marginRight"] = boxStyle.values["marginTop"] = boxStyle.values["marginBottom"] = 5;
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

    xcap = static_cast<int>((width()-boxStyle.values["marginLeft"]-boxStyle.values["marginRight"])*value[PlanePart::UP]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values["marginTop"]-boxStyle.values["marginBottom"])*value[PlanePart::UP]*0.01*0.5);

    poly << QPoint(boxStyle.values["marginLeft"],boxStyle.values["marginTop"])
         << QPoint(boxStyle.values["marginLeft"]+xcap, boxStyle.values["marginTop"]+ycap)
         << QPoint(width()-boxStyle.values["marginRight"]-xcap, boxStyle.values["marginTop"]+ycap)
         << QPoint(width()-boxStyle.values["marginRight"], boxStyle.values["marginTop"]);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.values["marginLeft"]-boxStyle.values["marginRight"])*value[PlanePart::DOWN]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values["marginTop"]-boxStyle.values["marginBottom"])*value[PlanePart::DOWN]*0.01*0.5);

    poly << QPoint(boxStyle.values["marginLeft"],height()-boxStyle.values["marginTop"])
         << QPoint(boxStyle.values["marginLeft"]+xcap, height()-boxStyle.values["marginTop"]-ycap)
         << QPoint(width()-boxStyle.values["marginRight"]-xcap, height()-boxStyle.values["marginTop"]-ycap)
         << QPoint(width()-boxStyle.values["marginRight"], height()-boxStyle.values["marginBottom"]);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.values["marginLeft"]-boxStyle.values["marginRight"])*value[PlanePart::LEFT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values["marginTop"]-boxStyle.values["marginBottom"])*value[PlanePart::LEFT]*0.01*0.5);

    poly << QPoint(boxStyle.values["marginLeft"],boxStyle.values["marginTop"])
         << QPoint(boxStyle.values["marginLeft"]+xcap, boxStyle.values["marginTop"]+ycap)
         << QPoint(boxStyle.values["marginLeft"]+xcap, height()-boxStyle.values["marginTop"]-ycap)
         << QPoint(boxStyle.values["marginLeft"], height()-boxStyle.values["marginBottom"]);

    painter.drawPolygon(poly);
    poly.clear();

    xcap = static_cast<int>((width()-boxStyle.values["marginLeft"]-boxStyle.values["marginRight"])*value[PlanePart::RIGHT]*0.01*0.5);
    ycap = static_cast<int>((height()-boxStyle.values["marginTop"]-boxStyle.values["marginBottom"])*value[PlanePart::RIGHT]*0.01*0.5);

    poly << QPoint(width()-boxStyle.values["marginRight"],boxStyle.values["marginTop"])
         << QPoint(width()-boxStyle.values["marginRight"]-xcap, boxStyle.values["marginTop"]+ycap)
         << QPoint(width()-boxStyle.values["marginRight"]-xcap, height()-boxStyle.values["marginTop"]-ycap)
         << QPoint(width()-boxStyle.values["marginRight"], height()-boxStyle.values["marginBottom"]);

    painter.drawPolygon(poly);
    poly.clear();

    painter.setPen(boxStyle.colors[BoxColors::LABEL]);
    painter.drawText(2*boxStyle.values["marginLeft"],height()/2+5,QString::number(value[PlanePart::LEFT])+" %");
    painter.drawText(width()-2*boxStyle.values["marginRight"]-textWidth[PlanePart::RIGHT],height()/2+5,QString::number(value[PlanePart::RIGHT])+" %");
    painter.drawText(boxStyle.values["marginLeft"] + (width()-(boxStyle.values["marginLeft"]+boxStyle.values["marginRight"])-textWidth[PlanePart::UP])/2,2*boxStyle.values["marginTop"]+20,QString::number(value[PlanePart::UP])+" %");
    painter.drawText(boxStyle.values["marginLeft"] + (width()-(boxStyle.values["marginLeft"]+boxStyle.values["marginRight"])-textWidth[PlanePart::DOWN])/2,height()-2*boxStyle.values["marginBottom"]-5,QString::number(value[PlanePart::DOWN])+" %");
}
