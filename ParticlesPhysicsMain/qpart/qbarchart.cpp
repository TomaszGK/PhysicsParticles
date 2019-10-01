#include "qbarchart.h"

QBarChart::QBarChart( double max, std::pair<bool,bool> scalability, ptrBarChart ptr, QWidget* parent )
: QBoxPainter { parent }, barChart { std::move(ptr) }
{
    setMaxOY(max);    
    isScalableUp = scalability.first;
    isScalableDown = scalability.second;
    background = cBackground;
    setAutoFillBackground(false);
    marginTop = 30;
    marginRight = 30;
    buttonStyleSelected = "QPushButton {""background-color: %1"";font-size: 16px;font: bold}";
    buttonStyleUnselected = "QPushButton {""background-color: %1"";font-size: 16px;}";    

    init();
}

void QBarChart::init()
{    
    if( parentWidget() != nullptr )
    {
        int width = parentWidget()->width();
        int size = static_cast<int>(barChart->getBins().size());
        int marginAdjustment = (width-(marginLeft+marginRight)) - size*((width-(marginLeft+marginRight))/size);

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

        buttons[DataVisualization::BARS] = std::make_unique<QPushButton>("B",this);
        buttons[DataVisualization::BARS]->resize(25,25);
        buttons[DataVisualization::BARS]->move(width-marginRight+(marginRight-25)/2,marginBottom+15);
        buttons[DataVisualization::BARS]->setStyleSheet(buttonStyleSelected.arg(cButtonActive.name()));

        buttons[DataVisualization::POINTS] = std::make_unique<QPushButton>("P",this);
        buttons[DataVisualization::POINTS]->resize(25,25);
        buttons[DataVisualization::POINTS]->move(width-marginRight+(marginRight-25)/2,marginBottom+45);
        buttons[DataVisualization::POINTS]->setStyleSheet(buttonStyleUnselected.arg(cButton.name()));

        buttons[DataVisualization::LINES] = std::make_unique<QPushButton>("L",this);
        buttons[DataVisualization::LINES]->resize(25,25);
        buttons[DataVisualization::LINES]->move(width-marginRight+(marginRight-25)/2,marginBottom+75);
        buttons[DataVisualization::LINES]->setStyleSheet(buttonStyleUnselected.arg(cButton.name()));

        connect( buttons[DataVisualization::BARS].get()   , &QPushButton::clicked , this, &QBarChart::onButtonClick );
        connect( buttons[DataVisualization::POINTS].get() , &QPushButton::clicked , this, &QBarChart::onButtonClick );
        connect( buttons[DataVisualization::LINES].get()  , &QPushButton::clicked , this, &QBarChart::onButtonClick );

    }
}

int QBarChart::calculateLabelPosition()
{
    QFontMetrics fm(parentWidget()->font());
    int pixelsWide = fm.horizontalAdvance(barChart->getLabel().c_str());

    return marginLeft + (parentWidget()->width()-(marginLeft+marginRight)-pixelsWide)/2;
}

void QBarChart::paint()
{

    if( barChart != nullptr )
    {

        int value {0};
        int valueLast {0};
        int index {0};
        int intensity {0};

        paintAxes();

        if( (isScalableUp && maxValue<barChart->getMax()) || (isScalableDown && maxValue>2*barChart->getMax()) )
        {
            if( barChart->getMax()>0 ) maxValue = 1.5*barChart->getMax();
        }

        double scaleFactor = (height()-(marginLeft+marginRight))/maxValue;

        for( const auto &barChartBin : barChart->getBins() )
        {
            value = static_cast<int>(barChartBin*scaleFactor);
            intensity = static_cast<int>(255.0*(barChartBin)/maxValue);
            if( intensity>255 ) intensity = 255;
            painter.setBrush(QBrush(QColor(intensity, 0, 255-intensity)));
            painter.setPen(QPen(QColor(intensity, 0, 255-intensity)));

            if( dataVisulization == DataVisualization::BARS )
            {               
                painter.drawRect(marginLeft+index*barWidth,height()-value-marginBottom-2,barWidth-1,value);
            }
            else if( dataVisulization == DataVisualization::POINTS )
            {
                painter.drawEllipse(marginLeft+index*barWidth,height()-value-marginBottom-2-barWidth,barWidth,barWidth);
            }
            else if( dataVisulization == DataVisualization::LINES )
            {
                if(index>0) painter.drawLine(marginLeft+(index-1)*barWidth,height()-valueLast-marginBottom-2-barWidth,marginLeft+index*barWidth,height()-value-marginBottom-2-barWidth);
            }

            ++index;
            valueLast = value;
        }

        drawCurrentValue();
        drawChartName();
    }

}

void QBarChart::drawCurrentValue()
{
    painter.setPen(QPen(cValue));
    painter.drawText(marginLeft,marginTop-7,QString::number(100*barChart->getBins().back(),'f',2));
}

void QBarChart::drawChartName()
{
    painter.setPen(QPen(cValue));
    painter.drawText(calculateLabelPosition(),marginTop-7,QString::fromStdString(barChart->getLabel()));
}

void QBarChart::onButtonClick()
{
    auto clickedButton = qobject_cast<QPushButton*>( sender() );
    if( clickedButton != nullptr )
    {
      buttons[dataVisulization]->setStyleSheet(buttonStyleUnselected.arg(cButton.name()));

      if( clickedButton->text() == "B" ) dataVisulization = DataVisualization::BARS;
      else if( clickedButton->text() == "P" ) dataVisulization = DataVisualization::POINTS;
      else if( clickedButton->text() == "L" ) dataVisulization = DataVisualization::LINES;

      buttons[dataVisulization]->setStyleSheet(buttonStyleSelected.arg(cButtonActive.name()));
    }
}
