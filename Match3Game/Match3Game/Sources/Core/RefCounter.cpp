#include "RefCounter.hpp"

void intrusive_ptr_add_ref(core::RefCounter* p)
{
	if (p && !p->__destroyed)
	{
		++p->__counter;
	}
}

void intrusive_ptr_release(core::RefCounter* p)
{
	if (p && --p->__counter == 0u)
	{
		p->__destroyed = true;
		delete p;
	}
}
