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

# select chargingStation
netedit.changeElement("chargingStation")

# set invalid efficiency (dummy)
netedit.changeDefaultValue(5, "dummyEfficiency")

# try to create chargingStation in mode "reference left"
netedit.leftClick(referencePosition, 250, 250)

# set invalid efficiency (negative)
netedit.changeDefaultValue(5, "-50")

# try to create chargingStation in mode "reference left"
netedit.leftClick(referencePosition, 250, 250)

# set invalid efficiency (> 1)
netedit.changeDefaultValue(5, "30")

# try to create chargingStation in mode "reference left"
netedit.leftClick(referencePosition, 250, 250)

# set valid efficiency
netedit.changeDefaultValue(5, "0.5")

# create chargingStation in mode "reference left"
netedit.leftClick(referencePosition, 250, 250)

# Check undo redo
netedit.undo(referencePosition, 1)
netedit.redo(referencePosition, 1)

# save additionals
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
