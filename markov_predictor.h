#ifndef MARKOV_PREDICTOR_H_INCLUDED
#define MARKOV_PREDICTOR_H_INCLUDED

#include "predictor.h"
#include "markov_state.h"

// Based on Dynamic Markov Coding by Gordon Cormack and Nigel Horspool
class markov_predictor : public predictor {
	unsigned int mem;
	markov_state* current_state;
	markov_state* states_begin;
	markov_state* states_end;
	markov_state* current_allocation;

	float bayesprediction(float a, float b);

public:
	float prediction;
	float likelihood;

	markov_predictor( unsigned long long mem );

	// Create a braid for the inital markov states
	void init_states();

	float get_prediction();

	float get_likelihood();

	void model( unsigned int bit );
};

#endif // MARKOV_PREDICTOR_H_INCLUDED
