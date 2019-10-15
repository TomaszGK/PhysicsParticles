#include "qbarchart.h"

QBarChart::QBarChart( double max,  ptrBarChart ptr, QWidget* parent )
: QBoxPainter { parent }, barChart { std::move(ptr) }
{
    setMaxOY(max);        
    setAutoFillBackground(false);
    boxStyle.marginTop = 30;
    boxStyle.marginRight = 30;

    init();
}

bool QBarChart::loadStyle(BoxStyles style)
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        adjustBoxDisplayValues();
        configureButtons();
        return true;
    }
    else return false;
}

void QBarChart::init()
{    
    if( parentWidget() != nullptr )
    {
        adjustBoxDisplayValues();

        buttons[DataVisualization::BARS] = std::make_unique<QPushButton>("",this);
        buttons[DataVisualization::BARS]->setIcon(QIcon(QPixmap(":/new/icons/images/bars.png")));
        buttons[DataVisualization::BARS]->setToolTip("Bars display");

        buttons[DataVisualization::POINTS] = std::make_unique<QPushButton>("",this);
        buttons[DataVisualization::POINTS]->setIcon(QIcon(QPixmap(":/new/icons/images/points.png")));
        buttons[DataVisualization::POINTS]->setToolTip("Points display");

        buttons[DataVisualization::LINES] = std::make_unique<QPushButton>("",this);
        buttons[DataVisualization::LINES]->setIcon(QIcon(QPixmap(":/new/icons/images/lines.png")));
        buttons[DataVisualization::LINES]->setToolTip("Lines display");

        configureButtons();

        connect( buttons[DataVisualization::BARS].get()   , &QPushButton::clicked , this, &QBarChart::onButtonClick );
        connect( buttons[DataVisualization::POINTS].get() , &QPushButton::clicked , this, &QBarChart::onButtonClick );
        connect( buttons[DataVisualization::LINES].get()  , &QPushButton::clicked , this, &QBarChart::onButtonClick );
    }
}

int QBarChart::calculateLabelPosition()
{
    QFontMetrics fm(parentWidget()->font());
    int pixelsWide = fm.horizontalAdvance(barChart->getLabel().c_str());

    return boxStyle.marginLeft + (parentWidget()->width()-(boxStyle.marginLeft+boxStyle.marginRight)-pixelsWide)/2;
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

        if( (boxStyle.isScalableUp && maxValue<barChart->getMax()) || (boxStyle.isScalableDown && maxValue>2*barChart->getMax()) )
        {
            if( barChart->getMax()>0 ) maxValue = 1.5*barChart->getMax();
        }

        double scaleFactor = (height()-(boxStyle.marginLeft+boxStyle.marginRight))/maxValue;

        for( const auto &barChartBin : barChart->getBins() )
        {
            value = static_cast<int>(barChartBin*scaleFactor);
            intensity = static_cast<int>(255.0*(barChartBin)/maxValue);
            if( intensity>255 ) intensity = 255;
            painter.setBrush(QBrush(QColor(intensity, 0, 255-intensity)));
            painter.setPen(QPen(QColor(intensity, 0, 255-intensity)));

            if( dataVisulization == DataVisualization::BARS )
            {               
                painter.drawRect(boxStyle.marginLeft+index*barWidth,height()-value-boxStyle.marginBottom-2,barWidth-1,value);
            }
            else if( dataVisulization == DataVisualization::POINTS )
            {
                painter.drawEllipse(boxStyle.marginLeft+index*barWidth,height()-value-boxStyle.marginBottom-2-barWidth,barWidth,barWidth);
            }
            else if( dataVisulization == DataVisualization::LINES )
            {
                if(index>0) painter.drawLine(boxStyle.marginLeft+(index-1)*barWidth,height()-valueLast-boxStyle.marginBottom-2-barWidth,boxStyle.marginLeft+index*barWidth,height()-value-boxStyle.marginBottom-2-barWidth);
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
    painter.setPen(QPen(boxStyle.cValue));
    painter.drawText(boxStyle.marginLeft,boxStyle.marginTop-7,QString::number(100*barChart->getBins().back(),'f',2));
}

void QBarChart::drawChartName()
{
    painter.setPen(QPen(boxStyle.cValue));
    painter.drawText(calculateLabelPosition(),boxStyle.marginTop-7,QString::fromStdString(barChart->getLabel()));
}

void QBarChart::adjustBoxDisplayValues()
{
    int width {parentWidget()->width()};
    int size  {static_cast<int>(barChart->getBins().size())};
    int marginAdjustment {(width-(boxStyle.marginLeft+boxStyle.marginRight)) - size*((width-(boxStyle.marginLeft+boxStyle.marginRight))/size)};

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

void QBarChart::configureButtons()
{
    buttons[DataVisualization::BARS]->resize(boxStyle.buttonWidth+boxStyle.buttonIndent,boxStyle.buttonHeight);
    buttons[DataVisualization::BARS]->move(parentWidget()->width()-boxStyle.buttonWidth-boxStyle.buttonIndent-1,2);
    buttons[DataVisualization::BARS]->setStyleSheet(boxStyle.buttonStyleSelected);

    buttons[DataVisualization::POINTS]->resize(boxStyle.buttonWidth,boxStyle.buttonHeight);
    buttons[DataVisualization::POINTS]->move(parentWidget()->width()-boxStyle.buttonWidth-1,boxStyle.buttonHeight+2);
    buttons[DataVisualization::POINTS]->setStyleSheet(boxStyle.buttonStyleUnselected);

    buttons[DataVisualization::LINES]->resize(boxStyle.buttonWidth,boxStyle.buttonHeight);
    buttons[DataVisualization::LINES]->move(parentWidget()->width()-boxStyle.buttonWidth-1,2*boxStyle.buttonHeight+2);
    buttons[DataVisualization::LINES]->setStyleSheet(boxStyle.buttonStyleUnselected);
}

void QBarChart::onButtonClick()
{
    auto clickedButton = qobject_cast<QPushButton*>( sender() );
    if( clickedButton != nullptr )
    {        
      buttons[dataVisulization]->setStyleSheet(boxStyle.buttonStyleUnselected);
      buttons[dataVisulization]->resize(boxStyle.buttonWidth,boxStyle.buttonHeight);
      buttons[dataVisulization]->move(parentWidget()->width()-boxStyle.buttonWidth-1,buttons[dataVisulization]->pos().y());

      if( clickedButton->toolTip().contains("Bars") )
      {
          dataVisulization = DataVisualization::BARS;
      }
      else if( clickedButton->toolTip().contains("Points") )
      {
          dataVisulization = DataVisualization::POINTS;
      }
      else if( clickedButton->toolTip().contains("Lines") )
      {
          dataVisulization = DataVisualization::LINES;
      }

      buttons[dataVisulization]->setStyleSheet(boxStyle.buttonStyleSelected);
      buttons[dataVisulization]->resize(boxStyle.buttonWidth+boxStyle.buttonIndent,boxStyle.buttonHeight);
      buttons[dataVisulization]->move(parentWidget()->width()-boxStyle.buttonWidth-boxStyle.buttonIndent-1,buttons[dataVisulization]->pos().y());
    }
}
