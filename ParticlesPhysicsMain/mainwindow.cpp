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

    // create BASIC simulation [ TAB 0 ]
    simulationTab[0] = SimulationType::BASIC;
    simulation[SimulationType::BASIC] = std::make_unique<QParticlesPhysicsManager>(SimulationType::BASIC,ui->ParticlesLayout_Tab0);
    simulation[SimulationType::BASIC]->add( ui->Layout1_Tab0 , BoxType::BARCHART , ActionType::M_VELOCITY , BoxStyles::BAR_CHART1 );
    simulation[SimulationType::BASIC]->add( ui->Layout2_Tab0 , BoxType::BARCHART , ActionType::M_KINETIC ,  BoxStyles::BAR_CHART2 );
    simulation[SimulationType::BASIC]->add( ui->Layout3_Tab0 , BoxType::HISTOGRAM1D , ActionType::M_VELOCITY_DIST );
    simulation[SimulationType::BASIC]->add( ui->Layout4_Tab0 , BoxType::GAUGE , ActionType::M_PRESSURE );
    simulation[SimulationType::BASIC]->setPlaneHitsPaint(true);

    ui->temperatureDialTab0->setValue(simulation[SimulationType::BASIC]->getTemperatureInPercent());
    ui->numberOfParticlesSliderTab0->setValue(50);
    ui->sizeOfParticleSliderTab0->setValue(simulation[SimulationType::BASIC]->getSizeOfParticleInPercent());
    ui->trackingOffButtonTab0->setDisabled(true);


    // create DIFFIUSION simulation [ TAB 1 ]
    simulationTab[1] = SimulationType::DIFFUSION;
    simulation[SimulationType::DIFFUSION] = std::make_unique<QParticlesPhysicsManager>(SimulationType::DIFFUSION,ui->ParticlesLayout_Tab1);
    simulation[SimulationType::DIFFUSION]->add( ui->Layout1_Tab1 , BoxType::BARCHART , ActionType::M_VELOCITY_BLUE , BoxStyles::BAR_CHART3 );
    simulation[SimulationType::DIFFUSION]->add( ui->Layout2_Tab1 , BoxType::BARCHART , ActionType::M_VELOCITY_RED , BoxStyles::BAR_CHART3 );
    simulation[SimulationType::DIFFUSION]->add( ui->Layout3_Tab1 , BoxType::BARDISPLAY , ActionType::M_DIFFIUSION );

    ui->temperatureLeftDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getTemperatureLeftInPercent());
    ui->temperatureRightDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getTemperatureRightInPercent());
    ui->particleSizeBlueDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getSizeOfParticleInPercent(ParticleType::BLUE));
    ui->particleSizeRedDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getSizeOfParticleInPercent(ParticleType::RED));


    // create BROWNIAN_MOTION simulation [ TAB 2 ]
    simulationTab[2] = SimulationType::BROWNIAN_MOTION;
    simulation[SimulationType::BROWNIAN_MOTION] = std::make_unique<QParticlesPhysicsManager>(SimulationType::BROWNIAN_MOTION,ui->ParticlesLayout_Tab2);
    simulation[SimulationType::BROWNIAN_MOTION]->add( ui->Layout1_Tab2 , BoxType::CIRCLECONTROL , ActionType::C_PARTICLES_FLOW );
    simulation[SimulationType::BROWNIAN_MOTION]->add( ui->Layout2_Tab2 , BoxType::TRACKINGPLOT , ActionType::M_POSITION );
    simulation[SimulationType::BROWNIAN_MOTION]->add( ui->Layout3_Tab2 , BoxType::HISTOGRAM1D , ActionType::M_VELOCITY_DIST );


    // create SANDBOX simulation [ TAB 3 ]
    simulationTab[3] = SimulationType::SANDBOX;
    simulation[SimulationType::SANDBOX] = std::make_unique<QParticlesPhysicsManager>(SimulationType::SANDBOX,ui->ParticlesLayout_Tab3);
    simulation[SimulationType::SANDBOX]->add( ui->Layout1_Tab3 , BoxType::INFODISPLAY , ActionType::D_TEMPERATURE );
    simulation[SimulationType::SANDBOX]->add( ui->Layout2_Tab3 , BoxType::HISTOGRAM1D , ActionType::M_VELOCITY_DIST );
    simulation[SimulationType::SANDBOX]->add( ui->Layout3_Tab3 , BoxType::HISTOGRAM1D , ActionType::M_MOMENTUM_DIST );

    ui->temperatureUpSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::UP));
    ui->temperatureDownSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::DOWN));
    ui->temperatureLeftSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::LEFT));
    ui->temperatureRightSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::RIGHT));

    ui->blueParticlesSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS1));
    ui->redParticlesSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS2));
    ui->greenParticlesSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS3));

    ui->blueParticlesSizeSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSizeOfParticleInPercent(ParticleType::GAS1));
    ui->redParticlesSizeSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSizeOfParticleInPercent(ParticleType::GAS2));
    ui->greenParticlesSizeSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSizeOfParticleInPercent(ParticleType::GAS3));

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

    calculationThread = simulation[current]->calculateNextPositionsInThread();

}

MainWindow::~MainWindow()
{
    simulation[current]->end();
    calculationThread.join();
    delete ui;
}

void MainWindow::update()
{    
    simulation[current]->paintLayouts();
    simulation[current]->handleControls();
    ui->calculationCount->display(static_cast<int>(simulation[current]->getAvgCalculationCount()));
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
        simulation[SimulationType::DIFFUSION]->setDividerGap(static_cast<int>(dividerGapPosition));
        dividerGapPosition += dividerGapStep;
        dividerGapStep += 0.1;
    }
}

void MainWindow::paint()
{
    simulation[current]->paintParticlesPlane();
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
     simulation[SimulationType::BASIC]->setTemperatureInPercent(value);
}

void MainWindow::on_numberOfParticlesSliderTab0_valueChanged( int value )
{
    simulation[SimulationType::BASIC]->setParticlesInPlaneInPercent(ParticleType::NORMAL,value);
    ui->numberOfParticlesLabelTab0->setText(QString::number(simulation[SimulationType::BASIC]->getNumberOfParticles(ParticleType::NORMAL)));
}

void MainWindow::on_sizeOfParticleSliderTab0_valueChanged( int value )
{
    simulation[SimulationType::BASIC]->setSizeOfParticlesInPercent( ParticleType::NORMAL, value );
    ui->sizeOfParticlesInfoLabelTab0->setText(QString::number(simulation[SimulationType::BASIC]->getSizeOfParticle()));
}

void MainWindow::on_planeSizeDialTab0_valueChanged( int value )
{
    ui->constraintLabelTab0->setText(QString::number(value));
    simulation[SimulationType::BASIC]->setPlaneWidthInPercent(value);
}

void MainWindow::on_runButtonTab0_clicked()
{
    simulation[SimulationType::BASIC]->run(true);
    ui->pauseButtonTab0->setEnabled(true);
    ui->runButtonTab0->setEnabled(false);
    ui->planeSizeDialTab0->setEnabled(true);
}

void MainWindow::on_pauseButtonTab0_clicked()
{
    simulation[SimulationType::BASIC]->pause(true);
    ui->pauseButtonTab0->setEnabled(false);
    ui->runButtonTab0->setEnabled(true);
    ui->planeSizeDialTab0->setEnabled(false);
}

void MainWindow::on_trackingOnButtonTab0_clicked()
{
    ui->trackingOnButtonTab0->setDisabled(true);
    ui->trackingOffButtonTab0->setDisabled(false);
    simulation[SimulationType::BASIC]->enableTracking();
}

void MainWindow::on_trackingOffButtonTab0_clicked()
{
    ui->trackingOffButtonTab0->setDisabled(true);
    ui->trackingOnButtonTab0->setDisabled(false);
    simulation[SimulationType::BASIC]->disableTracking();
}

void MainWindow::on_runButtonTab1_clicked()
{
    simulation[SimulationType::DIFFUSION]->run(true);
    ui->pauseButtonTab1->setEnabled(true);
    ui->runButtonTab1->setEnabled(false);
    ui->startButtonTab1->setEnabled(true);
}

void MainWindow::on_pauseButtonTab1_clicked()
{
    simulation[SimulationType::DIFFUSION]->pause(true);
    ui->pauseButtonTab1->setEnabled(false);
    ui->runButtonTab1->setEnabled(true);
    ui->startButtonTab1->setEnabled(false);
}

void MainWindow::on_tabWidget_currentChanged( int index )
{
    if( simulation[current]->getCalculationState() != "End" )
    {
        simulation[current]->end();
        calculationThread.join();        
    }

    calculationThread = simulation[simulationTab[index]]->calculateNextPositionsInThread();
    if( simulation[simulationTab[index]]->isPauseByUser() )
    {
        simulation[simulationTab[index]]->pause();
    }

    current = simulationTab[index];
}

void MainWindow::on_startButtonTab1_clicked()
{
    if( ui->startButtonTab1->text() == "OPEN" )
    {        
        ui->startButtonTab1->setText("RESET");
        dividerGapAnimationStart = true;
        simulation[SimulationType::DIFFUSION]->setAverageDiffusionTemperature();
    }
    else if( ui->startButtonTab1->text() == "RESET" )
    {
        ui->startButtonTab1->setText("OPEN");
        dividerGapAnimationStart = false;
        dividerGapPosition = 0;
        simulation[SimulationType::DIFFUSION]->reset();
        ui->particleSizeBlueDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getSizeOfParticleInPercent(ParticleType::BLUE));
        ui->particleSizeRedDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getSizeOfParticleInPercent(ParticleType::RED));
        ui->visualizationTypeParticleRadioButtonTab1->setChecked(true);
        ui->gapSizeSliderTab1->setValue(0);
    }

    ui->temperatureLeftDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getTemperatureLeftInPercent());
    ui->temperatureRightDialTab1->setValue(simulation[SimulationType::DIFFUSION]->getTemperatureRightInPercent());
}

void MainWindow::on_temperatureLeftDialTab1_valueChanged( int value )
{
    simulation[SimulationType::DIFFUSION]->setTemperatureLeftInPercent(value);
    ui->blueTemperatureLabelTab1->setText(QString::number(value));
}

void MainWindow::on_temperatureRightDialTab1_valueChanged(int value)
{
    simulation[SimulationType::DIFFUSION]->setTemperatureRightInPercent(value);
    ui->redTemperatureLabelTab1->setText(QString::number(value));
}

void MainWindow::on_particleSizeBlueDialTab1_valueChanged(int value)
{
    simulation[SimulationType::DIFFUSION]->setSizeOfParticlesInPercent(ParticleType::BLUE,value);
    ui->blueSizeLabelTab1->setText(QString::number(value));
}

void MainWindow::on_particleSizeRedDialTab1_valueChanged(int value)
{
    simulation[SimulationType::DIFFUSION]->setSizeOfParticlesInPercent(ParticleType::RED,value);
    ui->redSizeLabelTab1->setText(QString::number(value));
}

void MainWindow::on_visualizationTypeVelocityRadioButtonTab1_toggled( bool checked )
{
    if( checked ) simulation[SimulationType::DIFFUSION]->setVisualizationType(VisualizationType::VELOCITY);
    else simulation[SimulationType::DIFFUSION]->setVisualizationType(VisualizationType::PARTICLE);
}

void MainWindow::on_gapSizeSliderTab1_valueChanged( int value )
{
    if( ui->startButtonTab1->text() == "OPEN" && value>0 )
    {
        simulation[current]->setAverageDiffusionTemperature();
        ui->startButtonTab1->setText("RESET");
    }
    simulation[SimulationType::DIFFUSION]->setDividerGap(value);
}

void MainWindow::on_temperatureUpSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setSideTemperatureInPercent(PlaneSide::UP,value);
    simulation[SimulationType::SANDBOX]->setDisplay(ActionType::D_TEMPERATURE,PlaneSide::UP,value);
}

void MainWindow::on_temperatureRightSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setSideTemperatureInPercent(PlaneSide::RIGHT,value);
    simulation[SimulationType::SANDBOX]->setDisplay(ActionType::D_TEMPERATURE,PlaneSide::RIGHT,value);
}

void MainWindow::on_temperatureDownSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setSideTemperatureInPercent(PlaneSide::DOWN,value);
    simulation[SimulationType::SANDBOX]->setDisplay(ActionType::D_TEMPERATURE,PlaneSide::DOWN,value);
}

void MainWindow::on_temperatureLeftSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setSideTemperatureInPercent(PlaneSide::LEFT,value);
    simulation[SimulationType::SANDBOX]->setDisplay(ActionType::D_TEMPERATURE,PlaneSide::LEFT,value);
}

void MainWindow::on_horizontalForceTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setHorizontalForceInPercent(value);
    ui->horizontalForceSizeLabelTab2->setText(QString::number(value));
}

void MainWindow::on_verticalForceTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setVerticalForceInPercent(value);
    ui->verticalForceSizeLabelTab2->setText(QString::number(value));
}

void MainWindow::on_runButtonTab2_clicked()
{
    simulation[SimulationType::SANDBOX]->run(true);
    ui->pauseButtonTab2->setEnabled(true);
    ui->runButtonTab2->setEnabled(false);
}

void MainWindow::on_pauseButtonTab2_clicked()
{
    simulation[SimulationType::SANDBOX]->pause(true);
    ui->pauseButtonTab2->setEnabled(false);
    ui->runButtonTab2->setEnabled(true);
}

void MainWindow::on_blueParticlesSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setParticlesInPlane(ParticleType::GAS1,value);
    ui->particlesNumberGas1LabellTab2->setText(QString::number(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS1)));
    double posx = static_cast<double>(ui->blueParticlesSliderTab2->width()-10)*(static_cast<double>(value)/static_cast<double>(abs(ui->blueParticlesSliderTab2->maximum()-ui->blueParticlesSliderTab2->minimum())));
    ui->particlesNumberGas1LabellTab2->move(ui->blueParticlesSliderTab2->pos().x()-15+static_cast<int>(posx),ui->particlesNumberGas1LabellTab2->pos().y());
}

void MainWindow::on_redParticlesSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setParticlesInPlane(ParticleType::GAS2,value);
    ui->particlesNumberGas2LabellTab2->setText(QString::number(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS2)));
    double posx = static_cast<double>(ui->redParticlesSliderTab2->width()-10)*(static_cast<double>(value)/static_cast<double>(abs(ui->redParticlesSliderTab2->maximum()-ui->redParticlesSliderTab2->minimum())));
    ui->particlesNumberGas2LabellTab2->move(ui->redParticlesSliderTab2->pos().x()-15+static_cast<int>(posx),ui->particlesNumberGas2LabellTab2->pos().y());
}

void MainWindow::on_greenParticlesSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setParticlesInPlane(ParticleType::GAS3,value);
    ui->particlesNumberGas3LabellTab2->setText(QString::number(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS3)));
    double posx = static_cast<double>(ui->greenParticlesSliderTab2->width()-10)*(static_cast<double>(value)/static_cast<double>(abs(ui->greenParticlesSliderTab2->maximum()-ui->greenParticlesSliderTab2->minimum())));
    ui->particlesNumberGas3LabellTab2->move(ui->greenParticlesSliderTab2->pos().x()-15+static_cast<int>(posx),ui->particlesNumberGas3LabellTab2->pos().y());
}

void MainWindow::on_blueParticlesSizeSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setSizeOfParticlesInPercent(ParticleType::GAS1,value);
    ui->particlesSizeGas1LabellTab2->setText(QString::number(simulation[SimulationType::SANDBOX]->getSizeOfParticle(ParticleType::GAS1)));
    double posx = static_cast<double>(ui->blueParticlesSizeSliderTab2->width()-10)*(static_cast<double>(value)/static_cast<double>(abs(ui->blueParticlesSizeSliderTab2->maximum()-ui->blueParticlesSizeSliderTab2->minimum())));
    ui->particlesSizeGas1LabellTab2->move(ui->blueParticlesSizeSliderTab2->pos().x()-15+static_cast<int>(posx),ui->particlesSizeGas1LabellTab2->pos().y());
}

void MainWindow::on_redParticlesSizeSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setSizeOfParticlesInPercent(ParticleType::GAS2,value);
    ui->particlesSizeGas2LabellTab2->setText(QString::number(simulation[SimulationType::SANDBOX]->getSizeOfParticle(ParticleType::GAS2)));
    double posx = static_cast<double>(ui->redParticlesSizeSliderTab2->width()-10)*(static_cast<double>(value)/static_cast<double>(abs(ui->redParticlesSizeSliderTab2->maximum()-ui->redParticlesSizeSliderTab2->minimum())));
    ui->particlesSizeGas2LabellTab2->move(ui->redParticlesSizeSliderTab2->pos().x()-15+static_cast<int>(posx),ui->particlesSizeGas2LabellTab2->pos().y());
}

void MainWindow::on_greenParticlesSizeSliderTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setSizeOfParticlesInPercent(ParticleType::GAS3,value);
    ui->particlesSizeGas3LabellTab2->setText(QString::number(simulation[SimulationType::SANDBOX]->getSizeOfParticle(ParticleType::GAS3)));
    double posx = static_cast<double>(ui->greenParticlesSizeSliderTab2->width()-10)*(static_cast<double>(value)/static_cast<double>(abs(ui->greenParticlesSizeSliderTab2->maximum()-ui->greenParticlesSizeSliderTab2->minimum())));
    ui->particlesSizeGas3LabellTab2->move(ui->greenParticlesSizeSliderTab2->pos().x()-15+static_cast<int>(posx),ui->particlesSizeGas3LabellTab2->pos().y());
}

void MainWindow::on_visualizationTypeVelocityRadioButtonTab2_toggled( bool checked )
{
    if( checked ) simulation[SimulationType::SANDBOX]->setVisualizationType(VisualizationType::VELOCITY);
    else simulation[SimulationType::SANDBOX]->setVisualizationType(VisualizationType::PARTICLE);
}

void MainWindow::on_attractionForceTab2_valueChanged( int value )
{
    simulation[SimulationType::SANDBOX]->setAttractionForceInPercent( value );
    ui->atractionForceSizeLabelTab2->setText(QString::number(value));
}

void MainWindow::on_runButtonTab4_clicked()
{
    simulation[SimulationType::BROWNIAN_MOTION]->run(true);
    ui->pauseButtonTab4->setEnabled(true);
    ui->runButtonTab4->setEnabled(false);
}

void MainWindow::on_pauseButtonTab4_clicked()
{
    simulation[SimulationType::BROWNIAN_MOTION]->pause(true);
    ui->pauseButtonTab4->setEnabled(false);
    ui->runButtonTab4->setEnabled(true);
}

void MainWindow::on_massMoleculeSliderTab4_valueChanged( int value )
{
    simulation[SimulationType::BROWNIAN_MOTION]->setMassOfMoleculeInPercent( value );
    ui->massMoleculeLabelTab4->setText( QString::number(value) );
}

void MainWindow::on_stopMoleculePushButtonTab4_clicked()
{
    simulation[SimulationType::BROWNIAN_MOTION]->stopMolecule();
}

void MainWindow::on_clearTrackMoleculePushButtonTab4_clicked()
{
    simulation[SimulationType::BROWNIAN_MOTION]->clearMoleculeTrace();
}

void MainWindow::on_resetPushButton_tab2_clicked()
{
    simulation[SimulationType::SANDBOX]->reset();

    ui->verticalForceTab2->setValue(0);
    ui->horizontalForceTab2->setValue(0);
    ui->attractionForceTab2->setValue(0);

    ui->temperatureUpSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::UP));
    ui->temperatureDownSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::DOWN));
    ui->temperatureLeftSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::LEFT));
    ui->temperatureRightSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSideTemperatureInPercent(PlaneSide::RIGHT));

    ui->blueParticlesSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS1));
    ui->redParticlesSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS2));
    ui->greenParticlesSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getNumberOfParticles(ParticleType::GAS3));

    ui->blueParticlesSizeSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSizeOfParticleInPercent(ParticleType::GAS1));
    ui->redParticlesSizeSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSizeOfParticleInPercent(ParticleType::GAS2));
    ui->greenParticlesSizeSliderTab2->setValue(simulation[SimulationType::SANDBOX]->getSizeOfParticleInPercent(ParticleType::GAS3));
}

void MainWindow::on_randomPushButton_tab2_clicked()
{
     simulation[SimulationType::SANDBOX]->reset();

     ui->blueParticlesSizeSliderTab2->setValue(QRandomGenerator::global()->bounded(simulation[SimulationType::SANDBOX]->getSimulationInfo().maxSizeOfParticle.at(SimulationType::SANDBOX)));
     ui->redParticlesSizeSliderTab2->setValue(QRandomGenerator::global()->bounded(simulation[SimulationType::SANDBOX]->getSimulationInfo().maxSizeOfParticle.at(SimulationType::SANDBOX)));
     ui->greenParticlesSizeSliderTab2->setValue(QRandomGenerator::global()->bounded(simulation[SimulationType::SANDBOX]->getSimulationInfo().maxSizeOfParticle.at(SimulationType::SANDBOX)));

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
