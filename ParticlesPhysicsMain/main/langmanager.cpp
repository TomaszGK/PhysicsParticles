#include "langmanager.h"

Language LangManager::language = Language::ENG;

LangManager::TMap LangManager::translateMap =
{
   { Language::PL ,
                    {
                      { "Velocity" , "Prędkość" },
                      { "Blue Velocity" , "Niebieska Prędkość" },
                      { "Red Velocity" , "Czerwona Prędkość" },
                      { "Collisions" , "Ilość Kolizji" },
                      { "Plane Energy" , "Energia Pojemnika" },
                      { "Velocity Distribution", "Rozkład Prędkości"},
                      { "Momentum Distribution", "Rozkład Pędu"}
                    }
   },
   { Language::ENG ,
                    {
                      { "Empty", "Empty" }
                    }
   }
};

std::string LangManager::translate( const std::string& text )
{
    return ( translateMap.at(language).find(text) == translateMap.at(language).end() ) ? text : translateMap.at(language).at(text);
}
