#include "qpaintermanager.h"

#include <QTimer>
#include "particle.h"

QPainterManager::QPainterManager( cptrParticlesContainer ptrParticles, std::shared_ptr<const PlaneArea> ptrPlaneArea, QWidget* parent )
: QBoxPainter { parent }, particles { std::move(ptrParticles) }, planeArea { std::move(ptrPlaneArea) }
{
    boxStyle.cBackground = QColor(235, 235, 235);
    selectedParticleColor = QColor(32, 165, 56);
    particlePen = QPen(Qt::NoPen);
    particlePen.setWidth(1);
    displayVelocityVector.first = false;    

    setAutoFillBackground(false);

    init();
}

bool QPainterManager::loadStyle(BoxStyles style)
{
    return false;
}

void QPainterManager::init()
{
    if( planeArea != nullptr )
    {
        boxStyle.planeBorderWidth = static_cast<int>(planeArea->getPlaneBorderWidth());
    }
}

void QPainterManager::paint()
{
    if( particles != nullptr )
    {
        painter.translate(0,0);
        painter.save();       

        int size {0};
        int posx {0};
        int posy {0};       

        for( auto particle = particles->cend() ; particle-- != particles->cbegin() ; )
        {
            if( toTrackingPaint && particle->isTracking ) paintTracking(particle);

            size = particle->size;
            posx = static_cast<int>(particle->position.x)+boxStyle.planeBorderWidth;
            posy = static_cast<int>(particle->position.y)+boxStyle.planeBorderWidth;

            if( toTrackingPaint && !particle->isTracking ) particleColor = QColor(200,200,200);
            else particleColor = QColor(particle->color.R, particle->color.G, particle->color.B);

            paintParticle(posx,posy,size,particleColor);

            if( toHandlePlaneHits ) paintPlaneHit( particle );
        }        

        if( planeArea->getPlainDivider().isDividerInPlane() ) paintPlaneDivider();
        if( toVectorPaint ) handleCursorPosition();

        if( static_cast<int>(planeArea->getXConstraint())>boxStyle.planeBorderWidth ) paintPlaneConstraintWalls();

        painter.restore();
    }

}

void QPainterManager::paintPlaneConstraintWalls()
{
    painter.setPen(QPen(QColor(100, 100, 100)));
    painter.setBrush(QBrush(QColor(100, 100, 100)));
    painter.drawRect(static_cast<int>(planeArea->getXConstraint()),boxStyle.planeBorderWidth,3,geometry().height()-2*boxStyle.planeBorderWidth);
    painter.drawRect(geometry().width()-static_cast<int>(planeArea->getXConstraint()),boxStyle.planeBorderWidth,3,geometry().height()-2*boxStyle.planeBorderWidth);
    paintConstraintArrows();

    // mask arrows on edges by redrawing horizontal borders
    painter.setPen(QPen(boxStyle.cPlaneBorder));
    painter.setBrush(QBrush(boxStyle.cPlaneBorder));
    painter.drawRect(0,0,boxStyle.planeBorderWidth,geometry().height());
    painter.drawRect(geometry().width()-boxStyle.planeBorderWidth,0,boxStyle.planeBorderWidth,geometry().height());
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
    painter.setPen(boxStyle.cPlaneBorder);
    painter.setBrush(boxStyle.cPlaneBorder);
    painter.drawRect( planeArea->getPlainDivider().getUpperRect().first.x+boxStyle.planeBorderWidth, planeArea->getPlainDivider().getUpperRect().first.y+boxStyle.planeBorderWidth, planeArea->getPlainDivider().getUpperRect().second.x, planeArea->getPlainDivider().getUpperRect().second.y );
    painter.drawRect( planeArea->getPlainDivider().getLowerRect().first.x+boxStyle.planeBorderWidth, planeArea->getPlainDivider().getLowerRect().first.y+boxStyle.planeBorderWidth, planeArea->getPlainDivider().getLowerRect().second.x, planeArea->getPlainDivider().getLowerRect().second.y );
}

void QPainterManager::paintPlaneHit( citerParticle particle )
{
    double planeHitDiff {2};
    int    hitSize {particle->size};

    auto posx = static_cast<int>(particle->position.x)+boxStyle.planeBorderWidth;
    auto posy = static_cast<int>(particle->position.y)+boxStyle.planeBorderWidth;

    painter.setBrush(QColor(210,50,50));

    if( particle->position.y-particle->radius<planeHitDiff )
    {
        painter.drawRect( posx - hitSize , 0 , 2*hitSize , boxStyle.planeBorderWidth );
    }
    else if( particle->position.y+particle->radius>planeArea->getHeight()-planeHitDiff )
    {
        painter.drawRect( posx - hitSize , height() - boxStyle.planeBorderWidth , 2*hitSize , boxStyle.planeBorderWidth );
    }
    else if( particle->position.x-particle->radius<planeHitDiff )
    {
        painter.drawRect( 0 , posy - hitSize , boxStyle.planeBorderWidth , 2*hitSize );
    }
    else if( particle->position.x+particle->radius>planeArea->getWidth()-planeHitDiff )
    {
        painter.drawRect( width() - boxStyle.planeBorderWidth , posy - hitSize , boxStyle.planeBorderWidth , 2*hitSize );
    }
}

void QPainterManager::paintParticle( int posx , int posy , int size , QColor color )
{
    QRadialGradient gradient;

    gradient.setRadius(size/2);
    gradient.setCenter(posx,posy);
    gradient.setFocalPoint(posx,posy);
    gradient.setColorAt(0, boxStyle.cBackground);
    gradient.setColorAt(1, color);

    painter.setBrush(QBrush(gradient));
    painter.setPen(particlePen);
    painter.drawEllipse(posx-size/2,posy-size/2,size,size);
}

void QPainterManager::paintTracking( citerParticle particle )
{
    int size  {static_cast<int>(particle->radius/2)+1};
    int posx  {0};
    int posy  {0};
    int alpha {0};

    for( auto position = --particle->particlePositionsTracking.cend() ; position != particle->particlePositionsTracking.cbegin() ; --position )
    {
        posx = static_cast<int>(position->x+boxStyle.planeBorderWidth);
        posy = static_cast<int>(position->y+boxStyle.planeBorderWidth);
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
        position = particle->position + vect2D(boxStyle.planeBorderWidth,boxStyle.planeBorderWidth);

        if( abs(position.x-cursorPos.x())<particle->radius && abs(position.y-cursorPos.y())<particle->radius )
        {           
            displayVelocityVector.first = true;
            displayVelocityVector.second = particle;

            int posx   { static_cast<int>(particle->position.x)+boxStyle.planeBorderWidth };
            int posy   { static_cast<int>(particle->position.y)+boxStyle.planeBorderWidth };
            int number { particle->getCurrentVelocityPercent() };
            int shiftx {0};

            paintArrow( position + particle->velocity.getVectorOfLength(particle->radius) , particle->velocity.getVectorOfLength(100) , 25 , 6 , selectedParticleColor );
            paintParticle(posx,posy,40,selectedParticleColor);
            painter.setPen(QPen(QColor(10,15,10)));

            if( number<10 ) shiftx = 4;
            else if( number>9 && number<99 ) shiftx = 9;
            else shiftx = 13;

            painter.drawText(posx-shiftx,posy+5,QString::number(particle->getCurrentVelocityPercent()));
        }       
    }
}
