#pragma once

#include <vector>
#include <QString>

class QTextAnimation
{

    QString text;
    QString currentText;
    std::vector<int> cycleToLetterAppear;
    std::vector<int> cycleSpeed;
    std::vector<int> cycle;
    std::vector<int> mask;

    void generateCurrentText();
    int getNextNumber( size_t position );

public:

    QTextAnimation( QString _text );

    QTextAnimation( const QTextAnimation& ) = default;
    QTextAnimation( QTextAnimation&& ) = default;

    QTextAnimation& operator=( const QTextAnimation& ) = default;
    QTextAnimation& operator=( QTextAnimation&& ) = default;

    void reset( int baseCycleTime = 10 );
    bool update();
    QString getText() const noexcept { return text; }
    QString getCurrentText() const noexcept { return currentText; }

};
