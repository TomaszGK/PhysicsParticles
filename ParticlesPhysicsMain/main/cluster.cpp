#include "cluster.h"

Cluster::Cluster( int _id , coord2D _coordinates )
: id {_id}, coordinates {_coordinates}
{
    particlesInCluster = std::make_unique<ListParticleIters>();
    adjoinClusters = std::make_unique<VectorClusterIters>();
}

Cluster::Cluster( int _id, int x, int y )
: Cluster( _id , {x,y} )
{    
}

Cluster::Cluster( const Cluster &cluster )
{
    coordinates = cluster.coordinates;
    id = cluster.id;

    particlesInCluster = std::make_unique<ListParticleIters>(*cluster.particlesInCluster);
    adjoinClusters = std::make_unique<VectorClusterIters>(*cluster.adjoinClusters);
}

Cluster::Cluster( Cluster &&cluster ) noexcept
{
    coordinates = cluster.coordinates;
    id = cluster.id;

    particlesInCluster = std::move(cluster.particlesInCluster);
    adjoinClusters = std::move(cluster.adjoinClusters);
}

Cluster &Cluster::operator=( const Cluster &cluster )
{
    if( &cluster == this ) return *this;

    coordinates = cluster.coordinates;
    id = cluster.id;

    particlesInCluster = std::make_unique<ListParticleIters>(*cluster.particlesInCluster);
    adjoinClusters = std::make_unique<VectorClusterIters>(*cluster.adjoinClusters);

    return *this;
}

Cluster &Cluster::operator=( Cluster &&cluster ) noexcept
{
    coordinates = cluster.coordinates;
    id = cluster.id;

    particlesInCluster = std::move(cluster.particlesInCluster);
    adjoinClusters = std::move(cluster.adjoinClusters);

    return *this;
}
