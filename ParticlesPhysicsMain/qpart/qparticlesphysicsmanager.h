#pragma once

#include <QHBoxLayout>
#include <type_traits>
#include "qpaintermanager.h"
#include "qbarchart.h"
#include "qbardisplay.h"
#include "qhistogram1d.h"
#include "qinfodisplay.h"
#include "qcgaugewidget.h"
#include "qtrackingplot2d.h"
#include "qcirclecontrol.h"
#include "particlesphysicsmanager.h"

/** @file
 * @brief Class @ref QParticlesPhysicsManager
 */

/**
 * @class QParticlesPhysicsManager
 * @brief Adapts ParticlesPhysicsManager to QT environment.
 *
 * Manages creation a certain graphical layout by adding QGauge and QBoxPainter derived objects
 * to achive a given simulation requirements.
 * @warning Class cannot be assign, move or copy.
 * @author Tomasz Gburek
 * @date 2019
 */

/** @enum ControlType
 *  @brief Represents form of control in QBoxPainter derived objects.
 */
enum class ControlType
{
    CIRCLE_CONTROL , /**< circle control - used in QCircleCotrol */
    NO_CONTROL       /**< no control */
};

class QParticlesPhysicsManager : public ParticlesPhysicsManager
{

public:

    /**
     * @brief Constructor
     *
     * @param type                  simulation type
     * @param layout                QT layout for the particle plane
     */
    QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout );

    /**
     * @brief Copy constructor is deleted.
     */
    QParticlesPhysicsManager( const QParticlesPhysicsManager& ) = delete;

    /**
     * @brief Move constructor is deleted.
     */
    QParticlesPhysicsManager( QParticlesPhysicsManager&& ) = delete;

    /**
     * @brief Copy assigment operator is deleted.
     */
    QParticlesPhysicsManager& operator=( const QParticlesPhysicsManager& ) = delete;

    /**
     * @brief Move assigment operator is deleted.
     */
    QParticlesPhysicsManager& operator=( QParticlesPhysicsManager&& ) = delete;

    /**
     * @brief Adds a new QBarChart for a given layout.
     *
     * @param name                  name of bar chart
     * @param layout                a given layout for QBarChart    
     * @param style                 bar chart style
     */
    void addQBarChart( const std::string& name, QHBoxLayout* layout, BoxStyles style = BoxStyles::DEFAULT );

    /**
     * @brief Adds a new QCircleControl for a given layout.
     *
     * @param name                  name of circle control
     * @param layout                a given layout for QCircleControl
     */
    void addQCircleControl( const std::string& name, QHBoxLayout* layout );

    /**
     * @brief Adds a new QBarDisplay for a given layout.
     *
     * @param name                  name of bar display
     * @param layout                a given layout for QBarDisplay
     */
    void addQBarDisplay( const std::string& name, QHBoxLayout* layout );

    /**
     * @brief Adds a new QHistogram1D for a given layout.
     *
     * @param name                  name of histogram
     * @param layout                a given layout for QHistogram1D
     */
    void addQHistogram1D( const std::string& name, QHBoxLayout* layout );

    /**
     * @brief Adds a new QInfoDisplay for a given layout.
     *
     * @param name                  name of info display
     * @param layout                a given layout for QInfoDisplay
     */
    void addQInfoDisplay( const std::string& name, QHBoxLayout* layout );

    /**
     * @brief Adds a new QGauge for a given layout.
     *
     * @param name                  name of gauge
     * @param layout                a given layout for QGauge
     */
    void addQGauge( const std::string& name, QHBoxLayout* layout );

    /**
     * @brief Adds a new QTrackingPlot2D for a given layout.
     *
     * @param name                  name of tracking plot
     * @param layout                a given layout for QTrackingPlot2D
     */
    void addQTrackingPlot2D( const std::string& name, QHBoxLayout* layout );

    /**
     * @brief Sets new value to display in QInfoDisplay for a given box side.
     *
     * @param displayName           name of QInfoDisplay
     * @param side                  plane side corresponding to box side
     * @param newValue              new percent value to display
     */
    void setDisplay( std::string displayName, PlaneSide side, int value )
    {
        if( qBoxPainters.count(displayName) != 0 )
        {
            if( dynamic_cast<QInfoDisplay*>(qBoxPainters[displayName].get()) != nullptr )
            {
                dynamic_cast<QInfoDisplay*>(qBoxPainters[displayName].get())->setDisplay(side,value);
            }
        }
    }

    /**
     * @brief Paints particles plane.
     */
    void paintParticlesPlane();

    /**
     * @brief Paints layouts for QGauge and QBoxPainter derived objects. Updates data for them.
     */
    void paintLayouts();

    /**
     * @brief Handles QBoxPainter derived object having control.
     */
    void handleControls();

    /**
     * @brief Pauses simulation.
     *
     * Value userCall have to be true if pause is called from a user action.
     * Enables velocity vector paint for each particle.
     * @param userCall              use call flag
     */
    void pause( bool userCall = false )
    {
        particlesPaintManager->setVectorPaint(true);
        ParticlesPhysicsManager::pause(userCall);
    }

    /**
     * @brief Runs simulation.
     *
     * Value userCall have to be true if run is called from a user action.
     * Disables velocity vector paint for each particle.
     * @param userCall              use call flag
     */
    void run( bool userCall = false )
    {
        particlesPaintManager->setVectorPaint(false);
        ParticlesPhysicsManager::run(userCall);
    }

    /** Enables tracking of selected particle. */
    void enableTracking()
    {
        particlesPaintManager->setTrackingPaint(true);
        ParticlesPhysicsManager::enableTracking();
    }

    /** Disables tracking of selected particle. */
    void disableTracking()
    {
        particlesPaintManager->setTrackingPaint(false);
        ParticlesPhysicsManager::disableTracking();
    }

    /**
     * @brief Enables/Disables paint of plane hits.
     *
     * @param value                 true to enable otherwise disable
     */
    void setPlaneHitsPaint( bool value )
    {
        particlesPaintManager->setPlaneHitsPaint(value);
    }

private:

    /** Holds QPainterManager object. */
    std::unique_ptr<QPainterManager> particlesPaintManager;

    /** Maps unique pointers to QBoxPainter derived objects. */
    std::map<std::string,std::unique_ptr<QBoxPainter>> qBoxPainters;

    /** Maps control types for QBoxPainter derived objects. */
    std::map<std::string,ControlType> controlBoxType;

    /** Maps QGauges objects. */
    std::map<std::string,std::pair<std::unique_ptr<QcGaugeWidget>,std::unique_ptr<QcNeedleItem>>> qGauges;

    /** Gauge name label */
    QcLabelItem* gaugeNameLabel {nullptr};

    /**
     * @brief Creates particle paint manager.
     */
    void createParticlesPaintManager();

};
