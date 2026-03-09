/* Marmote and MarmoteMDP are free softwares: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Marmote is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Marmote. If not, see <http://www.gnu.org/licenses/>.

Copyright 2022 Emmanuel Hyon, Alain Jean-Marie*/

/**
 * @brief class to implement the 4 rooms example with a total reward MDP 
 * @author Hyon, lip6.
 * @version 0.1
 * @date July 2023
 *
 * This model is related to a Stochastic Shortest Path.
 * 
 * The state space has two dimensions 11 lines x 10 columns [0,10]*[0,9]
 * 
 * If we reach the state (9,2) we receive a reward of 1 and we go in state (10,2). 
 * In any state with line 10 we stay in this state without receiving anything
 * 
 * There is wall between line 4 and 5 that can be crossed 
 *  -> at states (4,2)->(5,2) and (4,7)->(5,7) 
 *  -> at states (5,2)->(4,2) and (5,7)->(4,7)
 * 
 * There is a wall between column 4 and 5 that can be crossed 
 * -> at states (2,4)->(2,5) and (7,4)->(7,5)
 * -> at states (2,5)->(2,4) and (7,5)->(7,4)
 * 
 * There is 4 actions: 0 is up 1 is down 2 is left and 3 is right 
 * 
 */



#include <iostream>
//#include <list>
//#include <vector>
#include <string>

using namespace std;

#include "marmoteCore/marmoteInterval.h"
#include "marmoteCore/marmoteBox.h"
#include "marmoteCore/marmoteSparseMatrix.h"
#include "marmoteCore/marmoteFullMatrix.h"
#include "marmoteMDP/marmoteTotalRewardMDP.h"
#include "marmoteMDP/marmoteFeedbackSolutionMDP.h"
#include "marmoteMDP/marmoteSolutionMDP.h"

#include <marmoteCore/marmotePolicy.h>
#include <marmoteLog/marmoteLog.h>


int main( int argc, char** argv )
{
    
    /* create indexes */
    stateType k; /* to iterate on states */
    stateType indexO; /* indexes of the initial and final states */
    stateType indexD; /* indexes of the initial and final states */ 
    int l,c; /* to get the indexes of lines and columns */

    double p=0.9; // the probability to move 

    /* create the state space as a marmoteBox */
    /* definitions of the size of the two dimensions: dimension 1 is 11 and dimension 2 is 10 */
    stateType dims[2]={11,10};

    // initialiazing log
    marmote::log::initialize();


    /* create the box*/
    MarmoteBox *stateSpace = new MarmoteBox(2,dims);

    // create the action space as an interval between 0 and 3
    MarmoteInterval *actionSpace = new MarmoteInterval(0,3);               
          
    // compute the size of the cardinal and store it in variable dim_SS
    stateType dim_SS = stateSpace->Cardinal();
    stateType dim_AS = actionSpace->Cardinal();
    
    /*Allocate buffers (to be used to know the index of the state) */
    /* allows to manage initial state (before transition) */
    MarmoteState statebufferO = stateSpace->StateBuffer();
    /* allows to manage final state (after transition) */
    MarmoteState statebufferD = stateSpace->StateBuffer();
    
    /* create and fill matrix for the action*/
    SparseMatrix *P0; 
    
    /* create the matrix for the Cost. This is a dim_SS * dim_AS matrix */
    FullMatrix *CostMat  = new FullMatrix(dim_SS, dim_AS);
    /* fill in the matrix */
    /* all the costs are equal to -1 except in 9,2 */
    /* initialize statebufferO such that it is the first state in the state space */
    stateSpace->FirstState(statebufferO);
    for(k=0;  k < dim_SS; k++) {
        /* computing the index of the state */
        indexO=stateSpace->Index(statebufferO);
        for(int a=0;a<4;a++){
            CostMat->setEntry(indexO,a,1);
        }
        stateSpace->NextState(statebufferO);
    }
    /* replacing the term in 9,2 with action UP to -1 */
    statebufferO[0]=9;
    statebufferO[1]=2;
    indexO=stateSpace->Index(statebufferO);
    CostMat->setEntry(indexO,0,-1.0);
    /* all costs in line 10 are equal to zero */
    statebufferO[0]=10;
    for(k=0;k<10;k++){
       statebufferO[1]=k; 
       indexO=stateSpace->Index(statebufferO);
       CostMat->setEntry(indexO,0,0.0);
       CostMat->setEntry(indexO,1,0.0);
       CostMat->setEntry(indexO,2,0.0);
       CostMat->setEntry(indexO,3,0.0);
    }
    
    
    /* create and initialize elements of MDP */
    string critere("min");
    
    /* create and initialize elements of solving */
    double epsilon = 0.0001;
    int maxIter=500;
    
    cout<<"Begining of building MDP"<<endl;
    /* create the MDP */
    TotalRewardMDP *mdpSSP = new TotalRewardMDP(critere, stateSpace, actionSpace, CostMat);
    cout << "End of building MDP" <<endl;

    cout<<"Add matrices"<<endl;
    
    /* Define matrix for action 0 (UP) */
    P0 = new SparseMatrix(dim_SS);
    for(l = 0 ; l < 10 ; l++)
    {    
        for(c = 0 ; c < 10 ; c++)
        {    
            /* define a state and get its index */
            statebufferO[0]=l; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
        
            if ((l==4) || (l==9)) {
                if  ( (l==4) &&  ( (c==2) || (c==7))) {
                    /* I am on a door two possibilities*/
                    /* either I move up with probability p */
                    statebufferD[0]=l+1;
                    statebufferD[1]=c;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P0->setEntry(indexO,indexD,p);
                    // either I stay
                    P0->setEntry(indexO,indexO,1-p);
                }
                else{
                    if  ( (l==9) && (c==2)){
                        /* I am on a door two possibilities*/
                        /* either I move up with probability p */
                        statebufferD[0]=l+1;
                        statebufferD[1]=c;
                        indexD=stateSpace->Index(statebufferD); //computing the index of destination
                        P0->setEntry(indexO,indexD,p);
                        // either I stay
                        P0->setEntry(indexO,indexO,1-p);
                    }
                    else{
                        // I am on the wall l=4 or l=9 I stay in the same state
                        P0->setEntry(indexO,indexO,1.0);
                    }
                }
            }
            else{
                // I am whatever in a rooms I have two possibilities
                // either i move up with probability p
                    statebufferD[0]=l+1;
                    statebufferD[1]=c;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P0->setEntry(indexO,indexD,p);
                    // either I stay
                    P0->setEntry(indexO,indexO,1-p);
            }
        }
    }
    /* fill in last line */
    for(c = 0 ; c < 10 ; c++)
    {    
            /* define a state and get its index */
            statebufferO[0]=10; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
            P0->setEntry(indexO,indexO,1.0);
    }
    
    mdpSSP->AddMatrix(0,P0);
    cout<<"Added matrix (action 0)"<< endl;
    
    SparseMatrix *P1 = new SparseMatrix(dim_SS);
    /* Define matrix for action 1 (DOWN) */
    for(l = 0 ; l < 10 ; l++)
    {    
        for(c = 0 ; c < 10 ; c++)
        {    
            /* define a state and get its index */
            statebufferO[0]=l; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
            
            if ((l==5) || (l==0)) {
                if ((l==5) && ( (c==2) || (c==7)) ){
                    // two possibilities I am on a door
                    // either I move down with probability p
                    statebufferD[0]=l-1;
                    statebufferD[1]=c;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P1->setEntry(indexO,indexD,p);
                    // either I stay
                    P1->setEntry(indexO,indexO,1-p);
                }
                else{
                    // I am on the wall l=5 or l=0 I stay in the same state
                    P1->setEntry(indexO,indexO,1.0);
                }
            }
            else{
                /* I am whatever in a rooms I have two possibilities
                  either I move down with probability p
                  or I stay with a probability 1-p */
                    statebufferD[0]=l-1;
                    statebufferD[1]=c;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P1->setEntry(indexO,indexD,p);
                    // either I stay
                    P1->setEntry(indexO,indexO,1-p);
            }
        }
    }
    /* fill in last line */
    for(c = 0 ; c < 10 ; c++)
    {    
            /* define a state and get its index */
            statebufferO[0]=10; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
            P1->setEntry(indexO,indexO,1.0);
    }
    
    mdpSSP->AddMatrix(1,P1);
    cout<<"Added matrix (action 1)"<< endl;
    
    /* Define matrix for action 2 (LEFT) */
    SparseMatrix *P2 = new SparseMatrix(dim_SS);
    for(l = 0 ; l < 10 ; l++)
    {    
        for(c = 0 ; c < 10 ; c++)
        {    
            /* define a state and get its index */
            statebufferO[0]=l; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
        
            if ((c==5) || (c==0)) {
                if ((c==5) && ( (l==2) || (l==7)) ){
                    /* I am on a door
                     either I move left with probability p
                     or I stay with propability 1-p */
                    statebufferD[0]=l;
                    statebufferD[1]=c-1;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P2->setEntry(indexO,indexD,p);
                    // either I stay
                    P2->setEntry(indexO,indexO,1-p);
                }
                else{
                    /* I am on the wall c=5 or c=0 I stay in the same state */
                    P2->setEntry(indexO,indexO,1.0);
                }
            }
            else{
                // I am whatever in a rooms I have two possibilities
                // either i move left with probability p
                    statebufferD[0]=l;
                    statebufferD[1]=c-1;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P2->setEntry(indexO,indexD,p);
                    // either I stay
                    P2->setEntry(indexO,indexO,1-p);
            }
        }
    }
    /* fill in last line */
    for(c = 0 ; c < 10 ; c++)
    {    
            /* define a state and get its index */
            statebufferO[0]=10; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
            P2->setEntry(indexO,indexO,1.0);
    }
    
    mdpSSP->AddMatrix(2,P2);
    cout<<"Added matrix (action 2)"<< endl;
    
    /* Define matrix for action 3 (RIGHT) */
    SparseMatrix *P3 = new SparseMatrix(dim_SS);
    for(l = 0 ; l < 10 ; l++)
    {    
        for(c = 0 ; c < 10 ; c++)
        {    
            /* define a state and get its index */
            statebufferO[0]=l; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
        
            if ((c==4) || (c==9)) {
                if ((c==4) && ( (l==2) || (l==7)) ){
                    /* I am on a door
                     either I move right with probability p
                     or I stay with propability 1-p */
                    statebufferD[0]=l;
                    statebufferD[1]=c+1;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P3->setEntry(indexO,indexD,p);
                    /* either I stay */
                    P3->setEntry(indexO,indexO,1-p);
                }
                else{
                    /* I am on the wall c=4 or c=9 I stay in the same state */
                    P3->setEntry(indexO,indexO,1.0);
                }
            }
            else{
                // I am whatever in a rooms I have two possibilities
                // either i move left with probability p
                    statebufferD[0]=l;
                    statebufferD[1]=c+1;
                    indexD=stateSpace->Index(statebufferD); //computing the index of destination
                    P3->setEntry(indexO,indexD,p);
                    // either I stay
                    P3->setEntry(indexO,indexO,1-p);
            }
        }
    }
    /* fill in last line */
    for(c = 0 ; c < 10 ; c++)
    {    
            /* define a state and get its index */
            statebufferO[0]=10; /* initialize the value of the first dim */
            statebufferO[1]=c; /* initialize the value of the second dim */
            indexO=stateSpace->Index(statebufferO);
            P3->setEntry(indexO,indexO,1.0);
    }
    
    mdpSSP->AddMatrix(3,P3);
    cout<<"Added matrix (action 3)"<< endl;
    
    
    cout<<"Finishing Adding matrices MDP"<< endl;
    cout<<"Writing MDP"<<endl;
    //mdpSSP->WriteMDP();
    
    cout<<endl<<endl<<"###############################"<<endl;

    cout<<"Printing solution from value iteration"<<endl;
    cout<<"Done : path length  "<< maxIter <<endl;
    SolutionMDP *optimum2 = mdpSSP->ValueIteration(epsilon, maxIter);
    cout<<"Print solution"<< endl;
    optimum2->Write();

    cout<<endl<<endl<<"################################"<<endl;

    cout<<"Printing solution from Value Iteration GS"<<endl;
    cout<<"Done : path length  "<< maxIter <<endl;
    FeedbackSolutionMDP *optimum3 = mdpSSP->ValueIterationGS(epsilon, maxIter);
    cout<<"Print solution"<< endl;
    optimum3->Write();
    delete optimum3;

    cout<<endl<<endl<<"###############################"<<endl;

    cout<<"Printing solution from Policy Iteration Modified"<<endl;
    cout<<"Done : path length  "<< maxIter <<endl;
    optimum3 = mdpSSP->PolicyIterationModified(epsilon, maxIter, epsilon*0.1, 75);
    cout<<"Print solution"<< endl;
    optimum3->Write();

    cout<<endl<<endl<<"################################"<<endl;

    cout<<"Printing solution from Policy Iteration Modified GS"<<endl;
    cout<<"Done : path length  "<< maxIter <<endl;
    FeedbackSolutionMDP *optimum4 = mdpSSP->PolicyIterationModifiedGS(epsilon, maxIter,0.01, 15);
    cout<<"Print solution"<< endl;
    optimum4->Write();


    /* to get FeedbackPolicy properties we should make a cast */
    FeedbackSolutionMDP * policy = nullptr;
    if ( dynamic_cast <FeedbackSolutionMDP *> (optimum2)  != NULL ){
        policy = dynamic_cast <FeedbackSolutionMDP *> (optimum2);
    }
    /* Now policy is of FeedbackSolutionMDP type */
    cout<<"Print solution by dimension (line by line) w.r.t dim: "<< 1 << endl;
    optimum2->WriteSolutionByDim(1,stateSpace);
    
    cout<<endl<<endl<<"################################"<<endl;
    
    cout<<"Print solution by dimension (line by line) with repsect to dim: " << 0 << endl;
    optimum2->WriteSolutionByDim(0,stateSpace);
    
    cout<<endl<<endl<<"################################"<<endl;
    
    cout<<"Printing State Space Path and value function"<<endl;
    /* initial state */
    stateSpace->FirstState(statebufferO);
    /* path */
    for(k=0; k<stateSpace->Cardinal();k++){
        /* getting the index of the state */
        indexO = stateSpace->Index(statebufferO);
        /* the different values of the states are in the array */
        l=statebufferO[0]; /* getting value of the first dimension of the box */
        c=statebufferO[1]; /* getting value of the second dimension of the box */
        cout<< "--line=" << l << " --column=" << c;
        if ((c<=4) && (l<=4))
            cout << " Rooms at Bottom Left  " ;
        if ((c<=4) && (l>=5))
            cout << " Rooms at Top Left     " ;
        if ((c>=5) && (l<=4))
            cout << " Rooms at Bottom Right " ;
        if ((c>=5) && (l>=5))
            cout << " Rooms at Top Right    " ;
        /* getting the values and the action at the index of the state */
        if (policy)
            cout<< " --Optimal action=" << policy->getActionIndex(indexO) << " --Optimal Value=" << policy->getValueIndex(indexO)  <<endl;
        /* Move to next state */
        stateSpace->NextState(statebufferO);
    }
    
    cout<<endl<<endl<<"###############################"<<endl;
    
    cout<<endl<<"********************************"<<endl;

    cout<<"Deleting"<<endl;
    delete mdpSSP;

    cout<<"Deleting 2 (MDP)"<<endl;

    delete optimum2;
    delete optimum3;
    delete optimum4;

    delete stateSpace;
    delete actionSpace;
    
    /* delete buffer */
    delete[] statebufferO;
    delete[] statebufferD;

    return 0;
}
