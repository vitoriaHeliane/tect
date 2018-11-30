:- module(mainSystem, [systemMenu/1]).

:- use_module("constants").
:- use_module("project").
:- use_module("utils").

createProject(LoggedUser):-
    tty_clear,
    project:createProject(LoggedUser).

listProject():-
    tty_clear,
    project:listProject().

requestAccess(LoggedUser):-
    tty_clear,
    constants:header(Header),
    writeln(Header),
    constants:request_access_header(RequestAccessHeader),
    writeln(RequestAccessHeader),
    writeln("Informe o ID do projeto:"),
    utils:readNumber(Id),
    project:project(Id, _, _, _) -> project:requestAccess(Id, LoggedUser); writeln("Id informado inválido.").

manageProject(LoggedUser):-
    tty_clear,
    constants:header(Header),
    writeln(Header),
    constants:manage_project_header(ManageProjectHeader),
    writeln(ManageProjectHeader),
    writeln("Informe o ID do projeto:"),
    utils:readNumber(Id),
    project:project(Id, _, _, _) -> (project:projectMenu(LoggedUser, Id)); writeln("Id informado inválido.").

printSystemMenu():-
    tty_clear,
    constants:header(Header),
    writeln(Header),
    constants:main_menu(MainMenu),
    writeln(MainMenu).

selectOption(Option, LoggedUser):- option(Option, LoggedUser),
    project:saveAllProjectData, writeln("Pressione qualquer tecla para continuar..."),
    get_char(_).

option(1, LoggedUser):- writeln("MEU USUARIO").
option(2, LoggedUser):- createProject(LoggedUser).
option(3, LoggedUser):- requestAccess(LoggedUser).
option(4, _):- listProject().
option(5, LoggedUser):- manageProject(LoggedUser).
option(6, LoggedUser):- writeln("GERAR RELATORIOS").
option(7, LoggedUser):- writeln("LOGOUT").
option(_,_):- writeln("Opção inválida!").

systemMenu(LoggedUser):-
    printSystemMenu,
    utils:readNumber(Option),
    Option =\= 8 -> selectOption(Option, LoggedUser), systemMenu(LoggedUser); writeln("Encerrando programa...").