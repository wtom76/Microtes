#pragma once
#include "../../microte.h"

namespace mct::data::microte_impl::a001
{
	class microte_impl : public microte
	{
	private:
		void _create_neighb(field& fld) const noexcept;
	public:
		using microte::microte;
		virtual ~microte_impl();
		virtual bool cycle(field& fld) override;
		virtual void draw(HDC dc) const override;

		static void populate(field& fld);
	};
}
