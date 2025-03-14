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

# select Entry detector
netedit.changeElement("detExit")

# Create Entry detector E3
netedit.leftClick(referencePosition, 250, 100)
netedit.leftClick(referencePosition, 180, 200)

# select Exit detector and create it
netedit.changeElement("detEntry")
netedit.leftClick(referencePosition, 250, 100)
netedit.leftClick(referencePosition, 400, 200)

# go to inspect mode
netedit.inspectMode()

# inspect Entry
netedit.leftClick(referencePosition, 200, 200)

# Change generic parameters with a dummy value
netedit.modifyAttribute(4, "dummyGenericParameters", True)

# Change generic parameters with a invalid format
netedit.modifyAttribute(4, "key1|key2|key3", True)

# Change generic parameters with a valid value
netedit.modifyAttribute(4, "key1=value1|key2=value2|key3=value3", True)

# Change generic parameters with a valid value (empty values)
netedit.modifyAttribute(4, "key1=|key2=|key3=", True)

# Change generic parameters with a valid value (all empty)
netedit.modifyAttribute(4, "", True)

# Change generic parameters with an invalid value (duplicated)
netedit.modifyAttribute(4, "key1duplicated=value1|key1duplicated=value2|key3=value3", True)

# Change generic parameters with a valid value
netedit.modifyAttribute(4, "key1=valueDuplicated|key2=valueDuplicated|key3=valueDuplicated", True)

# Change generic parameters with an invalid value (invalid key characters)
netedit.modifyAttribute(4, "keyInvalid.;%>%$$=value1|key2=value2|key3=value3", True)

# Change generic parameters with a invalid value (invalid value characters)
netedit.modifyAttribute(4, "key1=valueInvalid%;%$<>$$%|key2=value2|key3=value3", True)

# Change generic parameters with a valid value
netedit.modifyAttribute(4, "keyFinal1=value1|keyFinal2=value2|keyFinal3=value3", True)

# Check undo redo
netedit.undo(referencePosition, 10)
netedit.redo(referencePosition, 10)

# save additionals
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
