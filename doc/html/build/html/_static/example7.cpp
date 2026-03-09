// Example that constructs four 8x8 discrete-time Markov chain
// then performs structural analysis on them.
//
// Usage: example7
//

// Include necessary header files
#include <marmoteMarkovChain/marmoteMarkovChain.h>
#include <marmoteCore/marmoteSparseMatrix.h>

#include <ranges>
#include <thread>

// Forward declaration of utility
void analysis( MarkovChain* mc );

// The application code

int main( int argc, char** argv ) 
{
	// Create 4 discrete-time Markov chains of size 8.
	MarkovChain* c1 = new MarkovChain(8,DISCRETE);
	MarkovChain* c2 = new MarkovChain(8,DISCRETE);
	MarkovChain* c3 = new MarkovChain(8,DISCRETE);
	MarkovChain* c4 = new MarkovChain(8,DISCRETE);

	// Creating 4 transition matrices with size 8*8.
	SparseMatrix *P1 = new SparseMatrix(8);
	SparseMatrix *P2 = new SparseMatrix(8);
	SparseMatrix *P3 = new SparseMatrix(8);
	SparseMatrix *P4 = new SparseMatrix(8);

	// Add entries to the first transition matrix.
	P1->addToEntry(0,0,0.2);
	P1->addToEntry(0,1,0.8);
	P1->addToEntry(1,0,0.25);
	P1->addToEntry(1,1,0.25);
	P1->addToEntry(1,2,0.25);
	P1->addToEntry(1,3,0.25);
	P1->addToEntry(2,0,0.6);
	P1->addToEntry(2,2,0.4);
	P1->addToEntry(3,2,0.3);
	P1->addToEntry(3,3,0.2);
	P1->addToEntry(3,4,0.25);
	P1->addToEntry(3,5,0.25);
	P1->addToEntry(4,4,0.1);
	P1->addToEntry(4,5,0.3);
	P1->addToEntry(4,6,0.3);
	P1->addToEntry(4,7,0.3);
	P1->addToEntry(5,5,1.0);
	P1->addToEntry(6,4,0.5);
	P1->addToEntry(6,6,0.5);
	P1->addToEntry(7,4,0.4);
	P1->addToEntry(7,5,0.2);
	P1->addToEntry(7,6,0.2);
	P1->addToEntry(7,7,0.2);

	// Add entries to the second transition matrix.
	P2->addToEntry(0,0,0.2);
	P2->addToEntry(0,1,0.8);
	P2->addToEntry(1,0,0.25);
	P2->addToEntry(1,1,0.25);
	P2->addToEntry(1,2,0.25);
	P2->addToEntry(1,3,0.25);
	P2->addToEntry(2,0,0.6);
	P2->addToEntry(2,2,0.4);
	P2->addToEntry(3,2,0.3);
	P2->addToEntry(3,3,0.2);
	P2->addToEntry(3,4,0.25);
	P2->addToEntry(3,5,0.25);
	P2->addToEntry(4,4,0.1);
	P2->addToEntry(4,5,0.3);
	P2->addToEntry(4,6,0.3);
	P2->addToEntry(4,7,0.3);
	P2->addToEntry(5,5,0.5); // split transition 5->5 to 5->(5,7)
	P2->addToEntry(5,7,0.5); // with 1/2 proba each
	P2->addToEntry(6,4,0.5);
	P2->addToEntry(6,6,0.5);
	P2->addToEntry(7,4,0.4);
	P2->addToEntry(7,5,0.2);
	P2->addToEntry(7,6,0.2);
	P2->addToEntry(7,7,0.2);

	// Add entries to the third transition matrix.
	P3->addToEntry(0,0,0.2);
	P3->addToEntry(0,1,0.8);
	P3->addToEntry(1,0,0.25);
	P3->addToEntry(1,1,0.25);
	P3->addToEntry(1,2,0.25);
	P3->addToEntry(1,3,0.25);
	P3->addToEntry(2,0,0.6);
	P3->addToEntry(2,2,0.4);
	P3->addToEntry(3,2,0.3);
	P3->addToEntry(3,3,0.2);
	P3->addToEntry(3,4,0.25);
	P3->addToEntry(3,5,0.25);
	P3->addToEntry(4,4,0.1);
	P3->addToEntry(4,5,0.3);
	P3->addToEntry(4,6,0.3);
	P3->addToEntry(4,7,0.3);
	P3->addToEntry(5,3,0.5); // split transition 5->5 to 5->(3,5)
	P3->addToEntry(5,5,0.5); // with 1/2 proba each
	P3->addToEntry(6,4,0.5);
	P3->addToEntry(6,6,0.5);
	P3->addToEntry(7,4,0.4);
	P3->addToEntry(7,5,0.2);
	P3->addToEntry(7,6,0.2);
	P3->addToEntry(7,7,0.2);

	// Add entries to the fourth transition matrix.
	P4->addToEntry(0,1,1.0);
	P4->addToEntry(1,2,0.5);
	P4->addToEntry(1,3,0.5);
	P4->addToEntry(2,0,1.0);
	P4->addToEntry(3,4,1.0);
	P4->addToEntry(4,5,0.4);
	P4->addToEntry(4,6,0.6);
	P4->addToEntry(5,7,1.0);
	P4->addToEntry(6,7,1.0);
	P4->addToEntry(7,4,0.4);
	P4->addToEntry(7,0,0.6);

	// Assign the transition matrices to chains (respectively). 
	c1->set_generator(P1);
	c2->set_generator(P2);
	c3->set_generator(P3);
	c4->set_generator(P4); 
		
	//////////////////////////////////////////////////////////////////////////
	fprintf( stdout, "##### Analysis of chain #1\n" );
	analysis( c1 );
	fprintf( stdout, "\n##### Analysis of chain #2\n" );
	analysis( c2 );
	fprintf( stdout, "\n##### Analysis of chain #3\n" );
	analysis( c3 );
	fprintf( stdout, "\n##### Analysis of chain #4\n" );
	analysis( c4 );

	// Destruct the chains in order to free the allocated memory.
	printf("\n## Destruction of MCs\n");
	delete c1;
	delete c2;
	delete c3;
	delete c4;

	// Exit with OK code
  	return 0;
}

// Procedure that analyzes some MarkovChain. Tasks performed:
// - general diagnostic of the chain
// - list absorbing states
// - list communicating classes
// - list recurrent classes
// - print period
//
void analysis( MarkovChain* mc )
{
  SparseMatrix* sp = dynamic_cast<SparseMatrix*>(mc->generator());

  std::cout << "# Generator general diagnostic:" << endl;
  sp->Diagnose();

  std::cout << "# Absorbing states:" << endl;
  std::vector<cardinalType> abs = mc->AbsorbingStates();
  std::cout << "number = " << abs.size() << endl;
  std::cout << "list = ( ";
  for ( std::vector<cardinalType>::iterator iter = abs.begin(); iter != abs.end(); iter++ ) {
    std::cout << *iter << " ";
  }
  std::cout << ")" << endl;

  std::cout << "# Communicating classes:" << endl;
  std::vector<std::vector<cardinalType> > com = mc->CommunicatingClasses();
  std::cout << "number = " <<  com.size() << endl;
  std::cout << "list = ( ";
  for ( std::vector<std::vector<cardinalType> >::iterator iter = com.begin();
	iter != com.end(); iter++ ) {
    std::cout << "[ ";
    for ( std::vector<cardinalType>::iterator itr = (*iter).begin();
	  itr != (*iter).end(); itr++ ) {
      std::cout << *itr << " ";
    }
    std::cout << "] ";
  }
  std::cout << ")" << endl;

  std::cout << "# Recurrent classes:" << endl;
  std::vector<std::vector<cardinalType> > rec = mc->RecurrentClasses();
  std::cout << "number = " << rec.size() << endl;
  std::cout << "list = ( ";
  for ( std::vector<std::vector<cardinalType> >::iterator iter = rec.begin();
	iter != rec.end(); iter++ ) {
    std::cout << "[ ";
    for ( std::vector<cardinalType>::iterator itr = (*iter).begin();
	  itr != (*iter).end(); itr++ ) {
      std::cout << *itr << " ";
    }
    std::cout << "] ";
  }
  std::cout << ")" << endl;

  std::cout << "# Period: " << mc->Period() << endl;

}
