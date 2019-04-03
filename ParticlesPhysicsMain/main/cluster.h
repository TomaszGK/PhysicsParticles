#pragma once

#include "definitions.h"

class Cluster
{
    ptrListParticleIters particlesInCluster;
    ptrVectorClusterIters adjoinClusters;

    // cluster id
    int id {0};

    // cluster coordinates
    coord2D coordinates {0,0};

public:

    Cluster() = default;
    Cluster( int,coord2D );
    Cluster( int,int,int );
    Cluster( const Cluster& );
    Cluster( Cluster&& ) noexcept;

    Cluster& operator=( const Cluster& );
    Cluster& operator=( Cluster&& ) noexcept;

    inline void addParticle( iterParticle particle ) { particlesInCluster->push_back(std::move(particle)); }
    inline ptrVectorClusterIters& getAdjoinClusters() noexcept { return adjoinClusters; }
    inline ptrListParticleIters& getParticlesInCluster() noexcept { return particlesInCluster; }
    inline int getID() const noexcept { return id; }
    inline coord2D getCoordinates() const noexcept { return coordinates; }
    inline void removeParticle( const iterParticle& particle ) { particlesInCluster->remove(particle); }

    // information of cluster position inside the plane
    bool PLANE_INSIDE  {true};
    bool PLANE_BOUNDRY {false};
    bool PLANE_DIVIDER {false};
};
