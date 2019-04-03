#include "qinteractivelabel.h"

QInteractiveLabel::QInteractiveLabel( QWidget *parent )
: QLabel(parent)
{

}

void QInteractiveLabel::setPicture( const QString& filename )
{
    QPixmap pixmap(":/new/icons/images/"+filename);

    pixmap = pixmap.scaled( parentWidget()->size(), Qt::IgnoreAspectRatio);
    setPixmap(pixmap);
}

void QInteractiveLabel::enterEvent( QEvent* )
{
    setPicture("Einstein_tongue.jpg");
}

void QInteractiveLabel::leaveEvent( QEvent* )
{
    setPicture("science.jpg");
}
