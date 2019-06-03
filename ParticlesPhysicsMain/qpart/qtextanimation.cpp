#include "qtextanimation.h"
#include <QRandomGenerator>

QTextAnimation::QTextAnimation( QString _text )
: text {std::move(_text)}
{
    cyclesToLetterAppear.resize(static_cast<size_t>(text.size()));
    cycleLength.resize(static_cast<size_t>(text.size()));
    cycle.resize(static_cast<size_t>(text.size()));
    mask.resize(static_cast<size_t>(text.size()));
    reset();
}

void QTextAnimation::reset( int baseCyclesToAppear )
{
    for( auto &item : cyclesToLetterAppear )
    {
        item = baseCyclesToAppear + QRandomGenerator::global()->bounded(15);
    }

    for( auto &item : cycleLength )
    {
        item = 1 + QRandomGenerator::global()->bounded(5);
    }

    for( auto &item : cycle )
    {
        item = 0;
    }

    for( auto &item : mask )
    {
        item = QRandomGenerator::global()->bounded(10);
    }

    generateCurrentText();
}

void QTextAnimation::generateCurrentText()
{
    QString letter;
    currentText = "";    

    for( size_t index = 0 ; index < static_cast<size_t>(text.size()) ; ++index )
    {

        if( ++cycle[index] == cycleLength[index] ) cycle[index] = 0;

        if( cycle[index] == 0 )
        {
            --cyclesToLetterAppear[index];
            mask[index] = (mask[index]+1)%9;
        }

        if( cyclesToLetterAppear[index]>0 ) letter = QString::number(mask[index]);
        else letter = text[static_cast<int>(index)];

        currentText += letter ;
    }
}

bool QTextAnimation::update()
{
    if( text != currentText )
    {
        generateCurrentText();
        return true;
    }

    return false;
}
