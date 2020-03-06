#include "qcirclecontrol.h"
#include <QApplication>

QCircleControl::QCircleControl( QWidget* parentWidget )
: QBoxPainter {parentWidget}
{    
    init();
}

bool QCircleControl::loadStyle( BoxStyles style )
{
    if( boxStyle.loadStyleFromFile(style) )
    {
        // to do some needed recalculations
        return true;
    }
    else return false;
}

void QCircleControl::init()
{
    if( parentWidget() != nullptr )
    {
        origin.rx() = parentWidget()->width()/2;
        origin.ry() = parentWidget()->height()/2;

        bigCircleSize = ( parentWidget()->width() > parentWidget()->height() ) ?  origin.y()-1 :  origin.x()-1 ;
        indicatorPos = origin;        
        boxStyle.colors["cBackground"] = boxStyle.colors["cPlaneBorder"] = parentWidget()->palette().window().color();
    }
}

void QCircleControl::paint()
{
    QPoint cursorPos = mapFromGlobal(QCursor::pos());
    QColor currentSmallCircleColor {boxStyle.colors["cSmallCircleColor"]};
    double distance {0};

    if( smallCircleHooked || smallCircleHovered ) currentSmallCircleColor = boxStyle.colors["cSmallCircleHookedColor"];

    if( indicatorPos!=origin )
    {
        int angle {0};
        distance = sqrt( pow(cursorPos.x()-origin.x(),2) + pow(cursorPos.y()-origin.y(),2) );

        if( indicatorPos.x()>origin.x() && indicatorPos.y()<origin.y() ) angle = 0;
        else if( indicatorPos.x()>origin.x() && indicatorPos.y()>origin.y() ) angle = 270;
        else if( indicatorPos.x()<origin.x() && indicatorPos.y()>origin.y() ) angle = 180;
        else if( indicatorPos.x()<origin.x() && indicatorPos.y()<origin.y() ) angle = 90;

        painter.setBrush(QColor(static_cast<int>(distance)%255,200,255-(static_cast<int>(distance)%255)));
        painter.drawPie(origin.x()-bigCircleSize,origin.y()-bigCircleSize,2*bigCircleSize,2*bigCircleSize,angle*16,90*16);
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QBrush(boxStyle.colors["cBigCirclePenColor"]),2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter.drawEllipse( origin, bigCircleSize, bigCircleSize );

    paintMarks();

    painter.setBrush(QBrush(currentSmallCircleColor));
    painter.setPen(QPen(boxStyle.colors["cBigCirclePenColor"]));
    painter.drawLine(origin.x(),origin.y(),indicatorPos.x(),indicatorPos.y());
    painter.drawEllipse( indicatorPos, smallCircleSize, smallCircleSize );

    if( isCursorHookToSmallCircle(cursorPos) )
    {        
        if( distance < static_cast<double>(bigCircleSize - smallCircleSize) ) indicatorPos = cursorPos;
        else
        {
            auto vec = cursorPos-origin;
            vect2D limit {static_cast<double>(vec.x()),static_cast<double>(vec.y())};
            limit.setLength(bigCircleSize - smallCircleSize);
            vec.setX( origin.x() + static_cast<int>(limit.x) );
            vec.setY( origin.y() + static_cast<int>(limit.y) );
            indicatorPos = vec;
        }
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
