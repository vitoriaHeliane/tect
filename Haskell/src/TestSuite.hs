module TestSuite where
    import Constants
    import GeneralPrints
    import Validation
    import System.IO.Unsafe
    import System.Directory
    import System.FilePath
    import Data.List
    import Control.Monad
    import Control.DeepSeq
    import Prelude hiding (readFile)
    import System.IO.Strict (readFile)

    import TestCase

    data Suite = Suite {
        suiteId :: Int,
        suiteName :: String,
        suiteDescription :: String,
        projectId :: Int
    } deriving(Eq, Show)

    stringListToSuiteList :: [String] -> [Suite]
    stringListToSuiteList [] = []
    stringListToSuiteList (sId:(name:(sDes:(pId:strList)))) = (createSuite (read sId) name sDes (read pId)):(stringListToSuiteList strList)

    suiteListToString :: [Suite] -> String
    suiteListToString [] = []
    suiteListToString (suite:list) = (suiteToString suite) ++ (suiteListToString list)


    suiteListToStringList :: [Suite] -> [String]
    suiteListToStringList suiteList = lines (suiteListToString suiteList)

    readSuites :: Int -> IO [Suite]
    readSuites projId = do
        let filePath = data_folder_path ++ "/" ++ (show projId) ++ "/" ++ suites_file_path
        if unsafePerformIO $ doesFileExist filePath
            then do
                fileContents <- readFile filePath
                let contentsList = lines fileContents
                return (stringListToSuiteList contentsList)
            else do
                return []

    writeSuites :: Int -> [Suite] -> IO()
    writeSuites projId suites = do
        let filePath = data_folder_path ++ "/" ++ (show projId) ++ "/" ++ suites_file_path
        let projectFolderPath = data_folder_path ++ "/" ++ (show projId) ++ "/"
        let suitesToFile = (suiteListToString suites)

        if unsafePerformIO $ doesDirectoryExist data_folder_path
            then do
                if not (unsafePerformIO $ doesDirectoryExist projectFolderPath)
                    then do
                        createDirectory projectFolderPath
                    else do
                        putStrLn "Gravando suites de teste..."
            else do
                createDirectory (data_folder_path ++ "/")
                createDirectory projectFolderPath
        
        rnf suitesToFile `seq` (writeFile filePath $ suitesToFile)

    suiteToString :: Suite -> String
    suiteToString (Suite suiteId suiteName suiteDescription projectId) = (show suiteId) ++ "\n" ++ suiteName ++ "\n" ++ suiteDescription ++ "\n" ++ (show projectId) ++ "\n"

    createSuite :: Int -> String -> String -> Int -> Suite
    createSuite idInput nameInput descriptionInput projectIdInput = Suite {suiteId = idInput,
                                                                        suiteName = nameInput,
                                                                        suiteDescription = descriptionInput,
                                                                        projectId = projectIdInput}

    getSuiteId :: Suite -> Int
    getSuiteId (Suite {suiteId = id}) = id

    getSuiteName :: Suite -> String
    getSuiteName (Suite {suiteName = sName}) = sName

    generateNewSuiteId :: [Suite] -> Int
    generateNewSuiteId [] = 1
    generateNewSuiteId suites = (getSuiteId (last suites)) + 1
                                                                        
    createNewSuite :: Int -> IO()
    createNewSuite projId = do
        let suites = unsafePerformIO $ readSuites projId
        putStrLn header
        putStrLn create_suite_header
        putStrLn "Informe o Nome da Suite: "
        nameInput <- getLine
        putStrLn "Informe a descricão da Suite: "
        descrInput <- getLine
        let newSuite = createSuite (generateNewSuiteId suites) nameInput descrInput projId
        let newSuites = suites ++ (newSuite:[])
        writeSuites projId newSuites
        putStrLn "Suite criada com sucesso!"

    suitesToStringShow :: [Suite] -> String
    suitesToStringShow [] = []
    suitesToStringShow ((Suite {suiteId = id, suiteName = sName}):suites) = ("  " ++ (show id) ++ "  | " ++ sName ++ "\n") ++ line ++ "\n" ++ (suitesToStringShow suites)

    showSuites :: Int -> IO()
    showSuites projId = do
        let suites = unsafePerformIO $ readSuites projId
        putStrLn header
        putStrLn suite_list_header
        putStrLn line
        putStrLn table_header
        putStrLn line
        putStrLn (suitesToStringShow suites)

    searchSuite :: Int -> IO()
    searchSuite projId = do
        let suites = unsafePerformIO $ readSuites projId
        putStrLn header
        putStrLn search_suite_header
        putStrLn "Selecione o parâmetro de pesquisa:\n(1) ID\n(2) Nome da Suite\n"
        searchParameter <- getLine

        if isOptionValid searchParameter '1' '2'
            then do
                clearScreen
                putStrLn header
                if (searchParameter !! 0) == '1'
                    then do
                        putStrLn "Informe o ID da Suite:"
                        suiteId <- getLine
                        if isStringNumeric suiteId
                            then do
                                if isSuiteOnListId (read suiteId) suites
                                    then do
                                        let foundSuite = searchSuiteId (read suiteId) suites
                                        clearScreen
                                        putStrLn search_suite_header
                                        putStrLn("Suite encontrada:\n" ++ showSuite foundSuite)
                                    else do putStrLn "A suite com o ID informado não foi encontrada."
                            else do putStrLn "ID da Suite inválido."
                    else do
                        putStrLn "Informe o Nome da Suite:"
                        suiteName <- getLine
                        if isSuiteOnListName suiteName suites
                            then do
                                let foundSuite = searchSuiteName suiteName suites
                                clearScreen
                                putStrLn search_suite_header
                                putStrLn("Suite encontrada:\n" ++ showSuite foundSuite)
                            else do putStrLn "A suite com o nome informado não foi encontrada."
            else do
                putStrLn "Opção de seleção inválida!"

    showSuite :: Suite -> String
    showSuite (Suite suiteId suiteName suiteDescription projectId) =
        "Suite ID " ++ show suiteId ++ "\nNome: " ++ suiteName ++ "\nDescrição: " ++ suiteDescription ++ "\nID Projeto da Suite: " ++ show projectId ++ "\nTestes que passaram: " ++ (show (calculateStatiscs projectId suiteId)) ++ "%\n"

    searchSuiteId :: Int -> [Suite] -> Suite
    searchSuiteId suiteId [] = (Suite {suiteId = -1, suiteName = "NOT FOUND", suiteDescription = "NOT FOUND", projectId = -1})
    searchSuiteId suiteId (suite:suites)
        | (getSuiteId suite) == suiteId = suite
        | otherwise = searchSuiteId suiteId suites

    searchSuiteName :: String -> [Suite] -> Suite
    searchSuiteName sName [] = (Suite {suiteId = -1, suiteName = "NOT FOUND", suiteDescription = "NOT FOUND", projectId = -1})
    searchSuiteName sName (suite:suites)
        | (getSuiteName suite) == sName = suite
        | otherwise = searchSuiteName sName suites

    editSuite :: Int -> IO()
    editSuite projId = do
        let suites = unsafePerformIO $ readSuites projId
        putStrLn header
        putStrLn edit_suite_header
        putStrLn "Informe o ID da Suite:"
        suiteId <- getLine
        if isStringNumeric suiteId
            then do
                if isSuiteOnListId (read suiteId) suites
                    then do
                        let foundSuite = searchSuiteId (read suiteId) suites
                        clearScreen
                        putStrLn edit_suite_header
                        putStrLn("Dados atuais da Suite:\n" ++ showSuite foundSuite)
                        putStrLn "\n"
                        putStrLn "Informe o novo Nome da Suite: "
                        nameInput <- getLine
                        putStrLn "Informe a nova descricão da Suite: "
                        descrInput <- getLine
                        let editedSuite = generateEditedSuite foundSuite nameInput descrInput
                            newSuites = swapEditedSuite editedSuite suites
                        writeSuites projId newSuites
                        putStrLn "Suite editada com sucesso."
                    else do putStrLn "A suite com o ID informado não foi encontrada."
            else do putStrLn "ID da Suite inválido."

    generateEditedSuite :: Suite -> String -> String -> Suite
    generateEditedSuite (Suite {suiteId = sId, projectId = pId}) newName newDescription = (Suite sId newName newDescription pId)

    swapEditedSuite :: Suite -> [Suite] -> [Suite]
    swapEditedSuite editedSuite [] = []
    swapEditedSuite editedSuite (suite:suites)
        | (getSuiteId editedSuite) == (getSuiteId suite) = editedSuite:suites
        | otherwise = suite:(swapEditedSuite editedSuite suites)

    deleteSuiteFromList :: Int -> [Suite] -> [Suite]
    deleteSuiteFromList suiteId [] = []
    deleteSuiteFromList suiteId (suite:suites)
        | (getSuiteId suite) == suiteId = suites
        | otherwise = suite:(deleteSuiteFromList suiteId suites)

    isSuiteOnListName :: String -> [Suite] -> Bool
    isSuiteOnListName sName [] = False
    isSuiteOnListName sName (suite:suites)
        | (getSuiteName suite) == sName = True
        | otherwise = isSuiteOnListName sName suites

    isSuiteOnListId :: Int -> [Suite] -> Bool
    isSuiteOnListId suiteId [] = False
    isSuiteOnListId suiteId (suite:suites)
        | (getSuiteId suite) == suiteId = True
        | otherwise = isSuiteOnListId suiteId suites

    deleteSuiteFromSystem :: Int -> Int -> IO()
    deleteSuiteFromSystem projId suiteId = do
        let suites = unsafePerformIO $ readSuites projId
        if isSuiteOnListId suiteId suites
            then do
                let newSuites = deleteSuiteFromList suiteId suites
                writeSuites projId newSuites
                putStrLn "Suite excluída com sucesso."
            else do
                putStrLn "ID inválido, suite não cadastrada."

    deleteSuite :: Int -> IO()
    deleteSuite projId = do
        putStrLn header
        putStrLn delete_suite_header
        putStrLn "Informe o ID da Suite a ser deletada:"
        idToDelete <- getLine
        deleteSuiteFromSystem projId (read idToDelete)
    
    isSuiteOnSystemId :: Int -> Int -> IO Bool
    isSuiteOnSystemId projectId suiteId = do
        let suites = unsafePerformIO $ readSuites projectId
        return (isSuiteOnListId suiteId suites)

    callCaseMenu :: Int -> IO()
    callCaseMenu projectId = do
        putStrLn (header ++ "\n" ++ manage_test_suite)
        putStrLn "Informe o ID da suite a ser gerenciada:"
        suiteId <- getLine
        if isStringNumeric suiteId
            then do
                if unsafePerformIO $ isSuiteOnSystemId projectId (read suiteId)
                    then do
                        caseMenu projectId (read suiteId)
                    else do
                        putStrLn "A suite informada não está cadastrada no sistema."
            else do
                putStrLn "Suite"


    chooseAction :: Int -> Char -> IO()
    chooseAction projId option
        | option == create_suite = do createNewSuite projId
        | option == list_suites = do showSuites projId
        | option == search_suite = do searchSuite projId
        | option == edit_suite = do editSuite projId
        | option == delete_suite = do deleteSuite projId
        | option == manage_test_cases = do callCaseMenu projId
        | option == go_back = do print "GO BACK"
        | otherwise = do print invalid_option

    showSuiteMenu :: IO()
    showSuiteMenu = do
        printHeaderWithSubtitle suite_menu

    suiteMenu :: Int -> IO()
    suiteMenu projId = do
        showSuiteMenu
        putStrLn choose_option
        input <- getLine
        if isOptionValid input create_suite go_back
            then do
                let option = input !! 0
                if option == go_back
                    then systemPause
                    else do
                        clearScreen
                        chooseAction projId option
                        systemPause
                        suiteMenu projId
            else do
                putStrLn invalid_option
                systemPause
                suiteMenu projId