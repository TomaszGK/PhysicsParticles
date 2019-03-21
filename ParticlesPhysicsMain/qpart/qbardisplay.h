#pragma once

#include "qboxpainter.h"
#include "bardisplay.h"

class QBarDisplay : public QBoxPainter
{

    Q_OBJECT

public:
    explicit QBarDisplay( int _max = 100, ptrBarDisplay ptr = nullptr, QWidget* parentWidget = nullptr );

    QBarDisplay( const QBarDisplay& ) = default;
    QBarDisplay( QBarDisplay&& ) = default;

    QBarDisplay& operator=( const QBarDisplay& ) = default;
    QBarDisplay& operator=( QBarDisplay&& ) = default;   

    void init();

private:    

    std::vector<int> previousValues;

    ptrBarDisplay barDisplay;

    int    barWidth {0};
    double maxValue {1}; // maximum OY axis    
    int    max      {100};
    QColor upper {0,0,255};
    QColor lower {255,0,0};

    void paint();
    int  getNumberXPosition( const int& number );
};
