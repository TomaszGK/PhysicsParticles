#include "qboxpainter.h"

QBoxPainter::QBoxPainter( QWidget* parentWidget )
: QWidget {parentWidget}
{
    background = QBrush(QColor(145, 215, 215));    
}

void QBoxPainter::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), background);
    paintPlaneBorder();
    paint();
    painter.end();
}

void QBoxPainter::paintAxes()
{
    QPen linePen(QColor(100, 100, 100));
    painter.setPen(linePen);
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(marginLeft-1,marginTop-1,width()-(marginLeft+marginRight)+1,height()-(marginTop+marginBottom)+1);

    linePen.setColor(QColor(120, 120, 120));
    linePen.setStyle(Qt::DashLine);

    double axisDistance = (height()-(marginTop+marginBottom))/static_cast<double>(numberOfHorizontalAxes);
    int pos {0};

    for( int index=1 ; index<numberOfHorizontalAxes ; ++index )
    {
        painter.setPen(linePen);
        pos = marginTop-1+static_cast<int>(index*axisDistance);
        painter.drawLine(marginLeft+1,pos,width()-marginRight-1,pos);
    }

    axisDistance = (width()-(marginTop+marginBottom))/static_cast<double>(numberOfVerticalAxes);
    pos = 0;

    for( int index=1 ; index<numberOfVerticalAxes ; ++index )
    {
        painter.setPen(linePen);
        pos = marginLeft-1+static_cast<int>(index*axisDistance);
        painter.drawLine(pos,marginTop+1,pos,height()-marginBottom-1);
    }
}

void QBoxPainter::paintArrow( const vect2D& position, const vect2D& direction, const qreal& arrowSize, int arrowThick, const QColor& arrowColor )
{
    QPolygonF arrowHead;

    painter.setPen(QPen(QBrush(arrowColor),arrowThick));
    painter.setBrush(QBrush(arrowColor));

    vect2D directionCut = direction.getVectorOfLength( direction()-arrowSize );

    QPointF end { position.x+direction.x , position.y+direction.y };
    QLineF line { position.x+directionCut.x , position.y+directionCut.y , position.x , position.y };

    double angle = std::atan2(-line.dy(), line.dx());

    QPointF arrowP1 = end + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = end + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

    arrowHead << end << arrowP1 << arrowP2;
    painter.drawLine(line);
    painter.setPen(QPen(QBrush(arrowColor),3));
    painter.drawPolygon(arrowHead);
}

void QBoxPainter::paintTriangle( const vect2D &a, const vect2D &b, const vect2D &c, const QColor& color )
{
    QPainterPath path;
    path.moveTo(a.x,a.y);
    path.lineTo(b.x,b.y);
    path.lineTo(c.x,c.y);
    path.lineTo(a.x,a.y);

    painter.fillPath(path, QBrush(color));
}

int QBoxPainter::getStringWidthInPixels( const QString& name )
{
    QFontMetrics fm(parentWidget()->font());
    return fm.width(name);
}

void QBoxPainter::paintPlaneBorder()
{
    painter.setPen(QColor(cPlaneBorder));
    painter.setBrush(QBrush(cPlaneBorder));
    painter.drawRect(0,0,width(),planeBorderWidth);
    painter.drawRect(0,0,planeBorderWidth,height());
    painter.drawRect(width()-planeBorderWidth,0,planeBorderWidth,height());
    painter.drawRect(0,height()-planeBorderWidth,width(),planeBorderWidth);
}
