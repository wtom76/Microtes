#pragma once
#include "gene.h"

namespace mct::neural
{
	//---------------------------------------------------------------------------------------------------------
	// class net
	//---------------------------------------------------------------------------------------------------------
	class net
	{
		struct neuron
		{
			bool	driven_;        // undriven neurons have fixed output values
			float	output_{0.};
		};

		std::vector<gene>	connections_;
		std::vector<neuron>	neurons_;
	};
}
