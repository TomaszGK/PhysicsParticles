#include "simulationanalyzer.h"


SimulationAnalyzer::SimulationAnalyzer( cptrParticlesContainer _ptrParticles )
: ptrParticles {  std::move(_ptrParticles) }
{

}

void SimulationAnalyzer::resetPhysicsData()
{
    physicsInfo = PhysicsInfo();
}

void SimulationAnalyzer::update()
{

}
