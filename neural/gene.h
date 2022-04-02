#pragma once

namespace mct::neural
{
	//---------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------
	class gene
	{
		enum class type : char
		{
			null = 0,
			sensor,
			action,
			neuron
		};

		type	source_type_{type::null};
		long	source_num_{-1};
		type	sinc_type_{type::null};
		long	sinc_num_{-1};
		double	weight_{0.};
	};
}