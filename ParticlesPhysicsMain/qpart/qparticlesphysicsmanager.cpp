#include "qparticlesphysicsmanager.h"

QParticlesPhysicsManager::QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout )
: QObject {layout}, ParticlesPhysicsManager(type,layout->parentWidget()->width(),layout->parentWidget()->height() )
{
    particlesPaintManager = new QPainterManager();
    layout->addWidget( particlesPaintManager );

    connect(particlesPaintManager,&QPainterManager::particlePositionChanged,this,&QParticlesPhysicsManager::particlePositionChanged);
    connect(particlesPaintManager->findChild<QBoxEdit*>("EditBox"),&QBoxEdit::particleVelocityChanged,this,&QParticlesPhysicsManager::particleVelocityChanged);
}

void QParticlesPhysicsManager::add( QHBoxLayout* layout, BoxType boxType, ActionType actionType, BoxStyles style )
{
    switch( boxType )
    {
     case BoxType::BARCHART :
        if( analyzer->barCharts->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QBarChart(actionType,sqrt(analyzer->physicsInfo.maxRapidity*2)/3,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
            if( style != BoxStyles::DEFAULT ) qBoxPainters[actionType]->loadStyle(style);
        }
     break;

     case BoxType::BARDISPLAY :
        if( analyzer->barDisplays->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QBarDisplay(actionType,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
        }
     break;

     case BoxType::HISTOGRAM1D :
        if( analyzer->histograms1D->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QHistogram1D(actionType,160,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
        }
     break;

     case BoxType::TRACKINGPLOT :
        qBoxPainters[actionType] = new QTrackingPlot2D("Molecule Tracking Plot",layout->parentWidget());
        layout->addWidget( qBoxPainters[actionType] );
     break;

     case BoxType::GAUGE :
        addQGauge( actionType , layout );
     break;

     case BoxType::CIRCLECONTROL :
        qBoxPainters[actionType] = new QCircleControl(layout->parentWidget());
        controlBoxType[actionType] = ControlType::CIRCLE_CONTROL;
        layout->addWidget( qBoxPainters[actionType] );
     break;

     case BoxType::INFODISPLAY :
        qBoxPainters[actionType] = new QInfoDisplay();
        layout->addWidget( qBoxPainters[actionType] );
     break;
    }
}

void QParticlesPhysicsManager::addQGauge( ActionType type, QHBoxLayout* layout )
{
    qGauges[type] = { nullptr , nullptr };

    int range {1000};

    qGauges[type].first = new QcGaugeWidget();
    qGauges[type].first->addArc(55);
    QcDegreesItem* degreeItem = qGauges[type].first->addDegrees(65);
    degreeItem->setValueRange(0,range);
    degreeItem->setStep(range/10);
    QcColorBand *clrBand =  qGauges[type].first->addColorBand(50);
    clrBand->setValueRange(0,100);
    QcValuesItem *valueItem = qGauges[type].first->addValues(80);
    valueItem->setValueRange(0,range);
    valueItem->setStep(range/10);
    gaugeNameLabel = qGauges[type].first->addLabel(70);
    gaugeNameLabel->setText(LangManager::translate(QString("Pressure")));
    QcLabelItem *lab = qGauges[type].first->addLabel(40);
    lab->setText("0");
    qGauges[type].second = qGauges[type].first->addNeedle(60);
    qGauges[type].second->setLabel(lab);
    qGauges[type].second->setColor(Qt::blue);
    qGauges[type].second->setValueRange(0,range);
    qGauges[type].first->addBackground(7);

    // reset style sheet - needs default
    qGauges[type].first->setStyleSheet("");

    layout->addWidget( qGauges[type].first );
}

void QParticlesPhysicsManager::updateParticlesPlane()
{
    particlesPaintManager->update();
}

void QParticlesPhysicsManager::paintLayouts()
{
    updateBars();
    for( auto &painter : qBoxPainters ){ painter.second->update(); }
    for( auto &gauge : qGauges )
    {
        gaugeNameLabel->setText(LangManager::translate(gaugeNameLabel->text()));
        gauge.second.second->setCurrentValue( getPressureValue() );
    }
}

void QParticlesPhysicsManager::handleControls()
{
    for( auto &control : controlBoxType )
    {
        if( control.second == ControlType::CIRCLE_CONTROL )
        {
            QPoint forceOut = dynamic_cast<QCircleControl*>(qBoxPainters[control.first].data())->getIndicator();
            vect2D forceIn(0.00001*forceOut.x(),0.00001*forceOut.y());
            setForce(forceIn);
        }
    }
}

void QParticlesPhysicsManager::saveState( QString filename )
{
    if( filename.isEmpty() )
    {
        filename = "save"+ QString::number(QRandomGenerator::global()->bounded(100));
    }

    QDir pathDir {qApp->applicationDirPath()+"/templates/"};
    QString filepath {};

    filepath = pathDir.exists()?pathDir.path()+"/":"D:/Programming/GitHub/Qt/ParticlesPhysics/ParticlesPhysicsMain/templates/";
    filepath += filename+QStringLiteral(".json");

    QFile file {filepath};

    if( !file.open(QIODevice::WriteOnly) )
    {
       qWarning("Couldn't open save file");
       return;
    }

    pause();

    QJsonObject jsonMain;

    jsonMain["planeWidth"] = planeArea->getWidth();
    jsonMain["planeHeight"] = planeArea->getHeight();

    jsonMain["temperatureUP"] = getTemperature( PlanePart::UP , DataFormat::SCALAR );
    jsonMain["temperatureDown"] = getTemperature( PlanePart::DOWN , DataFormat::SCALAR );
    jsonMain["temperatureLeft"] = getTemperature( PlanePart::LEFT , DataFormat::SCALAR );
    jsonMain["temperatureRight"] = getTemperature( PlanePart::RIGHT , DataFormat::SCALAR );

    jsonMain["verticalForce"] = getForce( Axis::VERTICAL , DataFormat::SCALAR );
    jsonMain["horizontalForce"] = getForce( Axis::HORIZONTAL , DataFormat::SCALAR );
    jsonMain["attractionForce"] = analyzer->physicsInfo.attractionForce;

    QJsonArray jsonArray;
    for( const auto& particle : *particles )
    {
       QJsonObject jsonParticle;

       jsonParticle["positionX"] = particle.position.x;
       jsonParticle["positionY"] = particle.position.y;
       jsonParticle["velocityX"] = particle.velocity.x;
       jsonParticle["velocityY"] = particle.velocity.y;
       jsonParticle["radius"] = particle.radius;
       jsonParticle["type"] = static_cast<int>(particle.particleType);

       jsonArray.append(jsonParticle);
    }

    jsonMain["Particles"] = jsonArray;
    QJsonDocument saveDoc(jsonMain);
    file.write(saveDoc.toJson());

    run();
}

void QParticlesPhysicsManager::loadState( QString filename )
{
    QDir pathDir {qApp->applicationDirPath()+"/templates/"};
    QString filepath {};

    filepath = pathDir.exists()?pathDir.path()+"/":"D:/Programming/GitHub/Qt/ParticlesPhysics/ParticlesPhysicsMain/templates/";
    filepath += filename;

    QFile file {filepath};

    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        qDebug() << "Wrong file name\n";
        return;
    }

    QJsonDocument loadDoc( QJsonDocument::fromJson(file.readAll()) );
    auto jsonMain = loadDoc.object();

    setTemperature( PlanePart::UP , DataFormat::SCALAR , jsonMain["temperatureUP"].toDouble() );
    setTemperature( PlanePart::DOWN , DataFormat::SCALAR , jsonMain["temperatureDOWN"].toDouble() );
    setTemperature( PlanePart::LEFT , DataFormat::SCALAR , jsonMain["temperatureLEFT"].toDouble() );
    setTemperature( PlanePart::RIGHT , DataFormat::SCALAR , jsonMain["temperatureRIGHT"].toDouble() );

    setForce( Axis::VERTICAL , DataFormat::SCALAR , jsonMain["verticalForce"].toDouble() );
    setForce( Axis::HORIZONTAL , DataFormat::SCALAR , jsonMain["horizontalForce"].toDouble() );
    analyzer->physicsInfo.attractionForce = jsonMain["attractionForce"].toDouble();

    QJsonArray jsonArray = jsonMain["Particles"].toArray();
}

void QParticlesPhysicsManager::particlePositionChanged( citerParticle particle , vect2D newPosition )
{
    setParticlePosition(remove_constness(*particles,particle),newPosition);
}

void QParticlesPhysicsManager::particleVelocityChanged( citerParticle particle , vect2D newVelocity )
{
    setParticleVelocity(remove_constness(*particles,particle),newVelocity);
}
