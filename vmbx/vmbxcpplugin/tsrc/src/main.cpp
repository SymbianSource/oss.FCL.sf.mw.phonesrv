/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/
#include <QtTest/QtTest>
#include "ut_vmbxcpplugin.h"
#include "ut_vmbxuiengine.h"

// -----------------------------------------------------------------------------
// main()
// Main method implemented for directing test output to a file.
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    qDebug("main() >");
    
    QApplication app(argc, argv);

    qDebug("ut_vmbxuiengine");
    Ut_VmbxUiEngine tcUiEngine; 
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\vmbx\\ut_vmbxuiengine.txt";
    int result = QTest::qExec(&tcUiEngine, 3, pass);
    qDebug("ut_vmbxuiengine result %d", result);


    qDebug("ut_vmbxcpplugin");
    Ut_vmbxCpPlugin tcCpPlugin;
    pass[2] = "c:\\logs\\vmbx\\ut_vmbxcpplugin.txt";
    result = QTest::qExec(&tcCpPlugin, 3, pass);
    qDebug("ut_vmbxcpplugin result=%d", result);

    
    qDebug("main() <");
    return result;
    }
//End of file
