@ECHO OFF
IF NOT DEFINED DevEnvDir (
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall" x64
)

SET BinDir=bin
SET ObjDir=obj
SET ExeName=skandar
SET CCFlags=/nologo /O2 /W4 /ZI /sdl /EHsc
SET CCOptions=src\*.cpp /Fo./%ObjDir%/ /Fe:%ExeName%.exe /link /subsystem:console

IF /I "%1" == ""      GOTO :BUILD_DEFAULT
IF /I "%1" == "clean" GOTO :BUILD_CLEAN
IF /I "%1" == "test"  GOTO :BUILD_TEST
ECHO unknown option...
GOTO :EOF


:BUILD_DEFAULT
    ECHO Building...
    IF NOT EXIST %BinDir% MKDIR %BinDir%
    IF NOT EXIST %ObjDir% MKDIR %ObjDir%

    CL %CCFlags% %CCOptions%

    IF EXIST %ExeName%.exe MOVE %ExeName%.exe %BinDir% >NUL
    IF EXIST         *.idb MOVE         *.idb %BinDir% >NUL
    IF EXIST %ExeName%.ilk MOVE %ExeName%.ilk %BinDir% >NUL
    IF EXIST         *.pdb MOVE         *.pdb %BinDir% >NUL
GOTO :EOF


:BUILD_CLEAN
    ECHO Cleaning...
    IF EXIST %ExeName%.exe DEL %ExeName%.exe
    IF EXIST         *.idb DEL         *.idb
    IF EXIST %ExeName%.ilk DEL %ExeName%.ilk
    IF EXIST         *.pdb DEL         *.pdb
    IF EXIST         *.obj DEL         *.obj

    IF EXIST %BinDir% RMDIR /Q /S %BinDir%
    IF EXIST %ObjDir% RMDIR /Q /S %ObjDir%
GOTO :EOF


:BUILD_TEST
    ECHO Testing...
    SET CCObjs=obj\Bitboard.obj obj\Board.obj obj\Move.obj obj\MoveGenerator.obj obj\Search.obj obj\String.obj obj\Timer.obj obj\Uci.obj
    FOR %%f in (tests\Test*.cpp) DO (
        CL /nologo /Od /W4 /sdl /EHsc tests\%%~nf.cpp %CCObjs%
        %%~nf.exe
        DEL %%~nf.exe
        DEL %%~nf.obj
    )
GOTO :EOF
