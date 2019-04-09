#include "qpaintermanager.h"

#include <QTimer>
#include "particle.h"

QPainterManager::QPainterManager( cptrParticlesContainer ptr, std::shared_ptr<const PlaneArea> ptrArea, QWidget* parent )
: QBoxPainter { parent }, particles { std::move(ptr) }, planeArea { std::move(ptrArea) }
{
    background = QBrush(QColor(235, 235, 235));
    particlePen = QPen(Qt::NoPen);
    particlePen.setWidth(1);
    textPen = QPen(Qt::white);    
    textFont.setPixelSize(50);    
    displayVelocityVector.first = false;    

    setAutoFillBackground(false);

    init();
}

void QPainterManager::init()
{
    if( planeArea != nullptr )
    {
        planeBorderWidth = static_cast<int>(planeArea->getPlaneBorderWidth());
    }
}

void QPainterManager::paint()
{
    if( particles != nullptr )
    {
        painter.translate(0,0);
        painter.save();       
        QRadialGradient gradient;
        int size {0};
        int posx {0};
        int posy {0};
        double planeHitDiff {2};

        for( auto particle = particles->cend() ; particle-- != particles->cbegin() ; )
        {
            if( toTrackingPaint && particle->isTracking ) paintTracking(particle);

            size = static_cast<int>(particle->size);
            posx = static_cast<int>(particle->position.x)+planeBorderWidth;
            posy = static_cast<int>(particle->position.y)+planeBorderWidth;

            gradient.setRadius(size/2);

            gradient.setCenter(posx,posy);
            gradient.setFocalPoint(posx,posy);

            if( toTrackingPaint && !particle->isTracking ) particleColor = QColor(200,200,200);
            else particleColor = QColor(particle->color.R, particle->color.G, particle->color.B);

            if( displayVelocityVector.first )
            {
                if( displayVelocityVector.second != particle )
                {
                    //particleColor = QColor(190,190,190);
                }
                else
                {
                    size *= 1.5;
                }
            }

            gradient.setColorAt(0, background.color());
            gradient.setColorAt(1, particleColor);
            painter.setBrush(QBrush(gradient));
            painter.setPen(particlePen);
            painter.drawEllipse(posx-size/2,posy-size/2,size,size);

            if( toHandlePlaneHits )
            {
                /*
                if( particle->position.y-particle->radius<planeHitDiff )                             planeHits.push_back( {0,{posx,0}} );
                else if( particle->position.y+particle->radius>planeArea->getHeight()-planeHitDiff ) planeHits.push_back( {0,{posx,height()}} );
                else if( particle->position.x-particle->radius<planeHitDiff )                        planeHits.push_back( {0,{0,posy}} );
                else if( particle->position.x+particle->radius>planeArea->getWidth()-planeHitDiff )  planeHits.push_back( {0,{width(),posy}} );
                handlePlaneHit();
                */

                if( particle->position.y-particle->radius<planeHitDiff )                             paintPlaneHit( {posx,0} );
                else if( particle->position.y+particle->radius>planeArea->getHeight()-planeHitDiff ) paintPlaneHit( {posx,height()} );
                else if( particle->position.x-particle->radius<planeHitDiff )                        paintPlaneHit( {0,posy} );
                else if( particle->position.x+particle->radius>planeArea->getWidth()-planeHitDiff )  paintPlaneHit( {width(),posy} );
            }
        }

        painter.restore();

        if( planeArea->getPlainDivider().isDividerInPlane() ) paintPlaneDivider();
        if( toVectorPaint ) handleCursorPosition();

        if( static_cast<int>(planeArea->getXConstraint())>planeBorderWidth ) paintPlaneConstraintWalls();
    }

}

void QPainterManager::paintPlaneConstraintWalls()
{
    painter.setPen(QPen(QColor(100, 100, 100)));
    painter.setBrush(QBrush(QColor(100, 100, 100)));
    painter.drawRect(static_cast<int>(planeArea->getXConstraint()),planeBorderWidth,3,geometry().height()-2*planeBorderWidth);
    painter.drawRect(geometry().width()-static_cast<int>(planeArea->getXConstraint()),planeBorderWidth,3,geometry().height()-2*planeBorderWidth);
    paintConstraintArrows();

    // mask arrows on edges by redrawing horizontal borders
    painter.setPen(QPen(cPlaneBorder));
    painter.setBrush(QBrush(cPlaneBorder));
    painter.drawRect(0,0,planeBorderWidth,geometry().height());
    painter.drawRect(geometry().width()-planeBorderWidth,0,planeBorderWidth,geometry().height());
}

void QPainterManager::paintConstraintArrows()
{
    vect2D position  { -150 , static_cast<double>(geometry().height()/2) };
    vect2D direction { 150 + static_cast<double>(static_cast<int>(planeArea->getXConstraint())) , 0 };
    paintArrow( position , direction , 50 , 15 , QColor(100,60,250) );

    position.set( static_cast<double>(geometry().width()) + 150 , static_cast<double>(geometry().height()/2) );
    direction.set( -static_cast<double>(static_cast<int>(planeArea->getXConstraint())) - 150 + 3 , 0 );
    paintArrow( position , direction , 50 , 15 , QColor(100,60,250) );
}

void QPainterManager::paintPlaneDivider()
{
    painter.setPen(cPlaneBorder);
    painter.setBrush(cPlaneBorder);
    painter.drawRect( planeArea->getPlainDivider().getUpperRect().first.x+planeBorderWidth, planeArea->getPlainDivider().getUpperRect().first.y+planeBorderWidth, planeArea->getPlainDivider().getUpperRect().second.x, planeArea->getPlainDivider().getUpperRect().second.y );
    painter.drawRect( planeArea->getPlainDivider().getLowerRect().first.x+planeBorderWidth, planeArea->getPlainDivider().getLowerRect().first.y+planeBorderWidth, planeArea->getPlainDivider().getLowerRect().second.x, planeArea->getPlainDivider().getLowerRect().second.y );
}

void QPainterManager::paintPlaneHit( coord2D position, double timeAfterHit )
{
    double alpha = 255*(1-timeAfterHit/maxTimeAfterHit);
    QColor hitColor(210,50,50,static_cast<int>(alpha));

    painter.setPen(hitColor);
    painter.setBrush(hitColor);
    if( position.y == 0 ) painter.drawRect( position.x - static_cast<int>(maxTimeAfterHit - timeAfterHit) , position.y , 2*static_cast<int>(maxTimeAfterHit - timeAfterHit) , position.y + planeBorderWidth );
    else if( position.x == 0 ) painter.drawRect( position.x , position.y - static_cast<int>(maxTimeAfterHit - timeAfterHit) , position.x + planeBorderWidth , 2*static_cast<int>(maxTimeAfterHit - timeAfterHit)  );
    else if( position.y == height() ) painter.drawRect( position.x - static_cast<int>(maxTimeAfterHit - timeAfterHit) , position.y - planeBorderWidth , 2*static_cast<int>(maxTimeAfterHit - timeAfterHit) , planeBorderWidth );
    else if( position.x == width()  ) painter.drawRect( position.x - planeBorderWidth , position.y - static_cast<int>(maxTimeAfterHit - timeAfterHit) , planeBorderWidth , 2*static_cast<int>(maxTimeAfterHit - timeAfterHit)  );
}

void QPainterManager::paintTracking( citerParticle particle )
{
    int size  {8};
    int posx  {0};
    int posy  {0};
    double alpha {255};

    double step = 255.0/particle->particlePositionsTracking.size();

    for( auto position = --particle->particlePositionsTracking.cend() ; position != particle->particlePositionsTracking.cbegin() ; --position )
    {
        posx = static_cast<int>(position->x+planeBorderWidth);
        posy = static_cast<int>(position->y+planeBorderWidth);
        painter.setBrush(QBrush(QColor(120,120,120,static_cast<int>(alpha))));
        painter.drawEllipse(posx-size/2,posy-size/2,size,size);
        alpha -= step;
    }
}

void QPainterManager::handleCursorPosition()
{
    QPoint cursorPos = mapFromGlobal(QCursor::pos());
    vect2D position;

    displayVelocityVector.first = false;

    for( auto particle = particles->begin() ; particle !=  particles->end() ; ++particle )
    {
        position = particle->position + vect2D(planeBorderWidth,planeBorderWidth);

        if( abs(position.x-cursorPos.x())<particle->radius && abs(position.y-cursorPos.y())<particle->radius )
        {           
            displayVelocityVector.first = true;
            displayVelocityVector.second = particle;
            paintArrow( position + particle->velocity.getVectorOfLength(particle->radius) , particle->velocity.getVectorOfLength(100) , 20 , 3 , QColor(particle->color.R, particle->color.G, particle->color.B) );
            painter.setPen(QPen(QColor(10,100,10)));
            painter.drawText(static_cast<int>(position.x-10),static_cast<int>(position.y-particle->radius*1.5-5),QString::number(particle->getCurrentVelocityPercent())+" %");
        }       
    }
}

void QPainterManager::handlePlaneHit()
{
    auto hit = planeHits.begin();

    while( hit != planeHits.end() )
    {
        hit->first += 0.002 ;
        if( hit->first >= maxTimeAfterHit )
        {
            hit = planeHits.erase(hit);
        }
        else
        {
            paintPlaneHit(hit->second,hit->first);
            ++hit;
        }
    }

}
