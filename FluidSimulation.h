#ifndef FLUIDSIM_H
#define FLUIDSIM_H
#define N 20
#define IX(i, j, k) ((k + (j * (N+2))) + i * (N+2) * (N+2))

class FluidSimulation {
public:
    int n;
    int isSimulating = 0;
    float* vel_x, * vel_y, * vel_z;
    float* vel_x_prev, * vel_y_prev, * vel_z_prev;
    float* dens, * dens_prev;

    FluidSimulation()
    {
        n = N;
    }

    void vel_step(int n, float* vel_x, float* vel_y, float* vel_z, float* vel_x_prev, float* vel_y_prev, float* vel_z_prev, float visc, float dt);
    void dens_step(int n, float *dens, float* dens_prev, float* vel_x, float* vel_y, float* vel_z, float diff, float dt);

    void add_source(int n, float* x, float* s, float dt);
    void diffuse(int n, int b, float* x, float* x0, float diff, float dt);
    void advect(int n, int b, float* d, float* d0, float* vel_x, float* vel_y, float* vel_z, float dt);
    void project(int n, float* vel_x, float* vel_y, float* vel_z, float *p, float *div);
    void set_bnd(int n, int b, float* x);
    
};

#endif // FLUIDSIM_H
