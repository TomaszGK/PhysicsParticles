#include "qbarchart.h"

QBarChart::QBarChart( ActionType type, double max, QWidget* parent )
: QBoxPainter { parent }, barChart { Locator::getBarChart(type) }
{    
    setMaxOY(max);        
    setAutoFillBackground(false);
    boxStyle.values["marginTop"] = 30;
    boxStyle.values["marginRight"] = 30;

    qLabel  = barChart->getLabel().c_str();
    qLabelX = barChart->getLabelX().c_str();
    qLabelY = barChart->getLabelY().c_str();   

    init();
}

bool QBarChart::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        reconfigurateBarChartLayout();
        configureButtons();
        return true;
    }
    else return false;
}

void QBarChart::changeEvent( QEvent *event )
{
    if( event->type() == QEvent::LanguageChange )
    {
        buttons[DataVisualization::BARS]->setToolTip(tr("Bars display"));
        buttons[DataVisualization::POINTS]->setToolTip(tr("Points display"));
        buttons[DataVisualization::LINES]->setToolTip(tr("Lines display"));
        resetButton->setToolTip(tr("Reset"));
    }
    else
    {
        QWidget::changeEvent(event);
    }
}

void QBarChart::init()
{    
    if( parentWidget() != nullptr )
    {
        reconfigurateBarChartLayout();

        buttons[DataVisualization::BARS] = std::make_unique<QPushButton>("",this);
        buttons[DataVisualization::BARS]->setIcon(QIcon(QPixmap(":/new/icons/images/bars.png")));
        buttons[DataVisualization::BARS]->setToolTip(tr("Bars display"));

        buttons[DataVisualization::POINTS] = std::make_unique<QPushButton>("",this);
        buttons[DataVisualization::POINTS]->setIcon(QIcon(QPixmap(":/new/icons/images/points.png")));
        buttons[DataVisualization::POINTS]->setToolTip(tr("Points display"));

        buttons[DataVisualization::LINES] = std::make_unique<QPushButton>("",this);
        buttons[DataVisualization::LINES]->setIcon(QIcon(QPixmap(":/new/icons/images/lines.png")));
        buttons[DataVisualization::LINES]->setToolTip(tr("Lines display"));

        resetButton = std::make_unique<QPushButton>("",this);
        resetButton->setIcon(QIcon(QPixmap(":/new/icons/images/reset_barchart.png")));
        resetButton->setToolTip(tr("Reset"));

        configureButtons();

        connect( buttons[DataVisualization::BARS].get()   , &QPushButton::clicked , this, &QBarChart::onButtonClick );
        connect( buttons[DataVisualization::POINTS].get() , &QPushButton::clicked , this, &QBarChart::onButtonClick );
        connect( buttons[DataVisualization::LINES].get()  , &QPushButton::clicked , this, &QBarChart::onButtonClick );
        connect( resetButton.get()                        , &QPushButton::clicked , this, &QBarChart::onButtonClick );
    }
}

void QBarChart::paint()
{

    if( barChart != nullptr )
    {

        int value {0};
        int valueLast {0};
        const int binsSize {static_cast<int>(barChart->getBins().size()-1)};
        int index {binsSize};
        int intensity {0};

        paintAxes();

        if( (boxStyle.logics["isScalableUp"] && maxValue<barChart->getMax()) || (boxStyle.logics["isScalableDown"] && maxValue>2*barChart->getMax()) )
        {
            if( barChart->getMax()>0 ) maxValue = 1.5*barChart->getMax();
        }

        double scaleFactor = (height()-(boxStyle.values["marginLeft"]+boxStyle.values["marginRight"]))/maxValue;

        for( const auto &barChartBin : barChart->getBins() )
        {
            value = static_cast<int>(barChartBin*scaleFactor);
            intensity = static_cast<int>(255.0*(barChartBin)/maxValue);
            if( intensity>255 ) intensity = 255;
            painter.setBrush(QBrush(QColor(intensity, 0, 255-intensity)));
            painter.setPen(QPen(QColor(intensity, 0, 255-intensity)));

            if( dataVisulization == DataVisualization::BARS )
            {               
                painter.drawRect(boxStyle.values["marginLeft"]+index*barWidth,height()-value-boxStyle.values["marginBottom"]-2,barWidth-1,value);
            }
            else if( dataVisulization == DataVisualization::POINTS )
            {
                painter.drawEllipse(boxStyle.values["marginLeft"]+index*barWidth,height()-value-boxStyle.values["marginBottom"]-2-barWidth,barWidth,barWidth);
            }
            else if( dataVisulization == DataVisualization::LINES )
            {
                if(index!=binsSize) painter.drawLine(boxStyle.values["marginLeft"]+(index+1)*barWidth,height()-valueLast-boxStyle.values["marginBottom"]-2-barWidth,boxStyle.values["marginLeft"]+index*barWidth,height()-value-boxStyle.values["marginBottom"]-2-barWidth);
            }

            --index;
            valueLast = value;
        }

        if( boxStyle.logics["isValueDisplay"] ) drawCurrentValue();
        drawChartLabels();
    }

}

void QBarChart::drawCurrentValue()
{
    painter.setPen(QPen(boxStyle.colors["cValue"]));
    painter.drawText(boxStyle.values["marginLeft"],boxStyle.values["marginTop"]-7,QString::number(100*barChart->getBins().back(),'f',2));
}

void QBarChart::drawChartLabels()
{
    painter.setPen(QPen(boxStyle.colors["cLabelColor"]));
    painter.drawText(calculateCenterTextPosition(LangManager::translate(qLabel),boxStyle.values["marginLeft"],width()-boxStyle.values["marginRight"]),boxStyle.values["marginTop"]-7,LangManager::translate(qLabel));
    painter.drawText(calculateCenterTextPosition(LangManager::translate(qLabelX),boxStyle.values["marginLeft"],width()-boxStyle.values["marginRight"]),height()-10,LangManager::translate(qLabelX));
    painter.drawText(boxStyle.values["marginLeft"]/2-4,calculateCenterTextPosition(LangManager::translate(qLabelY),boxStyle.values["marginTop"],height()-boxStyle.values["marginBottom"]),LangManager::translate(qLabelY));
}

void QBarChart::reconfigurateBarChartLayout()
{
    int width {parentWidget()->width()};
    int size  {static_cast<int>(barChart->getBins().size())};
    int marginAdjustment {(width-(boxStyle.values["marginLeft"]+boxStyle.values["marginRight"])) - size*((width-(boxStyle.values["marginLeft"]+boxStyle.values["marginRight"]))/size)};

    if( marginAdjustment>0 )
    {
        if( marginAdjustment % 2 == 0 )
        {
            boxStyle.values["marginLeft"] += marginAdjustment/2;
            boxStyle.values["marginRight"] += marginAdjustment/2;
        }
        else
        {
            boxStyle.values["marginLeft"] += (marginAdjustment-1)/2;
            boxStyle.values["marginRight"] += ((marginAdjustment-1)/2+1);
        }
    }

    barWidth = (width-(boxStyle.values["marginLeft"]+boxStyle.values["marginRight"]))/size;
}

void QBarChart::configureButtons()
{
    buttons[DataVisualization::BARS]->resize(boxStyle.values["buttonWidth"]+boxStyle.values["buttonIndent"],boxStyle.values["buttonHeight"]);
    buttons[DataVisualization::BARS]->move(parentWidget()->width()-boxStyle.values["buttonWidth"]-boxStyle.values["buttonIndent"]-5,5);
    buttons[DataVisualization::BARS]->setStyleSheet(boxStyle.sheets["buttonStyleSelected"]);

    buttons[DataVisualization::POINTS]->resize(boxStyle.values["buttonWidth"],boxStyle.values["buttonHeight"]);
    buttons[DataVisualization::POINTS]->move(parentWidget()->width()-boxStyle.values["buttonWidth"]-5,boxStyle.values["buttonHeight"]+6);
    buttons[DataVisualization::POINTS]->setStyleSheet(boxStyle.sheets["buttonStyleUnselected"]);

    buttons[DataVisualization::LINES]->resize(boxStyle.values["buttonWidth"],boxStyle.values["buttonHeight"]);
    buttons[DataVisualization::LINES]->move(parentWidget()->width()-boxStyle.values["buttonWidth"]-5,2*boxStyle.values["buttonHeight"]+7);
    buttons[DataVisualization::LINES]->setStyleSheet(boxStyle.sheets["buttonStyleUnselected"]);

    resetButton->resize(boxStyle.values["buttonWidth"],boxStyle.values["buttonHeight"]);
    resetButton->move(parentWidget()->width()-boxStyle.values["buttonWidth"]-5,parentWidget()->height()-boxStyle.values["buttonHeight"]-10);
    resetButton->setStyleSheet(boxStyle.sheets["buttonStyleReset"]);
}

void QBarChart::onButtonClick()
{
    auto clickedButton = qobject_cast<QPushButton*>( sender() );
    if( clickedButton != nullptr )
    {
        if( clickedButton->toolTip().contains("Reset") )
        {
            barChart->resetBins();
        }
        else
        {
            buttons[dataVisulization]->setStyleSheet(boxStyle.sheets["buttonStyleUnselected"]);
            buttons[dataVisulization]->resize(boxStyle.values["buttonWidth"],boxStyle.values["buttonHeight"]);
            buttons[dataVisulization]->move(parentWidget()->width()-boxStyle.values["buttonWidth"]-5,buttons[dataVisulization]->pos().y());

            if( clickedButton->toolTip() == tr("Bars display") )
            {
                dataVisulization = DataVisualization::BARS;
            }
            else if( clickedButton->toolTip() == tr("Points display") )
            {
                dataVisulization = DataVisualization::POINTS;
            }
            else if( clickedButton->toolTip() == tr("Lines display") )
            {
                dataVisulization = DataVisualization::LINES;
            }

            buttons[dataVisulization]->setStyleSheet(boxStyle.sheets["buttonStyleSelected"]);
            buttons[dataVisulization]->resize(boxStyle.values["buttonWidth"]+boxStyle.values["buttonIndent"],boxStyle.values["buttonHeight"]);
            buttons[dataVisulization]->move(parentWidget()->width()-boxStyle.values["buttonWidth"]-boxStyle.values["buttonIndent"]-5,buttons[dataVisulization]->pos().y());
        }
    }
}
