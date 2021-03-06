#ifndef FUNCTIONS

#define FUNCTIONS

#include <math.h>
#include <vector>
#include "../fmt_def.h"

#define ARGS std::vector<numeric_fmt>

namespace user_functions{
  numeric_fmt u_sin(ARGS *a);
  numeric_fmt u_cos(ARGS *a);
  numeric_fmt u_tg(ARGS *a);
  numeric_fmt u_ctg(ARGS *a);
}


#endif
