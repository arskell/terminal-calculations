#include "functions.hpp"

numeric_fmt user_functions::u_sin(ARGS *a){
  return sin((*a)[0]);
}

numeric_fmt user_functions::u_cos(ARGS *a){
  return cos((*a)[0]);
}

numeric_fmt user_functions::u_tg(ARGS *a){
  return sin((*a)[0])/cos((*a)[0]);
}

numeric_fmt user_functions::u_ctg(ARGS *a){
  return cos((*a)[0])/sin((*a)[0]);
}



