#include <stdio.h>
#include "callback.h"

int cInit(typecallback cb)
{
	printf("<%s:%d> cInit \n",__func__,__LINE__);
	cb();
	return 0;
}
