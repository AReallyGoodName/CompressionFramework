#include "markov_state.h"

void markov_state::init( markov_state next[2] ) {
    for( int i = 0; i < 2; ++i ) {
        count[i] = 0.2f; // An initial value for the very first state
        this->next[i] = &next[i];
    }
}

float markov_state::total_count() {
    float total_count = 0.0f;
    for ( int i = 0; i < 2; ++i ) {
        total_count += count[i];
    }
    return total_count;
}

// On initialisation clone the state as per DMC cloning rules
void markov_state::init( int byte, markov_state *parent, markov_state *cloned_from ) {
    for( int i = 0; i < 2; ++i ) {
        count[i] = 0.0f;
    }
    float ratio = parent->count[byte] / cloned_from->total_count();
    for ( int i = 0; i < 2; ++i ) {
        count[i] = cloned_from->count[i] * ratio;
        cloned_from->count[i] -= count[i];
        next[i] = cloned_from->next[i];
    }
}
