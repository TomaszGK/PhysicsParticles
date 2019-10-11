#include "qbardisplay.h"

QBarDisplay::QBarDisplay( int max, ptrBarDisplay ptr, QWidget* parentWidget )
: QBoxPainter { parentWidget }, barDisplay { std::move(ptr) }, max { max }
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
                boxStyle.marginLeft += marginAdjustment/2;
                boxStyle.marginRight += marginAdjustment/2;
            }
            else
            {
                boxStyle.marginLeft += (marginAdjustment-1)/2;
                boxStyle.marginRight += ((marginAdjustment-1)/2+1);
            }
        }

        barWidth = (width-(boxStyle.marginLeft+boxStyle.marginRight))/size;
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

        for( size_t index=0 ; index<barDisplay->getSize() ; ++index )
        {
            posx = static_cast<int>(index)*barWidth;

            painter.setBrush(QBrush(boxStyle.cUpper));
            painter.setPen(QPen(boxStyle.cUpper));
            percent =  static_cast<double>(barDisplay->getUpperBox(index))/max;
            value = static_cast<int>((geometry().height()/2.8)*percent);
            if( value>0 ) painter.drawRect(static_cast<int>(boxStyle.marginLeft+posx),height()/2-value,barWidth-1,value);
            painter.drawText(boxStyle.marginLeft+posx+getCenteredTextPosition(value),height()/2-value-5,QString::number(barDisplay->getUpperBox(index)));

            painter.setBrush(QBrush(boxStyle.cLower));
            painter.setPen(QPen(boxStyle.cLower));
            percent = static_cast<double>(barDisplay->getLowerBox(index))/max;
            value = static_cast<int>((geometry().height()/2.8)*percent);            
            if( value>0 ) painter.drawRect(boxStyle.marginLeft+posx,height()/2,barWidth-1,value);
            painter.drawText(boxStyle.marginLeft+posx+getCenteredTextPosition(value),height()/2+value+17,QString::number(barDisplay->getLowerBox(index)));
        }
    }
}

int QBarDisplay::getCenteredTextPosition( int number )
{
    QFontMetrics fm(parentWidget()->font());
    int pixelsWide = fm.horizontalAdvance(QString::number(number));

    return (barWidth - pixelsWide)/2;
}
