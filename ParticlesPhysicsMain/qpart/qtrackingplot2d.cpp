#include "qtrackingplot2d.h"

QTrackingPlot2D::QTrackingPlot2D( cptrParticlesContainer ptr, QWidget *parent )
: QBoxPainter { parent }, particles { std::move(ptr) }
{
    trackingParticle = particles->begin();
    boxStyle.marginTop = boxStyle.marginBottom = boxStyle.marginLeft = boxStyle.marginRight = static_cast<int>(trackingParticle->radius);
    plotPen.setWidth(2);
}

void QTrackingPlot2D::init()
{

}

void QTrackingPlot2D::paint()
{
    paintAxes();

    if( trackingParticle->particlePositionsTracking.size()<2 ) return;

    coord2D pos1,pos2;
    double alpha {255};

    double step = 255.0/trackingParticle->particlePositionsTracking.size();

    for( auto position = --trackingParticle->particlePositionsTracking.cend() ; position != trackingParticle->particlePositionsTracking.cbegin() ; --position )
    {
        pos1.set(static_cast<int>(position->x+boxStyle.planeBorderWidth),static_cast<int>(position->y+boxStyle.planeBorderWidth));
        if( pos2() )
        {
            plotPen.setColor(QColor(120,120,120,static_cast<int>(alpha)));
            painter.setPen(plotPen);
            painter.drawLine(pos1.x,pos1.y,pos2.x,pos2.y);
            alpha -= step;
        }
        pos2 = pos1;
    }
}
