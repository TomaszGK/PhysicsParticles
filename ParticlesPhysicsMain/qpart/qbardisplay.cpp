#include "qbardisplay.h"
#include "particlesphysicsmanager.h"

QBarDisplay::QBarDisplay( ActionType type, QWidget* parentWidget )
: QBoxPainter { parentWidget } , barDisplay { ParticlesPhysicsManager::Locator::getBarDisplay(type) }
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

        for( int index=0 ; index<barDisplay->getSize() ; ++index )
        {
            auto upperValue = QString::number(barDisplay->getUpperBox(index));
            auto lowerValue = QString::number(barDisplay->getLowerBox(index));

            posx = index*barWidth;

            painter.setBrush(QBrush(boxStyle.cUpper));
            painter.setPen(QPen(boxStyle.cUpper));
            percent =  static_cast<double>(barDisplay->getUpperBox(index))/barDisplay->getUpperBoxSum();
            value = static_cast<int>((height()/2.8)*percent);
            if( value>0 ) painter.drawRect(boxStyle.marginLeft+posx,height()/2-value,barWidth-1,value);
            painter.drawText(calculateCenterTextPosition(upperValue,boxStyle.marginLeft+posx,boxStyle.marginLeft+barWidth+posx),height()/2-value-5,upperValue);

            painter.setBrush(QBrush(boxStyle.cLower));
            painter.setPen(QPen(boxStyle.cLower));
            percent = static_cast<double>(barDisplay->getLowerBox(index))/barDisplay->getLowerBoxSum();
            value = static_cast<int>((height()/2.8)*percent);
            if( value>0 ) painter.drawRect(boxStyle.marginLeft+posx,height()/2,barWidth-1,value);
            painter.drawText(calculateCenterTextPosition(lowerValue,boxStyle.marginLeft+posx,boxStyle.marginLeft+barWidth+posx),height()/2+value+17,lowerValue);
        }
    }
}
