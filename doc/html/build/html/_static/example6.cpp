// Example that creates MarmoteSet objects and enumerates them.
//
// Usage: main
//

// Include necessary header files to use marmoteCore and other functonalities.
#include <marmoteCore/marmoteInterval.h>
#include <marmoteCore/marmoteBox.h>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

// Forward declaration
void testSet( MarmoteSet* set, const string &name );

// The application code devised for testing the MarmoteBox object

int main( int argc, char** argv ) 
{
	stateType dim1[1] = {2};
        stateType dim2[2]= {2,3};

        // Creation of a 1-dimensional MarmoteBox with size 2
	MarmoteBox* bb1 = new MarmoteBox(1, dim1);
	// Creation of a 2-dimensional MarmoteBox with size 2x3
        MarmoteBox* bb2 = new MarmoteBox(2, dim2);
	// Creation of the MarmoteInterval [8..13]
        // MarmoteInterval* aa = new MarmoteInterval(8,13);

	// Statistics and enumeration of the 1-d box
	testSet( bb1, "Marmote Box #1" );

	// Test with a pointer of the generic type MarmoteSet
	MarmoteSet* cc = bb1;
	testSet( cc, "Pointer to Marmote Box #1" );

	// Statistics and enumeration of the 2-d box
	testSet( bb2, "Marmote Box #2" );
	
	// Test with a pointer of the generic type MarmoteSet
	cc = bb2;
	testSet( cc, "Pointer to Marmote Box #2" );

	// Statistics and enumeration of the interval
	// testSet( aa, "Marmote Interval" );

	// Test with a pointer of the generic type MarmoteSet
        // cc = aa;
	// testSet( cc, "Pointer to Marmote Interval" );

	// cleanup
	delete bb1;
	delete bb2;
	// delete aa;

	// Exit with OK code
  	return 0;
}

// The test procedure
// The input is:
// set: the object to be tested
// name: a string describing the test
//
void testSet( MarmoteSet* set, const string &name )
{
  std::cout << "###### " << name << " features:" << std::endl;
  std::cout << "# Cardinal = " << set->Cardinal() << ", NbDimensions "
            << set->tot_nb_dims() << std::endl;

  std::cout << "# Direct enumeration of " << name << ":" << std::endl;
  set->Enumerate( &std::cout );
  std::cout << std::endl;

  std::cout << "# Enumeration of " << name << " via through walk:" << std::endl;
  MarmoteState statebuffer = set->StateBuffer();
  set->FirstState(statebuffer);
  do {
    std::cout << "stateIndex: " << std::setw(10) << set->Index(statebuffer) << "\t state "
              << set->PrintState( statebuffer, FORMAT_STRUCTURED ) << std::endl;
    set->NextState(statebuffer);
  } while( !set->IsFirst(statebuffer) );

  std::cout << "#" << std::endl;

  std::cout << "# Enumeration of " << name << " via index access:" << std::endl;
  for ( stateType i = 0 ; i < set->Cardinal() ; i++ ) {
    set->DecodeState(i,statebuffer);
    std::cout << "stateIndex: " << std::setw(10) << i << "\t state"
              << set->PrintState( statebuffer, FORMAT_STRUCTURED ) << std::endl;
    set->NextState(statebuffer);
  }

  delete[] statebuffer;
}
