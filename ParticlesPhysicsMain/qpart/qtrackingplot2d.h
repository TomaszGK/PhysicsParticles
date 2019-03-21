#pragma once

#include "qboxpainter.h"
#include "particle.h"

class QTrackingPlot2D: public QBoxPainter
{

    Q_OBJECT

public:
    explicit QTrackingPlot2D( cptrParticlesContainer ptr, QWidget *parent = nullptr );

    QTrackingPlot2D( const QTrackingPlot2D& ) = default;
    QTrackingPlot2D( QTrackingPlot2D&& ) = default;

    QTrackingPlot2D& operator=( const QTrackingPlot2D& ) = default;
    QTrackingPlot2D& operator=( QTrackingPlot2D&& ) = default;

    void init();

private:

    QFont    textFont;
    QPen     plotPen;
    QPen     textPen;
    QColor   particleColor;

    cptrParticlesContainer particles;
    citerParticle trackingParticle;

    void paint();

};
