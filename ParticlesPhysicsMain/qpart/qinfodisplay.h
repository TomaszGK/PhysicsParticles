#pragma once

#include "qboxpainter.h"

class QInfoDisplay : public QBoxPainter
{

    Q_OBJECT

public:       

    explicit QInfoDisplay( QWidget* parentWidget = nullptr );

    QInfoDisplay( const QInfoDisplay& ) = default;
    QInfoDisplay( QInfoDisplay&& ) = default;

    QInfoDisplay& operator=( const QInfoDisplay& ) = default;
    QInfoDisplay& operator=( QInfoDisplay&& ) = default;

    void setDisplay( PlaneSide side, int _value );

private:    

    // values in percent
    std::map<PlaneSide,int> value { {PlaneSide::UP,0},{PlaneSide::DOWN,0},{PlaneSide::RIGHT,0},{PlaneSide::LEFT,0} };

    // string text width in pixels
    std::map<PlaneSide,int> textWidth { {PlaneSide::UP,0},{PlaneSide::DOWN,0},{PlaneSide::RIGHT,0},{PlaneSide::LEFT,0} };

    void paint() override;
    void init() override;

};
