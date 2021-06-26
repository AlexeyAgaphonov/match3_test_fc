#pragma once

namespace core
{
	struct RefCounter
	{
		size_t __counter = 0u;
		bool __destroyed = false;
		virtual ~RefCounter() {}
	};
}

void intrusive_ptr_add_ref(core::RefCounter* p);

void intrusive_ptr_release(core::RefCounter* p);