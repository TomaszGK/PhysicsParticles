#include "qbardisplay.h"

QBarDisplay::QBarDisplay( int max, ptrBarDisplay ptr, QWidget* parentWidget )
: QBoxPainter { parentWidget }, barDisplay { std::move(ptr) }, max { max }
{
    background = QBrush(QColor(145, 215, 215));
    setAutoFillBackground(false);
    init();
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
                marginLeft += marginAdjustment/2;
                marginRight += marginAdjustment/2;
            }
            else
            {
                marginLeft += (marginAdjustment-1)/2;
                marginRight += ((marginAdjustment-1)/2+1);
            }
        }

        barWidth = (width-(marginLeft+marginRight))/size;
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

            painter.setBrush(QBrush(upper));
            painter.setPen(QPen(upper));
            percent =  static_cast<double>(barDisplay->getUpperBox(index))/max;
            value = static_cast<int>((geometry().height()/2.8)*percent);
            if( value>0 ) painter.drawRect(static_cast<int>(marginLeft+posx),height()/2-value,barWidth-1,value);
            painter.drawText(marginLeft+posx+getCenteredTextPosition(value),height()/2-value-5,QString::number(barDisplay->getUpperBox(index)));

            painter.setBrush(QBrush(lower));
            painter.setPen(QPen(lower));
            percent = static_cast<double>(barDisplay->getLowerBox(index))/max;
            value = static_cast<int>((geometry().height()/2.8)*percent);            
            if( value>0 ) painter.drawRect(marginLeft+posx,height()/2,barWidth-1,value);
            painter.drawText(marginLeft+posx+getCenteredTextPosition(value),height()/2+value+17,QString::number(barDisplay->getLowerBox(index)));
        }
    }
}

int QBarDisplay::getCenteredTextPosition( int number )
{
    QFontMetrics fm(parentWidget()->font());
    int pixelsWide = fm.width(QString::number(number));

    return (barWidth - pixelsWide)/2;
}
