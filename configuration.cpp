#include "framework.h"
#include "configuration.h"

mct::configuration& mct::config() noexcept
{
	static configuration instance;
	return instance;
}
