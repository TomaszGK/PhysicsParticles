#include "qcirclecontrol.h"
#include <QApplication>

QCircleControl::QCircleControl( QWidget* parentWidget )
: QBoxPainter { parentWidget }
{
    init();
}

void QCircleControl::init()
{
    if( parentWidget() != nullptr )
    {
        origin.rx() = parentWidget()->width()/2;
        origin.ry() = parentWidget()->height()/2;

        bigCircleSize = ( parentWidget()->width() > parentWidget()->height() ) ?  origin.y()-1 :  origin.x()-1 ;
        indicatorPos = origin;
        background = parentWidget()->palette().background();
        cPlaneBorder = background.color();
    }
}

void QCircleControl::paint()
{
    QPoint cursorPos = mapFromGlobal(QCursor::pos());

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(bigCirclePenColor));
    painter.drawEllipse( origin, bigCircleSize, bigCircleSize );

    painter.setBrush(QBrush(smallCircleBrushColor));
    painter.setPen(QPen(smallCirclePenColor));
    painter.drawEllipse( indicatorPos, smallCircleSize, smallCircleSize );

    if( isCursorHookToSmallCircle(cursorPos) )
    {
        double distance = sqrt( pow(cursorPos.x()-origin.x(),2) + pow(cursorPos.y()-origin.y(),2) );
        if( distance < static_cast<double>(bigCircleSize - smallCircleSize) ) indicatorPos = mapFromGlobal(QCursor::pos());
    }

    if( !smallCircleHooked && indicatorPos!=origin )
    {
        indicatorPos = origin;
    }
}

bool QCircleControl::isCursorHookToSmallCircle( const QPoint& cursorPos )
{

     if( QApplication::mouseButtons() != Qt::LeftButton )
     {
         smallCircleHooked = false;
         return false;
     }
     if( smallCircleHooked ) return true;
     if( cursorPos.x()>(indicatorPos.x()+smallCircleSize) || cursorPos.x()<(indicatorPos.x()-smallCircleSize) ) return false;
     if( cursorPos.y()>(indicatorPos.y()+smallCircleSize) || cursorPos.y()<(indicatorPos.y()-smallCircleSize) ) return false;

     smallCircleHooked = true;
     return true;
}
