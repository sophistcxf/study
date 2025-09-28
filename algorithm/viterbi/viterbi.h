#ifndef VITERBI_H
#define VITERBI_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

typedef double Probability;
typedef std::string State;
typedef std::string Observation;
typedef std::vector<State> StateSpace;
typedef std::vector<State> StateSeq;
typedef std::vector<Observation> ObservationSpace;
typedef std::vector<Observation> ObservationSeq;
typedef std::map<State, Probability> InitialProb;
typedef std::map<State, std::map<State, Probability> > TransitionProb;
typedef std::map<Observation, std::map<State, Probability> > EmissionProb;

class HMM
{
public :
    HMM(){};

    void init(void);

    friend std::ostream& operator << (std::ostream& os, HMM&);

    const StateSpace& get_states() const 
    { return states_; };
    const ObservationSpace& get_observations() const 
    { return observations_;};

    InitialProb& get_start_probability() 
    { return initial_prob_; } ;

    TransitionProb& get_transition_probability() 
    { return transition_prob_; }

    EmissionProb& get_emission_probability() 
    { return emission_prob_; }

private:
    StateSpace states_;                 // 隐态
    ObservationSpace observations_;     // 观察状态序列
    InitialProb initial_prob_;
    // 
    TransitionProb transition_prob_;
    EmissionProb emission_prob_;
};

void forward_viterbi(const ObservationSeq& obs, 
        const StateSpace& states, 
        const InitialProb& start_p, 
        const TransitionProb& trans_p, 
        const EmissionProb& emit_p);  

#endif
