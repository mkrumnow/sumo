/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEDialog_About.cpp
/// @author  Jakob Erdmann
/// @date    Feb 2011
/// @version $Id$
///
// The "About" - dialog for NETEDIT, (adapted from GUIDialog_AboutSUMO)
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#ifdef HAVE_VERSION_H
#include <version.h>
#endif

#include <utils/common/MsgHandler.h>
#include <utils/foxtools/FXLinkLabel.h>
#include <utils/gui/images/GUIIconSubSys.h>
#include <utils/gui/div/GUIDesigns.h>

#include "GNEDialog_About.h"


// ===========================================================================
// method definitions
// ===========================================================================
GNEDialog_About::GNEDialog_About(FXWindow* parent) :
    FXDialogBox(parent, "About Eclipse SUMO netedit", GUIDesignDialogBox) {
    // set dialog icon
    setIcon(GUIIconSubSys::getIcon(ICON_NETEDIT));

    // create frame for main info
    FXHorizontalFrame* mainInfoFrame = new FXHorizontalFrame(this, GUIDesignAuxiliarHorizontalFrame);

    // Netedit icon
    new FXLabel(mainInfoFrame, "", GUIIconSubSys::getIcon(ICON_SUMO_LOGO), GUIDesignLabelIcon);

    // "SUMO <VERSION>"
    FXVerticalFrame* descriptionFrame = new FXVerticalFrame(mainInfoFrame, GUIDesignLabelAboutInfo);
    myHeadlineFont = new FXFont(getApp(), "Arial", 18, FXFont::Bold);
    FXLabel* neteditLabel = new FXLabel(descriptionFrame, "SUMO netedit " VERSION_STRING, nullptr, GUIDesignLabelAboutInfo);
    neteditLabel->setFont(myHeadlineFont);
    new FXLabel(descriptionFrame, "Network editor for Eclipse SUMO, the Simulation of Urban MObility", nullptr, GUIDesignLabelAboutInfo);
    new FXLabel(descriptionFrame, "Graphical editor for road networks and infrastructure.", nullptr, GUIDesignLabelAboutInfo);
    new FXLabel(descriptionFrame, HAVE_ENABLED, nullptr, GUIDesignLabelAboutInfo);

    // write HAVE_ENABLED with the current modules (except Windows) in debug mode
    std::string modules(HAVE_ENABLED);
    while ((modules.size() > 0) && (modules.front() != ' ')) {
        modules.erase(modules.begin());
    }
    WRITE_DEBUG(("Modules: " + modules).c_str());

    // copyright notice
    new FXLabel(this, "Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.", nullptr, GUIDesignLabelAboutInfo);
    new FXLabel(this, "This application is based on code provided by the Eclipse SUMO project.", nullptr, GUIDesignLabelAboutInfo);
    new FXLabel(this, "These core components are available under the conditions of the Eclipse Public License v2.", nullptr, GUIDesignLabelAboutInfo);
    (new FXLinkLabel(this, "SPDX-License-Identifier: EPL-2.0", nullptr, GUIDesignLabelAboutInfo))->setTipText("http://www.eclipse.org/legal/epl-v20.html");

    // link to homepage
    (new FXLinkLabel(this, "https://sumo.dlr.de", nullptr, GUIDesignLabelCenter))->setTipText("https://sumo.dlr.de");

    // centered ok-button
    FXHorizontalFrame* buttonFrame = new FXHorizontalFrame(this, GUIDesignHorizontalFrame);
    new FXHorizontalFrame(buttonFrame, GUIDesignAuxiliarHorizontalFrame);
    FXButton* OKButton = new FXButton(buttonFrame, "&OK\t\t", GUIIconSubSys::getIcon(ICON_ACCEPT), this, ID_ACCEPT, GUIDesignButtonOK);
    new FXHorizontalFrame(buttonFrame, GUIDesignAuxiliarHorizontalFrame);

    // focus OK button
    OKButton->setFocus();
}


void
GNEDialog_About::create() {
    FXDialogBox::create();
}


GNEDialog_About::~GNEDialog_About() {
    delete myHeadlineFont;
}


/****************************************************************************/
