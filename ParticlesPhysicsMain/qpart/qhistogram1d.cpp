#include "qhistogram1d.h"

QHistogram1D::QHistogram1D( ActionType type, double maxValue, QWidget* parent )
: QBoxPainter {parent}, histogram1D { Locator::getHistogram1D(type) }, maxValue {maxValue}
{
    boxStyle.colors[BoxColors::BACKGROUND] = QColor(145, 215, 215);
    setAutoFillBackground(false);    

    boxStyle.values[BoxValues::MARGIN_TOP] = 30;

    init();
}

bool QHistogram1D::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        // to do some needed recalculations
        return true;
    }
    else return false;
}

void QHistogram1D::init()
{
    if( parentWidget() != nullptr )
    {
        int width = parentWidget()->width();
        int size = static_cast<int>(histogram1D->getBins().size());
        int marginAdjustment = (width-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT])) - size*((width-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT]))/size);

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

        barWidth = (static_cast<double>(width)-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT]))/static_cast<double>(size);
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
        double scaleFactor = (height()-(boxStyle.values[BoxValues::MARGIN_TOP]+boxStyle.values[BoxValues::MARGIN_BOTTOM]))/maxValue;

        painter.setPen(QPen(QColor(150, 50, 250)));
        painter.setBrush(QBrush(QColor(150, 50, 250)));

        for( const auto &bin : histogram1D->getBins() )
        {
            value = static_cast<int>(bin*scaleFactor);
            intensity = static_cast<int>(255.0*index/histogram1D->getBins().size());
            if( intensity>255 ) intensity = 255;
            painter.setBrush(QBrush(QColor(intensity, 0, 255-intensity)));
            painter.setPen(QPen(QColor(intensity, 0, 255-intensity)));
            painter.drawRect(static_cast<int>(boxStyle.values[BoxValues::MARGIN_LEFT]+index*barWidth),height()-value-boxStyle.values[BoxValues::MARGIN_BOTTOM]-2,static_cast<int>(barWidth)-1,value);

            ++index;
        }

        drawHistogramName();
        if( histogram1D->getMarkingBin()>=0 ) drawMarking();
    }

}

void QHistogram1D::drawHistogramName()
{
    auto label = QString::fromStdString(histogram1D->getLabel());
    painter.setPen(boxStyle.colors[BoxColors::LABEL]);
    painter.drawText(calculateCenterTextPosition(label,boxStyle.values[BoxValues::MARGIN_LEFT],width()-boxStyle.values[BoxValues::MARGIN_RIGHT]),boxStyle.values[BoxValues::MARGIN_TOP]-7,label);
}

void QHistogram1D::drawMarking()
{
    int posx = static_cast<int>(boxStyle.values[BoxValues::MARGIN_LEFT]+histogram1D->getMarkingBin()*barWidth+0.5*barWidth);
    QPen pen(QColor(10,155,55));
    pen.setWidth(2);
    painter.setPen(pen);
    paintTriangle( vect2D(posx-6,boxStyle.values[BoxValues::MARGIN_TOP]) , vect2D(posx,boxStyle.values[BoxValues::MARGIN_TOP]+8) , vect2D(posx+6,boxStyle.values[BoxValues::MARGIN_TOP]) , QColor(10,155,55)  );
    painter.drawLine(posx,boxStyle.values[BoxValues::MARGIN_TOP],posx,height()-boxStyle.values[BoxValues::MARGIN_BOTTOM]-1);
}
