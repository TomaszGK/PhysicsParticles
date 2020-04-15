#include "qparticlesphysicsmanager.h"
#include <cmath>

QParticlesPhysicsManager::QParticlesPhysicsManager( SimulationType type, QHBoxLayout* layout )
: QObject {layout}, ParticlesPhysicsManager(type,layout->parentWidget()->width(),layout->parentWidget()->height() )
{
    particlesPaintManager = new QPainterManager(particles,planeArea);
    layout->addWidget( particlesPaintManager );

    connect(particlesPaintManager,&QPainterManager::particlePositionChanged,this,&QParticlesPhysicsManager::particlePositionChanged);
    connect(particlesPaintManager->findChild<QBoxEdit*>("EditBox"),&QBoxEdit::particleVelocityChanged,this,&QParticlesPhysicsManager::particleVelocityChanged);
}

void QParticlesPhysicsManager::add( QHBoxLayout* layout, BoxType boxType, ActionType actionType, BoxStyles style )
{
    if( layout->count()>0 ) return;

    switch( boxType )
    {
     case BoxType::BARCHART :
        if( analyzer->barCharts->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QBarChart(analyzer->barCharts->at(actionType),sqrt(analyzer->physicsInfo.maxRapidity*2)/3,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
            if( style != BoxStyles::DEFAULT ) qBoxPainters[actionType]->loadStyle(style);
        }
     break;

     case BoxType::BARDISPLAY :
        if( analyzer->barDisplays->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QBarDisplay(analyzer->barDisplays->at(actionType),layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
        }
     break;

     case BoxType::HISTOGRAM1D :
        if( analyzer->histograms1D->count(actionType) != 0 )
        {
            qBoxPainters[actionType] = new QHistogram1D(analyzer->histograms1D->at(actionType),160,layout->parentWidget());
            layout->addWidget( qBoxPainters[actionType] );
        }
     break;

     case BoxType::TRACKINGPLOT :
        qBoxPainters[actionType] = new QTrackingPlot2D(particles,"Molecule Tracking Plot",layout->parentWidget());
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
        filename = "NewTemplate";
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

    jsonMain["temperatureUP"] = static_cast<int>(round(getTemperature( PlanePart::UP , DataFormat::PERCENT )));
    jsonMain["temperatureDOWN"] = static_cast<int>(round(getTemperature( PlanePart::DOWN , DataFormat::PERCENT )));
    jsonMain["temperatureLEFT"] = static_cast<int>(round(getTemperature( PlanePart::LEFT , DataFormat::PERCENT )));
    jsonMain["temperatureRIGHT"] = static_cast<int>(round(getTemperature( PlanePart::RIGHT , DataFormat::PERCENT )));

    jsonMain["verticalForce"] = static_cast<int>(round(getForce( Axis::VERTICAL , DataFormat::PERCENT )));
    jsonMain["horizontalForce"] = static_cast<int>(round(getForce( Axis::HORIZONTAL , DataFormat::PERCENT )));
    jsonMain["attractionForce"] = static_cast<int>(round(getAttractionForceInPercent()));

    jsonMain["particleSizeGAS1"] = analyzer->simulationInfo.particleSize[ParticleType::GAS1];
    jsonMain["particleSizeGAS2"] = analyzer->simulationInfo.particleSize[ParticleType::GAS2];
    jsonMain["particleSizeGAS3"] = analyzer->simulationInfo.particleSize[ParticleType::GAS3];

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

    pause();

    QJsonDocument loadDoc( QJsonDocument::fromJson(file.readAll()) );
    auto jsonMain = loadDoc.object();

    double scaleX = planeArea->getWidth()/jsonMain["planeWidth"].toDouble();
    double scaleY = planeArea->getHeight()/jsonMain["planeHeight"].toDouble();

    analyzer->reset();


    setTemperature( PlanePart::UP , DataFormat::PERCENT , jsonMain["temperatureUP"].toInt() );
    setTemperature( PlanePart::DOWN , DataFormat::PERCENT , jsonMain["temperatureDOWN"].toInt() );
    setTemperature( PlanePart::LEFT , DataFormat::PERCENT , jsonMain["temperatureLEFT"].toInt() );
    setTemperature( PlanePart::RIGHT , DataFormat::PERCENT , jsonMain["temperatureRIGHT"].toInt() );


    setForce( Axis::VERTICAL , DataFormat::PERCENT , jsonMain["verticalForce"].toInt() );
    setForce( Axis::HORIZONTAL , DataFormat::PERCENT , jsonMain["horizontalForce"].toInt() );
    analyzer->physicsInfo.attractionForce = jsonMain["attractionForce"].toInt();

    analyzer->simulationInfo.particleSize[ParticleType::GAS1] = jsonMain["particleSizeGAS1"].toInt();
    analyzer->simulationInfo.particleSize[ParticleType::GAS2] = jsonMain["particleSizeGAS2"].toInt();
    analyzer->simulationInfo.particleSize[ParticleType::GAS3] = jsonMain["particleSizeGAS3"].toInt();

    QJsonArray jsonArray = jsonMain["Particles"].toArray();

    calculationStart = time = HRClock::now();
    removeAllParticles();
    removeParticlesFromClusters();

    foreach( const QJsonValue& value , jsonArray )
    {
        QJsonObject particle = value.toObject();
        vect2D position { particle["positionX"].toDouble()*scaleX , particle["positionY"].toDouble()*scaleY };
        vect2D velocity { particle["velocityX"].toDouble() , particle["velocityY"].toDouble() };
        ParticleType particleType = static_cast<ParticleType>(particle["type"].toInt());

        auto size = static_cast<int>(2.0*particle["radius"].toDouble()*scaleX);
        if( size < analyzer->simulationInfo.particleSizeInit[ParticleType::MINI] ) size = analyzer->simulationInfo.particleSizeInit[ParticleType::MINI];

        auto iterCluster = getClusterIter(static_cast<size_t>(position.x),static_cast<size_t>(position.y));
        particles->push_back(Particle(particleType,visualizationType,position,velocity,size,iterCluster));
        iterCluster->addParticle( std::prev(particles->end()) );

        analyzer->simulationInfo.numberOfParticles[particleType]++;
    }

    run();

    particlesPaintManager->update();
}

void QParticlesPhysicsManager::particlePositionChanged( citerParticle particle , vect2D newPosition )
{
    setParticlePosition(remove_constness(*particles,particle),newPosition);
}

void QParticlesPhysicsManager::particleVelocityChanged( citerParticle particle , vect2D newVelocity )
{
    setParticleVelocity(remove_constness(*particles,particle),newVelocity);
}
