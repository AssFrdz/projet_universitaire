// --- Standard C++ utilities used in this notebook ---
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// --- Marmote headers used in this lesson ---
#include <marmoteCore/marmoteCore>
#include <marmoteMarkovChain/marmoteMarkovChain>

// --- Convenience declarations for the cells below ---
using namespace std;
using namespace marmote;


// Create an interval [0..6].
MarmoteInterval* itl = new MarmoteInterval(0, 6);
cout << "name = " << itl->className() << endl;
cout << "cardinal = " << itl->Cardinal() << endl;
cout << itl->toString() << endl;

// Create a 2-d box with sizes 2 and 3.
stateType dims[2] = {2, 3};
MarmoteBox* box = new MarmoteBox(2, dims);
cout << "name = " << box->className() << endl;
cout << "cardinal = " << box->Cardinal() << endl;
cout << box->toString() << endl;
cout << box->Enumerate() << endl;


// Compute the index of state (1,0).
stateType st10[2] = {1, 0};
cout << box->Index(st10) << endl;

// Decode index 4 into a state buffer.
MarmoteState buf = box->StateBuffer();
box->DecodeState(4, buf);
cout << static_cast<MarmoteSet*>(box)->PrintState(buf, FORMAT_STRUCTURED) << endl;

// Change how states are printed.
stateWriteType polsave = marmote::stateWritePolicy();
marmote::setStateWritePolicy(STATE_INDEX);
cout << "Format with index: " << box->FormatState(4) << endl;
marmote::setStateWritePolicy(STATE_BOTH);
cout << "Format with both: " << box->FormatState(4) << endl;
marmote::setStateWritePolicy(STATE_FULL);
cout << "Format with full description: " << box->FormatState(4) << endl;
marmote::setStateWritePolicy(polsave);


// Membership tests on intervals and boxes.
stateType s4[1] = {4};
stateType s40[1] = {40};
stateType b44[2] = {4, 4};
stateType b11[2] = {1, 1};
cout << itl->Belongs(s4) << endl;
cout << itl->Belongs(s40) << endl;
cout << box->Belongs(b44) << endl;
cout << box->Belongs(b11) << endl;

// Enumerate the box through a state buffer.
marmote::setStateWritePolicy(STATE_BOTH);
MarmoteState bbuf = box->StateBuffer();
box->FirstState(bbuf);
bool isover = false;
while (!isover) {
    cout << box->FormatState(box->Index(bbuf)) << endl;
    box->NextState(bbuf);
    isover = box->IsFirst(bbuf);
}


// Binary sequences.
BinarySequence* biseq = new BinarySequence(6);
cout << "name = " << biseq->className() << endl;
cout << "cardinal = " << biseq->Cardinal() << endl;
cout << biseq->Enumerate() << endl;

MarmoteState sbuf = biseq->StateBuffer();
biseq->FirstState(sbuf);
isover = false;
while (!isover) {
    cout << biseq->FormatState(biseq->Index(sbuf)) << endl;
    biseq->NextState(sbuf);
    isover = biseq->IsFirst(sbuf);
}

stateType st1[6] = {1, 0, 1, 1, 0, 1};
stateType st2[6] = {2, 4, 1, 1, 0, 1};
cout << biseq->Belongs(st1) << " " << biseq->Index(st1) << endl;
cout << biseq->Belongs(st2) << " " << biseq->Index(st2) << endl;


// The set of all integers.
MarmoteIntegers* itg = new MarmoteIntegers();
cout << "name = " << itg->className() << endl;
cout << "cardinal = " << itg->Cardinal() << endl;
cout << itg->IsFinite() << endl;
stateType ip4[1] = {4};
stateType im3[1] = {-3};
cout << itg->Belongs(ip4) << endl;
cout << itg->Belongs(im3) << endl;
cout << itg->Index(im3) << endl;

// Simplices.
Simplex* splx = new Simplex(7, 3);
cout << splx->Enumerate() << endl;
stateType sx1[7] = {0, 0, 0, 0, 0, 0, 0};
stateType sx2[7] = {0, 0, 0, 0, 0, 0, 3};
stateType sx3[7] = {0, -1, 1, 0, 0, 0, 3};
cout << splx->Belongs(sx1) << endl;
cout << splx->Belongs(sx2) << endl;
cout << splx->Belongs(sx3) << endl;

BinarySimplex* bsplx = new BinarySimplex(7, 3);
cout << bsplx->Enumerate() << endl;


// Release the set objects and the state buffers allocated above.
delete bsplx;
delete splx;
delete itg;
delete[] sbuf;
delete biseq;
delete[] bbuf;
delete[] buf;
delete box;
delete itl;

