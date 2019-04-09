#include "qparticlesphysicsmanager.h"

QParticlesPhysicsManager::QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout )
: ParticlesPhysicsManager(type,layout->parentWidget()->width(),layout->parentWidget()->height() )
{
    createParticlesPaintManager();    
    layout->addWidget( particlesPaintManager.get() );
}

void QParticlesPhysicsManager::addQBarChart( const std::string& name, QHBoxLayout* layout, std::pair<bool,bool> scalability )
{
    if( barCharts.count(name) != 0 )
    {       
        qBoxPainters[name] = std::make_unique<QBarChart>(sqrt(physicsInfo.maxRapidity*2)/2,scalability,barCharts[name],layout->parentWidget());
        layout->addWidget( qBoxPainters[name].get() );        
    }
}

void QParticlesPhysicsManager::addQCircleControl( const std::string& name, QHBoxLayout* layout )
{
    qBoxPainters[name] = std::make_unique<QCircleControl>(layout->parentWidget());
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
        qBoxPainters[name] = std::make_unique<QHistogram1D>(100,histograms1D[name],layout->parentWidget());
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

    qGauges[name].first = std::make_unique<QcGaugeWidget>();
    qGauges[name].first->addArc(55);
    qGauges[name].first->addDegrees(65)->setValueRange(0,100);
    QcColorBand *clrBand =  qGauges[name].first->addColorBand(50);
    clrBand->setValueRange(0,100);
    qGauges[name].first->addValues(80)->setValueRange(0,100);
    qGauges[name].first->addLabel(70)->setText(QString::fromStdString(name));
    QcLabelItem *lab =  qGauges[name].first->addLabel(40);
    lab->setText("0");
    qGauges[name].second.reset( qGauges[name].first->addNeedle(60));
    qGauges[name].second->setLabel(lab);
    qGauges[name].second->setColor(Qt::blue);
    qGauges[name].second->setValueRange(0,100);
    qGauges[name].first->addBackground(7);

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

void QParticlesPhysicsManager::paintParticlesFrame()
{
    particlesPaintManager->update();
}

void QParticlesPhysicsManager::paintFrames()
{
    updateBars();    
    for( auto &painter : qBoxPainters ){ painter.second->update(); }
    for( auto &gauge : qGauges ){ gauge.second.second->setCurrentValue( getPressureInPercent() ); }
}
