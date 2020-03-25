#include "qboxedit.h"
#include "particle.h"

QBoxEdit::QBoxEdit( QWidget *parent )
: QBoxPainter { parent }
{
    setMouseTracking(true);
    setObjectName("EditBox");
    resize(150,200);

    velocityMagnitude = new QSlider(Qt::Horizontal,this);
    velocityMagnitude->resize(130,25);
    velocityMagnitude->move(10,165);
    velocityMagnitude->setRange(1,100);

    connect(velocityMagnitude,&QSlider::valueChanged,this,&QBoxEdit::on_velocityMagnitude_valueChanged);

    boxStyle.colors[BoxColors::BACKGROUND] = QColor(235,235,235);
}

bool QBoxEdit::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        return true;
    }
    else return false;
}

void QBoxEdit::setEditedParticle( std::optional<citerParticle> editedParticle_ )
{
    editedParticle = editedParticle_;
    origin = {width()/2,width()/2};
    if( editedParticle == std::nullopt ) return;
    velocityMagnitude->setValue(static_cast<int>(editedParticle.value()->velocity()*100));
}

void QBoxEdit::init()
{    
}

void QBoxEdit::paint()
{
    if( editedParticle == std::nullopt ) return;

    vect2D direction = editedParticle.value()->velocity.getVectorOfLength( width()/2 - 2*handlerSize);
    handlerPosition = { origin.x() + static_cast<int>(direction.x) , origin.y() + static_cast<int>(direction.y) };

    painter.translate(0,0);
    painter.save();

    painter.setBrush(QColor(250,250,250));
    painter.setPen(boxStyle.colors[BoxColors::BIG_CIRCLE_PEN]);
    painter.drawEllipse( origin , width()/2-2*handlerSize , width()/2-2*handlerSize );

    paintArrow( vect2D(origin.x(),origin.y()) , editedParticle.value()->velocity.getVectorOfLength(width()/2-3*handlerSize) , boxStyle.colors[BoxColors::SELECTED_PARTICLE] , {20,15,5} );

    painter.setBrush(QBrush(boxStyle.colors[BoxColors::SELECTED_PARTICLE]));
    painter.drawEllipse( origin , 4,4 );

    if( editBoxPaintMode[EditBoxPaintMode::HANDLER_HOVER] ) painter.setBrush(QColor(10,10,105));
    else painter.setBrush(QColor(80,80,230));

    painter.setPen(QColor(50,50,195));
    painter.drawEllipse( handlerPosition, handlerSize,handlerSize );

    painter.setPen(QColor(80,80,80));
    auto posx = static_cast<double>(velocityMagnitude->width()-10)*(static_cast<double>(velocityMagnitude->value())/static_cast<double>(abs(velocityMagnitude->maximum()-velocityMagnitude->minimum())));
    auto text = QString::number(velocityMagnitude->value());
    painter.drawText(velocityMagnitude->pos().x()+4+static_cast<int>(posx)-getStringWidthInPixels(text)/2,velocityMagnitude->pos().y()-8,text);

    painter.restore();
}

void QBoxEdit::mouseMoveEvent( QMouseEvent* event )
{
    if( editedParticle == std::nullopt ) return;

    if( !editBoxPaintMode[EditBoxPaintMode::HANDLER_SELECT] )
    {
        if( (handlerPosition-event->localPos()).manhattanLength() < handlerSize )
        {
            editBoxPaintMode[EditBoxPaintMode::HANDLER_HOVER] = true;
        }
        else editBoxPaintMode[EditBoxPaintMode::HANDLER_HOVER] = false;
    }
    else
    {
        vect2D newVelocity { event->localPos().x()-origin.x(),event->localPos().y()-origin.y() };
        newVelocity.setLength( editedParticle.value()->velocity() );
        emit particleVelocityChanged(editedParticle.value(),newVelocity);
    }
}

void QBoxEdit::mousePressEvent( QMouseEvent* )
{
    if( editBoxPaintMode[EditBoxPaintMode::HANDLER_HOVER] )
    {
         editBoxPaintMode[EditBoxPaintMode::HANDLER_SELECT] = true;
    }
}

void QBoxEdit::mouseReleaseEvent( QMouseEvent* )
{
    editBoxPaintMode[EditBoxPaintMode::HANDLER_SELECT] = false;
}

void QBoxEdit::on_velocityMagnitude_valueChanged( int value )
{
    if( editedParticle == std::nullopt ) return;
    vect2D newVelocity { editedParticle.value()->velocity };
    newVelocity.setLength( static_cast<double>(value)*0.01 );
    emit particleVelocityChanged(editedParticle.value(),newVelocity);
}
