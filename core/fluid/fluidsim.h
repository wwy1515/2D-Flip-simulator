#ifndef FLUIDSIM_H
#define FLUIDSIM_H

#include "../util/array2.h"
#include "../util/vec.h"
#include "../util/Timer.h"

#include <vector>

class FluidSim {

public:
   void initialize(float width, int ni_, int nj_);
   void set_boundary(float (*phi)(const Vec2f&));
   void advance(float dt);
   inline Array2f& getPhi() { return this->liquid_phi; }
   inline Array2f& getSolidPhi() { return this->nodal_solid_phi; }

   //Grid dimensions
   int ni,nj;
   float dx;
   Sim::Timer timer;
   //Fluid velocity
   Array2f u, v;
   Array2f temp_u, temp_v;
   //flip
   Array2f uDelta, vDelta;
   Array2f u_flip_weights, v_flip_weights;
   
   //Static geometry representation
   Array2f nodal_solid_phi;
   Array2f u_weights, v_weights;
   Array2c u_valid, v_valid;

   Array2f liquid_phi;
   
   std::vector<Vec2f> particles; //For marker particle simulation
   std::vector<Vec2f> particlesVel; //For Flip particle simulation
   float particle_radius;
   
   //Data arrays for extrapolation
   Array2c valid, old_valid;
   
   Vec2f get_velocity(const Vec2f& position);
   Vec2f get_delta_velocity(const Vec2f& position);
   void add_particle(const Vec2f& position);
   void seed_particles(float (*phi)(const Vec2f&),int grid_resolution);
   void compute_phi(); 
   bool pUseFlip;
private:

   Vec2f trace_rk2(const Vec2f& position, float dt);

   void advect_particles(float dt);

   

   float cfl();

   //fluid velocity operations
   void advect(float dt);
   void add_force(float dt);

   void project(float dt);
   void compute_weights();
   void solve_pressure(float dt);
   
   void constrain_velocity();

   
   void transfer_grid2particles();
   void transfer_particles2grid();

};

#endif
