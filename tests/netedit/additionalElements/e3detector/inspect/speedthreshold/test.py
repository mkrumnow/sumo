#!/usr/bin/env python
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2009-2019 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
# SPDX-License-Identifier: EPL-2.0

# @file    test.py
# @author  Pablo Alvarez Lopez
# @date    2016-11-25
# @version $Id$

# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit  # noqa

# Open netedit
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot)

# apply zoom
netedit.setZoom("25", "25", "25")

# go to additional mode
netedit.additionalMode()

# select E3
netedit.changeElement("e3Detector")

# create E3
netedit.leftClick(referencePosition, 250, 100)

# select entry detector
netedit.changeElement("detEntry")

# Create Entry detector E3 (for saving)
netedit.leftClick(referencePosition, 250, 100)
netedit.leftClick(referencePosition, 250, 200)

# select entry detector
netedit.changeElement("detExit")

# Create Exit detector E3 (for saving)
netedit.leftClick(referencePosition, 250, 100)
netedit.leftClick(referencePosition, 250, 450)

# go to inspect mode
netedit.inspectMode()

# inspect first E3
netedit.leftClick(referencePosition, 250, 100)

# Change parameter SpeedTreshold with a non valid value (dummy)
netedit.modifyAttribute(7, "dummySpeedTreshold", False)

# Change parameter SpeedTreshold with a non valid value (negative)
netedit.modifyAttribute(7, "-12.1", False)

# Change parameter SpeedTreshold with a valid value
netedit.modifyAttribute(7, "6.3", False)

# Check undos and redos
netedit.undo(referencePosition, 4)
netedit.redo(referencePosition, 4)

# save additionals
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
