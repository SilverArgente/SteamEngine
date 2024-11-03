#include "FluidSimulation.h"

void swap(float* x1, float* x2)
{
	float* temp = x1;
	x1 = x2;
	x2 = temp;
}

void set_bnd(int n, int b, float* x) {
	int i, j, k;

	// Set boundaries on the x-axis faces
	for (j = 1; j <= N; j++) {
		for (k = 1; k <= N; k++) {
			x[IX(0, j, k)] = (b == 1) ? -x[IX(1, j, k)] : x[IX(1, j, k)];
			x[IX(N + 1, j, k)] = (b == 1) ? -x[IX(N, j, k)] : x[IX(N, j, k)];
		}
	}

	// Set boundaries on the y-axis faces
	for (i = 1; i <= N; i++) {
		for (k = 1; k <= N; k++) {
			x[IX(i, 0, k)] = (b == 2) ? -x[IX(i, 1, k)] : x[IX(i, 1, k)];
			x[IX(i, N + 1, k)] = (b == 2) ? -x[IX(i, N, k)] : x[IX(i, N, k)];
		}
	}

	// Set boundaries on the z-axis faces
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			x[IX(i, j, 0)] = (b == 3) ? -x[IX(i, j, 1)] : x[IX(i, j, 1)];
			x[IX(i, j, N + 1)] = (b == 3) ? -x[IX(i, j, N)] : x[IX(i, j, N)];
		}
	}

	// Set edges by averaging neighboring values
	for (j = 1; j <= N; j++) {
		x[IX(0, j, 0)] = 0.5f * (x[IX(1, j, 0)] + x[IX(0, j, 1)]);
		x[IX(0, j, N + 1)] = 0.5f * (x[IX(1, j, N + 1)] + x[IX(0, j, N)]);
		x[IX(N + 1, j, 0)] = 0.5f * (x[IX(N, j, 0)] + x[IX(N + 1, j, 1)]);
		x[IX(N + 1, j, N + 1)] = 0.5f * (x[IX(N, j, N + 1)] + x[IX(N + 1, j, N)]);
	}
	for (i = 1; i <= N; i++) {
		x[IX(i, 0, 0)] = 0.5f * (x[IX(i, 1, 0)] + x[IX(i, 0, 1)]);
		x[IX(i, 0, N + 1)] = 0.5f * (x[IX(i, 1, N + 1)] + x[IX(i, 0, N)]);
		x[IX(i, N + 1, 0)] = 0.5f * (x[IX(i, N, 0)] + x[IX(i, N + 1, 1)]);
		x[IX(i, N + 1, N + 1)] = 0.5f * (x[IX(i, N, N + 1)] + x[IX(i, N + 1, N)]);
	}
	for (k = 1; k <= N; k++) {
		x[IX(0, 0, k)] = 0.5f * (x[IX(1, 0, k)] + x[IX(0, 1, k)]);
		x[IX(0, N + 1, k)] = 0.5f * (x[IX(1, N + 1, k)] + x[IX(0, N, k)]);
		x[IX(N + 1, 0, k)] = 0.5f * (x[IX(N, 0, k)] + x[IX(N + 1, 1, k)]);
		x[IX(N + 1, N + 1, k)] = 0.5f * (x[IX(N, N + 1, k)] + x[IX(N + 1, N, k)]);
	}

	// Set corners by averaging three adjacent values
	x[IX(0, 0, 0)] = 0.333f * (x[IX(1, 0, 0)] + x[IX(0, 1, 0)] + x[IX(0, 0, 1)]);
	x[IX(0, 0, N + 1)] = 0.333f * (x[IX(1, 0, N + 1)] + x[IX(0, 1, N + 1)] + x[IX(0, 0, N)]);
	x[IX(0, N + 1, 0)] = 0.333f * (x[IX(1, N + 1, 0)] + x[IX(0, N, 0)] + x[IX(0, N + 1, 1)]);
	x[IX(0, N + 1, N + 1)] = 0.333f * (x[IX(1, N + 1, N + 1)] + x[IX(0, N, N + 1)] + x[IX(0, N + 1, N)]);
	x[IX(N + 1, 0, 0)] = 0.333f * (x[IX(N, 0, 0)] + x[IX(N + 1, 1, 0)] + x[IX(N + 1, 0, 1)]);
	x[IX(N + 1, 0, N + 1)] = 0.333f * (x[IX(N, 0, N + 1)] + x[IX(N + 1, 1, N + 1)] + x[IX(N + 1, 0, N)]);
	x[IX(N + 1, N + 1, 0)] = 0.333f * (x[IX(N, N + 1, 0)] + x[IX(N + 1, N, 0)] + x[IX(N + 1, N + 1, 1)]);
	x[IX(N + 1, N + 1, N + 1)] = 0.333f * (x[IX(N, N + 1, N + 1)] + x[IX(N + 1, N, N + 1)] + x[IX(N + 1, N + 1, N)]);
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

void advect(int n, int b, float* d, float* d0, float* vel_x, float* vel_y, float* vel_z, float dt) {
	int i, j, k, i0, j0, k0, i1, j1, k1;
	float x, y, z, s0, t0, r0, s1, t1, r1, dt0;

	dt0 = dt * n;

	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			for (k = 1; k <= N; k++) {
				x = i - dt0 * vel_x[IX(i, j, k)];
				y = j - dt0 * vel_y[IX(i, j, k)];
				z = k - dt0 * vel_z[IX(i, j, k)];

				if (x < 0.5f) x = 0.5f; if (x > N + 0.5f) x = N + 0.5f; i0 = (int)x; i1 = i0 + 1;
				if (y < 0.5f) y = 0.5f; if (y > N + 0.5f) y = N + 0.5f; j0 = (int)y; j1 = j0 + 1;
				if (z < 0.5f) z = 0.5f; if (z > N + 0.5f) z = N + 0.5f; k0 = (int)z; k1 = k0 + 1;

				s1 = x - i0; s0 = 1.0f - s1;
				t1 = y - j0; t0 = 1.0f - t1;
				r1 = z - k0; r0 = 1.0f - r1;

				d[IX(i, j, k)] =
					s0 * (t0 * (r0 * d0[IX(i0, j0, k0)] + r1 * d0[IX(i0, j0, k1)]) +
						t1 * (r0 * d0[IX(i0, j1, k0)] + r1 * d0[IX(i0, j1, k1)])) +
					s1 * (t0 * (r0 * d0[IX(i1, j0, k0)] + r1 * d0[IX(i1, j0, k1)]) +
						t1 * (r0 * d0[IX(i1, j1, k0)] + r1 * d0[IX(i1, j1, k1)]));
			}
		}
	}

	set_bnd(N, b, d);
}

void project(int n, float* vel_x, float* vel_y, float* vel_z, float* p, float* div) {
	int i, j, k, iter;
	float h = 1.0f / n;

	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			for (k = 1; k <= N; k++) {
				div[IX(i, j, k)] = -0.5f * h * (
					vel_x[IX(i + 1, j, k)] - vel_x[IX(i - 1, j, k)] +
					vel_y[IX(i, j + 1, k)] - vel_y[IX(i, j - 1, k)] +
					vel_z[IX(i, j, k + 1)] - vel_z[IX(i, j, k - 1)]
					);
				p[IX(i, j, k)] = 0;
			}
		}
	}

	set_bnd(N, 0, div);
	set_bnd(N, 0, p);

	for (iter = 0; iter < 20; iter++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				for (k = 1; k <= N; k++) {
					p[IX(i, j, k)] = (div[IX(i, j, k)] +
						p[IX(i - 1, j, k)] + p[IX(i + 1, j, k)] +
						p[IX(i, j - 1, k)] + p[IX(i, j + 1, k)] +
						p[IX(i, j, k - 1)] + p[IX(i, j, k + 1)]) / 6.0f;
				}
			}
		}
		set_bnd(N, 0, p);
	}

	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			for (k = 1; k <= N; k++) {
				vel_x[IX(i, j, k)] -= 0.5f * (p[IX(i + 1, j, k)] - p[IX(i - 1, j, k)]) / h;
				vel_y[IX(i, j, k)] -= 0.5f * (p[IX(i, j + 1, k)] - p[IX(i, j - 1, k)]) / h;
				vel_z[IX(i, j, k)] -= 0.5f * (p[IX(i, j, k + 1)] - p[IX(i, j, k - 1)]) / h;
			}
		}
	}

	// Apply boundary conditions to the velocity fields
	set_bnd(N, 1, vel_x);
	set_bnd(N, 2, vel_y);
	set_bnd(N, 3, vel_z);
}

void dens_step(int n, float* dens, float* dens_prev, float* vel_x, float* vel_y, float* vel_z, float diff, float dt)
{
	add_source(n, dens, dens_prev, dt);
	swap(dens, dens_prev);
	diffuse(n, 0, dens, dens_prev, diff, dt);
	swap(dens, dens_prev);
	advect(n, 0, dens, dens_prev, vel_x, vel_y, vel_z, dt);

}

void vel_step(int n, float* vel_x, float* vel_y, float* vel_z,
	float* vel_x_prev, float* vel_y_prev, float* vel_z_prev,
	float visc, float dt)
{
	add_source(n, vel_x, vel_x_prev, dt);
	add_source(n, vel_y, vel_y_prev, dt);
	add_source(n, vel_z, vel_z_prev, dt);

	swap(vel_x_prev, vel_x);
	diffuse(N, 1, vel_x, vel_x_prev, visc, dt);

	swap(vel_y_prev, vel_y);
	diffuse(N, 2, vel_y, vel_y_prev, visc, dt);

	swap(vel_z_prev, vel_z);
	diffuse(N, 3, vel_z, vel_z_prev, visc, dt);

	project(N, vel_x, vel_y, vel_z, vel_x_prev, vel_y_prev);

	swap(vel_x_prev, vel_x);
	swap(vel_y_prev, vel_y);
	swap(vel_z_prev, vel_z);

	advect(n, 1, vel_x, vel_x_prev, vel_x_prev, vel_y_prev, vel_z_prev, dt);
	advect(n, 2, vel_y, vel_y_prev, vel_x_prev, vel_y_prev, vel_z_prev, dt);
	advect(n, 3, vel_z, vel_z_prev, vel_x_prev, vel_y_prev, vel_z_prev, dt);

	project(n, vel_x, vel_y, vel_z, vel_x_prev, vel_y_prev);
}
