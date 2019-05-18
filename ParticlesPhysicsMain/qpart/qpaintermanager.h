#pragma once

#include "qboxpainter.h"
#include "planearea.h"

class QPainterManager : public QBoxPainter
{

    Q_OBJECT

public:
    explicit QPainterManager( cptrParticlesContainer ptr, std::shared_ptr<const PlaneArea> ptrArea, QWidget *parent = nullptr );

    QPainterManager( const QPainterManager& ) = default;
    QPainterManager( QPainterManager&& ) = default;

    QPainterManager& operator=( const QPainterManager& ) = default;
    QPainterManager& operator=( QPainterManager&& ) = default;

    void setParticlesContainer( const cptrParticlesContainer& _particles ) { particles = _particles; }
    void setVectorPaint( bool _toVectorPaint ) noexcept { toVectorPaint = _toVectorPaint; }
    void setTrackingPaint( bool _toTrackingPaint ) noexcept { toTrackingPaint = _toTrackingPaint; }    
    void setPlaneHitsPaint( bool _toHandlePlaneHits ) noexcept { toHandlePlaneHits = _toHandlePlaneHits; }

private:

    QFont    textFont;
    QPen     particlePen;
    QPen     textPen;   
    QColor   particleColor;

    QPoint   mousePos;

    std::pair<bool,std::vector<Particle>::const_iterator> displayVelocityVector;

    cptrParticlesContainer particles;
    std::shared_ptr<const PlaneArea> planeArea;   

    bool toVectorPaint {false};
    bool toTrackingPaint {false};
    bool toHandlePlaneHits {false};

    void paint() override;
    void init() override;

    void paintPlaneConstraintWalls();
    void paintConstraintArrows();
    void paintPlaneDivider();    
    void paintPlaneHit( citerParticle particle );
    void paintTracking( citerParticle particle );
    void handleCursorPosition();   

};
