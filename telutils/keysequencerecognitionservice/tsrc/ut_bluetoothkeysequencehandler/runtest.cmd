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

call del \epoc32\winscw\c\data\ut_bluetoothsequencehandler.log
@echo on
:FINAL
call \epoc32\release\winscw\udeb\ut_bluetoothsequencehandler.exe -o c:\data\ut_bluetoothsequencehandler.log
call type \epoc32\winscw\c\data\ut_bluetoothsequencehandler.log
