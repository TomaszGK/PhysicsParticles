#pragma once

#include <vector>
#include <QString>

/** @file
 * @brief Class @ref QTextAnimation
 */

/**
 * @class QTextAnimation
 * @brief Performs text animation.
 *
 * Calculates next steps of text animation by changing of masks {1,..,9} for each text letter.
 * @author Tomasz Gburek
 * @date 2019
 */

class QTextAnimation
{   

public:

    /**
     * @brief Constructor
     *
     * @param text                  base text to animation
     */
    QTextAnimation( QString text );

    /**
     * @brief Resets text animation.
     *
     * @param baseCycleTime         base value cycles added to each element of cyclesToLetterAppear
     */
    void reset( int baseCyclesToAppear = 10 );

    /**
     * @brief Updates text animation.
     *
     * Calculate the next step of text animation.
     * @return true if animation has ended otherwise false
     */
    bool update();

    /**
     * @brief Gets original animation text.
     *
     * @return original text
     */
    QString getText() const noexcept { return text; }

    /**
     * @brief Gets current animation text.
     *
     * @return current text
     */
    QString getCurrentText() const noexcept { return currentText; }

private:

    QString text        ; /** original text */
    QString currentText ; /** current text */

    std::vector<int> cyclesToLetterAppear ; /** holds number of cycles to original letter appear for each element in QString text */
    std::vector<int> cycleLength          ; /** holds length of cycle for each element in QString text */
    std::vector<int> cycle                ; /** holds current cycle position for each element in QString text */
    std::vector<int> mask                 ; /** holds current letter mask for each element in QString text */

    /**
     * @brief Generates the current animation text by calculating the next step of animation.
     *
     */
    void generateCurrentText();

};
