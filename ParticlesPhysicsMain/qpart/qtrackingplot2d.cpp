#include "qtrackingplot2d.h"
#include "langmanager.h"
#include "particlesphysicsmanager.h"

QTrackingPlot2D::QTrackingPlot2D( QString _title, QWidget *parent )
: QBoxPainter { parent },  title { std::move(_title) }
{
    particles = ParticlesPhysicsManager::Locator::getConstParticles();
    if( particles != nullptr )
    {
        trackingParticle = particles->begin();
        boxStyle.marginTop = boxStyle.marginBottom = boxStyle.marginLeft = boxStyle.marginRight = 2*static_cast<int>(trackingParticle->radius);
    }
    plotPen.setWidth(2);
}

bool QTrackingPlot2D::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        // to do some needed recalculations
        return true;
    }
    else return false;
}

void QTrackingPlot2D::init()
{

}

void QTrackingPlot2D::paint()
{
    paintAxes();

    painter.setPen(QPen(boxStyle.cLabelColor));
    painter.drawText(calculateCenterTextPosition(LangManager::translate(title),boxStyle.marginLeft,width()-boxStyle.marginRight),boxStyle.marginTop-12,LangManager::translate(title));

    if( trackingParticle->particlePositionsTracking.size()<2 ) return;

    coord2D pos1,pos2;
    double alpha {255};

    double transformX { (width()-boxStyle.marginLeft-boxStyle.marginRight)/static_cast<double>(width()) };
    double transformY { (height()-boxStyle.marginTop-boxStyle.marginBottom)/static_cast<double>(height()) };

    double step = 255.0/trackingParticle->particlePositionsTracking.size();

    for( auto position = --trackingParticle->particlePositionsTracking.cend() ; position != trackingParticle->particlePositionsTracking.cbegin() ; --position )
    {
        pos1.set(boxStyle.marginLeft+static_cast<int>(position->x*transformX),boxStyle.marginTop+static_cast<int>(position->y*transformY));
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
