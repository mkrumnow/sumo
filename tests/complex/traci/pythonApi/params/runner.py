#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2008-2019 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
# SPDX-License-Identifier: EPL-2.0

# @file    runner.py
# @author  Jakob Erdmann
# @date    2015-02-06
# @version $Id$


from __future__ import print_function
from __future__ import absolute_import
import os
import sys

SUMO_HOME = os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..")
sys.path.append(os.path.join(os.environ.get("SUMO_HOME", SUMO_HOME), "tools"))
import traci  # noqa
import sumolib  # noqa


def step():
    s = traci.simulation.getCurrentTime() / 1000
    traci.simulationStep()
    return s


def setGetParam(objectType, object, objectID):
    print(objectType, 'foo="%s"' % object.getParameter(objectID, "foo"))
    object.setParameter(objectID, "foo", "42")
    print(objectType, 'foo="%s"' % object.getParameter(objectID, "foo"))


traci.start([sumolib.checkBinary('sumo'), "-c", "sumo.sumocfg"])
print("step", step())

# XXX test PoI, Polygon
objects = [
    ("vehicle", traci.vehicle, "veh0"),
    ("person", traci.person, "ped0"),
    ("edge", traci.edge, "1o"),
    ("lane", traci.lane, "1o_0"),
    ("vType", traci.vehicletype, "pType"),
    ("route", traci.route, "horizontal"),
    ("trafficlight", traci.trafficlight, "0"),
]

[setGetParam(*x) for x in objects]
print("step", step())

traci.close()
