#pragma once

#include <QHBoxLayout>
#include <QPoint>
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


/** @enum ControlType
 *  @brief Represents form of control in QBoxPainter derived objects.
 */
enum class ControlType
{
    CIRCLE_CONTROL , /**< circle control - used in QCircleCotrol */
    NO_CONTROL       /**< no control */
};

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

class QParticlesPhysicsManager final : public QObject , public ParticlesPhysicsManager
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param type                  simulation type
     * @param layout                QT layout for the particle plane
     */
    explicit QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout );

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
     * @brief Adds a new box type for a given layout.
     *
     * @param boxType               a given box type
     * @param actionType            a given action type
     * @param layout                a given layout for QBarChart
     * @param style                 bar chart style
     */
    void add( QHBoxLayout* layout, BoxType boxType, ActionType actionType, BoxStyles style = BoxStyles::DEFAULT );

    /**
     * @brief Sets new value to display in QInfoDisplay for a given box side.
     *
     * @param type                  type of action
     * @param side                  plane side corresponding to box side
     * @param newValue              new percent value to display
     */
    void setDisplay( ActionType type, PlanePart side, int value )
    {
        if( qBoxPainters.count(type) != 0 )
        {
            if( dynamic_cast<QInfoDisplay*>(qBoxPainters[type].data()) != nullptr )
            {
                dynamic_cast<QInfoDisplay*>(qBoxPainters[type].data())->setDisplay(side,value);
            }
        }
    }

    /**
     * @brief Updates plane of particles.
     */
    void updateParticlesPlane();

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
    void pause( bool userCall = false ) override
    {
        if( userCall ) particlesPaintManager->setAttribute(Qt::WA_TransparentForMouseEvents,false);
        ParticlesPhysicsManager::pause(userCall);
    }

    /**
     * @brief Runs simulation.
     *
     * Value userCall have to be true if run is called from a user action.
     * Disables velocity vector paint for each particle.
     * @param userCall              use call flag
     */
    void run( bool userCall = false ) override
    {
        if( userCall ) particlesPaintManager->setAttribute(Qt::WA_TransparentForMouseEvents);
        ParticlesPhysicsManager::run(userCall);
    }

    /** Enables tracking of selected particle. */
    void enableTracking() override
    {
        (*particlesPaintManager)[PaintMode::TRACKING] = true;
        ParticlesPhysicsManager::enableTracking();
    }

    /** Disables tracking of selected particle. */
    void disableTracking() override
    {
        (*particlesPaintManager)[PaintMode::TRACKING] = false;
        ParticlesPhysicsManager::disableTracking();
    }

    /**
     * @brief Sets percent value of plane width.
     * @param quantity              new percent value of plane width
     */
    virtual void setPlaneWidthInPercent( int quantity ) override
    {
        ParticlesPhysicsManager::setPlaneWidthInPercent(quantity);
        (*particlesPaintManager)[PaintMode::PLANE_CONSTRAINT] = (static_cast<int>(planeArea->getXConstraint())>0)?true:false;
    }

    /**
     * @brief Enables/Disables paint of plane hits.
     *
     * @param value                 true to enable otherwise disable
     */
    void setPlaneHitsPaint( bool value )
    {
        (*particlesPaintManager)[PaintMode::PLANE_HITS] = value;
    }

    /**
     * @brief Saves current state of particle plane to a given file name.
     *
     * Saves main state of particles ( e.g. positions and velocities ), plane temperatures and force values.
     * @param filename               file name
     */
    void saveState( QString filename = "" );

    /**
     * @brief Loads current state of particle plane from a given file name.
     *
     * Loads main state of particles ( e.g. positions and velocities ), plane temperatures and force values.
     * @param filename               file name
     */
    void loadState( QString filename );

private:

    /** Holds QPainterManager object. */
    QPointer<QPainterManager> particlesPaintManager;

    /** Maps unique pointers to QBoxPainter derived objects. */
    std::map<ActionType,QPointer<QBoxPainter>> qBoxPainters;

    /** Maps control types for QBoxPainter derived objects. */
    std::map<ActionType,ControlType> controlBoxType;

    /** Maps QGauges objects. */
    std::map<ActionType,std::pair<QPointer<QcGaugeWidget>,QPointer<QcNeedleItem>>> qGauges;

    /** Gauge name label */
    QcLabelItem* gaugeNameLabel {nullptr};

    /**
     * @brief Adds a new QGauge for a given layout.
     *
     * @param type                  action type
     * @param layout                a given layout for QGauge
     */
    void addQGauge( ActionType type, QHBoxLayout* layout );


private slots:

    /**
     * @brief Handles change of particle position.
     *
     * @param particle              particle iteartor
     * @param newPosition           new particle position
     */
    void particlePositionChanged( citerParticle particle , vect2D newPosition );

    /**
     * @brief Handles change of particle velocity.
     *
     * @param particle              particle iteartor
     * @param newVelocity           new particle velocity
     */
    void particleVelocityChanged( citerParticle particle , vect2D newVelocity );

};
