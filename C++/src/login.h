#pragma once

#include <iostream>
#include <cctype>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "validation.h"
#include "generalPrints.h"

struct user {
    std::string login;
    std::string name;
};

bool existingUserLogin(user *);
void saveLoggedUser(user *);
bool isUserLogged(user *);
void logout();
bool registerNewUser();
bool isFolderCreated (const char*);
bool createFolder(const char*);
bool isUserAlredyRegistered(std::string);
void printLoginMenu();
bool loginMenu(user*, bool*);