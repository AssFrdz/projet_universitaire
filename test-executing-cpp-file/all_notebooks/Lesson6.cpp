// --- Standard C++ utilities used in this notebook ---
#include <iomanip>
#include <iostream>
#include <string>

// --- Marmote headers used in this lesson ---
#include <marmoteCore/marmoteBox.h>
#include <marmoteCore/marmoteInterval.h>

// --- Convenience declarations for the cells below ---
using namespace std;
using namespace marmote;


// Define the test procedure used throughout the notebook.
void testSetNotebook(MarmoteSet* set, const string& name) {
    if (set == nullptr) {
        cout << "The set pointer is null. Re-run the creation cell before testing." << endl;
        return;
    }

    cout << "###### " << name << " features:" << endl;
    cout << "# Cardinal = " << set->Cardinal() << ", NbDimensions "
         << set->tot_nb_dims() << endl;

    cout << "# Direct enumeration of " << name << ":" << endl;
    set->Enumerate(&cout);
    cout << endl;

    cout << "# Enumeration of " << name << " via through walk:" << endl;
    MarmoteState statebuffer = set->StateBuffer();
    set->FirstState(statebuffer);
    do {
        cout << "stateIndex: " << setw(10) << set->Index(statebuffer) << "\t state ";
        set->PrintState(&cout, statebuffer, FORMAT_STRUCTURED);
        cout << endl;
        set->NextState(statebuffer);
    } while (!set->IsFirst(statebuffer));

    cout << "#" << endl;

    cout << "# Enumeration of " << name << " via index access:" << endl;
    for (stateType i = 0; i < set->Cardinal(); i++) {
        set->DecodeState(i, statebuffer);
        cout << "stateIndex: " << setw(10) << i << "\t state ";
        set->PrintState(&cout, statebuffer, FORMAT_STRUCTURED);
        cout << endl;
    }

    delete[] statebuffer;
}


// Create the dimensions used to build the two boxes.
stateType dim1[1] = {2};
stateType dim2[2] = {2, 3};

// Create a 1-dimensional MarmoteBox with size 2.
MarmoteBox* bb1 = new MarmoteBox(1, dim1);

// Create a 2-dimensional MarmoteBox with size 2 x 3.
MarmoteBox* bb2 = new MarmoteBox(2, dim2);


// Test the first box directly.
testSetNotebook(bb1, "Marmote Box #1");

// Test the same object through a generic MarmoteSet pointer.
MarmoteSet* cc = bb1;
testSetNotebook(cc, "Pointer to Marmote Box #1");

// Test the second box directly.
testSetNotebook(bb2, "Marmote Box #2");

// Test the same object through a generic MarmoteSet pointer.
cc = bb2;
testSetNotebook(cc, "Pointer to Marmote Box #2");


// Release the objects created in this lesson.
delete bb1;
bb1 = nullptr;
delete bb2;
bb2 = nullptr;

