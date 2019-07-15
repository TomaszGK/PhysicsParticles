#pragma once

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
     *
     * If text has no valid translation then it does not change.
     * @param text       given text to translate
     * @return translated text
     */
    static std::string translate( const std::string& text );

    static void setLanguage( Language lang ) { language = lang; }

private:

    using TMap = const std::map<Language,std::map<std::string,std::string>>;

    /** Application language */
    static Language language;

    /** Translation map */
    static TMap translateMap;

};
