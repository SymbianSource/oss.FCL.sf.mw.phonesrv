/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Introduces CSatUiIconHandler class, which handles
*                icon fetching for SatUi.
*
*/


#ifndef CSATUIICONHANDLER_H
#define CSATUIICONHANDLER_H

#include <e32base.h>
#include <rsatservice.h>

class CFbsBitmap;
class RSatSession;

/**
 *  Handles asynchronous icon fetching for SatUi.
 *
 *  Opens connection to SatClient by using received session attribute.
 *  Receives icon information and does the selection based on received icon id
 *  attribute. Asks the handle of the icon bitmap from the SatClient's
 *  RSatIcon class by giving the icon information and the package for handle,
 *  which is done asynchronously. For mean time, this class is waiting for
 *  completion of request by starting CActiveSchedulerWait, which is stopped
 *  when request is completed. After that the icon is duplicated via RSatIcon
 *  by giving the handle information. This is also an asynchronous call and
 *  CActiveSchedulerWait is started again. When completed the pointer to the
 *  bitmap is returned.
 *
 *  @lib satui.exedll
 *  @since S60 v3.1
 */
class CSatUiIconHandler : public CBase
    {

public: // Constructors and destructor

    /**
     * Constructor.
     */
    static CSatUiIconHandler* NewL();

    /*
     * Destructor.
     */
    virtual ~CSatUiIconHandler();

public: // New functions

    /**
     * The method, which fetches the icon.
     *
     * @since S60 v3.1
     * @param aSatSession Session information for SatClient interface.
     * @param aIconId Id for icon to be fetched.
     * @param aOptWidth Optimal width of the icon to be fetched.
     * @param aOptHeight Optimal height of the icon to be fetched.
     * @param aIconSupport Flag to signal if icons are not supported at all
     * @return A pointer to the icon bitmap.
     */
    CFbsBitmap* FetchIconL( const RSatSession& aSatSession,
                            const TUint8 aIconId,
                            const TInt aOptWidth,
                            const TInt aOptHeight,
                            TBool& aIconSupport );

private:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.1
     */
    CSatUiIconHandler();

    /**
     * Sorts the icons into order of superiority and saves the order into a list
     *
     * @since S60 v3.1
     * @param aIconEf List of icons
     * @param aSelectedIcons List into which order is saved
     * @param aOptWidth Optimal width of the icon to be fetched.
     * @param aOptHeight Optimal height of the icon to be fetched.
     */
    void SortIconsL( const RIconEf& aIconEf,
                     RArray<TInt>& aSelectedIcons,
                     const TInt aOptWidth,
                     const TInt aOptHeight ) const;

private: // data

    /**
     * RSatService of the SatClient.
     */
    RSatService iSatService;

    };

#endif // CSATUIICONHANDLER_H
