#
# Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:  Icon makefile for CBSUi application#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\z
else
	ZDIR=\epoc32\data\z
endif

# ----------------------------------------------------------------------------
# Configure these
# ----------------------------------------------------------------------------

TARGETDIR=$(ZDIR)\system\data
HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\cbsuiapp.mif
HEADERFILENAME=$(HEADERDIR)\cbsuiapp.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# CBSUi icons
# ----------------------------------------------------------------------------

RESOURCE :
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
		/c8,1 qgn_indi_cb_hot_add.bmp \
		/c8,1 qgn_indi_cb_kept_add.bmp \
		/c8,8 qgn_menu_cb_2.svg \
		/c8,8 qgn_menu_cb.svg \
		/c8,1 qgn_prop_cb_msg.bmp \
		/c8,1 qgn_prop_cb_msg_unread.bmp \
		/c8,1 qgn_prop_cb_subs.bmp \
		/c8,1 qgn_prop_cb_subs_unread.bmp \
		/c8,1 qgn_prop_cb_unsubs.bmp \
		/c8,1 qgn_prop_cb_unsubs_unread.bmp

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
