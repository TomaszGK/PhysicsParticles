#pragma once

#include <QObject>
#include "qboxpainter.h"

/** @file
 * @brief Class @ref QBoxEdit
 */

/**
 * @class QBoxEdit
 * @brief Implements visualisation of edit box.
 *
 * Paints edit box where some particle quantities can be modified.
 * @author Tomasz Gburek
 * @date 2020
 */

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

    /**
     * @brief Sets iterator to edited particle
     *
     * @param editedParticle_       particle iterator
     */
    void setEditedParticle( std::optional<citerParticle> editedParticle_ )
    {
       editedParticle = editedParticle_;
    }

private:

    /**
     * @brief Paints edit box.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QEditBox state.
     */
    void init() override;

    /** Stores constant iterator to edited particle. */
    std::optional<citerParticle> editedParticle {std::nullopt};

};
