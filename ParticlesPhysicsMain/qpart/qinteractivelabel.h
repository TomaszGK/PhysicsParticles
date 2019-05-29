#pragma once

#include <QLabel>
#include <QEvent>

/** @file
 * @brief Class @ref QInteractiveLabel
 */

/**
 * @class QInteractiveLabel
 * @brief Extends QLabel by adding interactive pictures.
 *
 * Adds to QLabel interactive pictures that swap on mouse enter/leave event.
 * @author Tomasz Gburek
 * @date 2019
 */

class QInteractiveLabel : public QLabel
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param enterPicture          entering picture name
     * @param leavePicture          leaving picture name
     * @param parent                ponter to parent widget
     */
    QInteractiveLabel( const QString& enterPicture, const QString& leavePicture, QWidget* parent = nullptr );

    /**
     * @brief Sets picture in QLabel pixel map.
     *
     * @param filename              picture name
     */
    void setPicture( const QString& filename );

protected:

    /**
     * @brief Handles enter mouse event on QLabel pixel map.
     */
    void enterEvent( QEvent *ev ) override;

    /**
     * @brief Handles leave mouse event from QLabel pixel map.
     */
    void leaveEvent( QEvent *ev ) override;

    QString enterPicture {}; /** enter picture name */
    QString leavePicture {}; /** leave picture name */

};

