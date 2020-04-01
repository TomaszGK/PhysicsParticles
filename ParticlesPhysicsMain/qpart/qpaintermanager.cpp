#include "qpaintermanager.h"
#include <QApplication>

QPainterManager::QPainterManager( QWidget* parent )
: QBoxPainter {parent}
{
    setMouseTracking(true);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    editBox = new QBoxEdit(this);    
    editBox->hide();

    boxStyle.colors[BoxColors::BACKGROUND] = QColor(235,235,235);
    particlePen = QPen(Qt::NoPen);
    particlePen.setWidth(1);

    setAutoFillBackground(false);

    particles = Locator::getParticles();
    planeArea = Locator::getPlaneArea();

    if( !particles || !planeArea ) throw std::invalid_argument("Particles or planeArea has nullptr");

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
    boxStyle.values[BoxValues::PLANE_BORDER_WIDTH] = static_cast<int>(planeArea->getPlaneBorderWidth());
    paintMode[PaintMode::DIVIDER] = planeArea->getPlainDivider().isDividerInPlane();
}

void QPainterManager::paint()
{
    painter.translate(0,0);
    painter.save();

    if( paintMode[PaintMode::TRACKING] ) paintTracking();

    for( auto particle = particles->cend() ; particle-- != particles->cbegin() ; )
    {
        attachParticleColor(particle);
        paintParticle(particle,particleColor);

        if( paintMode[PaintMode::PLANE_HITS] ) paintPlaneHit(particle);
    }

    if( paintMode[PaintMode::DIVIDER] ) paintPlaneDivider();        
    if( paintMode[PaintMode::PLANE_CONSTRAINT] ) paintPlaneConstraintWalls();

    if( !testAttribute(Qt::WA_TransparentForMouseEvents) )
    {
        if( paintMode[PaintMode::PARTICLE_VECTOR] ) paintParticleVelocityVector(selectedParticle.value());
        if( paintMode[PaintMode::EDIT] )
        {
            paintEditParticle();
            adjustBoxEditOrientation();
        }
    }

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
    paintArrow( position , direction , boxStyle.colors[BoxColors::CONSTRAINT_ARROW] , {50,50,15} );

    position.set( static_cast<double>(width()) + 150 , static_cast<double>(height()/2) );
    direction.set( (-1)*(150 + static_cast<double>(planeArea->getXConstraint())) , 0 );
    paintArrow( position , direction , boxStyle.colors[BoxColors::CONSTRAINT_ARROW] , {50,50,15} );
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

void QPainterManager::paintTracking()
{   
    if( trackingParticle == std::nullopt )
    {
        for( auto particle = particles->begin() ; particle !=  particles->end() ; ++particle )
        {
            if( particle->isTracking )
            {
                trackingParticle = particle;
                break;
            }
        }
    }

    int size  {static_cast<int>(trackingParticle.value()->radius/2)+1};
    int posx  {0};
    int posy  {0};
    int alpha {0};

    for( auto position = --trackingParticle.value()->particlePositionsTracking.cend() ; position != trackingParticle.value()->particlePositionsTracking.cbegin() ; --position )
    {
        posx = static_cast<int>(position->x+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
        posy = static_cast<int>(position->y+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
        painter.setBrush(QColor(120+alpha,120+alpha,120+alpha));
        painter.drawEllipse(posx-size/2,posy-size/2,size,size);                
        if( ++alpha>100 ) alpha = 100;
    }
}

void QPainterManager::paintParticleVelocityVector( citerParticle particle )
{
    auto position = particle->position + vect2D(boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
    auto normVelocity = static_cast<int>(100*particle->velocity());

    paintArrow( position + particle->velocity.getVectorOfLength(particle->radius) , particle->velocity.getVectorOfLength(100) , boxStyle.colors[BoxColors::SELECTED_PARTICLE] , {25,25,6} );
    paintParticle( static_cast<int>(position.x) , static_cast<int>(position.y) , 40 , boxStyle.colors[BoxColors::SELECTED_PARTICLE] );
    painter.setPen( boxStyle.colors[BoxColors::SELECTED_PARTICLE_LABEL] );
    painter.drawText(static_cast<int>(position.x)-QFontMetrics(this->font()).horizontalAdvance(QString::number(normVelocity))/2,static_cast<int>(position.y)+5,QString::number(normVelocity));
}

void QPainterManager::paintEditParticle()
{
    if( !selectedParticle ) return;
    paintParticle( selectedParticle.value() , boxStyle.colors[BoxColors::EDIT_SELECTED_PARTICLE] );    
}

void QPainterManager::attachParticleColor( citerParticle particle )
{
    if( ( paintMode[PaintMode::TRACKING] && ( trackingParticle != std::nullopt && trackingParticle.value()!=particle ) ) || paintMode[PaintMode::EDIT] )
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

bool QPainterManager::setOverlapParticle( const QPointF& moseposition )
{
    for( auto particle = particles->begin() ; particle !=  particles->end() ; ++particle )
    {
        auto position = particle->position + vect2D(boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);

        if( abs(position.x-moseposition.x())<particle->radius && abs(position.y-moseposition.y())<particle->radius )
        {
            selectedParticle = particle;
            particleShift.set( moseposition.x() - selectedParticle.value()->position.x , moseposition.y() - selectedParticle.value()->position.y );
            return true;
        }
    }
    return false;
}

void QPainterManager::adjustBoxEditOrientation()
{
    auto posx = static_cast<int>( selectedParticle.value()->position.x )+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];
    auto posy = static_cast<int>( selectedParticle.value()->position.y )+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];

    auto fitRight = posx + selectedParticle.value()->size/2+boxStyle.values[BoxValues::BOX_EDIT_INDENT] + editBox->width() < planeArea->getWidth();
    auto fitDown  = posy + editBox->height() < planeArea->getHeight();

    if( fitRight )
    {
        posx += selectedParticle.value()->size/2+boxStyle.values[BoxValues::BOX_EDIT_INDENT];
    }
    else
    {
        posx -= (selectedParticle.value()->size/2+boxStyle.values[BoxValues::BOX_EDIT_INDENT]+editBox->width());
    }

    if( fitDown )
    {
        posy -= selectedParticle.value()->size/2;
    }
    else
    {
        posy -= (editBox->height()-selectedParticle.value()->size/2);
    }

    editBox->move(posx,posy);
}

void QPainterManager::tryChangeParticlePosition( QPointF newPosition )
{
    if( !selectedParticle ) return;
    particlePositionChanged( selectedParticle.value() , {newPosition.x()-particleShift.x,newPosition.y()-particleShift.y} );
}

void QPainterManager::mouseMoveEvent( QMouseEvent *event )
{
    if( paintMode[PaintMode::EDIT] )
    {        
        if( event->buttons()&Qt::LeftButton ) tryChangeParticlePosition( event->localPos() );
    }
    else paintMode[PaintMode::PARTICLE_VECTOR] = setOverlapParticle(event->localPos());
}

void QPainterManager::mousePressEvent( QMouseEvent *event )
{
    if( paintMode[PaintMode::TRACKING] ) return;

    if( event->buttons()&Qt::LeftButton )
    {
        if( setOverlapParticle(event->localPos()) )
        {
            paintMode[PaintMode::EDIT] = true;
            paintMode[PaintMode::PARTICLE_VECTOR] = false;
            editBox->setEditedParticle(selectedParticle);
            editBox->show();
        }
        else
        {
            if( paintMode[PaintMode::EDIT] ) tryChangeParticlePosition( event->localPos() );
        }
    }
    if( event->buttons()&Qt::RightButton )
    {
        paintMode[PaintMode::EDIT] = false;
        paintMode[PaintMode::PARTICLE_VECTOR] = false;
        editBox->hide();
    }
}

void QPainterManager::mouseReleaseEvent( QMouseEvent* )
{

}

void QPainterManager::leaveEvent( QEvent* )
{
    paintMode[PaintMode::EDIT] = false;
    paintMode[PaintMode::PARTICLE_VECTOR] = false;
    editBox->hide();
    selectedParticle = std::nullopt;
}
