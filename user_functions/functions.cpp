#include "functions.hpp"

numeric_fmt user_functions::u_sin(ARGS *a){
  return sin(a->front());
}

numeric_fmt user_functions::u_cos(ARGS *a){
  return cos(a->front());
}

numeric_fmt user_functions::u_tg(ARGS *a){
  return sin(a->front())/cos(a->front());
}

numeric_fmt user_functions::u_ctg(ARGS *a){
  return cos(a->front())/sin(a->front());
}



