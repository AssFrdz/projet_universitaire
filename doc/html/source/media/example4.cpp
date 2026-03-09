// Example that reads Markov chains from files then outputs them
// to other files: either as whole Markov chains, either only their generators.
//
// Usage: example4
//

// Include necessary header files to use marmoteCore and other functonalities.
#include <marmoteMarkovChain/marmoteMarkovChain.h>
#include <marmoteCore/marmoteDiscreteDistribution.h>
#include <marmoteCore/marmoteSparseMatrix.h>
#include "marmoteLog.h"
#include <iostream>


// The application code devised for testing the input/output files of Markov chains

int main( int argc, char** argv ) 
{
    // Set up the logging system
    marmote::log::initialize(marmote::log::messageOnly);
    
    // Construct two Markov chains from the external input files with Ers format:
    // IO_example_in1.mcl and IO_example_in2.mcl.
    // Specification of the constructor that is used:
    // MarkovChain( format, param[], nbreParam, modelName )
    // @param format: he format in which the model is specified
    // @param param[]: parameter list
    // @param nbreParam: size of param[].
    // @param modelName: name of the model, typically prefix of files.
    // @return: a markovChain object.
    std::string name1 = "IO_example_in1";
    std::string name2 = "IO_example_in2";
    MarkovChain* c1 = new MarkovChain("Ers", nullptr, 0,name1);
    MarkovChain* c2 = new MarkovChain("Ers", nullptr, 0,name2);

    // Print the constructed chains to the stdout (to make sure they are well constructed).
    std::cout << "####################################" << std::endl;
    std::cout << "Constructed Chains:" << std::endl;
    std::cout << "Example1: " << name1+".mcl" << std::endl;
    c1->Write( &std::cout );

    std::cout << "####################################" << std::endl;
    std::cout << "Example2: " << name2+".mcl" << std::endl;
    c2->Write( &std::cout );
    std::cout << "####################################" << std::endl;

    // Write both chains to external Ers output files: respectively
    // IO_example_out1.mcl and IO_example_out2.mcl using the method "write(format,model_name)".
    c1->Store( "IO_example_out1", FORMAT_MARMOTE ); // << std::endl;
    c2->Store( "IO_example_out2", FORMAT_MARMOTE ); // << std::endl;

    // Write both transition matrices to external Marmote output files:
    // IO_example_mat1 and IO_example_mat2 using the method "Store( modelName, format )".
    if ( static_cast<TransitionStructure*>(nullptr) != c1->generator() ) {
      c1->generator()->Store( "IO_example_mat1", FORMAT_MARMOTE );
    }
    else {
      std::cout << "Chain 1 has no generator" << std::endl;
    }

    if ( static_cast<TransitionStructure*>(nullptr) != c2->generator() ) {
      c2->generator()-> Store( "IO_example_mat2", FORMAT_MARMOTE );
    }
    else {
      std::cout << "Chain 2 has no generator." << std::endl;
    }

    // Now both output files are created, we destruct the chains to free the
    // allocated memory. Deleting the chains will delete the generators.
    delete c1;
    delete c2;
  
    return 0;
}
