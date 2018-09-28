#include "reports.h"

using namespace std;

void generateReport(user loggedUser){
    char selectedOption;
    int projectId;
    int suiteId;
    vector<suite> suites;

    cout << "Digite a opção desejada:" << endl;
    cout << "Menu de geração de relatório" << endl;
    cout << "(1) Gerar relatório de uma suite de testes" << endl;
    cout << "(2) Gerar relatório de um projeto" << endl;
    cout << "(3) Gerar relatório de participação de usuário em todos os projetos" << endl;

    cin >> selectedOption;
    
    switch(selectedOption){
        case '1':
            cout << "Digite o código do projeto que contém a suite de testes:" << endl;
            cin >> projectId;
            bool isAllowed = false;
            vector<Project> projects;
            project = arquiveToArray();
            for(int i = 0; i < project.size(); i++){
                if(projects[i].id == projectId){
                    for(int j = 0; j < projects[i].users.size(); j++){
                        if(projects[i].users[j] == loggedUser){
                            isAllowed = true;
                        }
                    }
                }
            }
            if(isAllowed){
                cout << "Digite o código da suite de testes:" << endl;
                cin >> suiteId;

                suite = suiteOfReport;
                vector<suite> suitesOfProject;
                vector<Case> cases;
                cases = readCases();
                vector<Case> casesOfSuite;

                suites = readSuites();
                for(int j = 0; j < suites.size(); j++){
                    if(suites[j].projectId == projectId){
                        suitesOfProject.push_back(suites[j]);
                    }
                    if(suites[j].id == suiteId){
                        suiteOfReport = suites[j];
                    }
                }

                generateSuiteReport(suiteOfReport, suitesOfProject);
                cout << "" << endl;
                cout << "Relatório da suite de testes gerado com sucesso" << endl;
            }
            else{
                cout << "Usuário logado no sistema não tem acesso a esse projeto" << endl;
                cout << "Redirecionado ao menu inicial..." << endl;
                systemMenu();
            }
        case '2':
            suites = readSuites();
            vector<suite> projectSuites;
            vector<Project> projects;
            Project projectOfReport;
            projects = arquiveToArray();

            cout << "Digite o código do projeto base para o relatório:" << endl;
            cin >> projectId;

            for(int j = 0; j < suites.size(); j++){
                if(suites[j].projectId == projectId){
                    projectSuites.push_back(suites[i]);
                }
            }

            for(int i = 0; i < projects.size(); i++){
                if(projects[i].id == projectId){
                    projectOfReport = projects[i];
                }
            }

            generateProjectReport(projectOfReport, projectSuites);
            cout << "" << endl;
            cout << "Relatório do projeto gerado com sucesso" << endl;
        case '3':
            vector<Project> projects;
            projects = arquiveToArray();
            string userLogin;
            cout << "Digite o id do usuário base para o relatório" << endl;
            cin >> userLogin;

            generateUserReport(userLogin), projects);
    }

}

void generateSuiteReport(suite reportSuite, vector<Case> suiteTestCases){

    float passingPercentage;
    float notPassingPercentage;
    float problemsPercentage;
    float notExecutedPercentage;

    std ofstream reportFile(SUITES_PATH + reportSuite.id, std::ios::in);

    reportFile << "Relatório da suíte de testes " << reportSuite.name << endl;
    cout << "Relatório da suíte de testes " << reportSuite.name << endl;
    reportFile << "" << endl;
    cout << "" << endl;

    for(int i = 0; i < suiteTestCases.size(); i++){
        reportFile << "Nome do caso de testes: " << suiteTestCases[i].name << endl;
        cout << "Nome do caso de testes: " << suiteTestCases[i].name << endl;
        reportFile << "Objetivos: " << suiteTestCases[i].objectives << endl;
        cout << << "Objetivos: " << suiteTestCases[i].objectives << endl;
        reportFile << "Pré condições: " << suiteTestCases[i].preconditions << endl;
        cout << << "Pré condições: " << suiteTestCases[i].preconditions << endl;
        reportFile << "Passos: " << endl;
        cout << "Passos: " << endl;
        for(int j = 0; j < suiteTestCases[i].steps.size(); j++){
            reportFile << "Descrição: " << suiteTestCases[i].steps[j].description << endl;
            cout << "Descrição: " << suiteTestCases[i].steps[j].description << endl;
            reportFile << "Resultado esperado: " << suiteTestCases[i].steps[j].expectedResult; << endl;
            cout << << "Resultado esperado: " << suiteTestCases[i].steps[j].expectedResult; << endl;
        }
        reportFile << "" << endl;
        cout << "" << endl;
        reportFile << "Status do caso: " << suiteTestCases[i].status << endl;
        cout << "Status do caso: " << suiteTestCases[i].status << endl;
        reportFile << "" << endl;
        cout << "" << endl;

        if(suiteTestCases[i].status == string(CASE_NOT_EXECUTED)){
            notExecutedPercentage++;
        }
        else if(suiteTestCases[i].status == string(CASE_PASSED)){
            passingPercentage++;
        }
        else if(suiteTestCases[i].status == string(CASE_NOT_PASSED){
            notPassingPercentage++;
        }        
        else{
            problemsPercentage++;
        }
    }

    notExecutedPercentage /= suiteTestCases.size();
    notExecutedPercentage *= 100;

    passingPercentage /= suiteTestCases.size();
    passingPercentage *= 100;

    notPassingPercentage /= suiteTestCases.size();
    notPassingPercentage *= 100;

    problemsPercentage /= suiteTestCases.size();
    problemsPercentage *= 100;

    reportFile << "Percentual de casos de testes que passaram: " << string((int)passingPercentage) << endl;
    cout << "Percentual de casos de testes que passaram: " << string((int)passingPercentage) << endl;
    reportFile << "Percentual de casos de testes que não passaram: " << string((int)notPassingPercentage) << endl;
    cout << "Percentual de casos de testes que não passaram: " << string((int)notPassingPercentage) << endl;
    reportFile << "Percentual de casos de testes em que ocorreram problemas de execução: " << string((int)problemsPercentage) << endl;
    cout << "Percentual de casos de testes em que ocorreram problemas de execução: " << string((int)problemsPercentage) << endl;
    reportFile << "Percentual de casos de testes não executados: " << string((int)notExecutedPercentage) << endl;
    cout << "Percentual de casos de testes não executados: " << string((int)notExecutedPercentage) << endl;

}

void generateProjectReport(Project reportProject, vector<suite> projectSuites){

    float passingPercentage;
    int numberOfCases;

    std ofstream reportFile(PROJECT_FILE_PATH + reportSuite.id, std::ios::in);

    vector<Case> suiteCases;
    suiteCases = readCases(reportProject.id, projectSuites[i].id);

    reportFile << "Relatório do projeto de testes " << reportProject.name << endl;
    cout << "Relatório do projeto de testes " << reportProject.name << endl;
    reportFile << "Descrição do projeto:" << reportProject.description << endl;
    cout << "Descrição do projeto: " << reportProject.description << endl;
    reportFile << "Dono do projeto: " << reportProject.owner << endl;
    cout << "Dono do projeto: " << reportProject.owner << endl;
    reportFile << "" << endl;
    cout << "" << endl;

    for(int i = 0; i < projectSuites.size(); i++){

        reportFile << "Nome da suite de testes: " << projectSuites[i].name << endl;
        cout << "Nome da suite de testes: " << projectSuites[i].name << endl;
        reportFile << "Descrição da suite de testes: " << projectSuites[i].description << endl;
        cout << << "Descrição da suite de testes: " << projectSuites[i].description << endl;

        for(int j = 0; j < suiteCases.size(); j++){
            numberOfCases++;
            reportFile << "Nome do caso de testes: " << suiteTestCases[j].name << endl;
            cout << "Nome do caso de testes: " << suiteTestCases[j].name << endl;
            reportFile << "Objetivos: " << suiteTestCases[j].objectives << endl;
            cout << << "Objetivos: " << suiteTestCases[j].objectives << endl;
            reportFile << "Pré condições: " << suiteTestCases[j].preconditions << endl;
            cout << << "Pré condições: " << suiteTestCases[j].preconditions << endl;
            reportFile << "Passos: " << endl;
            cout << "Passos: " << endl;
            for(l = 0; l < suiteCases[j].steps.size(); l++){
                reportFile << "Descrição do passo: " << suiteCases[j].steps[l].description << endl;
                cout << "Descrição do passo: " << suiteCases[j].steps[l].description << endl;
                reportFile << "Resultado esperado: " << suiteCases[j].steps[l]].expectedResult; << endl;
                cout << << "Resultado esperado: " << suiteCases[j].steps[l].expectedResult; << endl;
            }
            reportFile << "" << endl;
            cout << "" << endl;
            reportFile << "Status do caso: " << suiteCases[].status << endl;
            cout << "Status do caso: " << suiteCases[i].status << endl;
            reportFile << "" << endl;
            cout << "" << endl;
            if(suiteCases[i].status == string(CASE_PASSED)){
                passingPercentage++;
            }
        }
    }

    passingPercentage /= numberOfCases;
    passingPercentage *= 100;

    reportFile << "Percentual de casos de testes que passaram: " << string((int)passingPercentage) << endl;
    cout << "Percentual de casos de testes que passaram: " << string((int)passingPercentage) << endl;

}

void generateUserReport(string userLogin, projects){

    std ofstream reportFile(USERS_FILE_PATH + userLogin, std::ios::in);
    int numberOfCreatedTests;
    int numberOfPassingTests;
    vector<Project> userProjects;
    vector<suite> suites;
    vector<Case> cases;
    suites = readSuites();

    reportFile << "Relatório de participação do usuário " << userLogin << endl;
    cout << "Relatório de participação do usuário" << userLogin << endl;
    reportFile << "" << endl;
    cout << "" << endl;
    reportFile << "Projetos com participação do usuário:" << endl;
    cout << "Projetos com participação do usuário:" << endl;

    for(int i = 0; i < projects.size(); i++){
        for(int j = 0; j < projects[i].users.size(); j++){
            if(projects[i].users[j].login == userLogin){
                userProjects.push_back(projects[i]);
            }
        }
    }  

    for(int i = 0; i < userProjects.size(); i++){
        reportFile << "Id do projeto: " << userProjects[i].id << endl;
        cout << "Id do projeto: " << userProjects[i].id << endl;
        reportFile << "Nome do projeto: " << userProjects[i].name << endl;
        cout << "Nome do projeto: " << userProjects[i].name << endl;
        reportFile << "Descrição do projeto:" << userProject[i].description << endl;
        cout << "Descrição do projeto: " << reportProject.description << endl;
        reportFile << "Dono do projeto: " << reportProject.owner << endl;
        cout << "Dono do projeto: " << reportProject.owner << endl;
        reportFile << "" << endl;
        cout << "" << endl;
        for(int j = 0; j < suites.size(); j++){
            if(suites[j].project)Id == userProject[i].id){
                reportFile << "Nome da suite de testes: " << projectSuites[i].name << endl;
                cout << "Nome da suite de testes: " << projectSuites[i].name << endl;
                reportFile << "Descrição da suite de testes: " << projectSuites[i].description << endl;
                cout << << "Descrição da suite de testes: " << projectSuites[i].description << endl;
                cases = readCases(userProjects[i].id, suites[j].id);
                for(int k = 0; k < cases.size(); k++){
                    numberOfCreatedTests++;
                    reportFile << "Nome do caso de testes: " << cases[k].name << endl;
                    cout << "Nome do caso de testes: " << cases[k].name << endl;
                    reportFile << "Objetivos: " << cases[k].objectives << endl;
                    cout << << "Objetivos: " << cases[k].objectives << endl;
                    reportFile << "Pré condições: " << cases[k].preconditions << endl;
                    cout << << "Pré condições: " << cases[k].preconditions << endl;
                    reportFile << "Passos: " << endl;
                    cout << "Passos: " << endl;
                    for(int l = 0; l < cases[k].users.size(); l++){
                        reportFile << "Descrição do passo: " << cases[k].steps[l].description << endl;
                        cout << "Descrição do passo: " << cases[k].steps[l].description << endl;
                        reportFile << "Resultado esperado: " << cases[k].steps[l]].expectedResult; << endl;
                        cout << << "Resultado esperado: " << cases[k].steps[l].expectedResult; << endl;
                    }
                    if(cases[k].status == string(CASE_PASSED)){
                        numberOfPassingTests++;
                    }
                }
            }
        }

    }

    float percentage;

    percentage = numberOfPassingTests / numberOfCreatedTests;

    reportFile << "Porcentagem de casos de teste criados pelo usuário que passaram: " <<  string(int(percentage)) << endl;
    cout << "Porcentagem de casos de teste criados pelo usuário que passaram: " <<  string(int(percentage)) << endl;

}