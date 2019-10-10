#include "qcirclecontrol.h"
#include <QApplication>

QCircleControl::QCircleControl( QWidget* parentWidget )
: QBoxPainter {parentWidget}
{    
    init();
}

bool QCircleControl::loadStyle(BoxStyles style)
{
    return false;
}

void QCircleControl::init()
{
    if( parentWidget() != nullptr )
    {
        origin.rx() = parentWidget()->width()/2;
        origin.ry() = parentWidget()->height()/2;

        bigCircleSize = ( parentWidget()->width() > parentWidget()->height() ) ?  origin.y()-1 :  origin.x()-1 ;
        indicatorPos = origin;        
        boxStyle.cBackground = boxStyle.cPlaneBorder = parentWidget()->palette().window().color();
    }
}

void QCircleControl::paint()
{
    QPoint cursorPos = mapFromGlobal(QCursor::pos());
    QColor currentSmallCircleColor {boxStyle.cSmallCircleColor};

    if( smallCircleHooked || smallCircleHovered ) currentSmallCircleColor = boxStyle.cSmallCircleHookedColor;

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QBrush(boxStyle.cBigCirclePenColor),2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter.drawEllipse( origin, bigCircleSize, bigCircleSize );

    paintMarks();

    painter.setBrush(QBrush(currentSmallCircleColor));
    painter.setPen(QPen(boxStyle.cBigCirclePenColor));
    painter.drawLine(origin.x(),origin.y(),indicatorPos.x(),indicatorPos.y());
    painter.drawEllipse( indicatorPos, smallCircleSize, smallCircleSize );

    if( isCursorHookToSmallCircle(cursorPos) )
    {
        double distance = sqrt( pow(cursorPos.x()-origin.x(),2) + pow(cursorPos.y()-origin.y(),2) );
        if( distance < static_cast<double>(bigCircleSize - smallCircleSize) ) indicatorPos = mapFromGlobal(QCursor::pos());
    }

    if( !smallCircleHooked && indicatorPos!=origin )
    {
        indicatorPos = origin;
        smallCircleHovered = false;
    }
}

void QCircleControl::paintMarks()
{
    painter.setPen(QPen(QBrush(QColor(80,80,90)),2,Qt::DashDotLine,Qt::RoundCap,Qt::RoundJoin));
    painter.drawLine(origin.x()-bigCircleSize,origin.y(),origin.x()+bigCircleSize,origin.y());
    painter.drawLine(origin.x(),origin.y()-bigCircleSize,origin.x(),origin.y()+bigCircleSize);
}

bool QCircleControl::isCursorHookToSmallCircle( const QPoint& cursorPos )
{
    if( smallCircleHooked && QApplication::mouseButtons() == Qt::LeftButton ) return true;

    smallCircleHooked = smallCircleHovered = false;

    if( cursorPos.x()>(indicatorPos.x()+smallCircleSize) || cursorPos.x()<(indicatorPos.x()-smallCircleSize) ) return false;
    if( cursorPos.y()>(indicatorPos.y()+smallCircleSize) || cursorPos.y()<(indicatorPos.y()-smallCircleSize) ) return false;

    smallCircleHovered = true;

    if( QApplication::mouseButtons() != Qt::LeftButton ) return false;

    smallCircleHooked = true;

    return true;
}
