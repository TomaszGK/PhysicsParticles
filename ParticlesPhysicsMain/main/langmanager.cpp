#include "langmanager.h"
#include <QDebug>

Language LangManager::language = Language::ENG;

template<>
const LangManager::TMap<std::string> LangManager::translateMap<std::string> =
{
   { Language::PL ,
                    {
                      { "Average Velocity of Gas Particles" , "Średnia Prędkość Cząsteczek Gazu" },
                      { "Time" , "Czas" },
                      { "Average Velocity of Blue Gas Particles" , "Średnia Prędkość Niebieskich Cząsteczek Gazu" },
                      { "Average Velocity of Red Gas Particles" , "Średnia Prędkość Czerwonych Cząsteczek Gazu" },
                      { "Collisions" , "Ilość Kolizji" },
                      { "Energy of Particle Hits" , "Energia Uderzeń Cząstek" },
                      { "Velocity Distribution", "Rozkład Prędkości" },
                      { "Momentum Distribution", "Rozkład Pędu" },
                      { "Pressure", "Ciśnienie" },
                      { "Molecule Tracking Plot", "Wykres Śladu Molekuły" }
                    }
   },
   { Language::ENG ,
                    {
                      { "Empty", "Empty" }
                    }
   }
};

template<>
const LangManager::TMap<QString> LangManager::translateMap<QString> =
[]()
{
    LangManager::TMap<QString> qtMap;
    for( const auto& [lang,translation] : LangManager::translateMap<std::string> )
    {
        for( const auto& [from,to] : translation )
        {
            qtMap[lang][QString(from.c_str())] = QString(to.c_str());
        }

    }
    return qtMap;
}();
