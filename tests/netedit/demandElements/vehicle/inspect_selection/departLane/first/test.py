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

# go to select mode
netedit.selectMode()

# select all using invert
netedit.selectionInvertDemand()

# go to inspect mode
netedit.inspectMode()

# inspect vehicle
netedit.leftClick(referencePosition, 91, 413)

# change departLane with an invalid value
netedit.modifyAttribute(2, "", True)

# change departLane with an invalid value
netedit.modifyAttribute(2, "dummyDepart", True)

# change departLane with a valid value
netedit.modifyAttribute(2, "first", True)

# Check undo redo
netedit.undo(referencePosition, 5)
netedit.redo(referencePosition, 5)

# save routes
netedit.saveRoutes(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
