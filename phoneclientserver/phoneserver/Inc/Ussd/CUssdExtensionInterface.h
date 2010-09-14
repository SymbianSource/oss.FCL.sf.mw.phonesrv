/*
 * Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  Extends the ussd functionality.
 *
 */
#ifndef __CUSSDEXTENSIONINTERFACE_H__
#define __CUSSDEXTENSIONINTERFACE_H__

// INCLUDES

#include <ecom/ecom.h>
#include <etelmm.h>
#include <ussdinterfaceconstants.hrh>
// FORWARD DECLARATIONS

// CONSTANTS
const TUid KUssdExtensionInterfaceUid =
    {
    KUssdExtensionInterfaceUidValue
    };

// CLASS DECLARATION

/**
 *  Base class for Ussd Extension Plugin.
 *
 *  This API allows customizing the USSD sending feature of the S60 Telephony 
 *  applications at runtime.
 *
 *  The API defines two roles:
 *  1) Validates the USSD string while sending. if it is unauthorized 
 *  in the context shall terminate the request.
 *  2) Validates the USSD string while receiving, the extension will get the first chance to consume the reply if needed
 *
 *
 *  Note:
 *  The customizer needs only one plug-in implementation for a variant, and so supports only rom-only plug-in.
 */
class CUssdExtensionInterface : public CBase
    {
public:
    // Constructors and destructor
    /**
     * NewL
     * Wraps ECom object instantitation. Will return the
     * default interface implementation, which matches to
     * given aOperationName.
     */
    static CUssdExtensionInterface* NewL();

    /**
     * NewL
     * Wraps ECom object instantitation. Will search for
     * interface implementation, which matches to given
     * aOperationName.
     * @param aOperationName name of requested implementation.
     * Implementations advertise their "name" as specified
     * in their resource file field
     * IMPLEMENTATION_INFO::default_data.
     */
    static CUssdExtensionInterface* NewL(const TDesC8& aMatch);

public:
    // API to be implemented by ECOM plug-in

    /**
     * Validate/Authenticate the Ussd string Before Sending from
     * the USSD Send handler
     * When Ussd Extention plugin return true, then ussd manager will continue sending of the ussd
     * Otherwise the transaction will be terminated with KErrAccessDenined 
     *
     * @param aMessageData  
     * @param aMessageAttributes .
     */
    virtual TBool
            ValidateUssdMsgSending(
                    const TDesC8& aMessageData,
                    const RMobileUssdMessaging::TMobileUssdAttributesV1& aMsgAttribute) = 0;
    /**
     * Validate/Authenticate the Ussd response received by the 
     * USSD receive handler
     *
     * @param aMessageData  
     * @return true implies that the ussd response is valid for the extension.
     * otherwise pass it to the observer. .
     */
    virtual TBool ValidateUssdResponse(const TDesC8& aMessage) = 0;
    /**
     * if ValidateUssdResponse returns true, then the extension plugin is waiting for the response 
     * so invoke the interface to deligate to the plugin.
     * Example:
     * ...
     *  TBool isValidForExtension = iUssdExtnInterface->ValidateUssdResponse(aMsgData);
     *  TInt errfromextension = KErrNone;
     *  if (isValidForExtension)
     *     {
     *     TRAPD(err, iUssdExtnInterface->ProcessReceivedUssdL(iDecodedMessage, errfromextension));
     *     ...
     *     
     * @param aMessageData  
     * @return error code
     * @leave On typical leave level problems. 
     */
    virtual void
            ProcessReceivedUssdL(const TDesC& aMessageData, TInt aError) = 0;

    /**
     * Notify the any network error to the extension, so that it can handle the internal states if required
     * @param aError 
     */
    virtual void NotifyNWError(TInt aError)=0;

public:
    /**
     * ~CUssdExtensionInterface
     *
     * Cleans up resources, specifically notify ECOM server that this
     * instance is being deleted.
     */
    virtual ~CUssdExtensionInterface();

private:
    TUid iDtor_ID_Key; // Identification on cleanup
    };
#include "CUssdExtensionInterface.inl"    
#endif // __CUSSDEXTENSIONINTERFACE_H__
