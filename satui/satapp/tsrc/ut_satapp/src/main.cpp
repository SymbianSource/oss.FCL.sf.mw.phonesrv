/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:Unit testing of SatApp
*
*
*/


#include <QtTest/QtTest>

#include "ut_satappmainhandler.h"
#include "ut_csatuiobserver.h"
#include "ut_satappeventhandler.h"
#include "ut_playtoneprovider.h"

int main(int argc, char *argv[])
{
    qDebug("Ut_SatApp: main() >");
    QApplication app(argc, argv);

    int result(0);
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\sat\\ut_SatAppMainHandler.txt";
    
    qDebug("Test SatAppMainHandler");
    Ut_SatAppMainHandler tc;
    result = QTest::qExec(&tc, 3, pass);
    qDebug("Ut_SatApp: main() test SatAppMainHandler, result=%d", result);

    //qDebug("Test CSatUiObserve");
    Ut_CSatUiObserver utCSatUiObserver;
    char *test[3];
    test[0] = argv[0];
    test[1] = "-o"; 
    test[2] = "c:\\logs\\sat\\ut_CSatUiObserver.txt";
    result = QTest::qExec(&utCSatUiObserver, 3, test);
    qDebug("Ut_SatApp: main() test CSatUiObserver<, result=%d", result);

    /*Ut_SatAppEventProvider utSatAppEventProvider;
    char *test_event[3];
    test_event[0] = argv[0];
    test_event[1] = "-o"; 
    test_event[2] = "c:\\logs\\sat\\ut_SatAppEventProvide.txt";
    result = QTest::qExec(&utSatAppEventProvider, 3, test_event);
    qDebug("Ut_SatApp: main() test SatAppEventProvide<, result=%d", result);*/

    Ut_SatAppPlayToneProvider utPlayTone;
    char *test_playtone[3];
    test_playtone[0] = argv[0];
    test_playtone[1] = "-o"; 
    test_playtone[2] = "c:\\logs\\sat\\Ut_SatAppPlayToneProvider.txt";
    result = QTest::qExec(&utPlayTone, 3, test_playtone);
    qDebug("Ut_SatApp: main() test SatAppPlayToneProvider<, result=%d", result);

    return result;
}

//End of file
