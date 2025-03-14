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
# @author  Michael Behrisch
# @author  Daniel Krajzewicz
# @date    2011-03-04
# @version $Id$

from __future__ import print_function
from __future__ import absolute_import
import os
import sys

SUMO_HOME = os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..")
sys.path.append(os.path.join(os.environ.get("SUMO_HOME", SUMO_HOME), "tools"))
import traci  # noqa
import sumolib  # noqa

traci.start([sumolib.checkBinary('sumo'), "-c", "sumo.sumocfg"])
for step in range(3):
    print("step", step)
    traci.simulationStep()
print("lanes", traci.lane.getIDList())
print("lane count", traci.lane.getIDCount())
laneID = "2fi_0"
print("examining", laneID)
print("length", traci.lane.getLength(laneID))
print("maxSpeed", traci.lane.getMaxSpeed(laneID))
print("width", traci.lane.getWidth(laneID))
print("allowed", traci.lane.getAllowed(laneID))
print("disallowed", traci.lane.getDisallowed(laneID))
print("linkNum", traci.lane.getLinkNumber(laneID))
print("links", traci.lane.getLinks(laneID))
if not traci.isLibsumo():
    print("linksExtended", traci.lane.getLinks(laneID, extended=True))
print("shape", traci.lane.getShape(laneID))
print("edge", traci.lane.getEdgeID(laneID))
print("CO2", traci.lane.getCO2Emission(laneID))
print("CO", traci.lane.getCOEmission(laneID))
print("HC", traci.lane.getHCEmission(laneID))
print("PMx", traci.lane.getPMxEmission(laneID))
print("NOx", traci.lane.getNOxEmission(laneID))
print("Fuel", traci.lane.getFuelConsumption(laneID))
print("Noise", traci.lane.getNoiseEmission(laneID))
print("Elec", traci.lane.getElectricityConsumption(laneID))
print("meanSpeed", traci.lane.getLastStepMeanSpeed(laneID))
print("occupancy", traci.lane.getLastStepOccupancy(laneID))
print("lastLength", traci.lane.getLastStepLength(laneID))
print("traveltime", traci.lane.getTraveltime(laneID))
print("numVeh", traci.lane.getLastStepVehicleNumber(laneID))
print("haltVeh", traci.lane.getLastStepHaltingNumber(laneID))
print("vehIds", traci.lane.getLastStepVehicleIDs(laneID))
print("waiting time", traci.lane.getWaitingTime(laneID))

traci.lane.setAllowed(laneID, ["taxi"])
print("after setAllowed", traci.lane.getAllowed(
    laneID), traci.lane.getDisallowed(laneID))
traci.lane.setDisallowed(laneID, ["bus"])
print("after setDisallowed", traci.lane.getAllowed(
    laneID), traci.lane.getDisallowed(laneID))
traci.lane.setMaxSpeed(laneID, 42.)
print("after setMaxSpeed", traci.lane.getMaxSpeed(laneID))
traci.lane.setLength(laneID, 123.)
print("after setLength", traci.lane.getLength(laneID))

print("foes", traci.lane.getFoes("1si_0", "3o_0"))

traci.lane.subscribe(laneID)
print(traci.lane.getSubscriptionResults(laneID))
for step in range(3, 6):
    print("step", step)
    traci.simulationStep()
    print(traci.lane.getSubscriptionResults(laneID))
traci.close()
