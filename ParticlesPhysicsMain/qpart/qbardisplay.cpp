#include "qbardisplay.h"

QBarDisplay::QBarDisplay( cptrBarDisplay barDisplay_ , QWidget* parentWidget )
: QBoxPainter { parentWidget } , barDisplay { barDisplay_ }
{    
    setAutoFillBackground(false);
    init();
}

bool QBarDisplay::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        // to do some needed recalculations
        return true;
    }
    else return false;
}

void QBarDisplay::init()
{
    if( parentWidget() != nullptr )
    {        
        int width = parentWidget()->width();
        int size = static_cast<int>(barDisplay->getSize());
        int marginAdjustment = width - size*(width/size);

        if( marginAdjustment>0 )
        {
            if( marginAdjustment % 2 == 0 )
            {
                boxStyle.values[BoxValues::MARGIN_LEFT] += marginAdjustment/2;
                boxStyle.values[BoxValues::MARGIN_RIGHT] += marginAdjustment/2;
            }
            else
            {
                boxStyle.values[BoxValues::MARGIN_LEFT] += (marginAdjustment-1)/2;
                boxStyle.values[BoxValues::MARGIN_RIGHT] += ((marginAdjustment-1)/2+1);
            }
        }

        barWidth = (width-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT]))/size;
    }
}

void QBarDisplay::paint()
{
    if( barDisplay != nullptr )
    {      
        int value {0};
        int posx {0};
        double percent {0};

        painter.setPen(QPen(QColor(120, 120, 120)));
        painter.drawLine(0,geometry().height()/2,geometry().width(),geometry().height()/2);

        for( int index=0 ; index<barDisplay->getSize() ; ++index )
        {
            auto upperValue = QString::number(barDisplay->getUpperBox(index));
            auto lowerValue = QString::number(barDisplay->getLowerBox(index));

            posx = index*barWidth;

            painter.setBrush(QColor(boxStyle.colors[BoxColors::UPPER]));
            painter.setPen(boxStyle.colors[BoxColors::UPPER]);
            percent =  static_cast<double>(barDisplay->getUpperBox(index))/barDisplay->getUpperBoxSum();
            value = static_cast<int>((height()/2.8)*percent);
            if( value>0 ) painter.drawRect(boxStyle.values[BoxValues::MARGIN_LEFT]+posx,height()/2-value,barWidth-1,value);
            painter.drawText(calculateCenterTextPosition(upperValue,boxStyle.values[BoxValues::MARGIN_LEFT]+posx,boxStyle.values[BoxValues::MARGIN_LEFT]+barWidth+posx),height()/2-value-5,upperValue);

            painter.setBrush(QColor(boxStyle.colors[BoxColors::LOWER]));
            painter.setPen(boxStyle.colors[BoxColors::LOWER]);
            percent = static_cast<double>(barDisplay->getLowerBox(index))/barDisplay->getLowerBoxSum();
            value = static_cast<int>((height()/2.8)*percent);
            if( value>0 ) painter.drawRect(boxStyle.values[BoxValues::MARGIN_LEFT]+posx,height()/2,barWidth-1,value);
            painter.drawText(calculateCenterTextPosition(lowerValue,boxStyle.values[BoxValues::MARGIN_LEFT]+posx,boxStyle.values[BoxValues::MARGIN_LEFT]+barWidth+posx),height()/2+value+17,lowerValue);
        }
    }
}
