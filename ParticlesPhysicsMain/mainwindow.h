#pragma once

#include <QMainWindow>
#include <QTimer>
#include <dialogabout.h>
#include <qparticlesphysicsmanager.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

    MainWindow( const MainWindow& ) = delete;
    MainWindow( MainWindow&& ) = delete;

    MainWindow& operator=( const MainWindow& ) = delete;
    MainWindow& operator=( MainWindow&& ) = delete;

public slots:

    void update();
    void animate();
    void paint();

private slots:

    void about_action();    

    void load_scheme();

    void on_numberOfParticlesSliderTab0_valueChanged(int value);

    void on_runButtonTab0_clicked();

    void on_pauseButtonTab0_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_runButtonTab1_clicked();

    void on_pauseButtonTab1_clicked();

    void on_startButtonTab1_clicked();

    void on_temperatureLeftDialTab1_valueChanged(int value);

    void on_temperatureRightDialTab1_valueChanged(int value);

    void on_sizeOfParticleSliderTab0_valueChanged(int value);

    void on_trackingOnButtonTab0_clicked();

    void on_trackingOffButtonTab0_clicked();

    void on_particleSizeBlueDialTab1_valueChanged(int value);

    void on_particleSizeRedDialTab1_valueChanged(int value);

    void on_visualizationTypeVelocityRadioButtonTab1_toggled(bool checked);

    void on_gapSizeSliderTab1_valueChanged(int value);

    void on_temperatureUpSliderTab2_valueChanged(int value);

    void on_temperatureRightSliderTab2_valueChanged(int value);

    void on_temperatureDownSliderTab2_valueChanged(int value);

    void on_temperatureLeftSliderTab2_valueChanged(int value);    

    void on_planeSizeDialTab0_valueChanged(int value);

    void on_horizontalForceTab2_valueChanged(int value);

    void on_verticalForceTab2_valueChanged(int value);

    void on_runButtonTab2_clicked();

    void on_pauseButtonTab2_clicked();

    void on_blueParticlesSliderTab2_valueChanged(int value);

    void on_redParticlesSliderTab2_valueChanged(int value);

    void on_greenParticlesSliderTab2_valueChanged(int value);

    void on_blueParticlesSizeSliderTab2_valueChanged(int value);

    void on_redParticlesSizeSliderTab2_valueChanged(int value);

    void on_greenParticlesSizeSliderTab2_valueChanged(int value);

    void on_visualizationTypeVelocityRadioButtonTab2_toggled(bool checked);

    void on_attractionForceTab2_valueChanged(int value);

    void on_temperatureDialTab0_valueChanged(int value);

    void on_runButtonTab4_clicked();

    void on_pauseButtonTab4_clicked();

    void on_massMoleculeSliderTab4_valueChanged(int value);

    void on_stopMoleculePushButtonTab4_clicked();

    void on_clearTrackMoleculePushButtonTab4_clicked();

    void on_resetPushButton_tab2_clicked();

    void on_randomPushButton_tab2_clicked();

private:

    Ui::MainWindow *ui;

    DialogAbout dialogAboutWindow;

    std::vector<std::unique_ptr<QParticlesPhysicsManager>> physicsSimulation;  

    QTimer paintTimer;
    QTimer updateTimer;
    QTimer animationTimer;

    QAction* menuItemPreviousCheck {nullptr};

    size_t currentIndex {0};
    double dividerGapPosition {0};
    double dividerGapStep {0.1};
    bool   dividerGapAnimationStart {false};

    std::thread calculationThread;

};
