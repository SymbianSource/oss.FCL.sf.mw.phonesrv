@echo off
rem
rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

@echo on
:FINAL

@echo off

rmdir /q /s coverage
mkdir coverage
cd coverage

if "%1"=="mt_dialpad" (
echo Instrumenting dialpad.dll
call qmake^
 ..\..\..\dialpad.pro
call ctcwrap^
 mingw32-make debug
call qmake^
 ..\%1\%1.pro
call mingw32-make^
 debug
if "%2"=="mt_keyhandler" (
call qmake^
 ..\%2\%2.pro
call mingw32-make^
 debug
)
call %1.exe
call %2.exe
) 

if not "%1"=="mt_dialpad" (
echo Instrumenting %1
call qmake^
 ..\%1\%1.pro
call ctcwrap^
 mingw32-make debug
call %1.exe
)

call ctcpost^
 mon.sym mon.dat^
 -p profile.txt
call ctc2html -i profile.txt
cd ..
echo All done!

