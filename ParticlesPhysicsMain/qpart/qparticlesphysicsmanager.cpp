#include "qparticlesphysicsmanager.h"

QParticlesPhysicsManager::QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout )
: ParticlesPhysicsManager(type,layout->parentWidget()->width(),layout->parentWidget()->height() )
{
    particlesPaintManager = new QPainterManager();
    layout->addWidget( particlesPaintManager );
}

void QParticlesPhysicsManager::add( QHBoxLayout* layout, BoxType boxType, ActionType actionType, BoxStyles style )
{
    switch( boxType )
    {
     case BoxType::BARCHART :
        if( analyzer->barCharts->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QBarChart(actionType,sqrt(analyzer->physicsInfo.maxRapidity*2)/3,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
            if( style != BoxStyles::DEFAULT ) qBoxPainters[actionType]->loadStyle(style);
        }
     break;

     case BoxType::BARDISPLAY :
        if( analyzer->barDisplays->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QBarDisplay(actionType,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
        }
     break;

     case BoxType::HISTOGRAM1D :
        if( analyzer->histograms1D->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QHistogram1D(actionType,160,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
        }
     break;

     case BoxType::TRACKINGPLOT :
        qBoxPainters[actionType] = new QTrackingPlot2D("Molecule Tracking Plot",layout->parentWidget());
        layout->addWidget( qBoxPainters[actionType] );
     break;

     case BoxType::GAUGE :
        addQGauge( actionType , layout );
     break;

     case BoxType::CIRCLECONTROL :
        qBoxPainters[actionType] = new QCircleControl(layout->parentWidget());
        controlBoxType[actionType] = ControlType::CIRCLE_CONTROL;
        layout->addWidget( qBoxPainters[actionType] );
     break;

     case BoxType::INFODISPLAY :
        qBoxPainters[actionType] = new QInfoDisplay();
        layout->addWidget( qBoxPainters[actionType] );
     break;
    }
}

void QParticlesPhysicsManager::addQGauge( ActionType type, QHBoxLayout* layout )
{
    qGauges[type] = { nullptr , nullptr };

    int range {1000};

    qGauges[type].first = new QcGaugeWidget();
    qGauges[type].first->addArc(55);
    QcDegreesItem* degreeItem = qGauges[type].first->addDegrees(65);
    degreeItem->setValueRange(0,range);
    degreeItem->setStep(range/10);
    QcColorBand *clrBand =  qGauges[type].first->addColorBand(50);
    clrBand->setValueRange(0,100);
    QcValuesItem *valueItem = qGauges[type].first->addValues(80);
    valueItem->setValueRange(0,range);
    valueItem->setStep(range/10);
    gaugeNameLabel = qGauges[type].first->addLabel(70);
    gaugeNameLabel->setText(LangManager::translate(QString("Pressure")));
    QcLabelItem *lab = qGauges[type].first->addLabel(40);
    lab->setText("0");
    qGauges[type].second = qGauges[type].first->addNeedle(60);
    qGauges[type].second->setLabel(lab);
    qGauges[type].second->setColor(Qt::blue);
    qGauges[type].second->setValueRange(0,range);
    qGauges[type].first->addBackground(7);

    // reset style sheet - needs default
    qGauges[type].first->setStyleSheet("");

    layout->addWidget( qGauges[type].first );
}

void QParticlesPhysicsManager::updateParticlesPlane()
{
    particlesPaintManager->update();
}

void QParticlesPhysicsManager::paintLayouts()
{
    updateBars();
    for( auto &painter : qBoxPainters ){ painter.second->update(); }
    for( auto &gauge : qGauges )
    {
        gaugeNameLabel->setText(LangManager::translate(gaugeNameLabel->text()));
        gauge.second.second->setCurrentValue( getPressureValue() );
    }
}

void QParticlesPhysicsManager::handleControls()
{
    for( auto &control : controlBoxType )
    {
        if( control.second == ControlType::CIRCLE_CONTROL )
        {
            QPoint forceOut = dynamic_cast<QCircleControl*>(qBoxPainters[control.first].data())->getIndicator();
            vect2D forceIn(0.00001*forceOut.x(),0.00001*forceOut.y());
            setForce(forceIn);
        }
    }
}
