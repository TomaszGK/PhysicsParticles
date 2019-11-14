#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    paintTimer(this), updateTimer(this), animationTimer(this)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    physicsSimulation.resize( static_cast<size_t>(ui->tabWidget->count()) );

    // create BASIC simulation
    physicsSimulation[0] = std::make_unique<QParticlesPhysicsManager>(SimulationType::BASIC,ui->particlesFrameTab0);
    physicsSimulation[0]->addQBarChart("velocity",ui->velocityBarChartTab0,BoxStyles::BAR_CHART1);
    physicsSimulation[0]->addQBarChart("kinetic",ui->kineticEnergyBarChartTab0,BoxStyles::BAR_CHART2);
    physicsSimulation[0]->addQHistogram1D("velocityDistribution",ui->velocityDistributionHistogram1DTab0);
    physicsSimulation[0]->addQGauge("Pressure",ui->pressureGaugeHorizontalLayoutTab0);
    physicsSimulation[0]->setPlaneHitsPaint(true);

    ui->temperatureDialTab0->setValue(physicsSimulation[0]->getTemperatureInPercent());    
    ui->numberOfParticlesSliderTab0->setValue(50);
    ui->sizeOfParticleSliderTab0->setValue(physicsSimulation[0]->getSizeOfParticleInPercent());    
    ui->trackingOffButtonTab0->setDisabled(true);


    // create DIFFIUSION simulation
    physicsSimulation[1] = std::make_unique<QParticlesPhysicsManager>(SimulationType::DIFFUSION,ui->particlesFrameTab1);
    physicsSimulation[1]->addQBarChart("velocityBlue",ui->velocityBlueBarChartTab1,BoxStyles::BAR_CHART3);
    physicsSimulation[1]->addQBarChart("velocityRed",ui->velocityRedBarChartTab1,BoxStyles::BAR_CHART3);
    physicsSimulation[1]->addQBarDisplay("diffiusion",ui->numBlueRedTab1);

    ui->temperatureLeftDialTab1->setValue(physicsSimulation[1]->getTemperatureLeftInPercent());
    ui->temperatureRightDialTab1->setValue(physicsSimulation[1]->getTemperatureRightInPercent());
    ui->particleSizeBlueDialTab1->setValue(physicsSimulation[1]->getSizeOfParticleInPercent(ParticleType::BLUE));
    ui->particleSizeRedDialTab1->setValue(physicsSimulation[1]->getSizeOfParticleInPercent(ParticleType::RED));


    // create BROWNIAN_MOTION simulation
    physicsSimulation[2] = std::make_unique<QParticlesPhysicsManager>(SimulationType::BROWNIAN_MOTION,ui->particlesFrameTab4);
    physicsSimulation[2]->addQTrackingPlot2D("trackingPlot",ui->trackingFrameTab4);
    physicsSimulation[2]->addQHistogram1D("velocityDistribution",ui->velocityDistributionLayoutTab4);
    physicsSimulation[2]->addQCircleControl("pushForceCircleControl",ui->circleControlLayoutTab4);

    // create SANDBOX simulation
    physicsSimulation[3] = std::make_unique<QParticlesPhysicsManager>(SimulationType::SANDBOX,ui->particlesFrameTab2);
    physicsSimulation[3]->addQInfoDisplay("infoDisplayTab2",ui->infoLayoutTab2);
    physicsSimulation[3]->addQHistogram1D("velocityDistribution",ui->velocityDistributionlLayoutTab2);
    physicsSimulation[3]->addQHistogram1D("momentumDistribution",ui->momentumDistributionlLayoutTab2);

    ui->temperatureUpSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::UP));
    ui->temperatureDownSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::DOWN));
    ui->temperatureLeftSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::LEFT));
    ui->temperatureRightSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::RIGHT));

    ui->blueParticlesSliderTab2->setValue(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS1));
    ui->redParticlesSliderTab2->setValue(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS2));
    ui->greenParticlesSliderTab2->setValue(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS3));

    ui->blueParticlesSizeSliderTab2->setValue(physicsSimulation[3]->getSizeOfParticleInPercent(ParticleType::GAS1));
    ui->redParticlesSizeSliderTab2->setValue(physicsSimulation[3]->getSizeOfParticleInPercent(ParticleType::GAS2));
    ui->greenParticlesSizeSliderTab2->setValue(physicsSimulation[3]->getSizeOfParticleInPercent(ParticleType::GAS3));

    // connect menu actions
    connect( ui->actionAbout, &QAction::triggered, this, &MainWindow::about_action );
    connect( ui->actionScheme1, &QAction::triggered, this, &MainWindow::load_scheme );
    connect( ui->actionScheme2, &QAction::triggered, this, &MainWindow::load_scheme );
    connect( ui->actionScheme3, &QAction::triggered, this, &MainWindow::load_scheme );
    connect( ui->actionSetPolish, &QAction::triggered, this, &MainWindow::change_language );
    connect( ui->actionSetEnglish, &QAction::triggered, this, &MainWindow::change_language );

    // connect main timers
    connect(&paintTimer, &QTimer::timeout, this, &MainWindow::paint);
    connect(&updateTimer, &QTimer::timeout, this, &MainWindow::update);
    connect(&animationTimer, &QTimer::timeout, this, &MainWindow::animate);

    paintTimer.start(2);
    updateTimer.start(100);
    animationTimer.start(10);

    calculationThread = physicsSimulation[currentIndex]->calculateNextPositionsInThread();

}

MainWindow::~MainWindow()
{
    physicsSimulation[currentIndex]->end();
    calculationThread.join();
    delete ui;
}

void MainWindow::update()
{    
    physicsSimulation[currentIndex]->paintLayouts();
    physicsSimulation[currentIndex]->handleControls();
    ui->calculationCount->display(static_cast<int>(physicsSimulation[currentIndex]->getAvgCalculationCount()));
}

void MainWindow::animate()
{   
    if( dividerGapAnimationStart )
    {
        if( dividerGapPosition>100 )
        {
            dividerGapPosition = 100;
            dividerGapStep = 0.0;
            dividerGapAnimationStart = false;
        }
        physicsSimulation[1]->setDividerGap(static_cast<int>(dividerGapPosition));
        dividerGapPosition += dividerGapStep;
        dividerGapStep += 0.1;
    }
}

void MainWindow::paint()
{
    physicsSimulation[currentIndex]->paintParticlesPlane();
}

void MainWindow::about_action()
{
    dialogAboutWindow.setFadeAnimation(500);
    dialogAboutWindow.show();
}

bool MainWindow::checkActionItem( QAction *menuItem , QMenu *menu )
{
    bool hasChecked {false};

    if( menuItem == nullptr ) return false;

    for( QAction *action : menu->actions() )
    {
       if( action->isChecked() )
       {
           hasChecked = true;
           action->setChecked(false);
       }
    }
    menuItem->setChecked(true);

    return hasChecked;
}

void MainWindow::load_scheme()
{   
    auto menuItem = qobject_cast<QAction*>( sender() );
    QString fileName {""};

    if( !checkActionItem(menuItem,ui->menuAbout) ) return;

    if( menuItem->text() == "Orange" ) fileName = "darkorange.qss";
    else if( menuItem->text() == "Blue" ) fileName = "styleblue.qss";

    if( fileName != "" )
    {
        //QFile File(":/css/css/"+fileName);
        //File.open(QFile::ReadOnly);
        //qApp->setStyleSheet(QLatin1String(File.readAll()));
    }
    else
    {
        qApp->setStyleSheet("");
    }
}

void MainWindow::change_language()
{
    auto menuItem = qobject_cast<QAction*>( sender() );

    if( !checkActionItem(menuItem,ui->menuLanguage) ) return;

    if( menuItem->objectName() == "actionSetPolish" )
    {
        qApp->removeTranslator(&qtLanguageTranslator);
        LangManager::setLanguage(Language::PL);

        if( qtLanguageTranslator.load( QApplication::applicationDirPath()+"/ParticlesPhysics_pl.qm") )
        {
            qApp->installTranslator(&qtLanguageTranslator);
        }

    }
    else if( menuItem->objectName() == "actionSetEnglish" )
    {
        LangManager::setLanguage(Language::ENG);
        qApp->removeTranslator(&qtLanguageTranslator);
    }

    ui->retranslateUi(this);
}

void MainWindow::on_temperatureDialTab0_valueChanged( int value )
{
     ui->temperatureLabelTab0->setText(QString::number(value));
     physicsSimulation[0]->setTemperatureInPercent(value);
}

void MainWindow::on_numberOfParticlesSliderTab0_valueChanged( int value )
{
    physicsSimulation[0]->setParticlesInPlaneInPercent(ParticleType::NORMAL,value);
    ui->numberOfParticlesLabelTab0->setText(QString::number(physicsSimulation[0]->getNumberOfParticles(ParticleType::NORMAL)));
}

void MainWindow::on_sizeOfParticleSliderTab0_valueChanged( int value )
{
    physicsSimulation[0]->setSizeOfParticlesInPercent( ParticleType::NORMAL, value );
    ui->sizeOfParticlesInfoLabelTab0->setText(QString::number(physicsSimulation[0]->getSizeOfParticle()));
}

void MainWindow::on_planeSizeDialTab0_valueChanged( int value )
{
    ui->constraintLabelTab0->setText(QString::number(value));
    physicsSimulation[0]->setPlaneWidthInPercent(value);
}

void MainWindow::on_runButtonTab0_clicked()
{
    physicsSimulation[0]->run(true);
    ui->pauseButtonTab0->setEnabled(true);
    ui->runButtonTab0->setEnabled(false);
    ui->planeSizeDialTab0->setEnabled(true);
}

void MainWindow::on_pauseButtonTab0_clicked()
{
    physicsSimulation[0]->pause(true);
    ui->pauseButtonTab0->setEnabled(false);
    ui->runButtonTab0->setEnabled(true);
    ui->planeSizeDialTab0->setEnabled(false);
}

void MainWindow::on_trackingOnButtonTab0_clicked()
{
    ui->trackingOnButtonTab0->setDisabled(true);
    ui->trackingOffButtonTab0->setDisabled(false);
    physicsSimulation[0]->enableTracking();
}

void MainWindow::on_trackingOffButtonTab0_clicked()
{
    ui->trackingOffButtonTab0->setDisabled(true);
    ui->trackingOnButtonTab0->setDisabled(false);
    physicsSimulation[0]->disableTracking();
}

void MainWindow::on_runButtonTab1_clicked()
{
    physicsSimulation[1]->run(true);
    ui->pauseButtonTab1->setEnabled(true);
    ui->runButtonTab1->setEnabled(false);
    ui->startButtonTab1->setEnabled(true);
}

void MainWindow::on_pauseButtonTab1_clicked()
{
    physicsSimulation[1]->pause(true);
    ui->pauseButtonTab1->setEnabled(false);
    ui->runButtonTab1->setEnabled(true);
    ui->startButtonTab1->setEnabled(false);
}

void MainWindow::on_tabWidget_currentChanged( int index )
{
    if( physicsSimulation[currentIndex]->getCalculationState() != "End" )
    {
        physicsSimulation[currentIndex]->end();
        calculationThread.join();        
    }

    calculationThread = physicsSimulation[static_cast<size_t>(index)]->calculateNextPositionsInThread();    
    if( physicsSimulation[static_cast<size_t>(index)]->isPauseByUser() )
    {
        physicsSimulation[static_cast<size_t>(index)]->pause();
    }

    currentIndex = static_cast<size_t>(index);
}

void MainWindow::on_startButtonTab1_clicked()
{
    if( ui->startButtonTab1->text() == "OPEN" )
    {        
        ui->startButtonTab1->setText("RESET");
        dividerGapAnimationStart = true;
        physicsSimulation[1]->setAverageDiffusionTemperature();
    }
    else if( ui->startButtonTab1->text() == "RESET" )
    {
        ui->startButtonTab1->setText("OPEN");
        dividerGapAnimationStart = false;
        dividerGapPosition = 0;
        physicsSimulation[1]->reset();
        ui->particleSizeBlueDialTab1->setValue(physicsSimulation[1]->getSizeOfParticleInPercent(ParticleType::BLUE));
        ui->particleSizeRedDialTab1->setValue(physicsSimulation[1]->getSizeOfParticleInPercent(ParticleType::RED));
        ui->visualizationTypeParticleRadioButtonTab1->setChecked(true);
        ui->gapSizeSliderTab1->setValue(0);
    }

    ui->temperatureLeftDialTab1->setValue(physicsSimulation[1]->getTemperatureLeftInPercent());
    ui->temperatureRightDialTab1->setValue(physicsSimulation[1]->getTemperatureRightInPercent());   
}

void MainWindow::on_temperatureLeftDialTab1_valueChanged( int value )
{
    physicsSimulation[1]->setTemperatureLeftInPercent(value);
    ui->blueTemperatureLabelTab1->setText(QString::number(value));
}

void MainWindow::on_temperatureRightDialTab1_valueChanged(int value)
{
    physicsSimulation[1]->setTemperatureRightInPercent(value);
    ui->redTemperatureLabelTab1->setText(QString::number(value));
}

void MainWindow::on_particleSizeBlueDialTab1_valueChanged(int value)
{
    physicsSimulation[1]->setSizeOfParticlesInPercent(ParticleType::BLUE,value);
    ui->blueSizeLabelTab1->setText(QString::number(value));
}

void MainWindow::on_particleSizeRedDialTab1_valueChanged(int value)
{
    physicsSimulation[1]->setSizeOfParticlesInPercent(ParticleType::RED,value);
    ui->redSizeLabelTab1->setText(QString::number(value));
}

void MainWindow::on_visualizationTypeVelocityRadioButtonTab1_toggled( bool checked )
{
    if( checked ) physicsSimulation[1]->setVisualizationType(VisualizationType::VELOCITY);
    else physicsSimulation[1]->setVisualizationType(VisualizationType::PARTICLE);
}

void MainWindow::on_gapSizeSliderTab1_valueChanged( int value )
{
    if( ui->startButtonTab1->text() == "OPEN" && value>0 )
    {
        physicsSimulation[currentIndex]->setAverageDiffusionTemperature();
        ui->startButtonTab1->setText("RESET");
    }
    physicsSimulation[1]->setDividerGap(value);
}

void MainWindow::on_temperatureUpSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setSideTemperatureInPercent(PlaneSide::UP,value);
    physicsSimulation[3]->setDisplay("infoDisplayTab2",PlaneSide::UP,value);
}

void MainWindow::on_temperatureRightSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setSideTemperatureInPercent(PlaneSide::RIGHT,value);
    physicsSimulation[3]->setDisplay("infoDisplayTab2",PlaneSide::RIGHT,value);
}

void MainWindow::on_temperatureDownSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setSideTemperatureInPercent(PlaneSide::DOWN,value);
    physicsSimulation[3]->setDisplay("infoDisplayTab2",PlaneSide::DOWN,value);
}

void MainWindow::on_temperatureLeftSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setSideTemperatureInPercent(PlaneSide::LEFT,value);
    physicsSimulation[3]->setDisplay("infoDisplayTab2",PlaneSide::LEFT,value);    
}

void MainWindow::on_horizontalForceTab2_valueChanged( int value )
{
    physicsSimulation[3]->setHorizontalForceInPercent(value);
    ui->horizontalForceSizeLabelTab2->setText(QString::number(value));
}

void MainWindow::on_verticalForceTab2_valueChanged( int value )
{
    physicsSimulation[3]->setVerticalForceInPercent(value);
    ui->verticalForceSizeLabelTab2->setText(QString::number(value));
}

void MainWindow::on_runButtonTab2_clicked()
{
    physicsSimulation[3]->run(true);
    ui->pauseButtonTab2->setEnabled(true);
    ui->runButtonTab2->setEnabled(false);
}

void MainWindow::on_pauseButtonTab2_clicked()
{
    physicsSimulation[3]->pause(true);
    ui->pauseButtonTab2->setEnabled(false);
    ui->runButtonTab2->setEnabled(true);
}

void MainWindow::on_blueParticlesSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setParticlesInPlane(ParticleType::GAS1,value);
    ui->particlesNumberGas1LabellTab2->setText(QString::number(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS1)));
}

void MainWindow::on_redParticlesSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setParticlesInPlane(ParticleType::GAS2,value);
    ui->particlesNumberGas2LabellTab2->setText(QString::number(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS2)));
}

void MainWindow::on_greenParticlesSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setParticlesInPlane(ParticleType::GAS3,value);
    ui->particlesNumberGas3LabellTab2->setText(QString::number(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS3)));
}

void MainWindow::on_blueParticlesSizeSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setSizeOfParticlesInPercent(ParticleType::GAS1,value);
    ui->particlesSizeGas1LabellTab2->setText(QString::number(physicsSimulation[3]->getSizeOfParticle(ParticleType::GAS1)));
}

void MainWindow::on_redParticlesSizeSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setSizeOfParticlesInPercent(ParticleType::GAS2,value);
    ui->particlesSizeGas2LabellTab2->setText(QString::number(physicsSimulation[3]->getSizeOfParticle(ParticleType::GAS2)));
}

void MainWindow::on_greenParticlesSizeSliderTab2_valueChanged( int value )
{
    physicsSimulation[3]->setSizeOfParticlesInPercent(ParticleType::GAS3,value);
    ui->particlesSizeGas3LabellTab2->setText(QString::number(physicsSimulation[3]->getSizeOfParticle(ParticleType::GAS3)));
}

void MainWindow::on_visualizationTypeVelocityRadioButtonTab2_toggled( bool checked )
{
    if( checked ) physicsSimulation[3]->setVisualizationType(VisualizationType::VELOCITY);
    else physicsSimulation[3]->setVisualizationType(VisualizationType::PARTICLE);
}

void MainWindow::on_attractionForceTab2_valueChanged( int value )
{
    physicsSimulation[3]->setAttractionForceInPercent( value );
    ui->atractionForceSizeLabelTab2->setText(QString::number(value));
}

void MainWindow::on_runButtonTab4_clicked()
{
    physicsSimulation[2]->run(true);
    ui->pauseButtonTab4->setEnabled(true);
    ui->runButtonTab4->setEnabled(false);
}

void MainWindow::on_pauseButtonTab4_clicked()
{
    physicsSimulation[2]->pause(true);
    ui->pauseButtonTab4->setEnabled(false);
    ui->runButtonTab4->setEnabled(true);
}

void MainWindow::on_massMoleculeSliderTab4_valueChanged( int value )
{
    physicsSimulation[2]->setMassOfMoleculeInPercent( value );
    ui->massMoleculeLabelTab4->setText( QString::number(value) );
}

void MainWindow::on_stopMoleculePushButtonTab4_clicked()
{
    physicsSimulation[2]->stopMolecule();    
}

void MainWindow::on_clearTrackMoleculePushButtonTab4_clicked()
{
    physicsSimulation[2]->clearMoleculeTrace();
}

void MainWindow::on_resetPushButton_tab2_clicked()
{
    physicsSimulation[3]->reset();

    ui->verticalForceTab2->setValue(0);
    ui->horizontalForceTab2->setValue(0);
    ui->attractionForceTab2->setValue(0);

    ui->temperatureUpSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::UP));
    ui->temperatureDownSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::DOWN));
    ui->temperatureLeftSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::LEFT));
    ui->temperatureRightSliderTab2->setValue(physicsSimulation[3]->getSideTemperatureInPercent(PlaneSide::RIGHT));

    ui->blueParticlesSliderTab2->setValue(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS1));
    ui->redParticlesSliderTab2->setValue(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS2));
    ui->greenParticlesSliderTab2->setValue(physicsSimulation[3]->getNumberOfParticles(ParticleType::GAS3));

    ui->blueParticlesSizeSliderTab2->setValue(physicsSimulation[3]->getSizeOfParticleInPercent(ParticleType::GAS1));
    ui->redParticlesSizeSliderTab2->setValue(physicsSimulation[3]->getSizeOfParticleInPercent(ParticleType::GAS2));
    ui->greenParticlesSizeSliderTab2->setValue(physicsSimulation[3]->getSizeOfParticleInPercent(ParticleType::GAS3));
}

void MainWindow::on_randomPushButton_tab2_clicked()
{
     physicsSimulation[3]->reset();

     ui->blueParticlesSizeSliderTab2->setValue(QRandomGenerator::global()->bounded(physicsSimulation[3]->getSimulationInfo().maxSizeOfParticle.at(SimulationType::SANDBOX)));
     ui->redParticlesSizeSliderTab2->setValue(QRandomGenerator::global()->bounded(physicsSimulation[3]->getSimulationInfo().maxSizeOfParticle.at(SimulationType::SANDBOX)));
     ui->greenParticlesSizeSliderTab2->setValue(QRandomGenerator::global()->bounded(physicsSimulation[3]->getSimulationInfo().maxSizeOfParticle.at(SimulationType::SANDBOX)));

     ui->blueParticlesSliderTab2->setValue(QRandomGenerator::global()->bounded(ui->blueParticlesSliderTab2->maximum()));
     ui->redParticlesSliderTab2->setValue(QRandomGenerator::global()->bounded(ui->redParticlesSliderTab2->maximum()));
     ui->greenParticlesSliderTab2->setValue(QRandomGenerator::global()->bounded(ui->greenParticlesSliderTab2->maximum()));

     ui->verticalForceTab2->setValue(100 - QRandomGenerator::global()->bounded(200));
     ui->horizontalForceTab2->setValue(100 - QRandomGenerator::global()->bounded(200));
     ui->attractionForceTab2->setValue(100 - QRandomGenerator::global()->bounded(200));
     ui->temperatureUpSliderTab2->setValue(QRandomGenerator::global()->bounded(100));
     ui->temperatureDownSliderTab2->setValue(QRandomGenerator::global()->bounded(100));
     ui->temperatureLeftSliderTab2->setValue(QRandomGenerator::global()->bounded(100));
     ui->temperatureRightSliderTab2->setValue(QRandomGenerator::global()->bounded(100));
}


void MainWindow::on_resetForcesSandbox_clicked()
{
    ui->verticalForceTab2->setValue(0);
    ui->horizontalForceTab2->setValue(0);
    ui->attractionForceTab2->setValue(0);
}
