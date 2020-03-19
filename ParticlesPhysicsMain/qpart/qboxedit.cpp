#include "qboxedit.h"
#include "particle.h"

QBoxEdit::QBoxEdit( QWidget *parent )
: QBoxPainter { parent }
{
    setMouseTracking(true);
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

    vect2D direction = editedParticle.value()->velocity.getVectorOfLength( width()/2 - 2*handlerSize);
    handlerPosition = { origin.x() + static_cast<int>(direction.x) , origin.y() + static_cast<int>(direction.y) };
}

void QBoxEdit::init()
{    
}

void QBoxEdit::paint()
{
    if( editedParticle == std::nullopt ) return;

    painter.translate(0,0);
    painter.save();

    painter.setBrush(QColor(250,250,250));
    painter.setPen(boxStyle.colors[BoxColors::BIG_CIRCLE_PEN]);
    painter.drawEllipse( origin , width()/2-2*handlerSize , width()/2-2*handlerSize );

    paintArrow( vect2D(origin.x(),origin.y()) , editedParticle.value()->velocity.getVectorOfLength(width()/2-3*handlerSize) , 20 , 5 , boxStyle.colors[BoxColors::SELECTED_PARTICLE] );

    painter.setBrush(QBrush(boxStyle.colors[BoxColors::SELECTED_PARTICLE]));
    painter.drawEllipse( origin , 5,5 );

    if( editBoxPaintMode[EditBoxPaintMode::HANDLER_HOVER] ) painter.setBrush(QColor(50,50,195));
    else painter.setBrush(QColor(10,10,105));
    painter.setPen(QColor(50,50,195));
    painter.drawEllipse( handlerPosition, handlerSize,handlerSize );

    painter.restore();
}

void QBoxEdit::mouseMoveEvent( QMouseEvent* event )
{
    if( editedParticle == std::nullopt ) return;

    if( (handlerPosition - event->localPos()).manhattanLength()<handlerSize )
    {
       editBoxPaintMode[EditBoxPaintMode::HANDLER_HOVER] = true;
    }
    else editBoxPaintMode[EditBoxPaintMode::HANDLER_HOVER] = false;
}

void QBoxEdit::mousePressEvent( QMouseEvent* )
{

}

void QBoxEdit::mouseReleaseEvent( QMouseEvent* )
{

}
