// === Region: Project Overview ===
// 
// Here is the skelton code of COMP2011 PA3
//
// Project TA: CHUNG, Peter (cspeter@cse.ust.hk)
// All input/output handlings are done in the skeleton code
// You do not need to cin/cout anything in your extra code!
//
// For code-level questions, please send a direct email to the above TA. 
// Asking questions with code in a discussion forum (e.g., Piazza) may cause plagiarism issues
// Usually, you will get the quickest response via a direct email.
//
// Assumptions:
// The course code is the unique key (i.e., there won't be duplicated course codes in the system). 
// This assumption is necessary for many operations (e.g., searching, insertions, and deletions, etc.)
//
// ================================= 

#include <iostream>
#include <cstring>
using namespace std;

// === Region: constants and structs ===
// The constants are structs are defined in this region
// ===================================

// constants
const int MAX_CODE = 10; // at most 10 characters (including the NULL character)
const int MAX_TITLE = 100; // at most 100 characters (including the NULL character)
struct CourseItem; // needed because Course needs to refer to CourseItem*
// A sorted linked list of Course. List items are sorted by the course code
struct Course {
    char code[MAX_CODE];        // course code
    char title[MAX_TITLE];      // course title
    int credit;                 // course credit
    Course* next;               // a pointer pointing to the next Course
    CourseItem* exclusions;     // a sorted linked list of CourseItem
    CourseItem* prerequisites;  // a sorted linked list of CourseItem
};
// A sorted linked list of CourseItem. List items are sorted by course->code
struct CourseItem {
    Course *course;             // pointing to the course
    CourseItem *next;           // a pointer pointing to the next CourseItem
};

// === Region: function declarations ===
// The function declarations. 
//
// If needed, you can add your own helper functions
//
// The implementation is done in the function definitions after the main() function
// ===================================


/**
 * function ll_print_all prints the linked lists stored in the system
 * @param head pointing to the head of the linked list
*/
void ll_print_all(const Course* head); // given, you cannot make any changes
    

/**
 * function ll_insert_prerequisite inserts a pre-requisite
 * Example: if COMP2011 requires COMP1022P, the targetCode is COMP2011 and the preCode is COMP1022P
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param preCode stores the course code of the pre-requisite 
 * @return true if the insertion is complete. false otherwise
*/
bool ll_insert_prerequisite(Course* head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE]);

/**
 * function ll_insert_exclusion inserts an exclusion
 * Example: if COMP2011 excludes COMP1022P, the targetCode is COMP2011 and the excludeCode is COMP1022P
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param excludeCode stores the course code to be excluded
 * @return true if the insertion is complete. false otherwise 
*/
bool ll_insert_exclusion(Course* head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]);

/**
 * function ll_delete_prerequisite deletes a pre-requisite
 * Example: Given the input, if there is a pre-requisite, the pre-requisite will be deleted
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param preCode stores the course code of pre-requisite
 * @return true if the deletion is complete. false otherwise
*/
bool ll_delete_prerequisite(Course* head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE]);

/**
 * function ll_delete_exclusion deletes an exclusion
 * Example: Given the input, if there is an exclusion, the exclusion will be deleted
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param excludeCode stores the course code of exclusion
 * @return true if the deletion is complete. false otherwise 
*/
bool ll_delete_exclusion(Course* head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]);

/**
 * function ll_insert_course inserts a course
 * Note: head is passed by reference because head may be changed during the insertion
 * @param head pointing to the head of the linked list
 * @param c is the course code 
 * @param t is the course title
 * @param cred is the course credit
 * @return true if the insertion is complete. false otherwise
*/
bool ll_insert_course(Course* &head, const char c[MAX_CODE], const char t[MAX_TITLE], int cred);

/**
 * function ll_delete_course deletes an existing course
 * Note: head is passed by reference because head may be changed during the deletion
 * @param head pointing to the head of the linked list
 * @param c is the course code
 * @return true if the deletion is complete. false otherwise
*/
bool ll_delete_course(Course* &head, const char c[MAX_CODE]);

/**
 * function ll_modify_course_title modifies the course title attribute of a course
 * @param head pointing to the head of the linked list
 * @param c is the course code
 * @param t is the new course title
 * @return true if the modification is complete. false otherwise 
*/
bool ll_modify_course_title(Course* head, const char c[MAX_CODE], const char t[MAX_TITLE]);

/**
 * function ll_modify_course_credit modifies the course credit attribute of a course
 * @param head pointing to the head of the linked list
 * @param c is the course code
 * @param cred is the new credit
 * @return true if the modification is complete. false otherwise
*/
bool ll_modify_course_credit(Course* head, const char c[MAX_CODE], int cred);



/**
 * function ll_cleanup_all cleans up the linked lists used in the system. It restores the original states (i.e., empty linked lists) of the system. 
 * No memory leak should be found after the clean up
 * Note: head is passed by reference because head may be changed during the clean up
 * @param head pointing to the head of the linked list
*/
void ll_cleanup_all(Course* &head);

// === Region: The main() function ===
// The main function is given
// DO NOT MODIFY anything inside the main() function
// ===================================

/**
 * function enter_credit: A helper function to enter a valid credit
 * ensure the credit is a non-negative integer
*/
int enter_credit() {
    int credit;
    while (true) {
        cout << "Enter the credit: " ;
        cin >> credit;
        if ( credit >= 0 )
            return credit;
        cout << "Invalid credit: " << credit << endl;
    }
}

/**
 * function main - the main function
*/
int main() {
    Course *clist = nullptr;
    enum MeunOption {
        OPTION_DISPLAY_CURRENT_LIST = 0,
        OPTION_INSERT_COURSE,
        OPTION_INSERT_PRE_REQUISITE,
        OPTION_INSERT_EXCLUSION,
        OPTION_DELETE_COURSE,
        OPTION_DELETE_PRE_REQUISITE,
        OPTION_DELETE_EXCLUSION,
        OPTION_MODIFY_COURSE_TITLE,
        OPTION_MODIFY_COURSE_CREDIT,
        OPTION_EXIT_WITHOUT_CLEANUP,
        OPTION_EXIT_WITH_CLEANUP, 
        MAX_MENU_OPTIONS
    };
    const int MAX_MENU_OPTIONS_LENGTH = 50;
    char menuOptions[MAX_MENU_OPTIONS][MAX_MENU_OPTIONS_LENGTH] = {
        "Display the current list",
        "Insert a new course",
        "Insert a pre-requisite",
        "Insert an exclusion",
        "Delete an existing course",
        "Delete an existing pre-requisite",
        "Delete an existing exclusion",
        "Modify a course title",
        "Modify a course credit",
        "Exit without clean up",
        "Exit with clean up"
    };
    
    int option = 0;
    int i = 0;
    int credit = 0;
    // It is tricky to check the input character array, write assumptions that we don't need to check
    char code[MAX_CODE] = ""; 
    char title[MAX_TITLE] = "" ;
    char targetCode[MAX_CODE] = "";
   
    bool ret = false;
    
    while (true) {
        cout << "=== Menu ===" << endl;
        for (i=0; i<MAX_MENU_OPTIONS; i++) 
            cout << i+1 << ": " << menuOptions[i] << endl; // shift by +1 when display

        cout << "Enter your option: ";
        cin >> option;

        option = option - 1; // shift by -1 after entering the option

        // The invalid menu option handling
        if ( option < 0 || option >= MAX_MENU_OPTIONS ) {
            cout << "Invalid option" << endl;
            continue;
        }

        // Exit operations handling
        if (option == OPTION_EXIT_WITHOUT_CLEANUP)
            break; // break the while loop
        else if (option == OPTION_EXIT_WITH_CLEANUP) {
            cout << "=== Cleanup ===" << endl;
            ll_cleanup_all(clist);
            ll_print_all(clist);
            break; //  break the while loop
        }
        
        switch (option) {
            case OPTION_DISPLAY_CURRENT_LIST:
                ll_print_all(clist);
                break;
            case OPTION_INSERT_COURSE:
                cout << "Enter a course code: ";
                cin >> code;
                cout << "Enter a course title: ";
                cin.ignore(); // necessary when mixing cin>> and cin.getline
                cin.getline(title, MAX_TITLE);
                credit = enter_credit();
                ret = ll_insert_course(clist, code, title, credit);
                if ( ret == false ) {
                     cout << "Failed to insert " << code << endl ;
                }
                else {
                    cout << code << " is successfully inserted" << endl ;
                    ll_print_all(clist);
                }
                break;
            case OPTION_INSERT_PRE_REQUISITE:
                cout << "Enter the target course code: ";
                cin >> targetCode;
                cout << "Enter the pre-requisite course code: ";
                cin >> code;
                if ( strcmp(targetCode, code) == 0) {
                    cout << "Cannot insert a pre-requisite to the same course " << code << endl;
                } else {
                    ret = ll_insert_prerequisite(clist, targetCode, code);
                    if ( ret == false ) {
                        cout << "Failed to insert pre-requisite " << targetCode << ", " << code << endl;
                    } else {
                        cout << "Inserted a pre-requisite " << targetCode << ", " << code << endl;
                        ll_print_all(clist);
                    }
                }
                break;
            case OPTION_INSERT_EXCLUSION:
                cout << "Enter the target course code: ";
                cin >> targetCode;
                cout << "Enter the exclusion course code: ";
                cin >> code;
                if ( strcmp(targetCode, code) == 0) {
                    cout << "Cannot insert an exclusion to the same course " << code << endl;
                } else {
                    ret = ll_insert_exclusion(clist, targetCode, code);
                    if ( ret == false ) {
                        cout << "Failed to insert exclusion " << targetCode << ", " << code << endl;
                    } else {
                        cout << "Inserted an exclusion " << targetCode << ", " << code << endl;
                        ll_print_all(clist);
                    }
                }
                break;
            case OPTION_DELETE_COURSE:
                cout << "Enter the course code: ";
                cin >> code ;
                ret = ll_delete_course(clist, code);
                if ( ret == false ) {
                    cout << "Failed to delete course " << code << endl;
                } else {
                    cout << "Deleted a course " << code << endl ;
                    ll_print_all(clist);
                }
                break;
            case OPTION_DELETE_PRE_REQUISITE:
                 cout << "Enter the target course code: ";
                cin >> targetCode;
                cout << "Enter the pre-requisite course code: ";
                cin >> code;
                ret = ll_delete_prerequisite(clist, targetCode, code);
                if ( ret == false ) {
                    cout << "Failed to delete pre-requisite " << targetCode << ", " << code << endl;
                } else {
                    cout << "Deleted a pre-requisite " << targetCode << ", " << code << endl;
                    ll_print_all(clist);
                }
                break;
            case OPTION_DELETE_EXCLUSION:
                cout << "Enter the target course code: ";
                cin >> targetCode;
                cout << "Enter the pre-requisite course code: ";
                cin >> code;
                ret = ll_delete_exclusion(clist, targetCode, code);
                if ( ret == false ) {
                    cout << "Failed to delete exclusion " << targetCode << ", " << code << endl;
                } else {
                    cout << "Deleted an exclusion " << targetCode << ", " << code << endl;
                    ll_print_all(clist);
                }
                break;
            case OPTION_MODIFY_COURSE_TITLE:
                cout << "Enter the course code: ";
                cin >> code;
                cout << "Enter the new course title: " ;
                cin.ignore();
                cin.getline(title, MAX_TITLE);
                ret = ll_modify_course_title(clist, code, title);
                if ( ret == false ) {
                    cout << "Failed to modify the course title of " << code << endl ;
                } else {
                    cout << "The course title of " << code << " is modified" << endl ;
                    ll_print_all(clist);
                }
                break;
            case OPTION_MODIFY_COURSE_CREDIT:
                cout << "Enter the course code: ";
                cin >> code ;
                credit = enter_credit();
                ret = ll_modify_course_credit(clist, code, credit);
                if ( ret == false ) {
                    cout << "Failed to modify the course credit of " << code << endl;
                } else {
                    cout << "The course credit of " << code << " is modified" << endl;
                    ll_print_all(clist);
                }
                break;
                
        } // end switch

    } // end while true
    return 0;
}


// === Region: function definitions ===
// You should implement the functions below
// ====================================


// This function is given
// DO NOT MODIFY THIS FUNCTION
// Otherwise, you may not be able to pass the ZINC test cases
void ll_print_all(const Course* head) {
    const Course *p;
    const CourseItem *q;
    int count;

    cout << "=== Course List (Code[Credits]) ===" << endl;
    count = 0;
    for (p = head; p!= nullptr; p = p->next) {
        cout << p->code << "[" << p->credit << "]" ;
        if ( p->next != nullptr )
            cout << " -> ";
        count++;
    }
    if ( count == 0 ) {
        cout << "No courses in the list";
    }
    cout << endl;

    cout << "=== Course titles ===" << endl ;
    count = 0;
    for (p = head; p!= nullptr; p = p->next) {
        cout << p->code << ": " << p->title << endl ;
        count++;
    }
    if ( count == 0 ) {
        cout << "No course titles" << endl;
    }

    cout << "=== Pre-requisites ===" << endl ;
    count = 0;
    for (p = head; p!= nullptr; p = p->next) {
        q = p->prerequisites;
        if ( q != nullptr ) {
            cout << p->code << ": ";
            for ( ; q != nullptr; q = q->next ) {
                cout << q->course->code ;
                if ( q->next != nullptr )
                    cout << ", " ;
            }
            count++;
            cout << endl;
        }
    }
    if ( count == 0 ) {
        cout << "No pre-requisites" << endl;
    }

    cout << "=== Exclusions ===" << endl ;
    count = 0;
    for (p = head; p!= nullptr; p = p->next) {
        q = p->exclusions;
        if ( q != nullptr ) {
            cout << p->code << ": ";
            for ( ; q != nullptr; q = q->next ) {
                cout << q->course->code ;
                if ( q->next != nullptr )
                    cout << ", " ;
            }
            count++;
            cout << endl;
        }
    }
    if ( count == 0 ) {
        cout << "No pre-exclusions" << endl;
    }

}



bool ll_insert_prerequisite(Course* head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE]) {   
    // TODO: Implementation of inserting a pre-requisite
    //find the desired course code 
    Course *p = head;
    if (p == nullptr) return false; //for insert into a empty course list
    for(; p!=nullptr; p = p->next){ //check for a valid course
        if (strcmp(p->code, targetCode) == 0) break; 
        if(p->next == nullptr && (strcmp(p->code, targetCode) != 0)) return false; //reaching the last one and no valid course name
    }    
    Course *q = head;
    for(; q!=nullptr; q = q->next){ //check for a valid pre-requisite from the course list
        if (strcmp(q->code, preCode) == 0) break; 
        if(q->next == nullptr && (strcmp(q->code, preCode) != 0)) return false; //reaching the last one and no valid course name
    }    
    //check if the same pre-requisite exists
    CourseItem *i = p->prerequisites;
    for(; i!= nullptr; i = i->next){
        if(strcmp(i->course->code, preCode) == 0) return false;
    }

    //create the node
    CourseItem *r = new CourseItem;
    r->course = q;
    r->next = nullptr;

    //insert a prequisite 
    CourseItem *prev = nullptr;
    CourseItem *current = p->prerequisites;
    if(current == nullptr ){   //empty pre-re list
        r->next = current;
        p->prerequisites = r;
        return true;
        }
           
    for(prev = nullptr, current=p->prerequisites; current!=nullptr; prev=current, current = current->next){ //finding the position to insert
        if(strcmp(current->course->code,preCode) > 0) break;
        }
    if(current == p->prerequisites){ //insert at the beginning
        r->next = current;
        p->prerequisites = r;
        return true;
    } else{ //insert at the middle //insert at the last
        r->next = current;
        prev->next = r;
        return true;
    }


}

bool ll_insert_exclusion(Course* head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]) {  
    // TODO: Implementation of inserting an exclusion
    Course *p = head; //find the desired course code 
    if (p == nullptr) return false; //for insert into a empty course list
    for(; p!=nullptr; p = p->next){ //check for a valid course
        if (strcmp(p->code, targetCode) == 0) break; 
        if(p->next == nullptr && (strcmp(p->code, targetCode) != 0)) return false; //reaching the last one and no valid course name
    }    
    Course *q = head;
    for(; q!=nullptr; q = q->next){ //check for a valid execlusion from the course list
        if (strcmp(q->code, excludeCode) == 0) break; 
        if(q->next == nullptr && (strcmp(q->code, excludeCode) != 0)) return false; //reaching the last one and no valid course name
    }    

    //check if the same execlusion exists !!!
    CourseItem *i = p->exclusions;
    for(; i!= nullptr; i = i->next){
        if(strcmp(i->course->code, excludeCode) == 0) return false;
    }

    //create the node
    CourseItem *r = new CourseItem;
    r->course = q;
    r->next = nullptr;

    //insert a prequisite 
    CourseItem *prev = nullptr;
    CourseItem *current = p->exclusions;
    if(current == nullptr ){   //empty pre-re list
        r->next = current;
        p->exclusions = r;
        return true;
        }
           
    for(prev = nullptr, current=p->exclusions; current!=nullptr; prev=current, current = current->next){ //finding the position to insert
        if(strcmp(current->course->code,excludeCode) > 0) break;
        }
    if(current == p->exclusions){ //insert at the beginning
        r->next = current;
        p->exclusions = r;
        return true;
    } else{ //insert at the middle //insert at the last
        r->next = current;
        prev->next = r;
        return true;
    }

}

bool ll_delete_prerequisite(Course* head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE]) {
    
    // TODO: Implementation of deleting a pre-requisite
    Course *p = head; //find the desired course code 
    if (p == nullptr) return false; //for delete from a empty list
    for(; p!=nullptr; p = p->next){ //check for a valid course
        if (strcmp(p->code, targetCode) == 0) break; 
        if(p->next == nullptr && (strcmp(p->code, targetCode) != 0)) return false; //reaching the last one and no valid course name
    }    
    //check for a valid pre-requisite
    CourseItem *q = p->prerequisites;
    if(q == nullptr) return false; // check for the empty pre-requisite list
    //searching the deleter course
    for(; q!=nullptr; q = q->next){
        if(strcmp(q->course->code, preCode) == 0) break;
        if(q->next == nullptr && (strcmp(q->course->code, preCode) != 0)) return false; //do not have the delete course
    }

    //delete the prerequisite
    CourseItem *prev = nullptr;
    CourseItem *current = p->prerequisites;
    for(; current!=nullptr; prev=current, current = current->next){ //finding the position to delete
        if(strcmp(current->course->code,preCode) == 0) break;
        }
    if(current == p->prerequisites){ //delete the first one
        p->prerequisites = p->prerequisites->next;
        delete current; 
        current = nullptr;
        q = nullptr;
        return true;
    } else{ //delete at the middle //delete at the last
        prev->next = current->next;
        delete current; 
        current = nullptr;
        q = nullptr;
        return true;
    }
    return false;
}

bool ll_delete_exclusion(Course* head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]) {
    
    // TODO: Implementation of deleting an exclusion
    Course *p = head; //find the desired course code 
    if (p == nullptr) return false; //for delete from a empty list
    for(; p!=nullptr; p = p->next){ //check for a valid course
        if (strcmp(p->code, targetCode) == 0) break; 
        if(p->next == nullptr && (strcmp(p->code, targetCode) != 0)) return false; //reaching the last one and no valid course name
    }    
    //check for a valid exclusions
    CourseItem *q = p->exclusions;
    if(q == nullptr) return false; // check for the empty exclusion list
    //searching the deleter course
    for(; q!=nullptr; q = q->next){
        if(strcmp(q->course->code, excludeCode) == 0) break;
        if(q->next == nullptr && (strcmp(q->course->code, excludeCode) != 0)) return false; //do not have the delete course
    }

    //delete the prerequisite
    CourseItem *prev = nullptr;
    CourseItem *current = p->exclusions;
    for(; current!=nullptr; prev=current, current = current->next){ //finding the position to delete
        if(strcmp(current->course->code,excludeCode) == 0) break;
        }
    if(current == p->exclusions){ //delete at the beginning
        p->exclusions = p->exclusions->next;
        delete current; 
        current = nullptr;
        q = nullptr;
        return true;
    } else{ //delete at the middle //delete at the last
        prev->next = current->next;
        delete current; 
        current = nullptr;
        q = nullptr;
        return true;
    }
    return false;
}


bool ll_insert_course(Course*& head, const char c[MAX_CODE], const char t[MAX_TITLE], int cred) {

    // TODO: Implementation of inserting a course
    //create the course
    Course *create = new Course;
    strcpy(create->code, c);
    strcpy(create->title, t);
    create -> credit = cred;
    create -> prerequisites = nullptr;
    create -> exclusions = nullptr;
    create -> next = nullptr;

    //inseting the course
    Course *prev = nullptr;
    Course *current = head;
    if(current == nullptr ){   //empty list
        create -> next = head;
        head = create;
        return true;
    }
    
    for (; current!= nullptr; prev = current, current = current->next){
        if(strcmp(current->code, c) == 0){ //find is there same course
            delete create;
            create = nullptr;
            return false;
        }
    }
    for(prev = nullptr, current=head; current!=nullptr; prev=current, current = current->next){ //finding the position to insert
        if(strcmp(current->code,c) > 0) break;
        }
    if(current == head){ //insert at the beginning
        create->next = current;
        head = create;
        return true;
    } else{ //insert at the middle //insert at the last
        create->next = current;
        prev->next = create;
        return true;
    }
         
}


bool ll_delete_course(Course*& head, const char c[MAX_CODE]) {

    // TODO: Implementation of deleting a course
    //check for a valid course code
    Course *p = head; //find the desired course code 
    if (p == nullptr) return false; //for delete from a empty list
    for(; p!=nullptr; p = p->next){ //check for a valid course
        if (strcmp(p->code, c) == 0) break; 
        if(p->next == nullptr && (strcmp(p->code, c) != 0)) return false; //reaching the last one and no valid course name
    }    

    //deleting the pre-requst & exclusion pointing to the course //questions about this part
    Course *current = head;
    for (current = head; current!= nullptr; current = current->next) //delete all the pre-request point to the course
        {ll_delete_prerequisite(head, current->code, c);
         ll_delete_exclusion(head, current->code, c);}
            
    //deleting all the pre-requst & esclusion of the course //memory leak
    CourseItem *q = nullptr;
    while(p->prerequisites != nullptr){ //deleting all the pre
        q = p->prerequisites;
        p->prerequisites = p->prerequisites->next;
        delete q;
    }
    while(p->exclusions != nullptr){  //deleting all the exc
        q = p->exclusions;
        p->exclusions = p->exclusions->next;
        delete q;
        } 
    q = nullptr;

    //deleting the course
    Course *prev = nullptr;
    for(current = head; current!=nullptr; prev=current, current = current->next){ //finding the position to delete
        if(strcmp(current->code,c) == 0) break;
        }
    if(current == head){ //delete at the beginning
        head->prerequisites = nullptr;
        head->exclusions = nullptr;
        head = head->next;
        delete current; 
        current = nullptr;
        p = nullptr;
        return true;
    } else{ //delete at the middle //delete at the last
        current->prerequisites = nullptr;
        current->exclusions = nullptr;
        prev->next = current->next;
        delete current; 
        current = nullptr;
        p = nullptr;
        return true;
    }
    return false; 
}

bool ll_modify_course_title(Course* head, const char c[MAX_CODE], const char t[MAX_TITLE]) {
    
    // TODO: Implementation of modifying a course title
    Course *p = head;
    for(; p!=nullptr; p = p->next){
        if(strcmp(p->code, c) == 0){
            strcpy(p->title, t);
            return true;
        }
    }
    return false; //for the invalid course code
}

bool ll_modify_course_credit(Course* head, const char c[MAX_CODE], int cred) {
    
    // TODO: Implementation of modifying a course credit
    Course *p = head;
    for(; p!=nullptr; p = p->next){
        if(strcmp(p->code, c) == 0){
            p->credit = cred;
            return true;
        }
    }
    return false; //for the invalid course code
}


void ll_cleanup_all(Course *&head) {
    
    // TODO: Implementation of clean up all
    //use delete to all??
    Course *p = head;
    CourseItem *q = nullptr;

    while(head!=nullptr){
        p = head;
        while(head->prerequisites != nullptr){
            q = head->prerequisites;
            head->prerequisites = head->prerequisites->next;
            delete q;
        }
        while(head->exclusions != nullptr){
            q = head->exclusions;
            head->exclusions = head->exclusions->next;
            delete q;
        }
        head = head->next;
        delete p;
    }
    p = nullptr;
    q = nullptr;
    head = nullptr;
}
