#include "qpaintermanager.h"
#include <QApplication>

QPainterManager::QPainterManager( QWidget* parent )
: QBoxPainter { parent }
{
    boxStyle.colors[BoxColors::BACKGROUND] = QColor(235,235,235);
    particlePen = QPen(Qt::NoPen);
    particlePen.setWidth(1);
    displayVelocityVector.first = false;    

    setAutoFillBackground(false);

    particles = Locator::getParticles();
    planeArea = Locator::getPlaneArea();

    init();
}

bool QPainterManager::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        // to do some necessary recalculations
        return true;
    }
    else return false;
}

void QPainterManager::init()
{    
    if( planeArea != nullptr )
    {
        boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] = static_cast<int>(planeArea->getPlaneBorderWidth());
        paintMode[PaintMode::DIVIDER] = planeArea->getPlainDivider().isDividerInPlane();
    }
}

void QPainterManager::paint()
{
    painter.translate(0,0);
    painter.save();

    for( auto particle = particles->cend() ; particle-- != particles->cbegin() ; )
    {
        attachParticleColor(particle);

        if( paintMode[PaintMode::TRACKING] && particle->isTracking ) paintTracking(particle);

        paintParticle(particle,particleColor);

        if( paintMode[PaintMode::PLANE_HITS] ) paintPlaneHit(particle);
     }

     if( paintMode[PaintMode::DIVIDER] ) paintPlaneDivider();
     if( paintMode[PaintMode::VECTOR] ) handleCursorPosition();
     if( paintMode[PaintMode::PLANE_CONSTRAINT] ) paintPlaneConstraintWalls();

     painter.restore();
}

void QPainterManager::paintPlaneConstraintWalls()
{
    painter.setPen(boxStyle.colors[BoxColors::PLANE_BORDER]);
    painter.setBrush(QColor(boxStyle.colors[BoxColors::PLANE_BORDER]));
    painter.drawRect(planeArea->getXConstraint(),boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],height()-2*boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
    painter.drawRect(width()-planeArea->getXConstraint()-boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],height()-2*boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);

    painter.setPen(boxStyle.colors[BoxColors::BACKGROUND]);
    painter.setBrush(QBrush(boxStyle.colors[BoxColors::BACKGROUND]));
    painter.drawRect(0,0,planeArea->getXConstraint(),height());
    painter.drawRect(width()-planeArea->getXConstraint(),0,planeArea->getXConstraint(),height());

    paintConstraintArrows();
}

void QPainterManager::paintConstraintArrows()
{
    vect2D position  { -150 , static_cast<double>(height()/2) };
    vect2D direction { 150 + static_cast<double>(planeArea->getXConstraint()) , 0 };
    paintArrow( position , direction , 50 , 15 , boxStyle.colors[BoxColors::CONSTRAINT_ARROW] );

    position.set( static_cast<double>(width()) + 150 , static_cast<double>(height()/2) );
    direction.set( (-1)*(150 + static_cast<double>(planeArea->getXConstraint())) , 0 );
    paintArrow( position , direction , 50 , 15 , boxStyle.colors[BoxColors::CONSTRAINT_ARROW] );
}

void QPainterManager::paintPlaneDivider()
{    
    painter.setPen(boxStyle.colors[BoxColors::PLANE_BORDER]);
    painter.setBrush(QColor(boxStyle.colors[BoxColors::PLANE_BORDER]));
    painter.drawRect( planeArea->getPlainDivider().getUpperRect().first.x+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH], planeArea->getPlainDivider().getUpperRect().first.y+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH], planeArea->getPlainDivider().getUpperRect().second.x, planeArea->getPlainDivider().getUpperRect().second.y );
    painter.drawRect( planeArea->getPlainDivider().getLowerRect().first.x+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH], planeArea->getPlainDivider().getLowerRect().first.y+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH], planeArea->getPlainDivider().getLowerRect().second.x, planeArea->getPlainDivider().getLowerRect().second.y );
}

void QPainterManager::paintPlaneHit( citerParticle particle )
{
    double planeHitDiff {2};
    int    hitSize {particle->size};

    auto posx = static_cast<int>(particle->position.x)+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];
    auto posy = static_cast<int>(particle->position.y)+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];

    painter.setBrush( QColor(boxStyle.colors[BoxColors::PLANE_HIT]) );

    if( particle->position.y-particle->radius<planeHitDiff )
    {
        painter.drawRect( posx - hitSize , 0 , 2*hitSize , boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] );
    }
    else if( particle->position.y+particle->radius>planeArea->getHeight()-planeHitDiff )
    {
        painter.drawRect( posx - hitSize , height() - boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] , 2*hitSize , boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] );
    }
    else if( particle->position.x-particle->radius<planeHitDiff )
    {
        painter.drawRect( 0 , posy - hitSize , boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] , 2*hitSize );
    }
    else if( particle->position.x+particle->radius>planeArea->getWidth()-planeHitDiff )
    {
        painter.drawRect( width() - boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] , posy - hitSize , boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] , 2*hitSize );
    }
}

void QPainterManager::paintParticle( int posx , int posy, int size, const QColor &color )
{
    QRadialGradient gradient;

    gradient.setRadius(size/2);
    gradient.setCenter(posx,posy);
    gradient.setFocalPoint(posx,posy);
    gradient.setColorAt(0, boxStyle.colors[BoxColors::BACKGROUND]);
    gradient.setColorAt(1,color);

    painter.setBrush(QBrush(gradient));
    painter.setPen(particlePen);
    painter.drawEllipse(posx-size/2,posy-size/2,size,size);
}

void QPainterManager::paintParticle( citerParticle particle , const QColor& color )
{
    QRadialGradient gradient;

    auto size = particle->size;
    auto posx = static_cast<int>(particle->position.x)+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];
    auto posy = static_cast<int>(particle->position.y)+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];

    gradient.setRadius(size/2);
    gradient.setCenter(posx,posy);
    gradient.setFocalPoint(posx,posy);
    gradient.setColorAt(0, boxStyle.colors[BoxColors::BACKGROUND]);
    gradient.setColorAt(1,color);

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
        posx = static_cast<int>(position->x+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
        posy = static_cast<int>(position->y+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
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
        position = particle->position + vect2D(boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);

        if( abs(position.x-cursorPos.x())<particle->radius && abs(position.y-cursorPos.y())<particle->radius )
        {           
            displayVelocityVector.first = true;
            displayVelocityVector.second = particle;

            auto normVelocity = static_cast<int>(100*particle->velocity());
            int posx { static_cast<int>(particle->position.x)+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] };
            int posy { static_cast<int>(particle->position.y)+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] };

            if(  QApplication::mouseButtons() == Qt::LeftButton )
            {
                paintParticle( particle , boxStyle.colors[BoxColors::EDIT_SELECTED_PARTICLE] );
            }
            else
            {
                paintArrow( position + particle->velocity.getVectorOfLength(particle->radius) , particle->velocity.getVectorOfLength(100) , 25 , 6 , boxStyle.colors[BoxColors::SELECTED_PARTICLE] );
                paintParticle( posx , posy , 40 , boxStyle.colors[BoxColors::SELECTED_PARTICLE] );
                painter.setPen( boxStyle.colors[BoxColors::SELECTED_PARTICLE_LABEL] );
                painter.drawText(posx-QFontMetrics(this->font()).horizontalAdvance(QString::number(normVelocity))/2,posy+5,QString::number(normVelocity));
            }
        }       
    }
}

void QPainterManager::attachParticleColor( citerParticle particle )
{
    if( paintMode[PaintMode::TRACKING] && !particle->isTracking )
    {
         particleColor = boxStyle.colors[BoxColors::DIM_PARTCILE];
    }
    else
    {
        if( particle->visualizationType == VisualizationType::VELOCITY )
        {
            unsigned char intensity = static_cast<unsigned char>(255.0*particle->velocity())%255;
            particleColor.setRgb(intensity,0,255-intensity);
        }
        else particleColor = boxStyle.colors[translation.at(particle->particleType)];
    }
}
