#include "system.h"
#include "constants.h"
#include "suiteTest.h"

#include "createrProject.h"
#include "menuProject.h"

#include "myUser.h"


using namespace std;

void running() {
    user loggedUser;
    bool isDone = false;
    
    while (!isDone) {
        if (loginMenu(&loggedUser, &isDone)){
            systemMenu(loggedUser, &isDone);
        }
    }
}

void systemMenu(user loggedUser, bool *isDone) {
    string optionInput;
    char selectedOption;
    do {
        do {
            printSystemMenu(loggedUser.name);

            optionInput = readOption();
            cin.ignore();

            if (isMenuInputStringValid(optionInput, MY_USER, MAIN_EXIT) == false) {
                printInvalidOptionMessage();
            }

        } while (isMenuInputStringValid(optionInput, MY_USER, MAIN_EXIT) == false);

        selectedOption = optionInput[0];

        switch(selectedOption){
            case MY_USER:
                myUserMenu();
                break;
            case CREATE_PROJECT:
<<<<<<< HEAD
                cout << "Projeto criado" << endl;
                testCaseMenu(1, 1);
=======
                createProject(loggedUser);
>>>>>>> master
                break;
            case ASK_FOR_ACCESS_PROJECT:
                askPermission(loggedUser);
                break;
            case SEARCH_PROJECT:
                // not implemented
                break;
            case EDIT_PROJECT:
                verifyUserToProject(loggedUser);
                break;
            case LOGOUT:
                cout << LOGOUT_MSG << endl;
                logout();
                break;
            case MAIN_EXIT:
                *isDone = true;
                return;
            default:
                cout << INVALID_OPTION << endl;
                break;
        }
    
        pauseSystem();
    } while(selectedOption != LOGOUT && selectedOption != MAIN_EXIT);
}

void printSystemMenu(string userName) {
    printHeader(MAIN_HEADER);
    cout << MAIN_MENU << endl;
}