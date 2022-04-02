#include "framework.h"
#include "util.h"

//---------------------------------------------------------------------------------------------------------
double mct::rand(double rand_min, double rand_max)
{
	static std::mt19937 rand(std::random_device{}());
	const double rand_factor{(rand_max - rand_min) / (rand.max() - rand.min())};
	return rand_min + (rand() - rand.min()) * rand_factor;
}
