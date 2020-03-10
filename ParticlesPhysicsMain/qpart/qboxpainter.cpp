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
    painter.fillRect( event->rect(), QBrush(boxStyle.colors[BoxColors::BACKGROUND]) );
    paintPlaneBorder();
    paint();
    painter.end();
}

void QBoxPainter::paintAxes()
{
    QPen linePen(boxStyle.colors[BoxColors::AXES]);
    painter.setPen(linePen);
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(boxStyle.values[BoxValues::MARGIN_LEFT]-1,boxStyle.values[BoxValues::MARGIN_TOP]-1,width()-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT])+1,height()-(boxStyle.values[BoxValues::MARGIN_TOP]+boxStyle.values[BoxValues::MARGIN_BOTTOM])+1);

    linePen.setColor(boxStyle.colors[BoxColors::INNER_FRAME]);
    linePen.setStyle(Qt::DashLine);
    painter.setPen(linePen);

    double axisDistance { (height()-(boxStyle.values[BoxValues::MARGIN_TOP]+boxStyle.values[BoxValues::MARGIN_BOTTOM]))/static_cast<double>(boxStyle.values[BoxValues::NUMBER_OF_HORIZONTAL_AXES]+1) };
    int pos {0};

    for( int index=0 ; index<boxStyle.values[BoxValues::NUMBER_OF_HORIZONTAL_AXES] ; ++index )
    {        
        pos = boxStyle.values[BoxValues::MARGIN_TOP]-1+static_cast<int>((index+1)*axisDistance);
        painter.drawLine(boxStyle.values[BoxValues::MARGIN_LEFT]+1,pos,width()-boxStyle.values[BoxValues::MARGIN_RIGHT]-1,pos);
    }

    axisDistance = (width()-(boxStyle.values[BoxValues::MARGIN_LEFT]+boxStyle.values[BoxValues::MARGIN_RIGHT]))/static_cast<double>(boxStyle.values[BoxValues::NUMBER_OF_VERTICAL_AXES]+1);
    pos = 0;

    for( int index=0 ; index<boxStyle.values[BoxValues::NUMBER_OF_VERTICAL_AXES] ; ++index )
    {        
        pos = boxStyle.values[BoxValues::MARGIN_LEFT]-1+static_cast<int>((index+1)*axisDistance);
        painter.drawLine(pos,boxStyle.values[BoxValues::MARGIN_TOP]+1,pos,height()-boxStyle.values[BoxValues::MARGIN_BOTTOM]-1);
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
    painter.setPen(boxStyle.colors[BoxColors::PLANE_BORDER]);
    painter.setBrush(QColor(boxStyle.colors[BoxColors::PLANE_BORDER]));
    painter.drawRect(0,0,width(),boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
    painter.drawRect(0,0,boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],height());
    painter.drawRect(width()-boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],0,boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],height());
    painter.drawRect(0,height()-boxStyle.values[BoxValues::PLANE_BORDER_WIDTH],width(),boxStyle.values[BoxValues::PLANE_BORDER_WIDTH]);
}
