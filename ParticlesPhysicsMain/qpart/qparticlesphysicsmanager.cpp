#include "qparticlesphysicsmanager.h"

QParticlesPhysicsManager::QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout )
: ParticlesPhysicsManager(type,layout->parentWidget()->width(),layout->parentWidget()->height() )
{
    createParticlesPaintManager();    
    layout->addWidget( particlesPaintManager.get() );
}

void QParticlesPhysicsManager::add( BoxType boxType, MeasurementType measurementType, QHBoxLayout *layout, BoxStyles style )
{
    switch( boxType )
    {
     case BoxType::BARCHART :
        if( barCharts->count(measurementType) != 0 )
        {
            qBoxPainters[measurementType] = std::make_unique<QBarChart>(measurementType,sqrt(physicsInfo.maxRapidity*2)/3,layout->parentWidget());
            layout->addWidget( qBoxPainters[measurementType].get() );
            if( style != BoxStyles::DEFAULT ) qBoxPainters[measurementType]->loadStyle(style);
        }
     break;

     case BoxType::BARDISPLAY :
        if( barDisplays->count(measurementType) != 0 )
        {
            qBoxPainters[measurementType] = std::make_unique<QBarDisplay>(measurementType,layout->parentWidget());
            layout->addWidget( qBoxPainters[measurementType].get() );
        }
     break;

     case BoxType::HISTOGRAM1D :
        if( histograms1D->count(measurementType) != 0 )
        {
            qBoxPainters[measurementType] = std::make_unique<QHistogram1D>(measurementType,160,layout->parentWidget());
            layout->addWidget( qBoxPainters[measurementType].get() );
        }
     break;

     case BoxType::TRACKINGPLOT :
        qBoxPainters[measurementType] = std::make_unique<QTrackingPlot2D>("Molecule Tracking Plot",layout->parentWidget());
        layout->addWidget( qBoxPainters[measurementType].get() );
     break;

     case BoxType::GAUGE :
        addQGauge( measurementType , layout );
     break;

     case BoxType::CIRCLECONTROL :
        qBoxPainters[measurementType] = std::make_unique<QCircleControl>(layout->parentWidget());
        controlBoxType[measurementType] = ControlType::CIRCLE_CONTROL;
        layout->addWidget( qBoxPainters[measurementType].get() );
     break;

     case BoxType::INFODISPLAY :
        qBoxPainters[measurementType] = std::make_unique<QInfoDisplay>();
        layout->addWidget( qBoxPainters[measurementType].get() );
     break;
    }
}

void QParticlesPhysicsManager::addQGauge( MeasurementType type, QHBoxLayout* layout )
{
    qGauges[type] = { nullptr , nullptr };

    int range {1000};

    qGauges[type].first = std::make_unique<QcGaugeWidget>();
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
    qGauges[type].second.reset( qGauges[type].first->addNeedle(60) );
    qGauges[type].second->setLabel(lab);
    qGauges[type].second->setColor(Qt::blue);
    qGauges[type].second->setValueRange(0,range);
    qGauges[type].first->addBackground(7);

    // reset style sheet - needs default
    qGauges[type].first->setStyleSheet("");

    layout->addWidget( qGauges[type].first.get() );
}

void QParticlesPhysicsManager::createParticlesPaintManager()
{
    particlesPaintManager = std::make_unique<QPainterManager>();
}

void QParticlesPhysicsManager::paintParticlesPlane()
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
            QPoint forceOut = dynamic_cast<QCircleControl*>(qBoxPainters[control.first].get())->getIndicator();
            vect2D forceIn(0.00001*forceOut.x(),0.00001*forceOut.y());
            setPushForce(forceIn);
        }
    }
}
