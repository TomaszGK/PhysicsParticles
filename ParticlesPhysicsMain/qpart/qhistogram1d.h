#pragma once

#include "qboxpainter.h"
#include "histogram1d.h"

class QHistogram1D : public QBoxPainter
{

    Q_OBJECT

public:

    explicit QHistogram1D( double max = 1, ptrHistogram1D ptr = nullptr, QWidget* parent = nullptr );

    QHistogram1D( const QHistogram1D& ) = default;
    QHistogram1D( QHistogram1D&& ) = default;

    QHistogram1D& operator=( const QHistogram1D& ) = default;
    QHistogram1D& operator=( QHistogram1D&& ) = default;

    void setMaxOY( double max ) { maxValue = max*1.6; }   
    void init();

private:

    ptrHistogram1D histogram1D;

    QColor cBackground {145,215,215};
    QColor cValue      {100,20,20};    

    double barWidth      {0};
    double maxValue      {100}; // maximum OY axis
    int    labelPosition {0};

    void paint();
    void drawHistogramName();
    void drawMarking();

};
