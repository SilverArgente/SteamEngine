#include "FluidSimulation.h"

void swap(float* x1, float* x2)
{
	float* temp = x1;
	x1 = x2;
	x2 = temp;
}

void set_bnd(int n, int b, float* x)
{
	int i;

	for (i = 1; i <= n; i++)
	{
		//x[]
	}
}

void add_source(int n, float* x, float* s, float dt)
{
	int i, size = (n + 2) * (n + 2);

	for (i = 0; i < size; i++)
		x[i] += dt * s[i];

}



void diffuse(int n, int b, float* x, float* x0, float diff, float dt)
{
	int i, j, k, m;
	float a = dt * diff * n * n * n;

	for (m = 0; m < 20; m++)
	{
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
			{
				for (k = 1; k <= n; k++)
				{
					x[IX(i, j, k)] = (x0[IX(i, j, k)] + a * (x[IX(i - 1, j, k)] + x[IX(i + 1, j, k)] + x[IX(i, j - 1, k)] + x[IX(i, j + 1, k)] + x[IX(i, j, k + 1)] + x[IX(i, j, k - 1)]) / (1 / 6 * a));
				}
			}
		}

		set_bnd(n, b, x);
	}
}

void advect(int n, int b, float* d, float* d0, float* vel_x, float* vel_y, float* vel_z, float dt)
{
	int i, j, k, i0, j0, k0, i1, j1, k1;
	float x, y, z, s0, t0, g0, s1, t1, g1, dt0;

	dt0 = dt * n;
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= n; i++)
		{
			for (k = 1; k <= n; i++)
			{
				x = i - dt0 * vel_x[IX(i, j, k)];
				y = j - dt0 * vel_y[IX(i, j, k)];
				z = k - dt0 * vel_z[IX(i, j, k)];

				if (x < 0.5)
					x = 0.5;
				if (x > n + 0.5)
					x = n + 0.5;
				i0 = (int)x;
				i1 = i0 + 1;

				if (y < 0.5)
					y = 0.5;
				if (y > n + 0.5)
					y = n + 0.5;
				j0 = (int)y;
				j1 = j0 + 1;

				if (z < 0.5)
					z = 0.5;
				if (z > n + 0.5)
					z = n + 0.5;
				k0 = (int)z;
				k1 = k0 + 1;

				s1 = x - i0;
				s0 = 1 - s1;
				t1 = y - j0;
				t0 = 1 - t1;
				g1 = z - k0;
				g0 = 1 - g1;

				//d[IX(i, j, k)] = s0 * (t0 * d0[IX(i0, j0, k0)] + t1 * d0[IX(i0, j1, k1)])

			}
		}
	}
}

void project(int n, float* vel_x, float* vel_y, float* vel_z, float* p, float* div)
{

}




void dens_step(int n, float* dens, float* dens_prev, float* vel_x, float* vel_y, float* vel_z, float diff, float dt)
{
	add_source(n, dens, dens_prev, dt);
	swap(dens, dens_prev);
	diffuse(n, 0, dens, dens_prev, diff, dt);
	swap(dens, dens_prev);
	advect(n, 0, dens, dens_prev, vel_x, vel_y, vel_z, dt);

}

void vel_step(int n, float* vel_x, float* vel_y, float* vel_z, float* vel_x_prev, float* vel_y_prev, float* vel_z_prev, float visc, float dt)
{

}