#include "viterbi.h"

int main(int argc, char* argv[])
{
  HMM h;

  h.init();

  forward_viterbi(h.get_observations(), 
                  h.get_states(), 
                  h.get_start_probability(), 
                  h.get_transition_probability(), 
                  h.get_emission_probability());
}
