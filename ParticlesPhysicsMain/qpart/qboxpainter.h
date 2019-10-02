#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "definitions.h"

/** @file
 * @brief Class @ref QBoxPainter
 */

/**
 * @class QBoxPainter
 * @brief Base class for all box painters.
 *
 * Paints primitive box using QT framework.
 * @author Tomasz Gburek
 * @date 2019
 */

class QBoxPainter : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param parentWidget          ponter to parent widget
     */
    explicit QBoxPainter( QWidget* parentWidget = nullptr );

    /**
     * @brief Gets box label.
     * @return box label
     */
    QString getLabel() const noexcept { return label; }

    /**
     * @brief Sets new box label.
     * @param neLabel               box new label
     */
    void setLabel( const QString& newLabel ) { label = newLabel; }

protected:

    QPainter painter    ; /** QT painter */
    QBrush   background ; /** backround brush */

    QColor cPlaneBorder {120,120,120} ; /** box(plane) border color */

    int  marginLeft             {20} ; /** box margin left in pixels */
    int  marginRight            {20} ; /** box margin right in pixels */
    int  marginTop              {20} ; /** box margin top in pixels */
    int  marginBottom           {20} ; /** box margin bottom in pixels */
    int  numberOfHorizontalAxes {5}  ; /** number of horizontal axes in grid */
    int  numberOfVerticalAxes   {5}  ; /** number of vertical axes in grid */
    int  planeBorderWidth       {1}  ; /** box(plane) border width */

    QString label {""} ; /** box label */

    /**
     * @brief Paints specific box.
     */
    virtual void paint() = 0;

    /**
     * @brief Inits specific box.
     *
     * Performs all necessary calulations after QBoxPainter is added to the layout
     * and when parentWidget != nullptr.
     */
    virtual void init() = 0;

    /**
     * @brief Paints on event.
     *
     * Sets basic paint parameters and calls specific paint() inside.
     * @param event                 QT paint event
     */
    void paintEvent( QPaintEvent *event ) override;

    /**
     * @brief Paints vertical and horizontal axes (grid).
     */
    void paintAxes();

    /**
     * @brief Paints box(plane) border.
     */
    void paintPlaneBorder();

    /**
     * @brief Paints arrow.
     *
     * @param position              base arrow position
     * @param direction             arrow direction
     * @param arrowSize             arrow head size
     * @param arrowThick            arrow border thick (pen size)
     * @param arrowColor            arrow color
     */
    void paintArrow( const vect2D& position, const vect2D& direction, const qreal& arrowSize, int arrowThick, const QColor& arrowColor );

    /**
     * @brief Paints triangle defines by {a,b,c} points.
     *
     * @param a                     point a
     * @param b                     point b
     * @param c                     point c
     * @param color                 triangle color
     */
    void paintTriangle( const vect2D& a, const vect2D& b, const vect2D& c, const QColor& color );

    /**
     * @brief Gets font length for a given string in pixels.
     *
     * @param name                  string
     * @return font length in pixels
     */
    int  getStringWidthInPixels( const QString& name );

};
