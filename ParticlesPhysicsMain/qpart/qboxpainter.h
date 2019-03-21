#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "definitions.h"

class QBoxPainter : public QWidget
{
    Q_OBJECT

public:

    explicit QBoxPainter( QWidget* parentWidget = nullptr );

    QBoxPainter( const QBoxPainter& ) = delete;
    QBoxPainter( QBoxPainter&& ) = delete;

    QBoxPainter& operator=( const QBoxPainter& ) = delete;
    QBoxPainter& operator=( QBoxPainter&& ) = delete;

    virtual void setDisplay( PlaneSide, int ){}
    virtual void init() = 0; // do all necessary calulations after QBoxPainter is added to layout and parentWidget != nullptr    

protected:

    QPainter painter;
    QBrush   background;    

    QColor cPlaneBorder {120,120,120};

    int marginLeft             {20};
    int marginRight            {20};
    int marginTop              {20};
    int marginBottom           {20};
    int numberOfHorizontalAxes {5};
    int numberOfVerticalAxes   {5};
    int planeBorderWidth       {1};

    void paintEvent(QPaintEvent *event) override;
    void paintAxes();
    void paintPlaneBorder();
    void paintArrow( const vect2D& position, const vect2D& direction, qreal arrowSize, int arrowThick, QColor arrowColor );
    void paintTriangle( const vect2D& a, const vect2D& b, const vect2D& c, QColor color );
    int  getStringWidthInPixels( QString name );

private:

    virtual void paint() = 0;

};
