// example10.cpp ---

#include <marmoteMarkovChain/marmoteMarkovChain.h>
#include <marmoteMarkovChain/marmoteSimulationResult.h>
#include <marmoteMarkovChain/General/marmoteHomogeneousMultidRandomWalk.h>
#include <marmoteMarkovChain/General/marmoteHomogeneousMultidBirthDeath.h>

#include <marmoteCore/marmoteDiscreteDistribution.h>
#include <marmoteCore/marmoteDiracDistribution.h>
#include <marmoteCore/marmoteHomogeneousMultidTransition.h>
#include <marmoteCore/marmoteBox.h>


#include <iostream>
#include <stdlib.h>
#include <string.h>

#define TOOLNAME "RandomWalks"

// Utility to print the trajectory stored in a SimulationResult object
void printTrajectory( SimulationResult* sr, MarmoteSet* sp )
{
    for ( unsigned long int i = 0; i < sr->trajectorySize(); i++ ) {
        fprintf( stdout, "%8ld", i );
        cardinalType zestate = sr->states()[i];
        sp->PrintState( &std::cout, zestate );
        fprintf( stdout, "\n" );
    }
}

// Main program
int main( int argc, char* argv[] ) {

    double tmax = 1.0e4; // default simulation length
  
    // parsing of command line
    int i = 1;
    bool Args_Ok = true;
    bool Help = false;
    if ( argc > 1 ) do {
            if ( 0 == strcmp( "-tmax_sim", argv[i] ) ) {
                i++;
                if ( ( i < argc ) && ( 1 == sscanf( argv[i], "%lf", &tmax ) ) )
                    i++;
                else {
                    printf( "%s: real number expected after -tmax_sim\n", TOOLNAME );
                    Args_Ok = false;
                }
            }
            else if ( ( 0 == strcmp( "-h", argv[i] ) ) || ( 0 == strcmp( "-help", argv[i] ) )
                    || ( 0 == strcmp( "-?", argv[i] ) ) ) {
                printf("Usage: %s ", TOOLNAME);
                printf("\n\t[-tmax_sim <real>] [-h] [-?]\n");
                Args_Ok = false;
                Help = true;
            }
            else {
                printf( "%s: skipping unknown argument '%s'\n", TOOLNAME, argv[i] );
                i++;
            }
        } while( Args_Ok && ( i < argc) );
  
    if ( !Args_Ok ) exit(-1);
    if ( Help ) exit(0);

    // The example simulates two Markov chains on a big grid.
    // One in continuous time, one in discrete time.
    // The transition "rates" are chosen so that they can be also
    // transition probabilities.
    stateType dims[4] = { 1000, 1000, 100, 100 };
    double rateup[4] = { 0.125, 0.125, 0.125, 0.125 };
    double ratedown[4] = { 0.125, 0.125, 0.125, 0.125 };
    DiscreteDistribution* iDis = new DiracDistribution(0);

    // First make simulation in large state space with continuous time
    printf("# Creation of a continuous-time Markov Chain via the HomogeneousMultidTransition structure\n");
    HomogeneousMultidTransition *gen =
            new HomogeneousMultidTransition( CONTINUOUS, 4, dims, rateup, ratedown );
    MarkovChain *mc = new MarkovChain(gen);

    // Verification of consistency of state space sizes
    MarmoteSet *zesp = new MarmoteBox( 4, dims );
    printf("# Printing the size of the state space\n");
    printf("Big box cardinal           = %lld\n", zesp->Cardinal() );
    printf("HomMultidTrans size        = %lld\n", gen->orig_size() );
    printf("MultiDimHom space cardinal = %lld\n",
            gen->orig_state_space()->Cardinal() );
    delete zesp;

    // Setup and execution of simulation. Option CACHE_NONE is chosen so that
    // no transition is stored permanently.
    mc->set_init_distribution( iDis ); // iDis objet will be copied
    printf("# Execution of the continuous-time simulation with the trajectory printed along the way\n"); 
    SimulationResult* sr =
            mc->SimulateChainCT_AllOpt( tmax, false, true, false, true, true, CACHE_NONE );

    // Output of the trajectory, with expansion of the state as vector of integers
    printf("# Printing again the trajectory\n");
    printTrajectory( sr, gen->orig_state_space() );

    // Cleanup of continuous-time experiment
    delete mc;
    delete sr;

    // Second make simulation in large state space with discrete time
    printf("# Creation of a discrete-time Markov Chain via the HomogeneousMultidTransition structure\n");
    gen = new HomogeneousMultidTransition( DISCRETE, 4, dims, rateup, ratedown );
    mc = new MarkovChain(gen);

    mc->set_init_distribution( iDis ); // iDis object will be copied
    printf("# Execution of the discrete-time simulation with the trajectory printed along the way\n"); 
    sr = mc->SimulateChainDT_AllOpt( static_cast<simLenType>(tmax), false, true,
            true, true, CACHE_NONE );

    printf("# Printing again the trajectory\n");
    printTrajectory( sr, gen->orig_state_space() );

    delete mc;
    delete sr;

    // Redo the continuous-time experiment with the RW MarkovChain object
    printf("# Direct creation of a continuous-time Markov Chain of type HomogeneousMultiDBirthDeath\n");
    mc = new HomogeneousMultiDBirthDeath( 4, dims, rateup, ratedown );
    
    mc->set_init_distribution( iDis ); // iDis object will be copied
    printf("# Execution of the continuous-time simulation with the trajectory printed along the way\n"); 
    sr = mc->SimulateChainCT_AllOpt( tmax, false, true, false,
            true, true, CACHE_NONE );

    printf("# Printing again the trajectory\n");
    printTrajectory( sr, mc->generator()->orig_state_space() );

    delete mc;
    delete sr;

    // Redo the discrete-time experiment with the RW MarkovChain object
    printf("# Direct creation of a discrete-time Markov Chain of type HomogeneousMultiDRandomWalk\n");
    mc = new HomogeneousMultiDRandomWalk( 4, dims, rateup, ratedown );
    
    mc->set_init_distribution( iDis ); // iDis object will be copied
    printf("# Execution of the discrete-time simulation with the trajectory printed along the way\n"); 
    sr = mc->SimulateChainDT_AllOpt( static_cast<simLenType>(tmax), false, true,
            true, true, CACHE_NONE );

    printf("# Printing again the trajectory\n");
    printTrajectory( sr, mc->generator()->orig_state_space() );

    delete mc;
    delete sr;

    // final cleanup
    delete iDis;

}

//
// example10.cpp ends here
