#include "qtextanimation.h"
#include <QRandomGenerator>

QTextAnimation::QTextAnimation( QString _text )
: text {std::move(_text)}
{
    cycleToLetterAppear.resize(static_cast<size_t>(text.size()));
    cycleSpeed.resize(static_cast<size_t>(text.size()));
    cycle.resize(static_cast<size_t>(text.size()));
    mask.resize(static_cast<size_t>(text.size()));
    reset();
}

void QTextAnimation::reset( int baseCycleTime )
{
    for( auto &item : cycleToLetterAppear )
    {
        item = baseCycleTime + QRandomGenerator::global()->bounded(15);
    }

    for( auto &item : cycleSpeed )
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
        int number = getNextNumber(index);
        if( number != mask[index] )
        {
            --cycleToLetterAppear[index];
            mask[index] = number;
        }

        if( cycleToLetterAppear[index]>0 ) letter = QString::number(mask[index]);
        else letter = text[static_cast<int>(index)];

        currentText += letter ;
    }
}

int QTextAnimation::getNextNumber( size_t position )
{
    int number = mask[position];

    cycle[position]++;
    if( cycle[position] == cycleSpeed[position] )
    {
        cycle[position] = 0;
        if( ++number>9 ) number=0;
    }

    return number;
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
