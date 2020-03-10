#include "qbarchart.h"

QBarChart::QBarChart( ActionType type, double max, QWidget* parent )
: QBoxPainter { parent }, barChart { Locator::getBarChart(type) }
{    
    setMaxOY(max);        
    setAutoFillBackground(false);
    boxStyle.values[BoxValues::MARGIN_TOP] = 30;
    boxStyle.values[BoxValues::MARGIN_RIGHT] = 30;

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

        if( (boxStyle.logics[BoxLogics::SCALABLE_UP] && maxValue<barChart->getMax()) || (boxStyle.logics[BoxLogics::SCALABLE_DOWN] && maxValue>2*barChart->getMax()) )
        {
            if( barChart->getMax()>0 ) maxValue = 1.5*barChart->getMax();
        }

        double scaleFactor = (height()-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT]))/maxValue;

        for( const auto &barChartBin : barChart->getBins() )
        {
            value = static_cast<int>(barChartBin*scaleFactor);
            intensity = static_cast<int>(255.0*(barChartBin)/maxValue);
            if( intensity>255 ) intensity = 255;
            painter.setBrush(QBrush(QColor(intensity, 0, 255-intensity)));
            painter.setPen(QPen(QColor(intensity, 0, 255-intensity)));

            if( dataVisulization == DataVisualization::BARS )
            {               
                painter.drawRect(boxStyle.values[BoxValues::MARGIN_LEFT]+index*barWidth,height()-value-boxStyle.values[BoxValues::MARGIN_BOTTOM]-2,barWidth-1,value);
            }
            else if( dataVisulization == DataVisualization::POINTS )
            {
                painter.drawEllipse(boxStyle.values[BoxValues::MARGIN_LEFT]+index*barWidth,height()-value-boxStyle.values[BoxValues::MARGIN_BOTTOM]-2-barWidth,barWidth,barWidth);
            }
            else if( dataVisulization == DataVisualization::LINES )
            {
                if(index!=binsSize) painter.drawLine(boxStyle.values[BoxValues::MARGIN_LEFT]+(index+1)*barWidth,height()-valueLast-boxStyle.values[BoxValues::MARGIN_BOTTOM]-2-barWidth,boxStyle.values[BoxValues::MARGIN_LEFT]+index*barWidth,height()-value-boxStyle.values[BoxValues::MARGIN_BOTTOM]-2-barWidth);
            }

            --index;
            valueLast = value;
        }

        if( boxStyle.logics[BoxLogics::VALUE_DISPLAY] ) drawCurrentValue();
        drawChartLabels();
    }

}

void QBarChart::drawCurrentValue()
{
    painter.setPen(boxStyle.colors[BoxColors::VALUE]);
    painter.drawText(boxStyle.values[BoxValues::MARGIN_LEFT],boxStyle.values[BoxValues::MARGIN_TOP]-7,QString::number(100*barChart->getBins().back(),'f',2));
}

void QBarChart::drawChartLabels()
{
    painter.setPen(boxStyle.colors[BoxColors::LABEL]);
    painter.drawText(calculateCenterTextPosition(LangManager::translate(qLabel),boxStyle.values[BoxValues::MARGIN_LEFT],width()-boxStyle.values[BoxValues::MARGIN_RIGHT]),boxStyle.values[BoxValues::MARGIN_TOP]-7,LangManager::translate(qLabel));
    painter.drawText(calculateCenterTextPosition(LangManager::translate(qLabelX),boxStyle.values[BoxValues::MARGIN_LEFT],width()-boxStyle.values[BoxValues::MARGIN_RIGHT]),height()-10,LangManager::translate(qLabelX));
    painter.drawText(boxStyle.values[BoxValues::MARGIN_LEFT]/2-4,calculateCenterTextPosition(LangManager::translate(qLabelY),boxStyle.values[BoxValues::MARGIN_TOP],height()-boxStyle.values[BoxValues::MARGIN_BOTTOM]),LangManager::translate(qLabelY));
}

void QBarChart::reconfigurateBarChartLayout()
{
    int width {parentWidget()->width()};
    int size  {static_cast<int>(barChart->getBins().size())};
    int marginAdjustment {(width-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT])) - size*((width-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT]))/size)};

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

void QBarChart::configureButtons()
{
    buttons[DataVisualization::BARS]->resize(boxStyle.values[BoxValues::BUTTON_WIDTH]+boxStyle.values[BoxValues::BUTTON_INDENT],boxStyle.values[BoxValues::BUTTON_HEIGHT]);
    buttons[DataVisualization::BARS]->move(parentWidget()->width()-boxStyle.values[BoxValues::BUTTON_WIDTH]-boxStyle.values[BoxValues::BUTTON_INDENT]-5,5);
    buttons[DataVisualization::BARS]->setStyleSheet(boxStyle.sheets[BoxSheets::BUTTON_SELECTED]);

    buttons[DataVisualization::POINTS]->resize(boxStyle.values[BoxValues::BUTTON_WIDTH],boxStyle.values[BoxValues::BUTTON_HEIGHT]);
    buttons[DataVisualization::POINTS]->move(parentWidget()->width()-boxStyle.values[BoxValues::BUTTON_WIDTH]-5,boxStyle.values[BoxValues::BUTTON_HEIGHT]+6);
    buttons[DataVisualization::POINTS]->setStyleSheet(boxStyle.sheets[BoxSheets::BUTTON_UNSELECTED]);

    buttons[DataVisualization::LINES]->resize(boxStyle.values[BoxValues::BUTTON_WIDTH],boxStyle.values[BoxValues::BUTTON_HEIGHT]);
    buttons[DataVisualization::LINES]->move(parentWidget()->width()-boxStyle.values[BoxValues::BUTTON_WIDTH]-5,2*boxStyle.values[BoxValues::BUTTON_HEIGHT]+7);
    buttons[DataVisualization::LINES]->setStyleSheet(boxStyle.sheets[BoxSheets::BUTTON_UNSELECTED]);

    resetButton->resize(boxStyle.values[BoxValues::BUTTON_WIDTH],boxStyle.values[BoxValues::BUTTON_HEIGHT]);
    resetButton->move(parentWidget()->width()-boxStyle.values[BoxValues::BUTTON_WIDTH]-5,parentWidget()->height()-boxStyle.values[BoxValues::BUTTON_HEIGHT]-10);
    resetButton->setStyleSheet(boxStyle.sheets[BoxSheets::BUTTON_RESET]);
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
            buttons[dataVisulization]->setStyleSheet(boxStyle.sheets[BoxSheets::BUTTON_UNSELECTED]);
            buttons[dataVisulization]->resize(boxStyle.values[BoxValues::BUTTON_WIDTH],boxStyle.values[BoxValues::BUTTON_HEIGHT]);
            buttons[dataVisulization]->move(parentWidget()->width()-boxStyle.values[BoxValues::BUTTON_WIDTH]-5,buttons[dataVisulization]->pos().y());

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

            buttons[dataVisulization]->setStyleSheet(boxStyle.sheets[BoxSheets::BUTTON_SELECTED]);
            buttons[dataVisulization]->resize(boxStyle.values[BoxValues::BUTTON_WIDTH]+boxStyle.values[BoxValues::BUTTON_INDENT],boxStyle.values[BoxValues::BUTTON_HEIGHT]);
            buttons[dataVisulization]->move(parentWidget()->width()-boxStyle.values[BoxValues::BUTTON_WIDTH]-boxStyle.values[BoxValues::BUTTON_INDENT]-5,buttons[dataVisulization]->pos().y());
        }
    }
}
