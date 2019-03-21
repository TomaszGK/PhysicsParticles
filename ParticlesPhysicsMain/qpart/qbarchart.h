#pragma once

#include <QPushButton>
#include "qboxpainter.h"
#include "barchart.h"

enum class DataVisualization { BARS , POINTS, LINES };

class QBarChart : public QBoxPainter
{

    Q_OBJECT

public:

    explicit QBarChart( double max = 1, std::pair<bool,bool> scalability = {true,true}, ptrBarChart ptr = nullptr, QWidget* parent = nullptr );

    QBarChart( const QBarChart& ) = default;
    QBarChart( QBarChart&& ) = default;

    QBarChart& operator=( const QBarChart& ) = default;
    QBarChart& operator=( QBarChart&& ) = default;

    void init();
    void setMaxOY( double max ) { maxValue = max*1.6; }
    void setScalabality( bool _isScalableUp , bool _isScalableDown ) { isScalableUp = _isScalableUp ; isScalableDown = _isScalableDown; }

private:

    ptrBarChart barChart;

    std::map<DataVisualization,std::unique_ptr<QPushButton>> buttons;
    QString buttonStyleSelected;
    QString buttonStyleUnselected;

    QColor cValue        {100,20,20};
    QColor cBackground   {145,215,215};
    QColor cButtonActive {145,105,125};
    QColor cButton       {200,200,200};
    QColor cButtonLabel  {15,15,15};

    int    barWidth       {0};
    double maxValue       {1};    // maximum OY axis
    bool   isScalableUp   {true};
    bool   isScalableDown {true};
    int    labelPosition  {0};

    DataVisualization dataVisulization { DataVisualization::BARS };

    void paint();
    void drawCurrentValue();
    void drawChartName();

private slots:

    void onButtonClick();

};
