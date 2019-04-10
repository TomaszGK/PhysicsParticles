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
    inline QPoint getIndicator() const noexcept
    {
        return indicatorPos-origin;
    }

private:

    QColor bigCirclePenColor       {80,80,90};
    QColor smallCircleColor        {155,155,155};
    QColor smallCircleHookedColor  {100,100,255};

    void paint();
    void paintMarks();
    bool isCursorHookToSmallCircle( const QPoint& cursorPos );

    QPoint indicatorPos {0,0}; // small circle position
    QPoint origin       {0,0};
    int    bigCircleSize {50};
    int  smallCircleSize {15};

    bool smallCircleHooked  {false};
    bool smallCircleHovered {false};

};

