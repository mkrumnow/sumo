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

# go to additional mode
netedit.additionalMode()

# select parkingArea
netedit.changeElement("parkingArea")

# change reference to center
netedit.changeDefaultValue(10, "reference center")

# create parkingArea in mode "reference center"
netedit.leftClick(referencePosition, 250, 165)

# go to inspect mode
netedit.inspectMode()

# inspect first parkingArea
netedit.leftClick(referencePosition, 250, 200)

# Change parameter RoadSideCapacity with a non valid value (dummy)
netedit.modifyAttribute(5, "dummyRoadSideCapacity", True)

# Change parameter RoadSideCapacity with a non valid value (double)
netedit.modifyAttribute(5, "2.3", True)

# Change parameter RoadSideCapacity with a non valid value (negative)
netedit.modifyAttribute(5, "-5", True)

# Change parameter RoadSideCapacity with a non valid value (negative)
netedit.modifyAttribute(5, "7", True)

# Check undos and redos
netedit.undo(referencePosition, 2)
netedit.redo(referencePosition, 2)

# save additionals
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
