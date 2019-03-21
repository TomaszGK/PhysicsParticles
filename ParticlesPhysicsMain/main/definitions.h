#pragma once

#include <vector>
#include <memory>
#include <future>
#include <list>
#include <unordered_map>
#include <map>
#include "random.hpp"
#include "vector2d.h"

class Cluster;
class Particle;
class BarChart;
class BarDisplay;
class Histogram1D;

enum class ThreadCalculationState { RUNNING , PAUSE , END  };

enum class SimulationType { BASIC , DIFFUSION , BROWNIAN_MOTION , SANDBOX };

enum class ParticleType { BLUE , RED , NORMAL , GAS1 , GAS2 , GAS3 , MINI };

enum class PlaneSide { UP , DOWN , RIGHT , LEFT };

// type of particle visualization
// VELOCITY - particle color depend on its velocity
// PARTICLE - particle color depend on its kind
enum class VisualizationType { VELOCITY , PARTICLE };

using HRClock = std::chrono::high_resolution_clock;
using Milliseconds = std::chrono::milliseconds;
using ptrBarDisplay = std::shared_ptr<BarDisplay>;
using ptrBarChart = std::shared_ptr<BarChart>;
using ptrHistogram1D = std::shared_ptr<Histogram1D>;
using ptrParticlesContainer = std::shared_ptr<std::vector<Particle>>;
using cptrParticlesContainer = std::shared_ptr<const std::vector<Particle>>;
using ptrClustersContainer = std::unique_ptr<std::vector<Cluster>>;
using vect2D = vector2D<double>;
using coord2D = vector2D<int>;
using iterParticle = std::vector<Particle>::iterator;
using citerParticle = std::vector<Particle>::const_iterator;
using iterCluster = std::vector<Cluster>::iterator;
using Random = effolkronium::random_static;
using MapClustersIterCoordinates = std::map<coord2D,iterCluster>;
using ListParticleIters = std::list<iterParticle>;
using VectorClusterIters = std::vector<iterCluster>;
using ptrListParticleIters = std::unique_ptr<ListParticleIters>;
using ptrVectorClusterIters = std::unique_ptr<VectorClusterIters>;
using table2D = std::vector<std::vector<iterCluster>>;

struct colorRGB
{
    int R {0};
    int G {0};
    int B {0};
};

struct SimulationInfo
{       
    int    maxPlaneXConstraint {30}; // maximum plane X axis constraint in percent of planeWidth
    int    calculationCount    {0};
    int    avgCalculationCount {0};
    int    minSizeOfParticle   {5};
    double maxTimeContribution {1.5}; // maximum time contribution in calculating th next particle positions

    std::map<SimulationType,int> maxSizeOfParticle {
                                                     {SimulationType::BASIC,20},
                                                     {SimulationType::DIFFUSION,40},
                                                     {SimulationType::BROWNIAN_MOTION,40},
                                                     {SimulationType::SANDBOX,30}
                                                   };

    std::map<SimulationType,bool> disjointParticles {
                                                      {SimulationType::BASIC,true},
                                                      {SimulationType::DIFFUSION,true},
                                                      {SimulationType::BROWNIAN_MOTION,false},
                                                      {SimulationType::SANDBOX,false}
                                                    };

    std::map<SimulationType,int> maxParticles {
                                                {SimulationType::BASIC,500},
                                                {SimulationType::DIFFUSION,300},
                                                {SimulationType::BROWNIAN_MOTION,1000},
                                                {SimulationType::SANDBOX,1000}
                                              };

    std::map<SimulationType,double> clusterRatio {
                                                   {SimulationType::BASIC,3},
                                                   {SimulationType::DIFFUSION,3},
                                                   {SimulationType::BROWNIAN_MOTION,2},
                                                   {SimulationType::SANDBOX,3}
                                                 };

    std::map<ParticleType,int> particleSize {
                                              {ParticleType::NORMAL,20},
                                              {ParticleType::BLUE,20},
                                              {ParticleType::RED,20},
                                              {ParticleType::GAS1,10},
                                              {ParticleType::GAS2,20},
                                              {ParticleType::GAS3,30},
                                              {ParticleType::MINI,5}
                                            };

    std::map<ParticleType,int> numberOfParticles {
                                                   {ParticleType::NORMAL,0},
                                                   {ParticleType::BLUE,0},
                                                   {ParticleType::RED,0},
                                                   {ParticleType::GAS1,0},
                                                   {ParticleType::GAS2,0},
                                                   {ParticleType::GAS3,0},
                                                   {ParticleType::MINI,0}
                                                 };

    std::map<ParticleType,const int> numberOfParticlesInit {
                                                             {ParticleType::NORMAL,250},
                                                             {ParticleType::BLUE,150},
                                                             {ParticleType::RED,150},
                                                             {ParticleType::GAS1,50},
                                                             {ParticleType::GAS2,50},
                                                             {ParticleType::GAS3,50},
                                                             {ParticleType::MINI,1000}
                                                           };

    std::map<ParticleType,const int> particleSizeInit  {
                                                         {ParticleType::NORMAL,20},
                                                         {ParticleType::BLUE,20},
                                                         {ParticleType::RED,20},
                                                         {ParticleType::GAS1,20},
                                                         {ParticleType::GAS2,20},
                                                         {ParticleType::GAS3,30},
                                                         {ParticleType::MINI,5}
                                                       };

    std::map<SimulationType,int> planeBorderSize {
                                                   {SimulationType::BASIC,8},
                                                   {SimulationType::DIFFUSION,1},
                                                   {SimulationType::BROWNIAN_MOTION,1},
                                                   {SimulationType::SANDBOX,1}
                                                 };
};

struct PhysicsInfo
{
    double       maxRapidity           {1.0}; // maximum rapidity constrain
    double       minRapidity           {0.0}; // minimum rapidity constrain
    double       maxSideForce       {0.0005}; // max absolute value of side force
    double       temperature           {0.1}; // boundries temperature in the particles plane - basic and entropy mode
    double       temperatureLeft       {0.1}; // boundries temperature in the left part of particles plane - diffusion mode
    double       temperatureRight      {0.6}; // boundries temperature in the right part of particles plane - diffusion mode
    double       horizontalForce       {0.0}; // horizontal force attracted by all particles
    double       attractionForce       {0.0}; // attraction force between particles
    double       maxAttractionForce  {0.001}; // maximum of attraction force between particles
    double       verticalForce         {0.0}; // vertical force attracted by all particles
    double       avgVelocityRed        {0.0}; // average velocity of red particles - diffusion mode
    double       avgVelocityBlue       {0.0}; // average velocity of blue particles - diffusion mode
    double       avgVelocity           {0.0}; // average velocity of all particles
    double       avgDistance           {0.0}; // average distance between particles
    int          numOfCollision        {0};   // number of particle collided with the plane
    int          numOfCollisionTP      {0};   // number of particle collided with the plane in a period of time
    int          numBlueParticlesLeft  {0};   // number of blue particles in the left part of plane - diffusion mode
    int          numBlueParticlesRight {0};   // number of blue particles in the right part of plane - diffusion mode
    int          numRedParticlesLeft   {0};   // number of red particles in the left part of plane - diffusion mode
    int          numRedParticlesRight  {0};   // number of red particles in the right part of plane - diffusion mode
    double       kineticEnergySum      {0};   // summed particle kinetic energies transfer to the plane
    double       kineticEnergySumTP    {0};   // summed particle kinetic energies transfer to the plane in a period of time
    double       energyFromPlaneSum    {0};   // summed energy transfered from the plane to particles
    double       energyFromPlaneSumTP  {0};   // summed energy transfered from the plane in a period of time
    double       collVelocityChange    {0};   // use in testing
    Milliseconds timePeriod          {200};   // period of time in miliseconds

    // temperature of a given plane side
    std::map<PlaneSide,double> planeSideTemperature {
                                                      {PlaneSide::UP,0.1},
                                                      {PlaneSide::DOWN,0.1},
                                                      {PlaneSide::RIGHT,0.1},
                                                      {PlaneSide::LEFT,0.1}
                                                    };
};

struct ClustersInfo
{
    int clusterRows      {0};
    int clusterColumns   {0};
    int clusterSize      {0};
    int numberOfClusters {0};
    const int clusterMinSizeX {10};
    const int clusterMinSizeY {10};
};

// C++14 make_unique
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// C++14 true future async
template<typename F, typename... Args>
auto trueAsync(F&& f, Args&&... args)
{
    return std::async( std::launch::async, std::forward<F>(f), std::forward<Args>(args)... );
}

// C++14 generic timing
template<typename F, typename... Args>
auto timing(F&& f, Args&&... args)
{
    auto start = std::chrono::steady_clock::now();
    f(std::forward<Args>(args)...);
    return std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count(); // value in seconds
};
