#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),    
    ui(new Ui::DialogAbout),
    updateTimer(this),
    labelPicture {"Einstein_tongue.jpg","science.jpg"}
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

    for( auto &label : labels ){ label.second->setText( animatedText[label.first]->getCurrentText()  ); }

    ui->layoutPicture->addWidget(&labelPicture);
    labelPicture.setPicture("science.jpg");

    connect(&updateTimer, &QTimer::timeout, this, &DialogAbout::update);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::update()
{
    int ends {0};

    for( auto &text : animatedText )
    {
        if( !text.second->update() ) ends++;
    }
    for( auto &label : labels )
    {
        label.second->setText( animatedText[label.first]->getCurrentText() );
    }

    if( static_cast<size_t>(ends) == animatedText.size() ) updateTimer.stop();
}

void DialogAbout::setFadeAnimation( int duration )
{
    updateTimer.start(20);
    for( auto &text : animatedText ){ text.second->reset(); }
    propertyAnimation->setDuration(duration);
    propertyAnimation->setStartValue(0.0);
    propertyAnimation->setEndValue(1.0);
    propertyAnimation->setEasingCurve(QEasingCurve::Linear);
    propertyAnimation->start();
}
