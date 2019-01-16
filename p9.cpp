#include <bits/stdc++.h>
using namespace std;
/*
    http://www.cplusplus.com/reference/atomic/atomic_compare_exchange_strong/
*/
enum e_ue_fsm_states_t{
    E_UE_INVALID_STATE = 0,
    E_UE_START_STATE = 1,          /* Just inited state */
    E_UE_REGISTRATION_STARTED,     /* Started Registration, sent
                                    * SUCI to AUSF.*/
    E_UE_AUTN_STARTED,             /* Started Authentication,
                                    * Sent RAND and AUTN to UE */
};
int main() {
    /*  for exmaple 
        one thread is already sent SUCI to AUSF after processig the 
        registration initialUeMessage and registratioRequestMessage
        And the same UE sends the intialUeMessage again

        Following code shows how second attemt will be avoided by a AMF thread
    */
	atomic<int> currentUeState(E_UE_REGISTRATION_STARTED /*get the state using the ueMap API only*/);
    int *expectedUeState = new int(E_UE_START_STATE);
    int equalToCurrentUeState = currentUeState;          /*keep last argument is same as first argument */

/*  code to print the types of the three required arguments
    string _x = typeid(&currentUeState).name();
    system(("echo " + _x + " | c++filt -t").c_str());
    
    string _y = typeid(expectedUeState).name();
    system(("echo " + _y + " | c++filt -t").c_str());
    
    string _z = typeid(equalToCurrentUeState).name();
    system(("echo " + _z + " | c++filt -t").c_str());*/

	bool res = atomic_compare_exchange_strong(&currentUeState,
                                            expectedUeState,
                                            equalToCurrentUeState);
	if(res == true) {
        /*
            proceed further for processing
        */
    }else {
        cout << *expectedUeState << endl; // prints the currentUeState instead
        cout <<"amf is already processing this amfUeNgapId"<<endl;
    }

}