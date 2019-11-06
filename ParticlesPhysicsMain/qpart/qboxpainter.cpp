#include "qboxpainter.h"

QBoxPainter::QBoxPainter( QWidget* parentWidget )
: QWidget {parentWidget}
{ 
    QFont font = this->font();
    font.setPointSize(12);
    this->setFont(font);
}

void QBoxPainter::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);    
    painter.fillRect( event->rect(), QBrush(boxStyle.cBackground) );
    paintPlaneBorder();
    paint();
    painter.end();
}

void QBoxPainter::paintAxes()
{
    QPen linePen(boxStyle.cAxesColor);
    painter.setPen(linePen);
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(boxStyle.marginLeft-1,boxStyle.marginTop-1,width()-(boxStyle.marginLeft+boxStyle.marginRight)+1,height()-(boxStyle.marginTop+boxStyle.marginBottom)+1);

    linePen.setColor(boxStyle.cInnerFrameColor);
    linePen.setStyle(Qt::DashLine);
    painter.setPen(linePen);

    double axisDistance { (height()-(boxStyle.marginTop+boxStyle.marginBottom))/static_cast<double>(boxStyle.numberOfHorizontalAxes+1) };
    int pos {0};

    for( int index=0 ; index<boxStyle.numberOfHorizontalAxes ; ++index )
    {        
        pos = boxStyle.marginTop-1+static_cast<int>((index+1)*axisDistance);
        painter.drawLine(boxStyle.marginLeft+1,pos,width()-boxStyle.marginRight-1,pos);
    }

    axisDistance = (width()-(boxStyle.marginLeft+boxStyle.marginRight))/static_cast<double>(boxStyle.numberOfVerticalAxes+1);
    pos = 0;

    for( int index=0 ; index<boxStyle.numberOfVerticalAxes ; ++index )
    {        
        pos = boxStyle.marginLeft-1+static_cast<int>((index+1)*axisDistance);
        painter.drawLine(pos,boxStyle.marginTop+1,pos,height()-boxStyle.marginBottom-1);
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
    return QFontMetrics(this->font()).horizontalAdvance(name);
}

int QBoxPainter::calculateCenterTextPosition( const QString &text , int begin , int end )
{
    auto pixelsWide = QFontMetrics(this->font()).horizontalAdvance(text);
    return begin + (end-begin-pixelsWide)/2;
}

void QBoxPainter::paintPlaneBorder()
{
    painter.setPen(QColor(boxStyle.cPlaneBorder));
    painter.setBrush(QBrush(boxStyle.cPlaneBorder));
    painter.drawRect(0,0,width(),boxStyle.planeBorderWidth);
    painter.drawRect(0,0,boxStyle.planeBorderWidth,height());
    painter.drawRect(width()-boxStyle.planeBorderWidth,0,boxStyle.planeBorderWidth,height());
    painter.drawRect(0,height()-boxStyle.planeBorderWidth,width(),boxStyle.planeBorderWidth);
}
