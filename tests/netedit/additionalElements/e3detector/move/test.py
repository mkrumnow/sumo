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
netedit.setZoom("25", "0", "25")

# go to additional mode
netedit.additionalMode()

# select E3
netedit.changeElement("e3Detector")

# create E3
netedit.leftClick(referencePosition, 250, 250)

# select entry detector
netedit.changeElement("detEntry")

# Create Entry detector (for saving)
netedit.selectAdditionalChild(6, 0)
netedit.leftClick(referencePosition, 400, 250)

# select entry detector
netedit.changeElement("detExit")

# Create detExit detector (for saving)
netedit.selectAdditionalChild(6, 0)
netedit.leftClick(referencePosition, 600, 250)

# change to move mode
netedit.moveMode()

# move E3 to up
netedit.moveElement(referencePosition, 120, 250, 120, 50)

# move E3 to down
netedit.moveElement(referencePosition, 120, 50, 120, 300)

# move E3 to left
netedit.moveElement(referencePosition, 120, 300, -100, 300)

# move E3 to right
netedit.moveElement(referencePosition, -100, 300, 450, 300)

# Check undos and redos
netedit.undo(referencePosition, 7)
netedit.redo(referencePosition, 7)

# save additionals
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
