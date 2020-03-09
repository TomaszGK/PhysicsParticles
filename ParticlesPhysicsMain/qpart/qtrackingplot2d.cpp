#include "qtrackingplot2d.h"
#include "langmanager.h"

QTrackingPlot2D::QTrackingPlot2D( QString _title, QWidget *parent )
: QBoxPainter { parent }, title { std::move(_title) }
{
    particles = Locator::getParticles();
    if( particles != nullptr )
    {
        trackingParticle = particles->begin();
        boxStyle.values["marginTop"] = boxStyle.values["marginBottom"] = boxStyle.values["marginLeft"] = boxStyle.values["marginRight"] = 2*static_cast<int>(trackingParticle->radius);
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

    painter.setPen(boxStyle.colors[BoxColors::LABEL]);
    painter.drawText(calculateCenterTextPosition(LangManager::translate(title),boxStyle.values["marginLeft"],width()-boxStyle.values["marginRight"]),boxStyle.values["marginTop"]-12,LangManager::translate(title));

    if( trackingParticle->particlePositionsTracking.size()<2 ) return;

    coord2D pos1,pos2;
    double alpha {255};

    double transformX { (width()-boxStyle.values["marginLeft"]-boxStyle.values["marginRight"])/static_cast<double>(width()) };
    double transformY { (height()-boxStyle.values["marginTop"]-boxStyle.values["marginBottom"])/static_cast<double>(height()) };

    double step = 255.0/trackingParticle->particlePositionsTracking.size();

    for( auto position = --trackingParticle->particlePositionsTracking.cend() ; position != trackingParticle->particlePositionsTracking.cbegin() ; --position )
    {
        pos1.set(boxStyle.values["marginLeft"]+static_cast<int>(position->x*transformX),boxStyle.values["marginTop"]+static_cast<int>(position->y*transformY));
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
