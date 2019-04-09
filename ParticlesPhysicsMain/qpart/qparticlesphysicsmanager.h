#pragma once

#include <QHBoxLayout>
#include "qpaintermanager.h"
#include "qbarchart.h"
#include "qbardisplay.h"
#include "qhistogram1d.h"
#include "qinfodisplay.h"
#include "qcgaugewidget.h"
#include "qtrackingplot2d.h"
#include "qcirclecontrol.h"
#include "particlesphysicsmanager.h"

class QParticlesPhysicsManager : public ParticlesPhysicsManager
{

    std::unique_ptr<QPainterManager>                     particlesPaintManager;    
    std::map<std::string,std::unique_ptr<QBoxPainter>>   qBoxPainters;

    std::map<std::string,std::pair<std::unique_ptr<QcGaugeWidget>,std::unique_ptr<QcNeedleItem>>> qGauges;

    void createParticlesPaintManager();

public:

    QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout );

    QParticlesPhysicsManager( const QParticlesPhysicsManager& ) = delete;
    QParticlesPhysicsManager( QParticlesPhysicsManager&& ) = delete;

    QParticlesPhysicsManager& operator=( const QParticlesPhysicsManager& ) = delete;
    QParticlesPhysicsManager& operator=( QParticlesPhysicsManager&& ) = delete;

    void addQBarChart( const std::string& name, QHBoxLayout* layout, std::pair<bool,bool> scalability = {true,true} );
    void addQCircleControl( const std::string& name, QHBoxLayout* layout );
    void addQBarDisplay( const std::string& name, QHBoxLayout* layout );
    void addQHistogram1D( const std::string& name, QHBoxLayout* layout );
    void addQInfoDisplay( const std::string& name, QHBoxLayout* layout );
    void addQGauge( const std::string& name, QHBoxLayout* layout );
    void addQTrackingPlot2D( const std::string& name, QHBoxLayout* layout );

    void setDisplay( std::string displayName, PlaneSide side, int value )
    {
        if( qBoxPainters.count(displayName) != 0 )
        {
            qBoxPainters[displayName]->setDisplay(side,value);
        }
    }

    void setPlaneWidthInPercent( int quantity )
    {
        ParticlesPhysicsManager::setPlaneWidthInPercent(quantity);        
    }

    void paintParticlesFrame();
    void paintFrames();

    void pause( bool userCall = false )
    {
        particlesPaintManager->setVectorPaint(true);
        ParticlesPhysicsManager::pause(userCall);
    }

    void run( bool userCall = false )
    {
        particlesPaintManager->setVectorPaint(false);
        ParticlesPhysicsManager::run(userCall);
    }

    void enableTracking()
    {
        particlesPaintManager->setTrackingPaint(true);
        ParticlesPhysicsManager::enableTracking();
    }

    void disableTracking()
    {
        particlesPaintManager->setTrackingPaint(false);
        ParticlesPhysicsManager::disableTracking();
    }

    void setPlaneHitsPaint( bool value )
    {
        particlesPaintManager->setPlaneHitsPaint(value);
    }

};
