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
# @date    2019-07-16
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
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot, ['--gui-testing-debug-gl'])

# go to demand mode
netedit.supermodeDemand()

# go to stop mode
netedit.stopMode()

# change stop type with a valid value
netedit.changeStopType("stopChargingStation")

# create stop
netedit.leftClick(referencePosition, 327, 220)

# enable until
netedit.changeDefaultBoolValue(6)

# create stop
netedit.leftClick(referencePosition, 327, 220)

# set invalid value
netedit.changeDefaultValue(7, "dummyValue")

# try to create stop
netedit.leftClick(referencePosition, 327, 220)

# set invalid value
netedit.changeDefaultValue(7, "-20")

# try to create stop
netedit.leftClick(referencePosition, 327, 220)

# set valid value
netedit.changeDefaultValue(7, "0")

# try to create stop
netedit.leftClick(referencePosition, 327, 220)

# set valid value
netedit.changeDefaultValue(7, "22.33")

# try to create stop
netedit.leftClick(referencePosition, 327, 220)

# Check undo redo
netedit.undo(referencePosition, 5)
netedit.redo(referencePosition, 5)

# save additionals
netedit.saveAdditionals(referencePosition)

# save routes
netedit.saveRoutes(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
