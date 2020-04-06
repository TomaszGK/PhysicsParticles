#include "qboxpainter.h"

QBoxPainter::QBoxPainter( QWidget* parentWidget )
: QWidget {parentWidget}
{ 
    QFont font = this->font();
    font.setPointSize(12);
    this->setFont(font);
}

void QBoxPainter::paintEvent( QPaintEvent *event )
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);    
    painter.fillRect( event->rect() , QBrush(boxStyle.colors[BoxColors::BACKGROUND]) );
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

void QBoxPainter::paintArrow( const vect2D& position, const vect2D& direction, const QColor& arrowColor, const ArrowShapeParameters& parameters )
{
    QPolygonF arrowHead;

    painter.setPen(QPen(QBrush(arrowColor),parameters.arrowThick));
    painter.setBrush(QBrush(arrowColor));

    vect2D directionCut = direction.getVectorOfLength( direction()-parameters.headLength );

    vect2D side = (!direction).getVectorOfLength(parameters.headThick/2);
    vect2D h { position.x+directionCut.x , position.y+directionCut.y };

    arrowHead << QPointF(position.x+direction.x,position.y+direction.y) << QPointF(h.x,h.y) + QPointF(side.x,side.y) << QPointF(h.x,h.y) - QPointF(side.x,side.y);

    painter.drawLine(QLineF(position.x+directionCut.x,position.y+directionCut.y,position.x,position.y));
    painter.setPen(QPen(QBrush(arrowColor),2));
    painter.drawPolygon(arrowHead);
}

void QBoxPainter::paintArrow( const QPoint &position, const QPoint &direction, const QColor& arrowColor, const ArrowShapeParameters& parameters )
{
    paintArrow(vect2D(position.x(),position.y()),vect2D(direction.x(),direction.y()),arrowColor,parameters);
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
