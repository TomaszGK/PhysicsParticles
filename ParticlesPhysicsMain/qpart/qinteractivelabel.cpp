#include "qinteractivelabel.h"

QInteractiveLabel::QInteractiveLabel( const QString& enterPicture, const QString& leavePicture, QWidget *parent )
:QLabel(parent), enterPicture {enterPicture} , leavePicture {leavePicture}
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
    setPicture(enterPicture);
}

void QInteractiveLabel::leaveEvent( QEvent* )
{
    setPicture(leavePicture);
}
