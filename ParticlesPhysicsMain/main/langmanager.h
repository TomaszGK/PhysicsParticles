#pragma once

#include <QString>
#include <QStaticText>
#include "definitions.h"

/** @file
 * @brief Class @ref LangManager
 */


/**
 * @class LangManager
 * @brief Manages application language in static manner.
 *
 * @author Tomasz Gburek
 * @date 2019
 */

class LangManager
{

public:

    /**
     * @brief default constructor
     */
    LangManager() = delete;

    /** @brief Copy constructor */
    LangManager( const LangManager& ) = delete;

    /** @brief Move constructor */
    LangManager( LangManager&& ) = delete;

    /** @brief Copy assigment operator */
    LangManager& operator=( const LangManager& ) = delete;

    /** @brief Move assigment operator */
    LangManager& operator=( LangManager&& ) = delete;

    /**
     * @brief Translates current text to the actual language.
     *
     * If text has no valid translation then it does not change.
     * @param text       given text to translate
     * @return translated text
     */
    template< typename T >
    static const T& translate( const T& text )
    {
        return ( translateMap<T>.at(language).find(text) == translateMap<T>.at(language).end() ) ? text : translateMap<T>.at(language).at(text);
    }

    static void setLanguage( Language lang ) { language = lang; }

private:

    template< typename T >
    using TMap = std::map<Language,std::map<T,T>>;

    /** Application language */
    static Language language;

    template< typename T >
    static const TMap<T> translateMap;

};
