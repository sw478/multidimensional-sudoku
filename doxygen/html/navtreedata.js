/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "mSudoku", "index.html", [
    [ "Database Implementation", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_database.html", [
      [ "sudokuFile & solutionFile", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_database.html#autotoc_md1", null ],
      [ "matrixFile", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_database.html#autotoc_md2", null ],
      [ "dimacsFile", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_database.html#autotoc_md3", null ],
      [ "enumFile", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_database.html#autotoc_md4", null ]
    ] ],
    [ "Project Specific File Formatting", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_file_formating.html", [
      [ "SUDOKU_FILE and SOLUTION_FILE Format", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_file_formating.html#autotoc_md6", null ],
      [ "MATRIX_FILE", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_file_formating.html#autotoc_md7", null ],
      [ "DIMACS_INPUT Format", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_file_formating.html#autotoc_md8", null ],
      [ "ENUMERATE_FILE Format", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_file_formating.html#autotoc_md9", null ]
    ] ],
    [ "Multidimensional Sudoku Explanation", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html", [
      [ "Terminology", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md11", null ],
      [ "n", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md12", null ],
      [ "containerSize", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md13", null ],
      [ "sudokuSize", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md14", null ],
      [ "span and containers", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md15", null ],
      [ "matrix", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md16", null ],
      [ "bSat", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md17", null ],
      [ "diagrams", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_m_sudoku_explanation.html#autotoc_md18", null ]
    ] ],
    [ "Project Requirements", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html", [
      [ "Program Options", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md20", null ],
      [ "Bash Script Options", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md21", null ],
      [ "DLX Solve", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md22", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md23", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md24", null ],
        [ "Notes", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md25", null ]
      ] ],
      [ "DLX Generate Full", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md26", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md27", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md28", null ]
      ] ],
      [ "DLX Generate Partial", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md29", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md30", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md31", null ]
      ] ],
      [ "DLX Enumerate", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md32", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md33", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md34", null ],
        [ "Note", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md35", null ]
      ] ],
      [ "Zchaff", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md36", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md37", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md38", null ]
      ] ],
      [ "BSat Solve (Create Dimacs)", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md39", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md40", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md41", null ]
      ] ],
      [ "BSat Solve (Process Zchaff Results)", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md42", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md43", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md44", null ]
      ] ],
      [ "BSat Generate (Create Dimacs)", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md45", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md46", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md47", null ]
      ] ],
      [ "BSat Generate (Process Zchaff Results)", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md48", [
        [ "Program args", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md49", null ],
        [ "Goals", "md__c___users_tr_user__v_s__code_multidimensional_sudoku_project_reqs.html#autotoc_md50", null ]
      ] ]
    ] ],
    [ "Multidimensional Sudoku README", "md__c___users_tr_user__v_s__code_multidimensional_sudoku__r_e_a_d_m_e.html", [
      [ "general notes", "md__c___users_tr_user__v_s__code_multidimensional_sudoku__r_e_a_d_m_e.html#autotoc_md52", null ],
      [ "user options (test.sh)", "md__c___users_tr_user__v_s__code_multidimensional_sudoku__r_e_a_d_m_e.html#autotoc_md53", null ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ]
      ] ]
    ] ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Structure Index", "classes.html", null ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"heuristic_8h.html",
"structdoubly.html#a1cfc780f715aa3f8156c6edb685d189b"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';