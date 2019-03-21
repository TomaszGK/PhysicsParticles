#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),    
    ui(new Ui::DialogAbout),
    updateTimer(this)
{
    ui->setupUi(this);    
    propertyAnimation = std::make_unique<QPropertyAnimation>(this, "windowOpacity");

    animatedText["Title"] =  std::make_unique<QTextAnimation>("Particles v. 0.2");
    labels["Title"] = ui->labelName;

    animatedText["Author"] = std::make_unique<QTextAnimation>("Tomasz Gburek");
    labels["Author"] = ui->labelAuthorName;

    animatedText["By"] = std::make_unique<QTextAnimation>("By");
    labels["By"] = ui->labelBy;

    animatedText["Copyright"] = std::make_unique<QTextAnimation>("2019 © All rights reserved");
    labels["Copyright"] = ui->labelCopyright;

    for( auto it = labels.begin(); it != labels.end(); it++ ){ it->second->setText( animatedText[it->first]->getCurrentText()  ); }

    connect(&updateTimer, &QTimer::timeout, this, &DialogAbout::update);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::update()
{
    int ends {0};

    for( auto it = animatedText.begin(); it != animatedText.end(); it++ )
    {
        if( !it->second->update() ) ends++;
    }
    for( auto it = labels.begin(); it != labels.end(); it++ )
    {
        it->second->setText( animatedText[it->first]->getCurrentText()  );
    }

    if( static_cast<size_t>(ends) == animatedText.size() ) updateTimer.stop();
}

void DialogAbout::setFadeAnimation( int duration )
{
    updateTimer.start(10);
    for( auto it = animatedText.begin(); it != animatedText.end(); it++ ){ it->second->reset(); }
    propertyAnimation->setDuration(duration);
    propertyAnimation->setStartValue(0.0);
    propertyAnimation->setEndValue(1.0);
    propertyAnimation->setEasingCurve(QEasingCurve::Linear);
    propertyAnimation->start();
}
