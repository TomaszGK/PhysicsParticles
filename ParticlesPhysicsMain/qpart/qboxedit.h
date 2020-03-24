#pragma once

#include <QObject>
#include <QSlider>
#include "qboxpainter.h"

/** @file
 * @brief Class @ref QBoxEdit
 */


/** @enum EditBoxPaintMode
 *  @brief Represents edit box paint mode.
 */
enum class EditBoxPaintMode
{
   HANDLER_HOVER,  /**< mouse is hover over handler */
   HANDLER_SELECT  /**< handler is selected */
};

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
    void setEditedParticle( std::optional<citerParticle> editedParticle_ );

private:    

    /** slider to set velocity magnitude */
    QPointer<QSlider> velocityMagnitude;

    /** map of paint modes for edit box */
    std::map<EditBoxPaintMode,bool> editBoxPaintMode {
                                                       { EditBoxPaintMode::HANDLER_HOVER , false },
                                                       { EditBoxPaintMode::HANDLER_SELECT , false }
                                                     };

    /** position of handler operating direction of velocity vector */
    QPoint handlerPosition;

    /** circle origin */
    QPoint origin;

    /** handler size */
    int handlerSize {10};

    /** Stores constant iterator to edited particle. */
    std::optional<citerParticle> editedParticle {std::nullopt};

    /**
     * @brief Paints edit box.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QEditBox state.
     */
    void init() override;    

private slots:

    /**
     * @brief Mouse move handler.
     *
     * @param event                 pointer to mouse event
     */
    void mouseMoveEvent( QMouseEvent *event ) override;

    /**
     * @brief Mouse press button handler.
     *
     * @param event                 pointer to mouse event
     */
    void mousePressEvent( QMouseEvent *event ) override;

    /**
     * @brief Mouse realease button handler.
     *
     * @param event                 pointer to mouse event
     */
    void mouseReleaseEvent( QMouseEvent *event ) override;

    /**
     * @brief Handles value changed signal from velocityMagnitude slider
     *
     * @param value                 new value
     */
    void on_velocityMagnitude_valueChanged( int value );

signals:

    /**
     * @brief Signal emitted when particle velocity has changed.
     *
     * @param particle              particle iteartor
     * @param newVelocity           new particle velocity
     */
    void particleVelocityChanged( citerParticle particle , vect2D newVelocity );

};
