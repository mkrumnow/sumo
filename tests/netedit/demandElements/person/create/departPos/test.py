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

# go to person mode
netedit.personMode()

# change person plan
netedit.changePersonPlan("walks", "walkEdges")

# set invalid depart
netedit.changeDefaultValue(6, "dummyDepartPos")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set invalid depart
netedit.changeDefaultValue(6, "-12")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set valid depart
netedit.changeDefaultValue(6, "5000")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set valid depart (output will be changed)
netedit.changeDefaultValue(6, "20")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set valid depart (output will be changed)
netedit.changeDefaultValue(6, "random")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set valid depart (output will be changed)
netedit.changeDefaultValue(6, "free")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set valid depart (output will be changed)
netedit.changeDefaultValue(6, "base")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set valid depart (output will be changed)
netedit.changeDefaultValue(6, "random_free")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# set valid depart (output will be changed)
netedit.changeDefaultValue(6, "last")

# try to create person
netedit.leftClick(referencePosition, 274, 414)
netedit.leftClick(referencePosition, 180, 60)

# press enter to create route
netedit.typeEnter()

# Check undo redo
netedit.undo(referencePosition, 6)
netedit.redo(referencePosition, 6)

# save routes
netedit.saveRoutes(referencePosition)

# save additionals
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
