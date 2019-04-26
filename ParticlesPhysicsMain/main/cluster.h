#pragma once

#include "definitions.h"

/** @file
 * @brief Class @ref Cluster
 */

/**
 * @class Cluster
 * @brief Defines a rectangle part of the particle plane.
 *
 * Stores particles, provides coordinates and connections between other clusters.
 * @author Tomasz Gburek
 * @date 2019
 */

class Cluster
{

    ptrListParticleIters  particlesInCluster ; /**< unique pointer owns list of iterators to particles inside cluster */
    ptrVectorClusterIters adjoinClusters     ; /**< unique pointer owns vector of iterators to connected (adjoin) clusters */

    int id {0}; /**< cluster id */

    coord2D coordinates {0,0}; /**< cluster 2D coordinates */

public:

    /**
     * @brief Constructor
     *
     * Note that it was defined as deafult, for the possibility of use operator[] inside std::map container.
     */
    Cluster() = default;

    /**
     * @brief Constructor
     * @param id              cluster id
     * @param coordinates     cluster 2D coordinates
     */
    Cluster( int id , coord2D coordinates );

    /**
     * @brief Constructor
     * @param id              cluster id
     * @param x               cluster X axis coordinate
     * @param y               cluster Y axis coordinate
     */
    Cluster( int id, int x, int y );

    /** @brief Copy constructor */
    Cluster( const Cluster& );

    /** @brief Move constructor */
    Cluster( Cluster&& ) noexcept;

    /** @brief Copy assigment operator */
    Cluster& operator=( const Cluster& );

    /** @brief Move assigment operator */
    Cluster& operator=( Cluster&& ) noexcept;

    /**
     * @brief Adds particle iterator to cluster.
     * @param iterParticle    particle iterator
     */
    inline void addParticle( iterParticle particle ) { particlesInCluster->push_back(std::move(particle)); }

    /**
     * @brief Returns adjoin clusters.
     * @return pointer to vector of iterators to connected (adjoin) clusters
     */
    inline ptrVectorClusterIters& getAdjoinClusters() noexcept { return adjoinClusters; }

    /**
     * @brief Returns particles in cluster.
     * @return pointer to list of iterators to particles inside cluster
     */
    inline ptrListParticleIters& getParticlesInCluster() noexcept { return particlesInCluster; }

    /**
     * @brief Returns cluster id.
     * @return cluster id
     */
    inline int getID() const noexcept { return id; }

    /**
     * @brief Returns cluster coordinates.
     * @return cluster coordinates
     */
    inline coord2D getCoordinates() const noexcept { return coordinates; }

    /**
     * @brief Removes a given particle from cluster.
     * @param iterParticle    iterator to remove particle
     */
    inline void removeParticle( const iterParticle& particle ) { particlesInCluster->remove(particle); }

    bool PLANE_INSIDE  {true}  ;  /**< state if cluster is located inside the particle plane and not connected with a plane boundry */
    bool PLANE_BOUNDRY {false} ;  /**< state if cluster is connected with a plane boundry */
    bool PLANE_DIVIDER {false} ;  /**< state if cluster is connected with the plane divider */

};
