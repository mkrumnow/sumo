/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEAdditionalHandler.h
/// @author  Pablo Alvarez Lopez
/// @date    Nov 2015
/// @version $Id$
///
// Builds additional objects for netedit
/****************************************************************************/
#ifndef GNEAdditionalHandler_h
#define GNEAdditionalHandler_h

// ===========================================================================
// included modules
// ===========================================================================

#include <config.h>

#include <utils/common/SUMOVehicleClass.h>
#include <utils/shapes/ShapeHandler.h>
#include <utils/xml/SUMOSAXAttributes.h>
#include <utils/xml/SUMOXMLDefinitions.h>


// ===========================================================================
// class declarations
// ===========================================================================

class GNEViewNet;
class GNEEdge;
class GNELane;
class GNEAdditional;
class GNEDemandElement;

// ===========================================================================
// class definitions
// ===========================================================================

/// @class GNEAdditionalHandler
/// @brief Builds additional objects for GNENet (busStops, chargingStations, detectors, etc..)
class GNEAdditionalHandler : public ShapeHandler {
public:

    /// @brief Stack used to save the last inserted element
    struct HierarchyInsertedAdditionals {

        /// @brief insert new element (called only in function myStartElement)
        void insertElement(SumoXMLTag tag);

        /// @brief commit element insertion (used to save last correct created element)
        void commitElementInsertion(GNEAdditional* additionalCreated);

        /// @brief pop last inserted element (used only in function myEndElement)
        void popElement();

        /// @brief retrieve additional parent correspond to current status of myInsertedElements
        GNEAdditional* retrieveAdditionalParent(GNEViewNet* viewNet, SumoXMLTag expectedTag) const;

        /// @brief return last additional inserted
        GNEAdditional* getLastInsertedAdditional() const;

    private:
        /// @brief vector used as stack
        std::vector<std::pair<SumoXMLTag, GNEAdditional*> > myInsertedElements;
    };

    /// @brief Constructor
    GNEAdditionalHandler(const std::string& file, GNEViewNet* viewNet, GNEAdditional* additionalParent = nullptr);

    /// @brief Destructor
    ~GNEAdditionalHandler();

    /// @name inherited from ShapeHandler
    /// @{
    /**@brief Called on the opening of a tag;
     * @param[in] element ID of the currently opened element
     * @param[in] attrs Attributes within the currently opened element
     * @exception ProcessError If something fails
     * @see GenericSAXHandler::myStartElement
     */
    void myStartElement(int element, const SUMOSAXAttributes& attrs);

    /** @brief Called when a closing tag occurs
     * @param[in] element ID of the currently opened element
     * @exception ProcessError If something fails
     * @see GenericSAXHandler::myEndElement
     */
    void myEndElement(int element);

    /**@brief get lane position
     * @param[in] poi poi ID
     * @param[in] laneID lane ID
     * @param[in] SlanePos position in the lane
     */
    Position getLanePos(const std::string& poiID, const std::string& laneID, double lanePos, double lanePosLat);
    /// @}

    /// @name building methods
    ///
    /// Called with parsed values, these methods build the additional.
    /// @{
    /**@brief Build additionals
     * @param[in] viewNet pointer to viewNet in wich additional will be created
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] tag tag of the additiona lto create
     * @param[in] attrs SUMOSAXAttributes with attributes
     * @param[in] HierarchyInsertedAdditionals pointer to HierarchyInsertedAdditionals (can be null)
     * @return true if was sucesfully created, false in other case
     */
    static bool buildAdditional(GNEViewNet* viewNet, bool allowUndoRedo, SumoXMLTag tag, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Builds a bus stop
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the bus stop
     * @param[in] lane The lane the bus stop is placed on
     * @param[in] startPos Begin position of the bus stop on the lane
     * @param[in] endPos End position of the bus stop on the lane
     * @param[in] name Name of busStop
     * @param[in] lines Names of the bus lines that halt on this bus stop
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the bus stop can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildBusStop(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, const std::string& startPos, const std::string& endPos, const std::string& name, const std::vector<std::string>& lines, int personCapacity, bool friendlyPosition, bool blockMovement);

    /**@brief Builds an Access
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] busStop GNEAdditional of this Access belongs
     * @param[in] lane The lane the Access is placed on
     * @param[in] pos position of the Access on the lane
     * @param[in[ length length of the Access
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the detector can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildAccess(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* busStop, GNELane* lane, const std::string& pos, const std::string& length, bool friendlyPos, bool blockMovement);

    /**@brief Builds a container stop
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the container stop
     * @param[in] lane The lane the container stop is placed on
     * @param[in] startPos Begin position of the container stop on the lane
     * @param[in] endPos End position of the container stop on the lane
     * @param[in] name Name of container stop
     * @param[in] lines Names of the bus lines that halt on this container stop
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the container stop can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildContainerStop(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, const std::string& startPos, const std::string& endPos, const std::string& name,
            const std::vector<std::string>& lines, bool friendlyPosition, bool blockMovement);

    /**@brief Builds a charging Station
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the charging Station
     * @param[in] lane The lane the charging Station is placed on
     * @param[in] startPos Begin position of the charging Station on the lane
     * @param[in] endPos End position of the charging Station on the lane
     * @param[in] name Name of charging station
     * @param[in] chargingPower power charged in every timeStep
     * @param[in] efficiency efficiency of the charge
     * @param[in] chargeInTransit enable or disable charge in transit
     * @param[in] chargeDelay delay in the charge
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the charging Station can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildChargingStation(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, const std::string& startPos, const std::string& endPos, const std::string& name,
            double chargingPower, double efficiency, bool chargeInTransit, SUMOTime chargeDelay, bool friendlyPosition, bool blockMovement);

    /**@brief Builds a Parking Area
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the Parking >Area
     * @param[in] lane The lane the Parking Area is placed on
     * @param[in] startPos Begin position of the Parking Area on the lane
     * @param[in] endPos End position of the Parking Area on the lane
     * @param[in] name Name of Parking Area
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] roadSideCapacity road side capacity of ParkingArea
     * @param[in] width ParkingArea's length
     * @param[in] length ParkingArea's length
     * @param[in] angle ParkingArea's angle
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the charging Station can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildParkingArea(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, const std::string& startPos, const std::string& endPos, const std::string& name,
                                           bool friendlyPosition, int roadSideCapacity, bool onRoad, double width, const std::string& length, double angle, bool blockMovement);

    /**@brief Builds a Parking Space
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] parkingAreaParent Pointer to Parking Area Parent
     * @param[in] pos ParkingSpace's X-Y position
     * @param[in] width ParkingArea's width
     * @param[in] length ParkingArea's length
     * @param[in] angle ParkingArea's angle
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the charging Station can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildParkingSpace(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* parkingAreaParent, Position pos, double width, double length, double angle, bool blockMovement);

    /**@brief Builds a induction loop detector (E1)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the detector
     * @param[in] lane The lane the detector is placed on
     * @param[in] pos position of the detector on the lane
     * @param[in] freq the aggregation period the values the detector collects shall be summed up.
     * @param[in] filename The path to the output file.
     * @param[in] vtypes list of vehicle types to be reported
     * @param[in] name E2 detector name
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the detector can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildDetectorE1(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, double pos, SUMOTime freq, const std::string& filename,
                                          const std::string& vehicleTypes, const std::string& name, bool friendlyPos, bool blockMovement);

    /**@brief Builds a single-lane Area Detector (E2)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the detector
     * @param[in] lane The lane the detector is placed on
     * @param[in] pos position of the detector on the lane
     * @param[in[ length length of the detector
     * @param[in] freq the aggregation period the values the detector collects shall be summed up.
     * @param[in] filename The path to the output file.
     * @param[in] vtypes list of vehicle types to be reported
     * @param[in] name E2 detector name
     * @param[in] timeThreshold The time-based threshold that describes how much time has to pass until a vehicle is recognized as halting
     * @param[in] speedThreshold The speed-based threshold that describes how slow a vehicle has to be to be recognized as halting
     * @param[in] jamThreshold The minimum distance to the next standing vehicle in order to make this vehicle count as a participant to the jam
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the detector can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildSingleLaneDetectorE2(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, double pos, double length, SUMOTime freq, const std::string& filename,
            const std::string& vehicleTypes, const std::string& name, SUMOTime timeThreshold, double speedThreshold, double jamThreshold, bool friendlyPos, bool blockMovement);

    /**@brief Builds a multi-lane Area Detector (E2)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the detector
     * @param[in] lanes The lanes the detector is placed on
     * @param[in] pos position of the detector on the first lane
     * @param[in] endPos position of the detector on the last lane
     * @param[in] freq the aggregation period the values the detector collects shall be summed up.
     * @param[in] filename The path to the output file.
     * @param[in] vtypes list of vehicle types to be reported
     * @param[in] name E2 detector name
     * @param[in] timeThreshold The time-based threshold that describes how much time has to pass until a vehicle is recognized as halting
     * @param[in] speedThreshold The speed-based threshold that describes how slow a vehicle has to be to be recognized as halting
     * @param[in] jamThreshold The minimum distance to the next standing vehicle in order to make this vehicle count as a participant to the jam
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the detector can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildMultiLaneDetectorE2(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, const std::vector<GNELane*>& lanes, double pos, double endPos, SUMOTime freq, const std::string& filename,
            const std::string& vehicleTypes, const std::string& name, SUMOTime timeThreshold, double speedThreshold, double jamThreshold, bool friendlyPos, bool blockMovement);

    /**@brief Builds a multi entry exit detector (E3)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the detector
     * @param[in] pos position of the detector in the map
     * @param[in] freq the aggregation period the values the detector collects shall be summed up.
     * @param[in] filename The path to the output file.
     * @param[in] vtypes list of vehicle types to be reported
     * @param[in] name E2 detector name
     * @param[in] timeThreshold The time-based threshold that describes how much time has to pass until a vehicle is recognized as halting
     * @param[in] speedThreshold The speed-based threshold that describes how slow a vehicle has to be to be recognized as halting
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the detector can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildDetectorE3(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, Position pos, SUMOTime freq, const std::string& filename, const std::string& vehicleTypes, const std::string& name,  SUMOTime timeThreshold, double speedThreshold, bool blockMovement);

    /**@brief Builds a entry detector (E3)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] E3Parent pointer to E3 detector parent
     * @param[in] lane The lane in which the entry detector is placed on
     * @param[in] pos position of the entry detector on the lane
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the entry detector can not be added to the net (invalid parent or lane)
     */
    static GNEAdditional* buildDetectorEntry(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* E3Parent, GNELane* lane, double pos, bool friendlyPos, bool blockMovement);

    /**@brief Builds a exit detector (E3)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] E3Parent pointer to E3 detector parent
     * @param[in] lane The lane in which the exit detector is placed on
     * @param[in] pos position of the exit detector on the lane
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the exit detector can not be added to the net (invalid parent or lane
     */
    static GNEAdditional* buildDetectorExit(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* E3Parent, GNELane* lane, double pos, bool friendlyPos, bool blockMovement);

    /**@brief Builds a Instant Induction Loop Detector (E1Instant)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the detector
     * @param[in] lane The lane the detector is placed on
     * @param[in] pos position of the detector on the lane
     * @param[in] filename The path to the output file.
     * @param[in] name E2 detector name
     * @param[in] vtypes list of vehicle types to be reported
     * @param[in] friendlyPos enable or disable friendly position
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the detector can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildDetectorE1Instant(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, double pos, const std::string& filename, const std::string& vehicleTypes, const std::string& name, bool friendlyPos, bool blockMovement);

    /**@brief builds a microscopic calibrator over a lane
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the calibrator
     * @param[in] lane The lane the calibrator is placed at
     * @param[in] pos The position on the edge the calibrator lies at
     * @param[in] name Calibrator name
     * @param[in] outfile te file in which write results
     * @return true if was sucesfully created, false in other case
     * @todo Is the position correct/needed
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the entry detector can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildCalibrator(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNELane* lane, double pos, const std::string& name, const std::string& outfile, SUMOTime freq, const std::string& routeprobe);

    /**@brief builds a microscopic calibrator over an edge
    * @param[in] viewNet viewNet in which element will be inserted
    * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
    * @param[in] id The id of the calibrator
    * @param[in] edge The edge the calibrator is placed at
    * @param[in] pos The position on the edge the calibrator lies at
    * @param[in] name Calibrator name
    * @param[in] outfile te file in which write results
    * @param[in] routeProbe route probe vinculated with this calibrator
    * @return true if was sucesfully created, false in other case
    * @todo Is the position correct/needed
    * @return true if was sucesfully created, false in other case
    * @exception InvalidArgument If the entry detector can not be added to the net (is duplicate)
    */
    static GNEAdditional* buildCalibrator(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNEEdge* edge, double pos, const std::string& name, const std::string& outfile, SUMOTime freq, const std::string& routeprobe);

    /**@brief builds a calibrator flow
    * @param[in] viewNet viewNet in which element will be inserted
    * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
    * @param[in] type The id of the vehicle's flow type to use for this vehicle's flow.
    * @param[in] route The id of the route the vehicle's flow shall drive along
    * @param[in] vehsPerHour number of vehicles per hour, equally spaced (not together with period or probability)
    * @param[in] speed 	The speed with which the vehicles shall enter the network. NOTE: this attribute is exclusive of CalibratorFlows!
    * @param[in] color This vehicle's flow's color
    * @param[in] departLane The lane on which the vehicle's flow shall be inserted; see #departLane. default: "first"
    * @param[in] departPos The position at which the vehicle's flow shall enter the net; see #departPos. default: "base"
    * @param[in] departSpeed The speed with which the vehicle's flow shall enter the network; see #departSpeed. default: 0
    * @param[in] arrivalLane The lane at which the vehicle's flow shall leave the network; see #arrivalLane. default: "current"
    * @param[in] arrivalPos The position at which the vehicle's flow shall leave the network; see #arrivalPos. default: "max"
    * @param[in] arrivalSpeed The speed with which the vehicle's flow shall leave the network; see #arrivalSpeed. default: "current"
    * @param[in] line A string specifying the id of a public transport line which can be used when specifying person rides
    * @param[in] personNumber The number of occupied seats when the vehicle's flow is inserted. default: 0
    * @param[in] containerNumber The number of occupied container places when the vehicle's flow is inserted. default: 0
    * @param[in] reroute List of intermediate edges that shall be passed on rerouting.
    * @param[in] via List of intermediate edges that shall be passed on rerouting.
    * @param[in] departPosLat The lateral position on the departure lane at which the vehicle's flow shall enter the net; see Simulation/SublaneModel. default: "center"
    * @param[in] arrivalPosLat The lateral position on the arrival lane at which the vehicle's flow shall arrive; see Simulation/SublaneModel. by default the vehicle's flow does not care about lateral arrival position
    * @param[in] begin first vehicle's flow departure time
    * @param[in] end end of departure interval (if undefined, defaults to 24 hours)
    * @return true if was sucesfully created, false in other case
    */
    static GNEAdditional* buildCalibratorFlow(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* calibratorParent, GNEDemandElement* route, GNEDemandElement* vType,
            const std::string& vehsPerHour, const std::string& speed, const RGBColor& color, const std::string& departLane, const std::string& departPos,
            const std::string& departSpeed, const std::string& arrivalLane, const std::string& arrivalPos, const std::string& arrivalSpeed,
            const std::string& line, int personNumber, int containerNumber, bool reroute, const std::string& departPosLat,
            const std::string& arrivalPosLat, SUMOTime begin, SUMOTime end);

    /**@brief builds a rerouter
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the rerouter
     * @param[in] pos position of the rerouter in the map
     * @param[in] edges The edges the rerouter is placed at
     * @param[in] prob The probability the rerouter reoutes vehicles with
     * @param[in] name Calibrator name
     * @param[in] file The file to read the reroute definitions from
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     */
    static GNEAdditional* buildRerouter(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, Position pos, const std::vector<GNEEdge*>& edges, double prob, const std::string& name, const std::string& file, bool off, SUMOTime timeThreshold, const std::string& vTypes, bool blockMovement);

    /**@brief builds a rerouter interval
    * @param[in] viewNet viewNet in which element will be inserted
    * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
    * @param[in] rerouterParent rerouter in which interval is placed
    * @param[in] begin begin of interval
    * @param[in] end end of interval
    * @return true if was sucesfully created, false in other case
    */
    static GNEAdditional* buildRerouterInterval(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* rerouterParent, SUMOTime begin, SUMOTime end);

    /**
    DOCUMENTAR
    */
    static GNEAdditional* buildClosingLaneReroute(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* rerouterIntervalParent, GNELane* closedLane, SVCPermissions permissions);

    /**
    DOCUMENTAR
    */
    static GNEAdditional* buildClosingReroute(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* rerouterIntervalParent, GNEEdge* closedEdge, SVCPermissions permissions);

    /**
    DOCUMENTAR
    */
    static GNEAdditional* builDestProbReroute(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* rerouterIntervalParent, GNEEdge* newEdgeDestination, double probability);

    /**
    DOCUMENTAR
    */
    static GNEAdditional* builParkingAreaReroute(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* rerouterIntervalParent, GNEAdditional* newParkignArea, double probability, bool visible);

    /**
    DOCUMENTAR
    */
    static GNEAdditional* buildRouteProbReroute(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* rerouterIntervalParent, const std::string& newRouteId, double probability);

    /**@brief builds a Route probe
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the routeprobe
     * @param[in] edge The edges the routeprobe is placed at
     * @param[in] freq the aggregation period the values the routeprobe collects shall be summed up.
     * @param[in] name Calibrator name
     * @param[in] file The file to read the routeprobe definitions from
     * @param[in] begin The time at which to start generating output
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the Route Probe can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildRouteProbe(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, GNEEdge* edge, const std::string& freq, const std::string& name, const std::string& file, SUMOTime begin);

    /**@brief Builds a VariableSpeedSign (lane speed additional)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id The id of the lane speed additional
     * @param[in] destLanes List of lanes affected by this speed additional
     * @param[in] name Calibrator name
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception InvalidArgument If the VariableSpeedSign can not be added to the net (is duplicate)
     */
    static GNEAdditional* buildVariableSpeedSign(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, Position pos, const std::vector<GNELane*>& destLanes, const std::string& name, bool blockMovement);

    /**@brief Builds a VariableSpeedSign Step
    * @param[in] viewNet viewNet in which element will be inserted
    * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
    * @param[in] VSSParent Variable Speed Sign parent
    * @param[in] time step's time
    * @param[in] speed step's speed
    * @return true if was sucesfully created, false in other case
    * @exception InvalidArgument If the Variable Speed Sign Step can not be added to the net (is duplicate)
    */
    static GNEAdditional* buildVariableSpeedSignStep(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* VSSParent, double time, double speed);

    /**@brief Builds a vaporizer (lane speed additional)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] edge edge in which this vaporizer is placed
     * @param[in] startTime time in which this vaporizer start
     * @param[in] endTime time in which this vaporizer ends
     * @param[in] name Vaporizer name
     * @return true if was sucesfully created, false in other case
     * @exception ProcessError If the XML definition file is errornous
     */
    static GNEAdditional* buildVaporizer(GNEViewNet* viewNet, bool allowUndoRedo, GNEEdge* edge, SUMOTime start, SUMOTime endTime, const std::string& name);

    /**@brief Builds a TAZ (Traffic Assignment Zone)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] id TAZ ID
     * @param[in] shape TAZ shape
     * @param[in] edges list of edges (note: This will create GNETAZSourceSinks/Sinks with default values)
     * @param[in] blockMovemet enable or disable block movement
     * @return true if was sucesfully created, false in other case
     * @exception ProcessError If the XML definition file is errornous
     */
    static GNEAdditional* buildTAZ(GNEViewNet* viewNet, bool allowUndoRedo, const std::string& id, const PositionVector& shape, const RGBColor& color, const std::vector<GNEEdge*>& edges, bool blockMovement);

    /**@brief Builds a TAZSource (Traffic Assignment Zone)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] TAZ Traffic Assignment Zone in which this TAZSource is palced
     * @param[in] ege edge in which TAZSource is placed
     * @param[in] departWeight depart weight of TAZSource
     * @return true if was sucesfully created, false in other case
     * @exception ProcessError If the XML definition file is errornous
     */
    static GNEAdditional* buildTAZSource(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* TAZ, GNEEdge* edge, double departWeight);

    /**@brief Builds a TAZSink (Traffic Assignment Zone)
     * @param[in] viewNet viewNet in which element will be inserted
     * @param[in] allowUndoRedo enable or disable remove created additional with ctrl + Z / ctrl + Y
     * @param[in] TAZ Traffic Assignment Zone in which this TAZSink is palced
     * @param[in] ege edge in which TAZSink is placed
     * @param[in] arrivalWeight arrival weight of TAZSink
     * @return true if was sucesfully created, false in other case
     * @exception ProcessError If the XML definition file is errornous
     */
    static GNEAdditional* buildTAZSink(GNEViewNet* viewNet, bool allowUndoRedo, GNEAdditional* TAZ, GNEEdge* edge, double arrivalWeight);

    /**@brief extracts the position, checks whether it shall be mirrored and checks whether it is within the lane.
     * @param[in] pos position of additional over lane
     * @param[in] lane The lane the position shall be valid for
     * @param[in] friendlyPos flag to indicate if friendlyPos is enabled
     * @param[in] additionalID ID of additional
     * @return The position on the lane
     */
    double getPosition(double pos, GNELane& lane, bool friendlyPos, const std::string& additionalID);

    /**@brief check if the position of a detector over a lane is valid
    * @param[in] pos pos position of detector
    * @param[in] laneLength Length of the lane
    * @param[in] friendlyPos Attribute of detector
    * @return true if the detector position is valid, false in otherweise
    */
    static bool checkAndFixDetectorPosition(double& pos, const double laneLength, const bool friendlyPos);

    /**@brief check if the position of a detector over a lane is valid
    * @param[in] startPos Start position of detector
    * @param[in] length length of detector
    * @param[in] laneLength Length of the lane
    * @param[in] friendlyPos Attribute of detector
    * @return true if the detector position is valid, false in otherweise
    */
    static bool fixE2DetectorPosition(double& pos, double& length, const double laneLength, const bool friendlyPos);

    /// @brief check if a GNEAccess can be created in a certain Edge
    static bool accessCanBeCreated(GNEAdditional* busStopParent, GNEEdge& edge);

    /// @brief check if an overlapping is produced in rerouter if a interval with certain begin and end is inserted
    static bool checkOverlappingRerouterIntervals(GNEAdditional* rerouter, SUMOTime newBegin, SUMOTime newEnd);

protected:
    /// @name parsing methods
    ///
    /// These methods parse the attributes for each of the described additional
    /// and call the according methods to build the additional
    /// @{
    /**@brief Builds a Vaporizer
     * @param[in] attrs SAX-attributes which define the vaporizer
     */
    static bool parseAndBuildVaporizer(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Builds a TAZ
     * @param[in] attrs SAX-attributes which define the vaporizer
     */
    static bool parseAndBuildTAZ(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Builds a TAZ Source
     * @param[in] attrs SAX-attributes which define the vaporizer
     */
    static bool parseAndBuildTAZSource(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Builds a TAZ Sink
     * @param[in] attrs SAX-attributes which define the vaporizer
     */
    static bool parseAndBuildTAZSink(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Variable Speed Signal (GNEViewNet* viewNet, bool allowUndoRedo, lane speed additional)
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildVariableSpeedSign(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Variable Speed Signal Step
    * @param[in] attrs SAX-attributes which define the additional
    */
    static bool parseAndBuildVariableSpeedSignStep(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a rerouter
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildRerouter(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Rerouter Interval
    * @param[in] attrs SAX-attributes which define the additional
    */
    static bool parseAndBuildRerouterInterval(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Closing Lane reroute
    * @param[in] attrs SAX-attributes which define the additional
    */
    static bool parseAndBuildRerouterClosingLaneReroute(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Closing Reroute
    * @param[in] attrs SAX-attributes which define the additional
    */
    static bool parseAndBuildRerouterClosingReroute(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Destiny Prob Reroute
    * @param[in] attrs SAX-attributes which define the additional
    */
    static bool parseAndBuildRerouterDestProbReroute(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a parkingAreaReroute
    * @param[in] attrs SAX-attributes which define the additional
    */
    static bool parseAndBuildRerouterParkingAreaReroute(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Route Prob Reroute
    * @param[in] attrs SAX-attributes which define the additional
    */
    static bool parseAndBuildRerouterRouteProbReroute(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a bus stop
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildBusStop(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses values and adds access to the current bus stop
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildAccess(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a container stop
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildContainerStop(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a charging station
     * @param[in] attrs SAXattributes which define the additional
     */
    static bool parseAndBuildChargingStation(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a parking area
     * @param[in] attrs SAXattributes which define the additional
     */
    static bool parseAndBuildParkingArea(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a parking space
     * @param[in] attrs SAXattributes which define the additional
     * @param[in] tag of the additional
     */
    static bool parseAndBuildParkingSpace(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a mesoscopic or microscopic calibrator
     * @param[in] attrs SAX-attributes which define the additional
     * @param[in] tag of the additional
     */
    static bool parseAndBuildCalibrator(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a induction loop detector (GNEViewNet* viewNet, bool allowUndoRedo, E1)
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildDetectorE1(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a lane area detector (GNEViewNet* viewNet, bool allowUndoRedo, E2)
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildDetectorE2(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a multi entry exit detector (GNEViewNet* viewNet, bool allowUndoRedo, E3)
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildDetectorE3(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Entry detector
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildDetectorEntry(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Exit detector
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildDetectorExit(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds a Instant induction loop detector (GNEViewNet* viewNet, bool allowUndoRedo, E1Instant)
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildDetectorE1Instant(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses his values and builds routeProbe
     * @param[in] attrs SAX-attributes which define the additional
     */
    static bool parseAndBuildRouteProbe(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /**@brief Parses flow values of Calibrators
     * @param[in] attrs SAX-attributes which define the flows
     */
    static bool parseAndBuildCalibratorFlow(GNEViewNet* viewNet, bool allowUndoRedo, const SUMOSAXAttributes& attrs, HierarchyInsertedAdditionals* insertedAdditionals);

    /// @}

private:
    /**@brief Parses his values and builds a Poly
     * @param[in] attrs SAX-attributes which define the Poly
     */
    void parseAndBuildPoly(const SUMOSAXAttributes& attrs);

    /**@brief Parses his values and builds a POI
     * @param[in] attrs SAX-attributes which define the shape
     */
    void parseAndBuildPOI(const SUMOSAXAttributes& attrs);

    /**@brief Parse generic parameter and insert it in the last created additional
     * @param[in] attrs SAX-attributes which define the generic parameter
     */
    void parseGenericParameter(const SUMOSAXAttributes& attrs);

    /// @brief pointer to View's Net
    GNEViewNet* myViewNet;

    /// @brief HierarchyInsertedAdditionals used for insert children
    HierarchyInsertedAdditionals myHierarchyInsertedAdditionals;

    /// @brief invalidate copy constructor
    GNEAdditionalHandler(const GNEAdditionalHandler& s) = delete;

    /// @brief invalidate assignment operator
    GNEAdditionalHandler& operator=(const GNEAdditionalHandler& s) = delete;
};


#endif
