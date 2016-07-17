#include <iostream>

#include "markov_predictor.h"

float markov_predictor::bayesprediction(float a, float b) {
    return (a + 0.01f) / (a + b + 0.02f);
}

markov_predictor::markov_predictor( unsigned long long mem ) {
    prediction = 0.5f;
    likelihood = 1.0f;
    this->mem = mem;
    int num_states = (mem / sizeof(markov_state));
    if ( num_states < 65536 ) {
        num_states = 65536;
    }
    std::cerr << "Number of states = " << num_states << std::endl;
    states_begin = new markov_state[num_states];
    states_end = states_begin + num_states;
    init_states();
}

// Create a braid for the inital markov states
void markov_predictor::init_states() {
    current_allocation = states_begin;

    for ( int i = 0; i < 256; ++i ) {
        current_allocation[i].init( states_begin );
    }

    current_allocation += 256;
    current_state = states_begin;
}

float markov_predictor::get_prediction() {
    return prediction;
}

float markov_predictor::get_likelihood() {
    return likelihood;
}

void markov_predictor::model( unsigned int bit ) {
    static int bitpos = 0;
    static int cur_data = 0;
    cur_data |= (bit << (7-bitpos));

    static int threshold = 2;
    bitpos++;
    if ( bitpos == 8 ) {
        if ( ( cur_data < 'a' || cur_data > 'z' ) ) {
            threshold = 5;
        }
        else {
            threshold = 2;
        }


        cur_data = 0;
        bitpos = 0;
    }

    // With the current state clone it if it meets the criteria
    // Test thresholds of dmc modelling
    if ( current_state->count[bit] >= threshold &&
       ( current_state->next[bit]->total_count() ) >= ( current_state->count[bit] + threshold ) &&
       current_allocation < states_end )
    {
        markov_state *cloned_from = current_state->next[bit];
        current_state->next[bit] = current_allocation;
        current_allocation->init( bit, current_state, cloned_from );
        current_allocation++;
    }

    // Increase count of the current state and go to the next state
    current_state->count[bit]++;
    current_state = current_state->next[bit];

    prediction = bayesprediction( current_state->count[0], current_state->count[1] );

}
