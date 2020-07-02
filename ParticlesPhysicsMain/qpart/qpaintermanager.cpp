#include "qpaintermanager.h"
#include <QApplication>

QPainterManager::QPainterManager( cptrParticlesContainer particles_ , cptrPlaneArea planeArea_ , QWidget *parent )
: QBoxPainter {parent}, particles {particles_}, planeArea { planeArea_ }
{
    setMouseTracking(true);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    editBox = new QBoxEdit(this);    
    editBox->hide();

    boxStyle.colors[BoxColors::BACKGROUND] = QColor(235,235,235);
    particlePen = QPen(Qt::NoPen);
    particlePen.setWidth(1);

    setAutoFillBackground(false);   

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

    //for( auto particle = particles->cend()-1 ; particle != particles->cbegin() ; --particle )
    for( auto particle = particles->cbegin() ; particle != particles->cend() ; ++particle )
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
            if( !isParticleOverlapWithCursor ) paintNewPositionOfParticle();
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

    for( auto position = trackingParticle.value()->particlePositionsTracking.rbegin() ; position != trackingParticle.value()->particlePositionsTracking.rend() ; ++position )
    {
        posx = static_cast<int>(position->x+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
        posy = static_cast<int>(position->y+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
        painter.setBrush(QColor(120+alpha,120+alpha,120+alpha));
        painter.drawEllipse(posx-size/2,posy-size/2,size,size);                
        if( ++alpha>100 ) alpha = 100;
    }
}

void QPainterManager::paintNewPositionOfParticle()
{
    if( !editedParticle ) return;

    auto pos = mapFromGlobal(QCursor::pos());
    QPointF posA {editedParticle.value()->position.x+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],editedParticle.value()->position.y+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]};

    QPen linePen(boxStyle.colors[BoxColors::AXES]);
    painter.setBrush(QBrush(Qt::NoBrush));
    linePen.setColor(boxStyle.colors[BoxColors::INNER_FRAME]);
    linePen.setStyle(Qt::DashLine);
    painter.setPen(linePen);
    painter.drawLine(QLineF(posA,pos));

    paintParticle( pos.x() , pos.y() , editedParticle.value()->size , QColor{100,200,200} );
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
    if( !editedParticle ) return;
    paintParticle( editedParticle.value() , boxStyle.colors[BoxColors::EDIT_SELECTED_PARTICLE] );
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
            particleShift.set( moseposition.x() - particle->position.x , moseposition.y() - particle->position.y );
            return true;
        }
    }
    return false;
}

void QPainterManager::adjustBoxEditOrientation()
{
    if( !editedParticle ) return;

    auto posx = static_cast<int>( editedParticle.value()->position.x )+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];
    auto posy = static_cast<int>( editedParticle.value()->position.y )+boxStyle.values[BoxValues::PLANE_BORDER_WIDTH];

    auto fitRight = posx + editedParticle.value()->size/2+boxStyle.values[BoxValues::BOX_EDIT_INDENT] + editBox->width() < planeArea->getWidth();
    auto fitDown  = posy + editBox->height() < planeArea->getHeight();

    if( fitRight )
    {
        posx += editedParticle.value()->size/2+boxStyle.values[BoxValues::BOX_EDIT_INDENT];
    }
    else
    {
        posx -= (editedParticle.value()->size/2+boxStyle.values[BoxValues::BOX_EDIT_INDENT]+editBox->width());
    }

    if( fitDown )
    {
        posy -= editedParticle.value()->size/2;
    }
    else
    {
        posy -= (editBox->height()-editedParticle.value()->size/2);
    }

    editBox->move(posx,posy);
}

void QPainterManager::tryChangeParticlePosition( QPointF newPosition )
{
    if( !editedParticle ) return;
    particlePositionChanged( editedParticle.value() , {newPosition.x()-boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],newPosition.y()-boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]} );
}

void QPainterManager::mouseMoveEvent( QMouseEvent *event )
{
    isParticleOverlapWithCursor = setOverlapParticle( event->localPos() );
    if( paintMode[PaintMode::EDIT] )
    {
        if( event->buttons()&Qt::LeftButton ) tryChangeParticlePosition( event->localPos() );        
    }
    else paintMode[PaintMode::PARTICLE_VECTOR] = isParticleOverlapWithCursor;
}

void QPainterManager::mousePressEvent( QMouseEvent *event )
{
    if( paintMode[PaintMode::TRACKING] ) return;

    if( event->buttons()&Qt::LeftButton )
    {
        if( isParticleOverlapWithCursor )
        {
            editedParticle = selectedParticle;
            paintMode[PaintMode::EDIT] = true;
            paintMode[PaintMode::PARTICLE_VECTOR] = false;
            editBox->setEditedParticle(editedParticle);
            editBox->show();           
        }
        else
        {
            if( paintMode[PaintMode::EDIT] ) tryChangeParticlePosition( event->localPos() );
        }
    }
    if( event->buttons()&Qt::RightButton )
    {
        editedParticle = std::nullopt;
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
