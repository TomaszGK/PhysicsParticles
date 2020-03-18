#include "qboxedit.h"
#include "particle.h"

QBoxEdit::QBoxEdit( QWidget *parent )
: QBoxPainter { parent }
{

}

bool QBoxEdit::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        return true;
    }
    else return false;
}

void QBoxEdit::init()
{

}

void QBoxEdit::paint()
{
    if( editedParticle == std::nullopt ) return;

    painter.translate(0,0);
    painter.save();

    vect2D position {static_cast<double>(width()/2),static_cast<double>(height()/2)};
    paintArrow( position , editedParticle.value()->velocity.getVectorOfLength(75) , 15 , 5 , boxStyle.colors[BoxColors::SELECTED_PARTICLE] );

    painter.restore();

}


