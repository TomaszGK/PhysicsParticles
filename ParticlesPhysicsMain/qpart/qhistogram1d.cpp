#include "qhistogram1d.h"

QHistogram1D::QHistogram1D( double max, ptrHistogram1D ptr, QWidget* parent )
: QBoxPainter { parent }, histogram1D { std::move(ptr) }
{
    setMaxOY(max);

    background = QBrush(QColor(145, 215, 215));
    setAutoFillBackground(false);    
    background = cBackground;
    marginTop = 30;

    init();
}

void QHistogram1D::init()
{
    if( parentWidget() != nullptr )
    {
        int width = parentWidget()->width();
        int size = static_cast<int>(histogram1D->getBins().size());
        int marginAdjustment = (width-(marginLeft+marginRight)) - size*((width-(marginLeft+marginRight))/size);

        QFontMetrics fm(parentWidget()->font());
        int pixelsWide = fm.width(histogram1D->getLabel().c_str());

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

        barWidth = (static_cast<double>(width)-(marginLeft+marginRight))/static_cast<double>(size);
        labelPosition = marginLeft + (width-(marginLeft+marginRight)-pixelsWide)/2;
    }
}

void QHistogram1D::paint()
{

    if( histogram1D != nullptr )
    {
        int value {0};
        size_t index {0};
        int intensity {0};

        paintAxes();

        maxValue = 1.5*histogram1D->getMax();
        double scaleFactor = (height()-(marginTop+marginBottom))/maxValue;

        painter.setPen(QPen(QColor(150, 50, 250)));
        painter.setBrush(QBrush(QColor(150, 50, 250)));

        for( const auto &bin : histogram1D->getBins() )
        {
            value = static_cast<int>(bin*scaleFactor);
            intensity = static_cast<int>(255.0*index/histogram1D->getBins().size());
            if( intensity>255 ) intensity = 255;
            painter.setBrush(QBrush(QColor(intensity, 0, 255-intensity)));
            painter.setPen(QPen(QColor(intensity, 0, 255-intensity)));
            painter.drawRect(static_cast<int>(marginLeft+index*barWidth),height()-value-marginBottom-2,static_cast<int>(barWidth)-1,value);

            ++index;
        }

        drawHistogramName();
        if( histogram1D->getMarkingBin()>=0 ) drawMarking();
    }

}

void QHistogram1D::drawHistogramName()
{
    painter.setPen(QPen(cValue));
    painter.drawText(labelPosition,marginTop-7, QString::fromStdString(histogram1D->getLabel()));
}

void QHistogram1D::drawMarking()
{
    int posx = static_cast<int>(marginLeft+histogram1D->getMarkingBin()*barWidth+0.5*barWidth);
    QPen pen(QColor(10,155,55));
    pen.setWidth(2);
    painter.setPen(pen);
    paintTriangle( vect2D(posx-6,marginTop) , vect2D(posx,marginTop+8) , vect2D(posx+6,marginTop) , QColor(10,155,55)  );
    painter.drawLine(posx,marginTop,posx,height()-marginBottom-1);
}
