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

# go to vehicle mode
netedit.vehicleMode()

# select flow
netedit.changeElement("flow")

# set invalid line
netedit.changeDefaultValue(12, "%%%%%%")

# try to create flow
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 280, 60)

# press enter to create flow
netedit.typeEnter()

# set valid Line
netedit.changeDefaultValue(12, "ownLine")

# create flow
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 280, 60)

# press enter to create flow
netedit.typeEnter()

# set empty line
netedit.changeDefaultValue(12, "")

# create flow
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 280, 60)

# press enter to create flow
netedit.typeEnter()

# Check undo redo
netedit.undo(referencePosition, 3)
netedit.redo(referencePosition, 3)

# save routes
netedit.saveRoutes(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
