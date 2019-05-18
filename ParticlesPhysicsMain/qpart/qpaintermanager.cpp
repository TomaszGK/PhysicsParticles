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

            if( toHandlePlaneHits ) paintPlaneHit( particle );
        }        

        if( planeArea->getPlainDivider().isDividerInPlane() ) paintPlaneDivider();
        if( toVectorPaint ) handleCursorPosition();

        if( static_cast<int>(planeArea->getXConstraint())>planeBorderWidth ) paintPlaneConstraintWalls();

        painter.restore();
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

void QPainterManager::paintPlaneHit( citerParticle particle )
{
    double planeHitDiff {2};
    int    hitSize {particle->size};

    auto posx = static_cast<int>(particle->position.x)+planeBorderWidth;
    auto posy = static_cast<int>(particle->position.y)+planeBorderWidth;

    painter.setBrush(QColor(210,50,50));

    if( particle->position.y-particle->radius<planeHitDiff )
    {
        painter.drawRect( posx - hitSize , 0 , 2*hitSize , planeBorderWidth );
    }
    else if( particle->position.y+particle->radius>planeArea->getHeight()-planeHitDiff )
    {
        painter.drawRect( posx - hitSize , height() - planeBorderWidth , 2*hitSize , planeBorderWidth );
    }
    else if( particle->position.x-particle->radius<planeHitDiff )
    {
        painter.drawRect( 0 , posy - hitSize , planeBorderWidth , 2*hitSize );
    }
    else if( particle->position.x+particle->radius>planeArea->getWidth()-planeHitDiff )
    {
        painter.drawRect( width() - planeBorderWidth , posy - hitSize , planeBorderWidth , 2*hitSize );
    }
}

void QPainterManager::paintTracking( citerParticle particle )
{
    int size  {static_cast<int>(particle->radius/2)+1};
    int posx  {0};
    int posy  {0};
    int alpha {0};

    for( auto position = --particle->particlePositionsTracking.cend() ; position != particle->particlePositionsTracking.cbegin() ; --position )
    {
        posx = static_cast<int>(position->x+planeBorderWidth);
        posy = static_cast<int>(position->y+planeBorderWidth);        
        painter.setBrush(QColor(120+alpha,120+alpha,120+alpha));
        painter.drawEllipse(posx-size/2,posy-size/2,size,size);                
        if( ++alpha>100 ) alpha = 100;
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
