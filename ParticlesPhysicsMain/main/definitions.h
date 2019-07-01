#pragma once

#define GSL_THROW_ON_CONTRACT_VIOLATION
#include <gsl_assert>

#include <vector>
#include <memory>
#include <future>
#include <list>
#include <unordered_map>
#include <map>
#include "random.hpp"
#include "vector2d.h"

/** @file
 * @brief Definitions
 */

class Cluster;
class Particle;
class BarChart;
class BarDisplay;
class Histogram1D;

/** @enum ThreadCalculationState
 *  @brief Representing calculation thread state
 */
enum class ThreadCalculationState
{
    RUNNING , /**< calculation in thread is running */
    PAUSE   , /**< calculation in thread is pausing */
    END       /**< calculation is ended  */
};

/** @enum SimulationType
 *  @brief Representing simulation type
 */
enum class SimulationType
{
    BASIC           , /**< basic simulation type */
    DIFFUSION       , /**< diffiusion simulation type */
    BROWNIAN_MOTION , /**< brownian motion simulation type */
    SANDBOX           /**< sandbox simulation type */
};

/** @enum ParticleType
 *  @brief Representing the particle type
 */
enum class ParticleType
{
    BLUE        , /**< used in DIFFUSION type */
    RED         , /**< used in DIFFUSION type */
    NORMAL      , /**< used in BASIC type */
    GAS1        , /**< used in SANDBOX type */
    GAS2        , /**< used in SANDBOX type */
    GAS3        , /**< used in SANDBOX type */
    MINI        , /**< used in BROWNIAN_MOTION type */
    MACROSCOPIC   /**< used in BROWNIAN_MOTION type */
};

/** @enum PlaneSide
 *  @brief Representing particle plane side
 */
enum class PlaneSide
{
    UP    , /**< upper particle side */
    DOWN  , /**< down particle side */
    RIGHT , /**< right particle side */
    LEFT    /**< left particle side */
};

/** @enum VisualizationType
 *  @brief Representing the particle visualization
 */
enum class VisualizationType
{
    VELOCITY , /**< particle color depends on particle velocity */
    PARTICLE   /**< particle color depends on particle type */
};

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

/** @struct colorRGB
 *  @brief Defines color Red Green and Blue values (0-255)
 */
struct colorRGB
{
    unsigned char R {0};
    unsigned char G {0};
    unsigned char B {0};
};

/** @struct SimulationInfo
 *  @brief Defines main simulation parameters
 */
struct SimulationInfo
{       
    int    maxPlaneXConstraint  {30}  ; /**< maximum value of plane X axis constraint (in percent of planeWidth value) */
    int    calculationCount     {0}   ; /**< current number of calculateNextPositions calls, sets to zero after end of time period */
    int    avgCalculationCount  {0}   ; /**< average number of calculateNextPositions calls in a period of time */
    int    minSizeOfParticle    {5}   ; /**< minimum size of particle (in pixels) */
    double maxTimeContribution  {1.5} ; /**< maximum time contribution in calculating the next particle positions */
    double planeFillCoefficient {2.5} ; /**< defines a capacity of the particle plane, increase it to reduce plane capacity */

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
                                              {ParticleType::MINI,5},
                                              {ParticleType::MACROSCOPIC,40}
                                            };

    std::map<ParticleType,int> numberOfParticles {
                                                   {ParticleType::NORMAL,0},
                                                   {ParticleType::BLUE,0},
                                                   {ParticleType::RED,0},
                                                   {ParticleType::GAS1,0},
                                                   {ParticleType::GAS2,0},
                                                   {ParticleType::GAS3,0},
                                                   {ParticleType::MINI,0},
                                                   {ParticleType::MACROSCOPIC,0}
                                                 };

    std::map<ParticleType,const int> numberOfParticlesInit {
                                                             {ParticleType::NORMAL,250},
                                                             {ParticleType::BLUE,150},
                                                             {ParticleType::RED,150},
                                                             {ParticleType::GAS1,50},
                                                             {ParticleType::GAS2,50},
                                                             {ParticleType::GAS3,50},
                                                             {ParticleType::MINI,1000},
                                                             {ParticleType::MACROSCOPIC,1}
                                                           };

    std::map<ParticleType,const int> particleSizeInit  {
                                                         {ParticleType::NORMAL,20},
                                                         {ParticleType::BLUE,20},
                                                         {ParticleType::RED,20},
                                                         {ParticleType::GAS1,10},
                                                         {ParticleType::GAS2,20},
                                                         {ParticleType::GAS3,30},
                                                         {ParticleType::MINI,5},
                                                         {ParticleType::MACROSCOPIC,40}
                                                       };

    std::map<SimulationType,int> planeBorderSize {
                                                   {SimulationType::BASIC,8},
                                                   {SimulationType::DIFFUSION,1},
                                                   {SimulationType::BROWNIAN_MOTION,1},
                                                   {SimulationType::SANDBOX,1}
                                                 };
};

/** @struct PhysicsInfo
 *  @brief Defines global physics quantities describes states of particles
 */
struct PhysicsInfo
{
    double       maxRapidity           {1.0}; /**< maximum rapidity constrain */
    double       minRapidity           {0.0}; /**< minimum rapidity constrain */
    double       maxSideForce       {0.0005}; /**< maximum value of side force */
    double       temperature           {0.1}; /**< boundries temperature in the particles plane - SimulationType::BASIC and SimulationType::SANDBOX mode */
    double       temperatureLeft       {0.1}; /**< boundries temperature in the left part of particles plane - SimulationType::DIFFUSION mode */
    double       temperatureRight      {0.6}; /**< boundries temperature in the right part of particles plane - SimulationType::DIFFUSION mode */
    double       attractionForce       {0.0}; /**< attraction force between particles */
    vect2D       pushForce         {0.0,0.0}; /**< push force attracted by all particles - may simulate gravity */
    double       maxAttractionForce  {0.001}; /**< maximum of attraction force between particles */
    double       avgVelocityRed        {0.0}; /**< average velocity of red particles - SimulationType::DIFFUSION mode */
    double       avgVelocityBlue       {0.0}; /**< average velocity of blue particles - SimulationType::DIFFUSION mode */
    double       avgVelocity           {0.0}; /**< average velocity of all particles */
    double       avgDistance           {0.0}; /**< average distance between particles */
    int          numOfCollision        {0};   /**< number of particle collided with the plane */
    double       numOfCollisionTP      {0.0}; /**< number of particle collided with the plane in a period of time */
    int          numBlueParticlesLeft  {0};   /**< number of blue particles in the left part of plane - SimulationType::DIFFUSION mode */
    int          numBlueParticlesRight {0};   /**< number of blue particles in the right part of plane - SimulationType::DIFFUSION mode */
    int          numRedParticlesLeft   {0};   /**< number of red particles in the left part of plane - SimulationType::DIFFUSION mode */
    int          numRedParticlesRight  {0};   /**< number of red particles in the right part of plane - SimulationType::DIFFUSION mode */
    double       kineticEnergySum      {0};   /**< summed particle kinetic energies transfer to the plane */
    double       kineticEnergySumTP    {0};   /**< summed particle kinetic energies transfer to the plane in a period of time */
    double       averageKineticEnergy  {0};   /**< average kinetic energy of particles */
    double       energyFromPlaneSum    {0};   /**< summed energy transfered from the plane to particles */
    double       energyFromPlaneSumTP  {0};   /**< summed energy transfered from the plane in a period of time */
    double       collVelocityChange    {0};   /**< use in testing */
    Milliseconds timePeriod          {200};   /**< period of time in miliseconds */

    /**< temperature of a given plane side */
    std::map<PlaneSide,double> planeSideTemperature {
                                                      {PlaneSide::UP,0.1},
                                                      {PlaneSide::DOWN,0.1},
                                                      {PlaneSide::RIGHT,0.1},
                                                      {PlaneSide::LEFT,0.1}
                                                    };
};

/** @struct ClustersInfo
 *  @brief Defines common parameters of clusters
 */
struct ClustersInfo
{
    int clusterRows            {0}; /**< number of cluster rows */
    int clusterColumns         {0}; /**< number of cluster columns */
    int clusterSize            {0}; /**< size of cluster */
    int numberOfClusters       {0}; /**< number of all clusters */
    const int clusterMinSizeX {10}; /**< minimum horizontal size of cluster */
    const int clusterMinSizeY {10}; /**< minimum vertical size of cluster */
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
