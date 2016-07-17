#ifndef MARKOV_STATE_H_INCLUDED
#define MARKOV_STATE_H_INCLUDED

class markov_state {
public:
	float count[2];
	markov_state *next[2]; // The next state to navigate to on a '0' or a '1'

	// Settings for the initial states
	void init( markov_state next[2] );

	float total_count();

	// On initialisation clone the state as per DMC cloning rules
	void init( int byte, markov_state *parent, markov_state *cloned_from );
};

#endif // MARKOV_STATE_H_INCLUDED
