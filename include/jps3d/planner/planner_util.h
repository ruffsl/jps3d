/**
 * @file planner_util_base.h
 * @brief JPS::JPSPlanner Abstract Class
 */
#ifndef JPS_PLANNER_BASE_H
#define JPS_PLANNER_BASE_H

#include <jps3d/common/data_type.h>
#include <jps3d/planner/graph_search.h>
#include <jps3d/collision_checking/map_util.h>

class GraphSearch;
/**
 * @brief Abstract base for planning
 */
template <int Dim>
class JPSPlanner
{
  public:
    /**
     * @brief Simple constructor
     * @param verbose enable debug mode
     */
    JPSPlanner(bool verbose = false);

    ///Set map util for collistion checking
    void setMapUtil(const std::shared_ptr<JPS::MapUtil<Dim>>& map_util);
    /**
     * @brief Status of the planner
     *
     * 0 --- exit normally;
     * -1 --- no path found;
     * 1, 2 --- start or goal is not free.
     */
    int status();
    ///Get the modified path
    vec_Vecf<Dim> getPath();
    ///Get the raw path
    vec_Vecf<Dim> getRawPath();
    ///remove redundant points on the same line
    vec_Vecf<Dim> removePts(const vec_Vecf<Dim> &path);
    ///Remove some corner waypoints
    vec_Vecf<Dim> optimize(const vec_Vecf<Dim> &path);
    ///Must be called before run the planning thread
    void updateMap();
    ///Need to be specified in Child class, main planning function
    bool plan(const Vecf<Dim> &start, const Vecf<Dim> &goal, decimal_t eps = 1, bool use_jps = true);
    ///Get the nodes in open set
    vec_Vecf<Dim> getOpenSet() const;
    ///Get the nodes in close set
    vec_Vecf<Dim> getCloseSet() const;
    ///Get all the nodes
    vec_Vecf<Dim> getAllSet() const;
  protected:
    ///Assume using 3D voxel map for all 2d and 3d planning
    std::shared_ptr<JPS::MapUtil<Dim>> map_util_;
    ///The planner
    std::shared_ptr<JPS::GraphSearch> graph_search_;
    ///Raw path from planner
    vec_Vecf<Dim> raw_path_;
    ///Modified path for future usage
    vec_Vecf<Dim> path_;
    ///Flag indicating the success of planning
    int status_ = 0;
    ///Enabled for printing info
    bool planner_verbose_;
    ///1-D map array
    std::vector<char> cmap_;
};

///Planner for 2D OccMap
typedef JPSPlanner<2> GraphSearch2DUtil;

///Planner for 3D VoxelMap
typedef JPSPlanner<3> GraphSearch3DUtil;

#endif
