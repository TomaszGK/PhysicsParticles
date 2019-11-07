#include "qparticlesphysicsmanager.h"

QParticlesPhysicsManager::QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout )
: ParticlesPhysicsManager(type,layout->parentWidget()->width(),layout->parentWidget()->height() )
{
    createParticlesPaintManager();    
    layout->addWidget( particlesPaintManager.get() );
}

void QParticlesPhysicsManager::addQBarChart( const std::string& name, QHBoxLayout* layout, BoxStyles style )
{
    if( barCharts.count(name) != 0 )
    {       
        qBoxPainters[name] = std::make_unique<QBarChart>(sqrt(physicsInfo.maxRapidity*2)/3,barCharts[name],layout->parentWidget());
        layout->addWidget( qBoxPainters[name].get() );
        if( style != BoxStyles::DEFAULT ) qBoxPainters[name]->loadStyle(style);
    }
}

void QParticlesPhysicsManager::addQCircleControl( const std::string& name, QHBoxLayout* layout )
{
    qBoxPainters[name] = std::make_unique<QCircleControl>(layout->parentWidget());
    controlBoxType[name] = ControlType::CIRCLE_CONTROL;
    layout->addWidget( qBoxPainters[name].get() );
}

void QParticlesPhysicsManager::addQBarDisplay( const std::string& name, QHBoxLayout* layout )
{
    if( barDisplays.count(name) != 0 )
    {        
        qBoxPainters[name] = std::make_unique<QBarDisplay>(simulationInfo.numberOfParticlesInit[ParticleType::RED],barDisplays[name],layout->parentWidget());        
        layout->addWidget( qBoxPainters[name].get() );        
    }
}

void QParticlesPhysicsManager::addQHistogram1D( const std::string& name, QHBoxLayout* layout )
{
    if( histograms1D.count(name) != 0 )
    {       
        qBoxPainters[name] = std::make_unique<QHistogram1D>(160,histograms1D[name],layout->parentWidget());
        layout->addWidget( qBoxPainters[name].get() );        
    }
}

void QParticlesPhysicsManager::addQInfoDisplay(  const std::string& name, QHBoxLayout* layout  )
{    
    qBoxPainters[name] = std::make_unique<QInfoDisplay>();    
    layout->addWidget( qBoxPainters[name].get() );
}

void QParticlesPhysicsManager::addQGauge( const std::string& name, QHBoxLayout* layout )
{
    qGauges[name] = { nullptr , nullptr };

    int range {1000};

    qGauges[name].first = std::make_unique<QcGaugeWidget>();
    qGauges[name].first->addArc(55);
    QcDegreesItem* degreeItem = qGauges[name].first->addDegrees(65);
    degreeItem->setValueRange(0,range);
    degreeItem->setStep(range/10);
    QcColorBand *clrBand =  qGauges[name].first->addColorBand(50);
    clrBand->setValueRange(0,100);
    QcValuesItem *valueItem = qGauges[name].first->addValues(80);
    valueItem->setValueRange(0,range);
    valueItem->setStep(range/10);
    gaugeNameLabel = qGauges[name].first->addLabel(70);
    gaugeNameLabel->setText(QString::fromStdString(LangManager::translate(name)));
    QcLabelItem *lab = qGauges[name].first->addLabel(40);
    lab->setText("0");
    qGauges[name].second.reset( qGauges[name].first->addNeedle(60) );
    qGauges[name].second->setLabel(lab);
    qGauges[name].second->setColor(Qt::blue);
    qGauges[name].second->setValueRange(0,range);
    qGauges[name].first->addBackground(7);

    // reset style sheet - needs default
    qGauges[name].first->setStyleSheet("");

    layout->addWidget( qGauges[name].first.get() );
}

void QParticlesPhysicsManager::addQTrackingPlot2D( const std::string& name, QHBoxLayout *layout )
{
    qBoxPainters[name] = std::make_unique<QTrackingPlot2D>(particles,layout->parentWidget());    
    layout->addWidget( qBoxPainters[name].get() );
}

void QParticlesPhysicsManager::createParticlesPaintManager()
{
    particlesPaintManager = std::make_unique<QPainterManager>(particles,planeArea);
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
