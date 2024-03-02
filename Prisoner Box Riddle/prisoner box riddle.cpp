#include <iostream>
#include <ctime>
using namespace std;

/* -------------------------------------------------------------- */
/* This part serves as a pseudo random number generator for auto grade purpose only */
/* You are suggested not to refer to these codes in this assignment. */
/* You are not allowed to use global variables in this course. */
unsigned int next_num = 1; // Here we initiate an unsigned integer to be used in the following functions.

unsigned int pa1_rand() // This function is used to return a pseudo random number from 0 to 32767.
{
    next_num = next_num * 1103515245 + 2011;
    return static_cast<unsigned int>(next_num / 65536) % 32768;
}
void pa1_srand(unsigned int seed) // This function is used to set a seed of the pseudo random number generator.
{
    next_num = seed;
}
/* Everytime you call pa1_rand(), you will get a new pseudo random number. For the same seed, the sequence of pseudo 
   random number is fixed. For example, when seed = 3, the sequence of pseudo random number is fixed to be [17746, 
   30897, 9622, 18921, 4034, 17510, 24152, 14388, 23665, 31532, ...]. When seed = 5, the sequence of pseudo random 
   number is fixed to be [18655, 32247, 9873, 9718, 26373, 27678, 5314, 22512, 31845, 22885, ...] */
/* -------------------------------------------------------------- */



const int MAX_BOXES = 1000; // This constant stores the largest length of boxes[]. You can use it in your code.

// This function is used to fill the 1D array with a random sequence of unique numbers from 1 to the user input for the number of prisoners
void placeSlips(int boxes[], int num_prisoners)
{
    for (int i = 0; i < num_prisoners; i++)
        boxes[i] = -1;
    int counter = 0;
    while (counter < num_prisoners)
    {
        int num = pa1_rand() % (num_prisoners);
        if (boxes[num] == -1)
        {
            boxes[num] = counter;
            counter++;
        }
    }
}

// TASK 1: Simulate an actual riddle room
bool simulateRoom(const int boxes[], int num_prisoners, int num_trials)
{
    int success_number = 0;
    for(int i = 0; i < num_prisoners; i++){ // prisoners get in one by one
        int pointer = i;

        for (int j = 0; j < num_trials; j++){ // open boxes with the given trial
            if (boxes[pointer] == i){
                success_number += 1;
                break;
            }
            else {
                pointer = boxes[pointer];
            }

        }

    }
    if (success_number == num_prisoners)
        return true;
    else
        return false;
}


// TASK 2: Display certain statistics for a given room
void statsRoom(const int boxes[], int num_prisoners, int num_trials)
{
    int success_number = 0; 
    for(int i = 0; i < num_prisoners; i++){ 
        int pointer = i;
        for (int k = 0; k < num_trials; k++){  // finding the number of success poisoners
            if (boxes[pointer] == i){
                success_number += 1;
                break;
            }
            else {
                pointer = boxes[pointer];
            }
        }
    }

    // finding loop
    int reference[num_prisoners] = {};  // store the number remain     
    for (int j=0; j< num_prisoners; j++) reference[j] = boxes[j]; // set the value for reference as boxes
    int current_loop[num_prisoners] = {};  // store the current loop  
    int current_loop_length = 0;
    int number_of_loops = 0;
    int longest_loop_length = 0;
    int smallest_loop_length = num_prisoners;
    int longest_loop[num_prisoners] = {};   // store the longest loop

    bool flag = true, flag2 = true, flag3 = false;
    int currentloopindex = 0, temp = 0, smallest = 0, movepointer = -1, startpointer = -1;


    while (true){
        flag = true;
        for (int w =0; w < num_prisoners; w++){  // test if reference become all 0
            if (reference[w] != 0) {
                flag = false;
                break;
            }
        }
        if (flag) break;
        //cout<<"first loop"<<endl;

        for (int k = 0; k<num_prisoners;k++) current_loop[k] = 0;  // initalize the current_loop
        if (smallest == 0){   // only for the first loop
            for (int a=0; a<num_prisoners;a++){
                if(reference[a] == smallest){
                    startpointer = a;
                    movepointer = a;
                    break;
                }
            }
        }
        while(true){ // for all loop

            if (reference[movepointer] == startpointer){        
                current_loop[currentloopindex] = reference[movepointer];
                number_of_loops += 1;
                current_loop_length += 1;
                reference[movepointer] = 0;
                break;
            } else{
                    current_loop[currentloopindex] = reference[movepointer];
                    current_loop_length += 1;
                    currentloopindex += 1;
                    temp = movepointer;
                    movepointer = reference[movepointer];
                    reference[temp] = 0;
            }

        }

        if (smallest_loop_length > current_loop_length) smallest_loop_length = current_loop_length;
        if(longest_loop_length < current_loop_length){
            longest_loop_length = current_loop_length;
            for(int j=0; j<num_prisoners; j++) longest_loop[j]=0;
            for(int k=0; k < current_loop_length;k++) longest_loop[k] = current_loop[k];
        }
        currentloopindex = 0;
        current_loop_length = 0;

        smallest = num_prisoners, movepointer = -1, startpointer = -1; //find the smallest slip number in reference
        for (int i=0; i < num_prisoners; i++){
            if ((reference[i] < smallest) && (reference[i]!= 0)){
                 smallest = reference[i]; 
                 movepointer = i; 
                 startpointer = i;
            }
        }

    }


    // length of array = sizeof(array) / size(array[0])
    cout << "The number of prisoners who find their slips: " << success_number << endl;
    cout << "Number of loops: " << number_of_loops << endl;
    cout << "Smallest loop length: " << smallest_loop_length << endl;
    cout << "Longest loop length: " << longest_loop_length << endl;
    cout << "Largest loop: ";
    for (int d = 0; d < longest_loop_length; d++){
            cout << longest_loop[d]<<" ";
     }
    cout << endl;
}

// TASK 3: Find the number of instances in 1000 rooms when the prisoners will all be able to escape
double successRooms(int boxes[], int num_prisoners, int num_trials) //  suceess rate of 1000 rooms basically repeating it a 1000 times
{
    double successroom = 0;
    for (int i = 0; i<1000; i++){
        placeSlips(boxes, num_prisoners);
        if(simulateRoom(boxes, num_prisoners, num_trials))
            successroom += 1;
    }

     return successroom;
}

// TASK 4: Nice guard will help the prisoners to successfully leave the room by breaking any loop which is greater than the number of trials
//         Return true if the intervention was applied, else return false
bool niceGuard(int boxes[], int num_prisoners, int num_trials) // test 211, 222?
{
    // checking for the number of trian first??
    double chance = static_cast<double>(num_prisoners) / 2;
    if (num_trials  < chance) return false;

    int currentloopindex = 0; int current_loop_length = 0;
    int current_loop[num_prisoners] = {};  // store the current loop  

    for (int i=0; i<num_prisoners;i++){ // for all loop
        int movepointer = i;
        current_loop[currentloopindex] = i;
        currentloopindex += 1;
        while (true){
            if (boxes[movepointer] == i){  
                current_loop[currentloopindex] = boxes[movepointer];      
                current_loop_length += 1;
                break;
            } else{
                    current_loop[currentloopindex] = boxes[movepointer];
                    current_loop_length += 1;
                    currentloopindex += 1;
                    movepointer = boxes[movepointer];
            }
        }


        if (current_loop_length > num_trials){  
            int temp = 0, store = 0, first = 0, second = 0, firstindex = 0, secondindex = 0;
            bool even = (current_loop_length%2 == 0)? true : false;  // 03125 //4123 // two people one trial?? //handle for odd and even case?
            store = current_loop_length/2; //5
            first = current_loop[0]; //i
            second = current_loop[num_trials]; //store
            for (int a=0; a<num_prisoners; a++){
                if (boxes[a]==first) firstindex = a;
                if (boxes[a]==second) secondindex = a;
            }
            boxes[firstindex] = second;
            boxes[secondindex] = first;
            /*for (int j = 0; j < num_prisoners; j++)
                cout << boxes[j] << " ";
            cout << endl;*/

            return true;

        }

        currentloopindex = 0;
        current_loop_length = 0;
        for (int k = 0; k<num_prisoners; k++) current_loop[k] = 0;
    }

    return false;
}

// DO NOT WRITE ANYTHING AFTER THIS LINE. ANYTHING AFTER THIS LINE WILL BE REPLACED.

int main()
{

    int num_prisoners, num_trials, seed;
    // set boxes, priosoners

    cout << "Enter the number of prisoners (from 1 to 1000) :" << endl;
    cin >> num_prisoners;
    cout << "Enter the number of boxes each prisoner can open (from 1 to 1000) :" << endl;
    cin >> num_trials;
    cout << "Enter the seed for randomization else enter 0 and the system will randomize for you :" << endl;
    cin >> seed;

    if (seed == 0)
    {
        pa1_srand(time(NULL));
    }
    else
    {
        pa1_srand(seed);
    }

    int boxes[MAX_BOXES]; // array for the boxes
    for (int i = 0; i < MAX_BOXES; i++)
        boxes[i] = 0;

    int choice;
    cout << "Please choose an option:" << endl;
    cout << "0.Print the boxes" << endl;
    cout << "1.Simulate a room" << endl;
    cout << "2.Compute statistics of a room:" << endl;
    cout << "3.Compute success rate over 1000 rooms" << endl;
    cout << "4.Apply nice guard intervention in a room" << endl;
    cin >> choice;

    switch (choice)
    {
    case 0:
        placeSlips(boxes, num_prisoners);
        for (int i = 0; i < num_prisoners; i++)
            cout << boxes[i] << " ";
        cout << endl;
        break;
    case 1:
        placeSlips(boxes, num_prisoners);
        if (simulateRoom(boxes, num_prisoners, num_trials) == true)
        {
            cout << "Success! All prisoners found their slip." << endl;
        }
        else
        {
            cout << "Failure! Not all prisoners were able to find their slip." << endl;
        }
        break;
    case 2:
        placeSlips(boxes, num_prisoners);
        statsRoom(boxes, num_prisoners, num_trials);
        break;
    case 3:
    {

        double prisoners_left = successRooms(boxes, num_prisoners, num_trials);
        cout << "All prisoners were able to leave " << prisoners_left << "/1000 of the rooms." << endl;
    }
    break;
    case 4:
    {
        placeSlips(boxes, num_prisoners);
        int copy_arr[MAX_BOXES];
        for (int i = 0; i < num_prisoners; i++)
        {
            copy_arr[i] = boxes[i]; // copying the array for checking if no more than 2 swaps are made
        }

        bool nice_guard_val = niceGuard(boxes, num_prisoners, num_trials);

        int swap_count = 0;
        for (int j = 0; j < num_prisoners; j++)
        {
            if (copy_arr[j] != boxes[j])
            {
                swap_count++;
            }
        }
        if (swap_count > 2)
        {
            cout << "Illegal intervention as you have more than 1 swap " << endl;
        }
        else
        {
            if (nice_guard_val == false)
                cout << "No intervention needed." << endl;
            else
            {
                cout << "Intervention applied." << endl;
                // validity function
                bool prisoners_left = simulateRoom(boxes, num_prisoners, num_trials);
                // should return true
                if (prisoners_left == true)
                    cout << "All the prisoners were able to escape" << endl;
                else
                {
                    cout << "All the prisoners were not able to escape" << endl;
                }
            }
        }
    }
    break;
    }

    return 0;
}