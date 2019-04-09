#pragma once

#include "qboxpainter.h"

class QCircleControl : public QBoxPainter
{

    Q_OBJECT

public:
    explicit QCircleControl( QWidget* parentWidget = nullptr );

    QCircleControl( const QCircleControl& ) = default;
    QCircleControl( QCircleControl&& ) = default;

    QCircleControl& operator=( const QCircleControl& ) = default;
    QCircleControl& operator=( QCircleControl&& ) = default;

    void init();

private:

    QColor bigCirclePenColor {55,55,55};
    QColor smallCirclePenColor {55,55,55};
    QColor smallCircleBrushColor {155,155,155};

    void paint();
    bool isCursorHookToSmallCircle( const QPoint& cursorPos );

    QPoint indicatorPos {0,0}; // small circle position
    QPoint origin       {0,0};
    int    bigCircleSize {50};
    int  smallCircleSize {15};

    bool smallCircleHooked {false};

};

