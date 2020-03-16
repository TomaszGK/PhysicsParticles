#include "qboxedit.h"

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

}


