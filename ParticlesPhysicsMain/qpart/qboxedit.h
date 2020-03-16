#pragma once

#include <QObject>
#include "qboxpainter.h"

class QBoxEdit : public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param parent                pointer to parent widget
     */
    explicit QBoxEdit( QWidget *parent = nullptr );

    /**
     * @brief Loads style type from xml file and recalulate box display values.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * Recalculate box display values by adjusting marigins and align bar width.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

private:

    /**
     * @brief Paints edit box.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QEditBox state.
     */
    void init() override;

};
