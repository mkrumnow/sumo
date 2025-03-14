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
# @author  Jakob Erdmann
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
import traci.constants as tc  # noqa
import sumolib  # noqa


def step():
    s = traci.simulation.getTime()
    traci.simulationStep()
    return s


def posToString(pos):
    if pos[0] == tc.INVALID_DOUBLE_VALUE:
        return "invalid"
    else:
        return str(pos)


def check(vehID):
    print("vehicles", traci.vehicle.getIDList())
    print("vehicle count", traci.vehicle.getIDCount())
    print("examining", vehID)
    print("speed", traci.vehicle.getSpeed(vehID))
    print("speedLat", traci.vehicle.getLateralSpeed(vehID))
    print("speed w/o traci", traci.vehicle.getSpeedWithoutTraCI(vehID))
    print("acceleration", traci.vehicle.getAcceleration(vehID))
    print("pos", posToString(traci.vehicle.getPosition(vehID)))
    print("pos3D", posToString(traci.vehicle.getPosition3D(vehID)))
    print("angle", traci.vehicle.getAngle(vehID))
    print("road", traci.vehicle.getRoadID(vehID))
    print("lane", traci.vehicle.getLaneID(vehID))
    print("laneIndex", traci.vehicle.getLaneIndex(vehID))
    print("type", traci.vehicle.getTypeID(vehID))
    print("routeID", traci.vehicle.getRouteID(vehID))
    print("routeIndex", traci.vehicle.getRouteIndex(vehID))
    print("route", traci.vehicle.getRoute(vehID))
    print("lanePos", traci.vehicle.getLanePosition(vehID))
    print("color", traci.vehicle.getColor(vehID))
    print("bestLanes", traci.vehicle.getBestLanes(vehID))
    print("CO2", traci.vehicle.getCO2Emission(vehID))
    print("CO", traci.vehicle.getCOEmission(vehID))
    print("HC", traci.vehicle.getHCEmission(vehID))
    print("PMx", traci.vehicle.getPMxEmission(vehID))
    print("NOx", traci.vehicle.getNOxEmission(vehID))
    print("fuel", traci.vehicle.getFuelConsumption(vehID))
    print("noise", traci.vehicle.getNoiseEmission(vehID))
    print("Elec", traci.vehicle.getElectricityConsumption(vehID))
    print("traveltime", traci.vehicle.getAdaptedTraveltime(vehID, 0, "1o"))
    print("effort", traci.vehicle.getEffort(vehID, 0, "1o"))
    print("route valid", traci.vehicle.isRouteValid(vehID))
    print("signals", traci.vehicle.getSignals(vehID))
    print("routingMode", traci.vehicle.getRoutingMode(vehID))
    print("length", traci.vehicle.getLength(vehID))
    print("maxSpeed", traci.vehicle.getMaxSpeed(vehID))
    print("speedFactor", traci.vehicle.getSpeedFactor(vehID))
    print("allowedSpeed", traci.vehicle.getAllowedSpeed(vehID))
    print("accel", traci.vehicle.getAccel(vehID))
    print("decel", traci.vehicle.getDecel(vehID))
    print("emergencyDecel", traci.vehicle.getEmergencyDecel(vehID))
    print("apparentDecel", traci.vehicle.getApparentDecel(vehID))
    print("actionStepLength", traci.vehicle.getActionStepLength(vehID))
    print("lastActionTime", traci.vehicle.getLastActionTime(vehID))
    print("imperfection", traci.vehicle.getImperfection(vehID))
    print("tau", traci.vehicle.getTau(vehID))
    print("vClass", traci.vehicle.getVehicleClass(vehID))
    print("emissionclass", traci.vehicle.getEmissionClass(vehID))
    print("shape", traci.vehicle.getShapeClass(vehID))
    print("MinGap", traci.vehicle.getMinGap(vehID))
    print("width", traci.vehicle.getWidth(vehID))
    print("height", traci.vehicle.getHeight(vehID))
    print("lcStrategic", traci.vehicle.getParameter(vehID, "laneChangeModel.lcStrategic"))
    print("lcCooperative", traci.vehicle.getParameter(vehID, "laneChangeModel.lcCooperative"))
    print("lcSpeedGain", traci.vehicle.getParameter(vehID, "laneChangeModel.lcSpeedGain"))
    print("maxSpeedLat", traci.vehicle.getMaxSpeedLat(vehID))
    print("minGapLat", traci.vehicle.getMinGapLat(vehID))
    print("lateralAlignment", traci.vehicle.getLateralAlignment(vehID))
    print("lanePosLat", traci.vehicle.getLateralLanePosition(vehID))
    print("person number", traci.vehicle.getPersonNumber(vehID))
    print("personCapacity", traci.vehicle.getPersonCapacity(vehID))
    print("waiting time", traci.vehicle.getWaitingTime(vehID))
    print("accumulated waiting time", traci.vehicle.getAccumulatedWaitingTime(vehID))
    print("driving dist", traci.vehicle.getDrivingDistance(vehID, "4fi", 2.))
    print("driving dist 2D", traci.vehicle.getDrivingDistance2D(vehID, 100., 100.))
    print("line", traci.vehicle.getLine(vehID))
    print("via", traci.vehicle.getVia(vehID))
    print("lane change state right", traci.vehicle.getLaneChangeState(vehID, -1))
    print("lane change state left", traci.vehicle.getLaneChangeState(vehID, 1))
    print("lane change able right", traci.vehicle.couldChangeLane(vehID, -1))
    print("lane change able left", traci.vehicle.couldChangeLane(vehID, 1))
    print("lane change wish right", traci.vehicle.wantsAndCouldChangeLane(vehID, -1))
    print("lane change wish left", traci.vehicle.wantsAndCouldChangeLane(vehID, 1))


def checkOffRoad(vehID):
    print(("veh", vehID,
           "speed", traci.vehicle.getSpeed(vehID),
           "pos", posToString(traci.vehicle.getPosition(vehID)),
           "pos3d", posToString(traci.vehicle.getPosition3D(vehID)),
           "angle", traci.vehicle.getAngle(vehID),
           "road", traci.vehicle.getRoadID(vehID),
           "lane", traci.vehicle.getLaneID(vehID),
           "lanePos", traci.vehicle.getLanePosition(vehID),
           "CO2", traci.vehicle.getCO2Emission(vehID)
           ))


traci.start([sumolib.checkBinary('sumo'), "-c", "sumo.sumocfg",
             '--ignore-route-errors',
             '--vehroute-output', 'vehroutes.xml',
             '--additional-files',
             'input_additional.add.xml,input_additional2.add.xml',
             "--default.speeddev", "0"])
for i in range(3):
    print("step", step())
vehID = "horiz"
check(vehID)
traci.vehicle.subscribe(vehID)
print(traci.vehicle.getSubscriptionResults(vehID))
traci.vehicle.setLateralAlignment(vehID, "arbitrary")
traci.vehicle.changeSublane(vehID, 0.2)
for i in range(3):
    print("step", step())
    print(traci.vehicle.getSubscriptionResults(vehID))
traci.vehicle.setLength(vehID, 1.0)
traci.vehicle.setMaxSpeed(vehID, 9.0)
traci.vehicle.setSpeedFactor(vehID, 1.1)
traci.vehicle.setAccel(vehID, 1.1)
traci.vehicle.setEmergencyDecel(vehID, 3.0)
traci.vehicle.setDecel(vehID, 5.1)
print("new decel=%s, automatically increased emergencyDecel=%s" % (
    traci.vehicle.getDecel(vehID), traci.vehicle.getEmergencyDecel(vehID)))
traci.vehicle.setEmergencyDecel(vehID, 5.2)
traci.vehicle.setApparentDecel(vehID, 5.3)
traci.vehicle.setActionStepLength(vehID, 1.0, False)
traci.vehicle.setImperfection(vehID, 0.1)
traci.vehicle.setTau(vehID, 1.1)
traci.vehicle.setVehicleClass(vehID, "bicycle")
traci.vehicle.setEmissionClass(vehID, "zero")
traci.vehicle.setShapeClass(vehID, "bicycle")
traci.vehicle.setMinGap(vehID, 1.1)
traci.vehicle.setWidth(vehID, 1.1)
traci.vehicle.setHeight(vehID, 1.6)
traci.vehicle.setMinGapLat(vehID, 0.5)
traci.vehicle.setMaxSpeedLat(vehID, 1.5)
traci.vehicle.setColor(vehID, (255, 0, 0, 255))
traci.vehicle.setLine(vehID, "S46")
traci.vehicle.setVia(vehID, ["3o", "4o"])
traci.vehicle.setAdaptedTraveltime(vehID, "1o", 55, 0, 1000)
traci.vehicle.setEffort(vehID, "1o", 54, 0, 1000)
if not traci.isLibsumo():
    # legacy API
    traci.vehicle.setAdaptedTraveltime(vehID, 0, 1000, "1o", 55)
    traci.vehicle.setEffort(vehID, 0, 1000, "1o", 54)
traci.vehicle.setParameter(vehID, "foo", "bar")
traci.vehicle.setParameter(vehID, "laneChangeModel.lcStrategic", "2.0")
traci.vehicle.setSignals(vehID, 12)
traci.vehicle.setRoutingMode(vehID, traci.constants.ROUTING_MODE_AGGREGATED)
traci.vehicle.setStop(vehID, "2fi", pos=55.0, laneIndex=0, duration=2, flags=1)
sys.stderr.flush()

check(vehID)
traci.vehicle.setAdaptedTraveltime(vehID, "1o")
traci.vehicle.setEffort(vehID, "1o")
print("reset traveltime", traci.vehicle.getAdaptedTraveltime(vehID, 0, "1o"))
print("reset effort", traci.vehicle.getEffort(vehID, 0, "1o"))
traci.vehicle.setAdaptedTraveltime(vehID, "1o", 23)
traci.vehicle.setEffort(vehID, "1o", 24)
print("set traveltime (default range)", traci.vehicle.getAdaptedTraveltime(vehID, 0, "1o"))
print("set effort (default range)", traci.vehicle.getEffort(vehID, 0, "1o"))
try:
    check("bla")
except traci.TraCIException as e:
    if traci.isLibsumo():
        print(e, file=sys.stderr)
        sys.stderr.flush()
    print("recovering from exception after asking for unknown vehicle")
traci.vehicle.addLegacy("1", "horizontal")
traci.vehicle.setStop("1", "2fi", pos=50.0, laneIndex=0, duration=1, flags=1)
check("1")
traci.vehicle.changeTarget("1", "4fi")
print("routeID", traci.vehicle.getRouteID(vehID))
print("route", traci.vehicle.getRoute(vehID))
print("step", step())
traci.vehicle.add("2", "horizontal", line="t")
print("getIDList", traci.vehicle.getIDList())
for i in range(6):
    print("step", step())
    if traci.vehicle.getSpeed("1") == 0:
        traci.vehicle.resume("1")
    print(traci.vehicle.getSubscriptionResults(vehID))
    print(traci.vehicle.getNextStops(vehID))
check("2")
print("nextTLS", traci.vehicle.getNextTLS("2"))
traci.vehicle.setSpeedMode(vehID, 0)  # disable all checks
traci.vehicle.setSpeed(vehID, 20)
print("speedmode", traci.vehicle.getSpeedMode(vehID))
print("lanechangemode", traci.vehicle.getLaneChangeMode(vehID))
print("slope", traci.vehicle.getSlope(vehID))
print("leader", traci.vehicle.getLeader("2"))
traci.vehicle.subscribeLeader("2")
for i in range(6):
    print("step", step())
    print(traci.vehicle.getSubscriptionResults("2"))
    print(traci.vehicle.getSubscriptionResults(vehID))
    print(traci.vehicle.getNextStops(vehID))
traci.vehicle.remove("1")
try:
    traci.vehicle.addLegacy("anotherOne", "horizontal", pos=-1)
except traci.TraCIException as e:
    if traci.isLibsumo():
        print(e, file=sys.stderr)
try:
    check("anotherOne")
except traci.TraCIException as e:
    if traci.isLibsumo():
        print(e, file=sys.stderr)
        sys.stderr.flush()
traci.vehicle.moveTo(vehID, "1o_0", 40)
print("step", step())
print(traci.vehicle.getSubscriptionResults(vehID))
print("step", step())
print(traci.vehicle.getSubscriptionResults(vehID))
print("speed before moveToXY", traci.vehicle.getSpeed(vehID))
traci.vehicle.moveToXY(vehID, "1o", 0, 482.49, 501.31, 0)
print("step", step())
print("speed after moveToVTD", traci.vehicle.getSpeed(vehID))
print(traci.vehicle.getSubscriptionResults(vehID))
print("step", step())
print(traci.vehicle.getSubscriptionResults(vehID))
# test different departure options
traci.vehicle.addLegacy("departInThePast", "horizontal", depart=5)
print("step", step())
print("vehicles", traci.vehicle.getIDList())
traci.vehicle.addLegacy("departInTheFuture", "horizontal", depart=30)
for i in range(9):
    print("step", step())
    print("vehicles", traci.vehicle.getIDList())
# XXX this doesn't work. see #1721
traci.vehicle.addLegacy("departTriggered", "horizontal", "triggered")
print("step", step())
print("vehicles", traci.vehicle.getIDList())
# test for setting a route with busstops
routeTestVeh = "routeTest"
traci.vehicle.addLegacy(routeTestVeh, "horizontal")
print("step", step())
print("vehicle '%s' routeID=%s" %
      (routeTestVeh, traci.vehicle.getRouteID(routeTestVeh)))
traci.vehicle.setRouteID(routeTestVeh, "withStop")
print("step", step())
print("vehicle '%s' routeID=%s" %
      (routeTestVeh, traci.vehicle.getRouteID(routeTestVeh)))
for i in range(14):
    print("step", step())
    print("vehicle", routeTestVeh,
          "lane", traci.vehicle.getLaneID(routeTestVeh),
          "lanePos", traci.vehicle.getLanePosition(routeTestVeh),
          "stopped", traci.vehicle.isStopped(routeTestVeh))
# test for adding a new vehicle with a route with busstop
routeTestVeh = "routeTest2"
traci.vehicle.addLegacy(routeTestVeh, "withStop")
for i in range(14):
    print("step", step())
    print("vehicle", routeTestVeh,
          "lane", traci.vehicle.getLaneID(routeTestVeh),
          "lanePos", traci.vehicle.getLanePosition(routeTestVeh),
          "stopped", traci.vehicle.isStopped(routeTestVeh))
# test for adding a veh and a busstop
busVeh = "bus"
traci.vehicle.addLegacy(busVeh, "horizontal")
traci.vehicle.setBusStop(busVeh, "busstop1", duration=2)
for i in range(14):
    print("step", step())
    print("vehicle", busVeh,
          "lane", traci.vehicle.getLaneID(busVeh),
          "lanePos", traci.vehicle.getLanePosition(busVeh),
          "stopped", traci.vehicle.isStopped(busVeh))
# test for adding a trip
traci.route.add("trip", ["3si"])
traci.vehicle.addLegacy("triptest", "trip")
traci.vehicle.setVia("triptest", ["2o"])
traci.vehicle.changeTarget("triptest", "4si")
print("triptest route:", traci.vehicle.getRoute("triptest"))
# test returned values of parking vehicle
parkingVeh = "parking"
traci.vehicle.addLegacy(parkingVeh, "horizontal")
traci.vehicle.setStop(parkingVeh, "2fi", pos=20.0, laneIndex=0, duration=10,
                      flags=traci.constants.STOP_PARKING)
print("nextStop:", traci.vehicle.getNextStops(parkingVeh))
for i in range(20):
    print("step", step())
    checkOffRoad(parkingVeh)
# test moveTo of parking vehicle
parkingVeh = "parking2"
traci.vehicle.addLegacy(parkingVeh, "horizontal")
traci.vehicle.setStop(parkingVeh, "2fi", pos=20.0, laneIndex=0, duration=10,
                      flags=traci.constants.STOP_PARKING)
for i in range(8):
    print("step", step())
traci.vehicle.moveTo(parkingVeh, "1o_0", 40)
for i in range(8):
    print("step", step())
    checkOffRoad(parkingVeh)
# test modifying a vehicle before insertion
offRoad = "offRoad"
traci.vehicle.addLegacy("blocker", "horizontal")
traci.vehicle.addLegacy(offRoad, "horizontal")
checkOffRoad(offRoad)
traci.vehicle.setSpeedFactor(offRoad, 1.1)
traci.vehicle.moveTo(offRoad, "1o_0", 40)
for i in range(3):
    print("step", step())
    checkOffRoad(offRoad)
# test modifying a teleporting vehicle
tele = "collider"
traci.vehicle.addLegacy("victim", "horizontal")
traci.vehicle.setStop("victim", "2fi", pos=5.0, laneIndex=0, duration=10)
# block the next lane to avoid instant insertion after teleport
traci.vehicle.addLegacy("block_2si", "horizontal")
traci.vehicle.moveTo("block_2si", "2si_1", 205)
traci.vehicle.setLength("block_2si", 200)
# cause collision on insertion
traci.vehicle.addLegacy(tele, "horizontal")
traci.vehicle.moveTo(tele, "2fi_0", 3)
for i in range(5):
    checkOffRoad(tele)
    print("step", step())
traci.vehicle.moveTo(tele, "1o_0", 40)
for i in range(3):
    checkOffRoad(tele)
    print("step", step())
# moveToXY to off-route edge
print("bus waiting time", traci.vehicle.getWaitingTime("bus"))
print("bus accumulated waiting time", traci.vehicle.getAccumulatedWaitingTime("bus"))
moved = "movedVeh"
traci.vehicle.addLegacy(moved, "vertical")
print("step", step())
traci.vehicle.moveToXY(moved, "dummy", 0, 448.99, 491.19, 90, 0)
print("step", step())
check(moved)
# add vehicle and route between taz
traci.vehicle.addLegacy("tazVeh", "withTaz2")
print("tazVeh edges", traci.vehicle.getRoute("tazVeh"))
print("step", step())
print("tazVeh pos=%s edges=%s" % (traci.vehicle.getLanePosition(
    "tazVeh"), traci.vehicle.getRoute("tazVeh")))
# add vehicle and attempt to route between disconnected edges
traci.vehicle.addLegacy("failVeh", "failRoute")
print("failVeh edges", traci.vehicle.getRoute("failVeh"))
for i in range(5):
    print("step", step())
    print("failVeh pos=%s edges=%s" % (traci.vehicle.getLanePosition("failVeh"),
                                       traci.vehicle.getRoute("failVeh")))
# add vehicle and reroute by travel time
traci.vehicle.addLegacy("rerouteTT", "horizontal")
traci.vehicle.rerouteTraveltime("rerouteTT")
# reroute again but travel times should only be updated once
traci.vehicle.rerouteTraveltime("rerouteTT")
traci.vehicle.addLegacy("rerouteEffort", "horizontal")
traci.vehicle.rerouteEffort("rerouteEffort")
print("step", step())
print(traci.vehicle.getSubscriptionResults(vehID))

parkingAreaVeh = "pav"
traci.vehicle.addLegacy(parkingAreaVeh, "horizontal")
traci.vehicle.setParkingAreaStop(parkingAreaVeh, "parkingArea1", duration=2)
for i in range(18):
    print("step", step())
    print("pav edge=%s pos=%s stopState=%s" %
          (traci.vehicle.getRoadID(parkingAreaVeh),
           traci.vehicle.getLanePosition(parkingAreaVeh),
           traci.vehicle.getStopState(parkingAreaVeh)
           ))

electricVeh = "elVeh"
traci.vehicle.addLegacy(electricVeh, "horizontal", typeID="electric")
traci.vehicle.setParameter(electricVeh, "device.battery.maximumBatteryCapacity", "40000")
traci.vehicle.setParameter(electricVeh, "device.battery.vehicleMass", "1024")
print("has battery device: %s" % traci.vehicle.getParameter(electricVeh, "has.battery.device"))
print("has vehroute device: %s" % traci.vehicle.getParameter(electricVeh, "has.vehroute.device"))
print("has rerouting device: %s" % traci.vehicle.getParameter(electricVeh, "has.rerouting.device"))
traci.vehicle.setParameter(electricVeh, "has.rerouting.device", "true")
print("has rerouting device: %s" % traci.vehicle.getParameter(electricVeh, "has.rerouting.device"))
print("routing period:", traci.vehicle.getParameter(electricVeh, "device.rerouting.period"))
traci.vehicle.setParameter(electricVeh, "device.rerouting.period", "45")
print("routing period:", traci.vehicle.getParameter(electricVeh, "device.rerouting.period"))
print("edge rerouting traveltime:", traci.vehicle.getParameter(electricVeh, "device.rerouting.edge:2si"))
traci.vehicle.setParameter(electricVeh, "device.rerouting.edge:2si", "123")
print("edge rerouting traveltime:", traci.vehicle.getParameter(electricVeh, "device.rerouting.edge:2si"))

traci.vehicle.setType(electricVeh, "long")
check(electricVeh)
traci.vehicle.setLength(electricVeh, 8)
traci.vehicle.setMaxSpeed(electricVeh, 10)
check(electricVeh)
traci.vehicle.setType(electricVeh, "long")
check(electricVeh)
traci.vehicle.setLength(electricVeh, 8)
traci.vehicle.setMaxSpeed(electricVeh, 10)
check(electricVeh)
traci.vehicle.setEmissionClass(electricVeh, "Energy/unknown")

try:
    print(traci.vehicle.getParameter(electricVeh, "device.foo.bar"))
except traci.TraCIException as e:
    if traci.isLibsumo():
        print(e, file=sys.stderr)
    print("recovering from exception after asking for unknown device")
try:
    print(traci.vehicle.getParameter(electricVeh, "device.battery.foobar"))
except traci.TraCIException as e:
    if traci.isLibsumo():
        print(e, file=sys.stderr)
    print("recovering from exception after asking for unknown device parameter")

traci.vehicle.subscribe(electricVeh, [tc.VAR_POSITION, tc.VAR_POSITION3D])
for i in range(10):
    step()
    print(('%s speed="%s" consumed="%s" charged="%s" cap="%s" maxCap="%s" station="%s" mass=%s emissionClass=%s ' +
           'electricityConsumption=%s') % (
        electricVeh,
        traci.vehicle.getSpeed(electricVeh),
        traci.vehicle.getParameter(electricVeh, "device.battery.energyConsumed"),
        traci.vehicle.getParameter(electricVeh, "device.battery.energyCharged"),
        traci.vehicle.getParameter(electricVeh, "device.battery.actualBatteryCapacity"),
        traci.vehicle.getParameter(electricVeh, "device.battery.maximumBatteryCapacity"),
        traci.vehicle.getParameter(electricVeh, "device.battery.chargingStationId"),
        traci.vehicle.getParameter(electricVeh, "device.battery.vehicleMass"),
        traci.vehicle.getEmissionClass(electricVeh),
        traci.vehicle.getElectricityConsumption(electricVeh),
    ))
    print(sorted(traci.vehicle.getSubscriptionResults(electricVeh).items()))
# test for adding a trip
traci.route.add("trip2", ["3si", "4si"])
traci.vehicle.addLegacy("triptest2", "trip2", typeID="reroutingType")
print("triptest route:", traci.vehicle.getRoute("triptest2"))
step()
print("triptest route:", traci.vehicle.getRoute("triptest2"))
# test for adding a vehicle without specifying the route
traci.vehicle.addLegacy("noRouteGiven", "")
step()
print("noRouteGiven routeID: %s edges: %s" % (
    traci.vehicle.getRouteID("noRouteGiven"),
    traci.vehicle.getRoute("noRouteGiven")))
# test for adding a vehicle with explicit departLane
traci.vehicle.add("vehDepartLane", "horizontal", departLane="0")
step()
# done
traci.close()
