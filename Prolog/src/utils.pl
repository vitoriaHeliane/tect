:- module(utils, []).

:- use_module(constants).

readString(String) :- read_line_to_string(user_input, String).

systemPause() :-
    constants:pause_msg(Msg),
    writeln(Msg),
    read_line_to_string(user_input, _).

printHeader :-
    tty_clear,
    constants:header(Header),
    writeln(Header).

printHeaderAndSubtitle(Subtitle) :-
    printHeader,
    writeln(Subtitle).

validPassword(Password, true) :-
    atom_string(Atom, Password),
    atom_length(Atom, Length),
    Length > 3.
validPassword( false) :-
    constants:password_should_contains_min_characters(Msg),
    writeln(Msg).

validString(_, String, true) :-
    atom_string(Atom, String),
    atom_length(Atom, Length),
    Length > 0.
validString(Field,_, false) :-
    constants:string_not_empty(Msg),
    write(Field),
    writeln(Msg).

showPausedMsg(Msg) :-
    writeln(Msg),
    systemPause.

readOption(Option):-
    constants:choose_option(Msg),
    writeln(Msg),
    readNumber(Option).

readNumber(Number) :-
    read_line_to_codes(user_input, Codes),
    string_to_atom(Codes, Atom),
    atom_number(Atom, Number).

readLine(Stream, Line):-
    get0(Stream, Char),
    checkCharAndReadRest(Char, Chars, Stream),
    atom_chars(Line, Chars).

checkCharAndReadRest(10, [], _) :- !.  % Return
checkCharAndReadRest(-1, [], _) :- !.  % End of Stream
checkCharAndReadRest(end_of_file, [], _) :- !.
checkCharAndReadRest(Char, [Char|Chars], Stream) :-
    get0(Stream, NextChar),
    checkCharAndReadRest(NextChar, Chars, Stream).

createDirectory(Directory) :-
    exists_directory(Directory) -> true;
    make_directory(Directory).