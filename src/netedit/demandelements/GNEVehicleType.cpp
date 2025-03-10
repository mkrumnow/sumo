/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEVehicleType.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Jan 2019
/// @version $Id$
///
// Definition of Vehicle Types in NETEDIT
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================

#include <netedit/GNENet.h>
#include <netedit/GNEViewNet.h>
#include <netedit/GNEUndoList.h>
#include <netedit/GNEViewParent.h>
#include <netedit/changes/GNEChange_Attribute.h>
#include <netedit/frames/GNESelectorFrame.h>
#include <utils/common/StringTokenizer.h>
#include <utils/emissions/PollutantsInterface.h>
#include <utils/gui/div/GUIGlobalSelection.h>

#include "GNEVehicleType.h"


// ===========================================================================
// member method definitions
// ===========================================================================


GNEVehicleType::GNEVehicleType(GNEViewNet* viewNet, const std::string& vTypeID, const SUMOVehicleClass& defaultVClass, SumoXMLTag tag) :
    GNEDemandElement(vTypeID, viewNet, GLO_VTYPE, tag, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}),
                 SUMOVTypeParameter(vTypeID),
                 myDefaultVehicleType(true),
myDefaultVehicleTypeModified(false) {
    // set default vehicle class
    vehicleClass = defaultVClass;
    parametersSet |= VTYPEPARS_VEHICLECLASS_SET;
    // init Rail Visualization Parameters
    initRailVisualizationParameters();
}


GNEVehicleType::GNEVehicleType(GNEViewNet* viewNet, const SUMOVTypeParameter& vTypeParameter, SumoXMLTag tag) :
    GNEDemandElement(vTypeParameter.id, viewNet, GLO_VTYPE, tag, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}),
SUMOVTypeParameter(vTypeParameter),
myDefaultVehicleType(false),
myDefaultVehicleTypeModified(false) {
    // if we're creating a Person Type, set manually VClass
    if (tag == SUMO_TAG_PTYPE) {
        vehicleClass = SVC_PEDESTRIAN;
        parametersSet |= VTYPEPARS_VEHICLECLASS_SET;
    }
    // init Rail Visualization Parameters
    initRailVisualizationParameters();
}


GNEVehicleType::GNEVehicleType(GNEViewNet* viewNet, const std::string& vTypeID, GNEVehicleType* vTypeOriginal) :
    GNEDemandElement(vTypeID, viewNet, GLO_VTYPE, vTypeOriginal->getTagProperty().getTag(), {}, {}, {}, {}, {}, {}, {}, {}, {}, {}),
SUMOVTypeParameter(*vTypeOriginal),
myDefaultVehicleType(false),
myDefaultVehicleTypeModified(false) {
    // change manually the ID (to avoid to use the ID of vTypeOriginal)
    id = vTypeID;
    // init Rail Visualization Parameters
    initRailVisualizationParameters();
}


GNEVehicleType::~GNEVehicleType() {}


void
GNEVehicleType::writeDemandElement(OutputDevice& device) const {
    // only write default vehicle types if it was modified
    if (myDefaultVehicleType) {
        if (myDefaultVehicleTypeModified) {
            write(device);
        }
    } else {
        write(device);
    }
}


GNEEdge*
GNEVehicleType::getFromEdge() const {
    return nullptr;
}


GNEEdge*
GNEVehicleType::getToEdge() const {
    return nullptr;
}


SUMOVehicleClass
GNEVehicleType::getVClass() const {
    return vehicleClass;
}


const RGBColor&
GNEVehicleType::getColor() const {
    return color;
}


void
GNEVehicleType::compute() {
    // Nothing to compute
}


void
GNEVehicleType::startGeometryMoving() {
    // VTypes cannot be moved
}


void
GNEVehicleType::endGeometryMoving() {
    // VTypes cannot be moved
}


void
GNEVehicleType::moveGeometry(const Position&) {
    // VTypes cannot be moved
}


void
GNEVehicleType::commitGeometryMoving(GNEUndoList*) {
    // VTypes cannot be moved
}


void
GNEVehicleType::updateGeometry() {
    // Currently this additional doesn't own a Geometry
}


Position
GNEVehicleType::getPositionInView() const {
    return Position();
}


std::string
GNEVehicleType::getParentName() const {
    return myViewNet->getNet()->getMicrosimID();
}


Boundary
GNEVehicleType::getCenteringBoundary() const {
    // Vehicle Types doesn't have boundaries
    return Boundary(-0.1, -0.1, 0.1, 0.1);
}


void
GNEVehicleType::drawGL(const GUIVisualizationSettings&) const {
    // Vehicle Types aren't draw
}


void
GNEVehicleType::selectAttributeCarrier(bool changeFlag) {
    if (!myViewNet) {
        throw ProcessError("ViewNet cannot be nullptr");
    } else {
        gSelected.select(getGlID());
        // add object of list into selected objects
        myViewNet->getViewParent()->getSelectorFrame()->getLockGLObjectTypes()->addedLockedObject(GLO_VTYPE);
        if (changeFlag) {
            mySelected = true;
        }
    }
}


void
GNEVehicleType::unselectAttributeCarrier(bool changeFlag) {
    if (!myViewNet) {
        throw ProcessError("ViewNet cannot be nullptr");
    } else {
        gSelected.deselect(getGlID());
        // remove object of list of selected objects
        myViewNet->getViewParent()->getSelectorFrame()->getLockGLObjectTypes()->removeLockedObject(GLO_VTYPE);
        if (changeFlag) {
            mySelected = false;

        }
    }
}


std::string
GNEVehicleType::getAttribute(SumoXMLAttr key) const {
    // obtain default values depending of vehicle class
    VClassDefaultValues defaultValues(vehicleClass);
    switch (key) {
        case SUMO_ATTR_ID:
            return getDemandElementID();
        // CFM Attributes
        case SUMO_ATTR_ACCEL:
        case SUMO_ATTR_DECEL:
        case SUMO_ATTR_APPARENTDECEL:
        case SUMO_ATTR_EMERGENCYDECEL:
        case SUMO_ATTR_SIGMA:
        case SUMO_ATTR_TAU:
            // this CFM has default values
            return getCFParamString(key, myTagProperty.getDefaultValue(key));
        // JM Attributes
        case SUMO_ATTR_JM_CROSSING_GAP:
        case SUMO_ATTR_JM_IGNORE_KEEPCLEAR_TIME:
        case SUMO_ATTR_JM_DRIVE_AFTER_YELLOW_TIME:
        case SUMO_ATTR_JM_DRIVE_AFTER_RED_TIME:
        case SUMO_ATTR_JM_DRIVE_RED_SPEED:
        case SUMO_ATTR_JM_IGNORE_FOE_PROB:
        case SUMO_ATTR_JM_IGNORE_FOE_SPEED:
        case SUMO_ATTR_JM_SIGMA_MINOR:
        case SUMO_ATTR_JM_TIMEGAP_MINOR:
            // this JM has default values
            return getJMParamString(key, myTagProperty.getDefaultValue(key));
        case SUMO_ATTR_IMPATIENCE:
            if (wasSet(VTYPEPARS_IMPATIENCE_SET)) {
                return toString(impatience);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_IMPATIENCE);
            }
        // LCM Attributes
        case SUMO_ATTR_LCA_STRATEGIC_PARAM:
        case SUMO_ATTR_LCA_COOPERATIVE_PARAM:
        case SUMO_ATTR_LCA_SPEEDGAIN_PARAM:
        case SUMO_ATTR_LCA_KEEPRIGHT_PARAM:
        case SUMO_ATTR_LCA_SUBLANE_PARAM:
        case SUMO_ATTR_LCA_OPPOSITE_PARAM:
        case SUMO_ATTR_LCA_PUSHY:
        case SUMO_ATTR_LCA_PUSHYGAP:
        case SUMO_ATTR_LCA_ASSERTIVE:
        case SUMO_ATTR_LCA_IMPATIENCE:
        case SUMO_ATTR_LCA_TIME_TO_IMPATIENCE:
        case SUMO_ATTR_LCA_ACCEL_LAT:
        case SUMO_ATTR_LCA_LOOKAHEADLEFT:
        case SUMO_ATTR_LCA_SPEEDGAINRIGHT:
        case SUMO_ATTR_LCA_MAXSPEEDLATSTANDING:
        case SUMO_ATTR_LCA_MAXSPEEDLATFACTOR:
        case SUMO_ATTR_LCA_TURN_ALIGNMENT_DISTANCE:
        case SUMO_ATTR_LCA_OVERTAKE_RIGHT:
            /* case SUMO_ATTR_LCA_EXPERIMENTAL1: */
            return getLCParamString(key, myTagProperty.getDefaultValue(key));
        //
        case SUMO_ATTR_COLLISION_MINGAP_FACTOR:
        case SUMO_ATTR_TMP1:
        case SUMO_ATTR_TMP2:
        case SUMO_ATTR_TMP3:
        case SUMO_ATTR_TMP4:
        case SUMO_ATTR_TMP5:
        case SUMO_ATTR_CF_PWAGNER2009_TAULAST:
        case SUMO_ATTR_CF_PWAGNER2009_APPROB:
        case SUMO_ATTR_CF_IDMM_ADAPT_FACTOR:
        case SUMO_ATTR_CF_IDMM_ADAPT_TIME:
        case SUMO_ATTR_CF_WIEDEMANN_SECURITY:
        case SUMO_ATTR_CF_WIEDEMANN_ESTIMATION:
        case SUMO_ATTR_TRAIN_TYPE:
        case SUMO_ATTR_K:
        case SUMO_ATTR_CF_KERNER_PHI:
        case SUMO_ATTR_CF_IDM_DELTA:
        case SUMO_ATTR_CF_IDM_STEPPING:
            return getCFParamString(key, "");
        // Mutable values
        case SUMO_ATTR_LENGTH:
            if (wasSet(VTYPEPARS_LENGTH_SET)) {
                return toString(length);
            } else {
                return toString(defaultValues.length);
            }
        case SUMO_ATTR_MINGAP:
            if (wasSet(VTYPEPARS_MINGAP_SET)) {
                return toString(minGap);
            } else {
                return toString(defaultValues.minGap);
            }
        case SUMO_ATTR_MAXSPEED:
            if (wasSet(VTYPEPARS_MAXSPEED_SET)) {
                return toString(maxSpeed);
            } else {
                return toString(defaultValues.maxSpeed);
            }
        case SUMO_ATTR_SPEEDFACTOR:
            if (wasSet(VTYPEPARS_SPEEDFACTOR_SET)) {
                return toString(speedFactor.getParameter()[0]);
            } else {
                return toString(defaultValues.speedFactor.getParameter()[0]);
            }
        case SUMO_ATTR_SPEEDDEV:
            if (wasSet(VTYPEPARS_SPEEDFACTOR_SET)) {
                return toString(speedFactor.getParameter()[1]);
            } else {
                return toString(defaultValues.speedFactor.getParameter()[1]);
            }
        case SUMO_ATTR_PERSON_CAPACITY:
            if (wasSet(VTYPEPARS_PERSON_CAPACITY)) {
                return toString(personCapacity);
            } else {
                return toString(defaultValues.personCapacity);
            }
        case SUMO_ATTR_CONTAINER_CAPACITY:
            if (wasSet(VTYPEPARS_CONTAINER_CAPACITY)) {
                return toString(containerCapacity);
            } else {
                return toString(defaultValues.containerCapacity);
            }
        case SUMO_ATTR_OSGFILE:
            if (wasSet(VTYPEPARS_OSGFILE_SET)) {
                return osgFile;
            } else {
                return defaultValues.osgFile;
            }
        case SUMO_ATTR_COLOR:
            if (wasSet(VTYPEPARS_COLOR_SET)) {
                return toString(color);
            } else {
                return "";
            }
        case SUMO_ATTR_VCLASS:
            if (wasSet(VTYPEPARS_VEHICLECLASS_SET)) {
                return toString(vehicleClass);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_VCLASS);
            }
        case SUMO_ATTR_EMISSIONCLASS:
            if (wasSet(VTYPEPARS_EMISSIONCLASS_SET)) {
                return PollutantsInterface::getName(emissionClass);
            } else {
                return PollutantsInterface::getName(defaultValues.emissionClass);
            }
        case SUMO_ATTR_GUISHAPE:
            if (wasSet(VTYPEPARS_SHAPE_SET)) {
                return getVehicleShapeName(shape);
            } else {
                return getVehicleShapeName(defaultValues.shape);
            }
        case SUMO_ATTR_WIDTH:
            if (wasSet(VTYPEPARS_WIDTH_SET)) {
                return toString(width);
            } else {
                return toString(defaultValues.width);
            }
        case SUMO_ATTR_HEIGHT:
            if (wasSet(VTYPEPARS_HEIGHT_SET)) {
                return toString(height);
            } else {
                return toString(defaultValues.height);
            }
        case SUMO_ATTR_IMGFILE:
            if (wasSet(VTYPEPARS_IMGFILE_SET)) {
                return imgFile;
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_IMGFILE);
            }
        case SUMO_ATTR_LANE_CHANGE_MODEL:
            if (wasSet(VTYPEPARS_LANE_CHANGE_MODEL_SET)) {
                return SUMOXMLDefinitions::LaneChangeModels.getString(lcModel);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_LANE_CHANGE_MODEL);
            }
        case SUMO_ATTR_CAR_FOLLOW_MODEL:
            if (wasSet(VTYPEPARS_CAR_FOLLOW_MODEL)) {
                return SUMOXMLDefinitions::CarFollowModels.getString(cfModel);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_CAR_FOLLOW_MODEL);
            }
        case SUMO_ATTR_BOARDING_DURATION:
            if (wasSet(VTYPEPARS_BOARDING_DURATION)) {
                return time2string(boardingDuration);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_BOARDING_DURATION);
            }
        case SUMO_ATTR_LOADING_DURATION:
            if (wasSet(VTYPEPARS_LOADING_DURATION)) {
                return time2string(loadingDuration);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_LOADING_DURATION);
            }
        case SUMO_ATTR_LATALIGNMENT:
            if (wasSet(VTYPEPARS_LATALIGNMENT_SET)) {
                return toString(latAlignment);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_LATALIGNMENT);
            }
        case SUMO_ATTR_MINGAP_LAT:
            if (wasSet(VTYPEPARS_MINGAP_LAT_SET)) {
                return toString(minGapLat);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_MINGAP_LAT);
            }
        case SUMO_ATTR_MAXSPEED_LAT:
            if (wasSet(VTYPEPARS_MAXSPEED_LAT_SET)) {
                return toString(maxSpeedLat);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_MAXSPEED_LAT);
            }
        case SUMO_ATTR_ACTIONSTEPLENGTH:
            if (wasSet(VTYPEPARS_ACTIONSTEPLENGTH_SET)) {
                return toString(actionStepLength);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_ACTIONSTEPLENGTH);
            }
        case SUMO_ATTR_PROB:
            if (wasSet(VTYPEPARS_PROBABILITY_SET)) {
                return toString(defaultProbability);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_PROB);
            }
        case SUMO_ATTR_HASDRIVERSTATE:
            if (wasSet(VTYPEPARS_HASDRIVERSTATE_SET)) {
                return toString(hasDriverState);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_HASDRIVERSTATE);
            }
        case SUMO_ATTR_CARRIAGE_LENGTH:
            if (wasSet(VTYPEPARS_CARRIAGE_LENGTH_SET)) {
                return toString(carriageLength);
            } else {
                return toString(defaultValues.carriageLength);
            }
        case SUMO_ATTR_LOCOMOTIVE_LENGTH:
            if (wasSet(VTYPEPARS_LOCOMOTIVE_LENGTH_SET)) {
                return toString(locomotiveLength);
            } else {
                return toString(defaultValues.locomotiveLength);
            }
        case SUMO_ATTR_CARRIAGE_GAP:
            if (wasSet(VTYPEPARS_CARRIAGE_GAP_SET)) {
                return toString(carriageGap);
            } else {
                return myTagProperty.getDefaultValue(SUMO_ATTR_CARRIAGE_GAP);
            }
        case GNE_ATTR_GENERIC:
            return getGenericParametersStr();
        case GNE_ATTR_DEFAULT_VTYPE:
            return toString((getDemandElementID() == DEFAULT_VTYPE_ID) ||
                            (getDemandElementID() == DEFAULT_PEDTYPE_ID) ||
                            (getDemandElementID() == DEFAULT_BIKETYPE_ID));
        case GNE_ATTR_DEFAULT_VTYPE_MODIFIED:
            if (myDefaultVehicleType) {
                return toString(myDefaultVehicleTypeModified);
            } else {
                return toString(false);
            }
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


double
GNEVehicleType::getAttributeDouble(SumoXMLAttr key) const {
    // obtain default values depending of vehicle class
    VClassDefaultValues defaultValues(vehicleClass);
    switch (key) {
        case SUMO_ATTR_LENGTH:
            if (wasSet(VTYPEPARS_LENGTH_SET)) {
                return length;
            } else {
                return defaultValues.length;
            }
        case SUMO_ATTR_MINGAP:
            if (wasSet(VTYPEPARS_MINGAP_SET)) {
                return minGap;
            } else {
                return defaultValues.minGap;
            }
        case SUMO_ATTR_WIDTH:
            if (wasSet(VTYPEPARS_WIDTH_SET)) {
                return width;
            } else {
                return defaultValues.width;
            }
        case SUMO_ATTR_HEIGHT:
            if (wasSet(VTYPEPARS_HEIGHT_SET)) {
                return height;
            } else {
                return defaultValues.height;
            }
        case SUMO_ATTR_MAXSPEED:
            if (wasSet(VTYPEPARS_MAXSPEED_SET)) {
                return maxSpeed;
            } else {
                return defaultValues.maxSpeed;
            }
        default:
            throw InvalidArgument(getTagStr() + " doesn't have a double attribute of type '" + toString(key) + "'");
    }
}


void
GNEVehicleType::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
        case SUMO_ATTR_ID:
            undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), key, value));
            break;
        // CFM Attributes
        case SUMO_ATTR_ACCEL:
        case SUMO_ATTR_DECEL:
        case SUMO_ATTR_APPARENTDECEL:
        case SUMO_ATTR_EMERGENCYDECEL:
        case SUMO_ATTR_SIGMA:
        case SUMO_ATTR_TAU:
        case SUMO_ATTR_COLLISION_MINGAP_FACTOR:
        case SUMO_ATTR_TMP1:
        case SUMO_ATTR_TMP2:
        case SUMO_ATTR_TMP3:
        case SUMO_ATTR_TMP4:
        case SUMO_ATTR_TMP5:
        case SUMO_ATTR_CF_PWAGNER2009_TAULAST:
        case SUMO_ATTR_CF_PWAGNER2009_APPROB:
        case SUMO_ATTR_CF_IDMM_ADAPT_FACTOR:
        case SUMO_ATTR_CF_IDMM_ADAPT_TIME:
        case SUMO_ATTR_CF_WIEDEMANN_SECURITY:
        case SUMO_ATTR_CF_WIEDEMANN_ESTIMATION:
        case SUMO_ATTR_TRAIN_TYPE:
        case SUMO_ATTR_K:
        case SUMO_ATTR_CF_KERNER_PHI:
        case SUMO_ATTR_CF_IDM_DELTA:
        case SUMO_ATTR_CF_IDM_STEPPING:
        // JM Attributes
        case SUMO_ATTR_JM_CROSSING_GAP:
        case SUMO_ATTR_JM_IGNORE_KEEPCLEAR_TIME:
        case SUMO_ATTR_JM_DRIVE_AFTER_YELLOW_TIME:
        case SUMO_ATTR_JM_DRIVE_AFTER_RED_TIME:
        case SUMO_ATTR_JM_DRIVE_RED_SPEED:
        case SUMO_ATTR_JM_IGNORE_FOE_PROB:
        case SUMO_ATTR_JM_IGNORE_FOE_SPEED:
        case SUMO_ATTR_JM_SIGMA_MINOR:
        case SUMO_ATTR_JM_TIMEGAP_MINOR:
        case SUMO_ATTR_IMPATIENCE:
        // LCM Attributes
        case SUMO_ATTR_LCA_STRATEGIC_PARAM:
        case SUMO_ATTR_LCA_COOPERATIVE_PARAM:
        case SUMO_ATTR_LCA_SPEEDGAIN_PARAM:
        case SUMO_ATTR_LCA_KEEPRIGHT_PARAM:
        case SUMO_ATTR_LCA_SUBLANE_PARAM:
        case SUMO_ATTR_LCA_OPPOSITE_PARAM:
        case SUMO_ATTR_LCA_PUSHY:
        case SUMO_ATTR_LCA_PUSHYGAP:
        case SUMO_ATTR_LCA_ASSERTIVE:
        case SUMO_ATTR_LCA_IMPATIENCE:
        case SUMO_ATTR_LCA_TIME_TO_IMPATIENCE:
        case SUMO_ATTR_LCA_ACCEL_LAT:
        case SUMO_ATTR_LCA_LOOKAHEADLEFT:
        case SUMO_ATTR_LCA_SPEEDGAINRIGHT:
        case SUMO_ATTR_LCA_MAXSPEEDLATSTANDING:
        case SUMO_ATTR_LCA_MAXSPEEDLATFACTOR:
        case SUMO_ATTR_LCA_TURN_ALIGNMENT_DISTANCE:
        case SUMO_ATTR_LCA_OVERTAKE_RIGHT:
        /* case SUMO_ATTR_LCA_EXPERIMENTAL1: */
        //
        case SUMO_ATTR_LENGTH:
        case SUMO_ATTR_MINGAP:
        case SUMO_ATTR_MAXSPEED:
        case SUMO_ATTR_SPEEDFACTOR:
        case SUMO_ATTR_SPEEDDEV:
        case SUMO_ATTR_COLOR:
        case SUMO_ATTR_VCLASS:
        case SUMO_ATTR_EMISSIONCLASS:
        case SUMO_ATTR_GUISHAPE:
        case SUMO_ATTR_WIDTH:
        case SUMO_ATTR_HEIGHT:
        case SUMO_ATTR_IMGFILE:
        case SUMO_ATTR_LANE_CHANGE_MODEL:
        case SUMO_ATTR_CAR_FOLLOW_MODEL:
        case SUMO_ATTR_PERSON_CAPACITY:
        case SUMO_ATTR_CONTAINER_CAPACITY:
        case SUMO_ATTR_BOARDING_DURATION:
        case SUMO_ATTR_LOADING_DURATION:
        case SUMO_ATTR_LATALIGNMENT:
        case SUMO_ATTR_MINGAP_LAT:
        case SUMO_ATTR_MAXSPEED_LAT:
        case SUMO_ATTR_ACTIONSTEPLENGTH:
        case SUMO_ATTR_PROB:
        case SUMO_ATTR_HASDRIVERSTATE:
        case SUMO_ATTR_OSGFILE:
        case SUMO_ATTR_CARRIAGE_LENGTH:
        case SUMO_ATTR_LOCOMOTIVE_LENGTH:
        case SUMO_ATTR_CARRIAGE_GAP:
        case GNE_ATTR_GENERIC:
            // if we change the original value of a default vehicle Type, change also flag "myDefaultVehicleType"
            if (myDefaultVehicleType) {
                undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), true, GNE_ATTR_DEFAULT_VTYPE_MODIFIED, "true"));
            }
            undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), true, key, value));
            break;
        case GNE_ATTR_DEFAULT_VTYPE_MODIFIED:
            undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), true, key, value));
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool
GNEVehicleType::isValid(SumoXMLAttr key, const std::string& value) {
    // a empty value is always valid except for IDs
    if ((key != SUMO_ATTR_ID) && value.empty()) {
        return true;
    }

    switch (key) {
        case SUMO_ATTR_ID:
            // Vtypes and PTypes shares namespace
            if (SUMOXMLDefinitions::isValidVehicleID(value) &&
                    (myViewNet->getNet()->retrieveDemandElement(SUMO_TAG_VTYPE, value, false) == nullptr) &&
                    (myViewNet->getNet()->retrieveDemandElement(SUMO_TAG_PTYPE, value, false) == nullptr)) {
                return true;
            } else {
                return false;
            }
        // CFM Attributes
        case SUMO_ATTR_SIGMA:
            return canParse<double>(value) && (parse<double>(value) >= 0) && (parse<double>(value) <= 1);
        case SUMO_ATTR_ACCEL:
        case SUMO_ATTR_DECEL:
        case SUMO_ATTR_APPARENTDECEL:
        case SUMO_ATTR_EMERGENCYDECEL:
        case SUMO_ATTR_TAU:
        case SUMO_ATTR_COLLISION_MINGAP_FACTOR:
        case SUMO_ATTR_TMP1:
        case SUMO_ATTR_TMP2:
        case SUMO_ATTR_TMP3:
        case SUMO_ATTR_TMP4:
        case SUMO_ATTR_TMP5:
        case SUMO_ATTR_CF_PWAGNER2009_TAULAST:
        case SUMO_ATTR_CF_PWAGNER2009_APPROB:
        case SUMO_ATTR_CF_IDMM_ADAPT_FACTOR:
        case SUMO_ATTR_CF_IDMM_ADAPT_TIME:
        case SUMO_ATTR_CF_WIEDEMANN_SECURITY:
        case SUMO_ATTR_CF_WIEDEMANN_ESTIMATION:
        case SUMO_ATTR_K:
        case SUMO_ATTR_CF_KERNER_PHI:
        case SUMO_ATTR_CF_IDM_DELTA:
        case SUMO_ATTR_CF_IDM_STEPPING:
            return canParse<double>(value);
        case SUMO_ATTR_TRAIN_TYPE:
            // rail string
            return SUMOXMLDefinitions::TrainTypes.hasString(value);
        // JM Attributes
        case SUMO_ATTR_JM_CROSSING_GAP:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_JM_IGNORE_KEEPCLEAR_TIME:
            return canParse<double>(value) && (parse<double>(value) >= -1);
        case SUMO_ATTR_JM_DRIVE_AFTER_YELLOW_TIME:
            return canParse<double>(value) && (parse<double>(value) >= -1);
        case SUMO_ATTR_JM_DRIVE_AFTER_RED_TIME:
            return canParse<double>(value) && (parse<double>(value) >= -1);
        case SUMO_ATTR_JM_DRIVE_RED_SPEED:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_JM_IGNORE_FOE_PROB:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_JM_IGNORE_FOE_SPEED:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_JM_SIGMA_MINOR:
            return canParse<double>(value) && (parse<double>(value) >= 0) && (parse<double>(value) <= 1);
        case SUMO_ATTR_JM_TIMEGAP_MINOR:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_IMPATIENCE:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        // LCM Attributes
        case SUMO_ATTR_LCA_STRATEGIC_PARAM:
        case SUMO_ATTR_LCA_COOPERATIVE_PARAM:
        case SUMO_ATTR_LCA_SPEEDGAIN_PARAM:
        case SUMO_ATTR_LCA_KEEPRIGHT_PARAM:
        case SUMO_ATTR_LCA_SUBLANE_PARAM:
        case SUMO_ATTR_LCA_OPPOSITE_PARAM:
        case SUMO_ATTR_LCA_PUSHY:
            return canParse<double>(value);
        case SUMO_ATTR_LCA_PUSHYGAP:
        case SUMO_ATTR_LCA_IMPATIENCE:
        case SUMO_ATTR_LCA_MAXSPEEDLATSTANDING:
        case SUMO_ATTR_LCA_TURN_ALIGNMENT_DISTANCE: // 0 mean disabled
        case SUMO_ATTR_LCA_TIME_TO_IMPATIENCE:      // 0 mean disabled
        case SUMO_ATTR_LCA_OVERTAKE_RIGHT:          // 0 mean disabled
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_LCA_ASSERTIVE:
        case SUMO_ATTR_LCA_LOOKAHEADLEFT:
        case SUMO_ATTR_LCA_SPEEDGAINRIGHT:
        case SUMO_ATTR_LCA_ACCEL_LAT:
        case SUMO_ATTR_LCA_MAXSPEEDLATFACTOR:
            return canParse<double>(value) && (parse<double>(value) > 0);
        /* case SUMO_ATTR_LCA_EXPERIMENTAL1:
            return true;
        */
        //
        case SUMO_ATTR_LENGTH:
            return canParse<double>(value) && (parse<double>(value) > 0);
        case SUMO_ATTR_MINGAP:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_MAXSPEED:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_SPEEDFACTOR:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_SPEEDDEV:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_COLOR:
            if (value.empty()) {
                return true;
            } else {
                return canParse<RGBColor>(value);
            }
        case SUMO_ATTR_VCLASS:
            return canParseVehicleClasses(value);
        case SUMO_ATTR_EMISSIONCLASS:
            // check if given value correspond to a string of PollutantsInterface::getAllClassesStr()
            for (const auto& i : PollutantsInterface::getAllClassesStr()) {
                if (value == i) {
                    return true;
                }
            }
            return false;
        case SUMO_ATTR_GUISHAPE:
            if (value == "all") {
                return false;
            } else {
                return canParseVehicleShape(value);
            }
        case SUMO_ATTR_WIDTH:
            return canParse<double>(value);
        case SUMO_ATTR_HEIGHT:
            return canParse<double>(value);
        case SUMO_ATTR_IMGFILE:
            return SUMOXMLDefinitions::isValidFilename(value);
        case SUMO_ATTR_LANE_CHANGE_MODEL:
            return SUMOXMLDefinitions::LaneChangeModels.hasString(value);
        case SUMO_ATTR_CAR_FOLLOW_MODEL:
            return SUMOXMLDefinitions::CarFollowModels.hasString(value);
        case SUMO_ATTR_PERSON_CAPACITY:
            return canParse<int>(value);
        case SUMO_ATTR_CONTAINER_CAPACITY:
            return canParse<int>(value);
        case SUMO_ATTR_BOARDING_DURATION:
            return canParse<double>(value);
        case SUMO_ATTR_LOADING_DURATION:
            return canParse<double>(value);
        case SUMO_ATTR_LATALIGNMENT:
            return SUMOXMLDefinitions::LateralAlignments.hasString(value);
        case SUMO_ATTR_MINGAP_LAT:
            return canParse<double>(value);
        case SUMO_ATTR_MAXSPEED_LAT:
            return canParse<double>(value);
        case SUMO_ATTR_ACTIONSTEPLENGTH:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_PROB:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_HASDRIVERSTATE:
            return canParse<bool>(value);
        case SUMO_ATTR_OSGFILE:
            return SUMOXMLDefinitions::isValidFilename(value);
        case SUMO_ATTR_CARRIAGE_LENGTH:
            return canParse<double>(value) && (parse<double>(value) >= -1);
        case SUMO_ATTR_LOCOMOTIVE_LENGTH:
            return canParse<double>(value) && (parse<double>(value) >= -1);
        case SUMO_ATTR_CARRIAGE_GAP:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case GNE_ATTR_GENERIC:
            return isGenericParametersValid(value);
        case GNE_ATTR_DEFAULT_VTYPE_MODIFIED:
            if (myDefaultVehicleType) {
                return canParse<bool>(value);
            } else {
                return false;
            }
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNEVehicleType::enableAttribute(SumoXMLAttr /*key*/, GNEUndoList* /*undoList*/) {

}

bool
GNEVehicleType::isAttributeEnabled(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_LENGTH:
            return wasSet(VTYPEPARS_LENGTH_SET);
        case SUMO_ATTR_MINGAP:
            return wasSet(VTYPEPARS_MINGAP_SET);
        case SUMO_ATTR_MAXSPEED:
            return wasSet(VTYPEPARS_MAXSPEED_SET);
        case SUMO_ATTR_SPEEDFACTOR:
            return wasSet(VTYPEPARS_SPEEDFACTOR_SET);
        case SUMO_ATTR_SPEEDDEV:
            return wasSet(VTYPEPARS_SPEEDFACTOR_SET);
        case SUMO_ATTR_PERSON_CAPACITY:
            return wasSet(VTYPEPARS_PERSON_CAPACITY);
        case SUMO_ATTR_CONTAINER_CAPACITY:
            return wasSet(VTYPEPARS_CONTAINER_CAPACITY);
        case SUMO_ATTR_OSGFILE:
            return wasSet(VTYPEPARS_OSGFILE_SET);
        case SUMO_ATTR_CARRIAGE_LENGTH:
            return wasSet(VTYPEPARS_CARRIAGE_LENGTH_SET);
        case SUMO_ATTR_LOCOMOTIVE_LENGTH:
            return wasSet(VTYPEPARS_LOCOMOTIVE_LENGTH_SET);
        case SUMO_ATTR_CARRIAGE_GAP:
            return wasSet(VTYPEPARS_CARRIAGE_GAP_SET);
        default:
            return true;
    }
}


std::string
GNEVehicleType::getPopUpID() const {
    return getTagStr();
}


std::string
GNEVehicleType::getHierarchyName() const {
    return getTagStr() + ": " + getAttribute(SUMO_ATTR_ID) ;
}


void
GNEVehicleType::overwriteVType(GNEDemandElement* vType, SUMOVTypeParameter* newVTypeParameter, GNEUndoList* undoList) {
    // open undo list and overwritte all values of default VType
    undoList->p_begin("update default " + vType->getTagStr() + " '" + DEFAULT_VTYPE_ID + "'");
    // CFM values
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_ACCEL, "").empty()) {
        vType->setAttribute(SUMO_ATTR_ACCEL, toString(newVTypeParameter->getCFParam(SUMO_ATTR_ACCEL, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_DECEL, "").empty()) {
        vType->setAttribute(SUMO_ATTR_DECEL, toString(newVTypeParameter->getCFParam(SUMO_ATTR_DECEL, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_APPARENTDECEL, "").empty()) {
        vType->setAttribute(SUMO_ATTR_APPARENTDECEL, toString(newVTypeParameter->getCFParam(SUMO_ATTR_APPARENTDECEL, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_EMERGENCYDECEL, "").empty()) {
        vType->setAttribute(SUMO_ATTR_EMERGENCYDECEL, toString(newVTypeParameter->getCFParam(SUMO_ATTR_EMERGENCYDECEL, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_SIGMA, "").empty()) {
        vType->setAttribute(SUMO_ATTR_SIGMA, toString(newVTypeParameter->getCFParam(SUMO_ATTR_SIGMA, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_TAU, "").empty()) {
        vType->setAttribute(SUMO_ATTR_TAU, toString(newVTypeParameter->getCFParam(SUMO_ATTR_TAU, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_COLLISION_MINGAP_FACTOR, "").empty()) {
        vType->setAttribute(SUMO_ATTR_COLLISION_MINGAP_FACTOR, toString(newVTypeParameter->getCFParam(SUMO_ATTR_COLLISION_MINGAP_FACTOR, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_TMP1, "").empty()) {
        vType->setAttribute(SUMO_ATTR_TMP1, toString(newVTypeParameter->getCFParam(SUMO_ATTR_TMP1, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_TMP2, "").empty()) {
        vType->setAttribute(SUMO_ATTR_TMP2, toString(newVTypeParameter->getCFParam(SUMO_ATTR_TMP2, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_TMP3, "").empty()) {
        vType->setAttribute(SUMO_ATTR_TMP3, toString(newVTypeParameter->getCFParam(SUMO_ATTR_TMP3, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_TMP4, "").empty()) {
        vType->setAttribute(SUMO_ATTR_TMP4, toString(newVTypeParameter->getCFParam(SUMO_ATTR_TMP4, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_TMP5, "").empty()) {
        vType->setAttribute(SUMO_ATTR_TMP5, toString(newVTypeParameter->getCFParam(SUMO_ATTR_TMP5, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_PWAGNER2009_TAULAST, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_PWAGNER2009_TAULAST, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_PWAGNER2009_TAULAST, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_PWAGNER2009_APPROB, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_PWAGNER2009_APPROB, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_PWAGNER2009_APPROB, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_IDMM_ADAPT_FACTOR, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_IDMM_ADAPT_FACTOR, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_IDMM_ADAPT_FACTOR, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_IDMM_ADAPT_TIME, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_IDMM_ADAPT_TIME, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_IDMM_ADAPT_TIME, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_WIEDEMANN_SECURITY, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_WIEDEMANN_SECURITY, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_WIEDEMANN_SECURITY, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_WIEDEMANN_ESTIMATION, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_WIEDEMANN_ESTIMATION, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_WIEDEMANN_ESTIMATION, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_TRAIN_TYPE, "").empty()) {
        vType->setAttribute(SUMO_ATTR_TRAIN_TYPE, toString(newVTypeParameter->getCFParam(SUMO_ATTR_TRAIN_TYPE, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_K, "").empty()) {
        vType->setAttribute(SUMO_ATTR_K, toString(newVTypeParameter->getCFParam(SUMO_ATTR_K, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_KERNER_PHI, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_KERNER_PHI, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_KERNER_PHI, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_IDM_DELTA, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_IDM_DELTA, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_IDM_DELTA, 0)), undoList);
    }
    if (!newVTypeParameter->getCFParamString(SUMO_ATTR_CF_IDM_STEPPING, "").empty()) {
        vType->setAttribute(SUMO_ATTR_CF_IDM_STEPPING, toString(newVTypeParameter->getCFParam(SUMO_ATTR_CF_IDM_STEPPING, 0)), undoList);
    }
    // JM values
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_CROSSING_GAP, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_CROSSING_GAP, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_CROSSING_GAP, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_IGNORE_KEEPCLEAR_TIME, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_IGNORE_KEEPCLEAR_TIME, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_IGNORE_KEEPCLEAR_TIME, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_DRIVE_AFTER_YELLOW_TIME, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_DRIVE_AFTER_YELLOW_TIME, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_DRIVE_AFTER_YELLOW_TIME, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_DRIVE_AFTER_RED_TIME, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_DRIVE_AFTER_RED_TIME, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_DRIVE_AFTER_RED_TIME, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_DRIVE_RED_SPEED, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_DRIVE_RED_SPEED, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_DRIVE_RED_SPEED, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_IGNORE_FOE_PROB, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_IGNORE_FOE_PROB, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_IGNORE_FOE_PROB, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_IGNORE_FOE_SPEED, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_IGNORE_FOE_SPEED, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_IGNORE_FOE_SPEED, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_SIGMA_MINOR, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_SIGMA_MINOR, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_SIGMA_MINOR, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_JM_TIMEGAP_MINOR, "").empty()) {
        vType->setAttribute(SUMO_ATTR_JM_TIMEGAP_MINOR, toString(newVTypeParameter->getCFParam(SUMO_ATTR_JM_TIMEGAP_MINOR, 0)), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_IMPATIENCE_SET)) {
        vType->setAttribute(SUMO_ATTR_IMPATIENCE, toString(newVTypeParameter->impatience), undoList);
    }
    // LCM values
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_STRATEGIC_PARAM, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_STRATEGIC_PARAM, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_STRATEGIC_PARAM, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_COOPERATIVE_PARAM, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_COOPERATIVE_PARAM, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_COOPERATIVE_PARAM, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_SPEEDGAIN_PARAM, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_SPEEDGAIN_PARAM, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_SPEEDGAIN_PARAM, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_KEEPRIGHT_PARAM, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_KEEPRIGHT_PARAM, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_KEEPRIGHT_PARAM, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_SUBLANE_PARAM, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_SUBLANE_PARAM, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_SUBLANE_PARAM, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_OPPOSITE_PARAM, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_OPPOSITE_PARAM, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_OPPOSITE_PARAM, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_PUSHY, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_PUSHY, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_PUSHY, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_PUSHYGAP, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_PUSHYGAP, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_PUSHYGAP, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_ASSERTIVE, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_ASSERTIVE, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_ASSERTIVE, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_IMPATIENCE, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_IMPATIENCE, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_IMPATIENCE, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_TIME_TO_IMPATIENCE, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_TIME_TO_IMPATIENCE, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_TIME_TO_IMPATIENCE, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_ACCEL_LAT, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_ACCEL_LAT, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_ACCEL_LAT, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_LOOKAHEADLEFT, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_LOOKAHEADLEFT, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_LOOKAHEADLEFT, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_SPEEDGAINRIGHT, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_SPEEDGAINRIGHT, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_SPEEDGAINRIGHT, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_MAXSPEEDLATSTANDING, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_MAXSPEEDLATSTANDING, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_MAXSPEEDLATSTANDING, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_MAXSPEEDLATFACTOR, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_MAXSPEEDLATFACTOR, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_MAXSPEEDLATFACTOR, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_TURN_ALIGNMENT_DISTANCE, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_TURN_ALIGNMENT_DISTANCE, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_TURN_ALIGNMENT_DISTANCE, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_OVERTAKE_RIGHT, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_OVERTAKE_RIGHT, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_OVERTAKE_RIGHT, 0)), undoList);
    }
    if (!newVTypeParameter->getJMParamString(SUMO_ATTR_LCA_EXPERIMENTAL1, "").empty()) {
        vType->setAttribute(SUMO_ATTR_LCA_EXPERIMENTAL1, toString(newVTypeParameter->getCFParam(SUMO_ATTR_LCA_EXPERIMENTAL1, 0)), undoList);
    }
    //
    if (newVTypeParameter->wasSet(VTYPEPARS_LENGTH_SET)) {
        vType->setAttribute(SUMO_ATTR_LENGTH, toString(newVTypeParameter->length), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_MINGAP_SET)) {
        vType->setAttribute(SUMO_ATTR_MINGAP, toString(newVTypeParameter->minGap), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_MAXSPEED_SET)) {
        vType->setAttribute(SUMO_ATTR_MAXSPEED, toString(newVTypeParameter->maxSpeed), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_SPEEDFACTOR_SET)) {
        vType->setAttribute(SUMO_ATTR_SPEEDFACTOR, toString(newVTypeParameter->speedFactor.getParameter()[0]), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_SPEEDFACTOR_SET)) {
        vType->setAttribute(SUMO_ATTR_SPEEDDEV, toString(newVTypeParameter->speedFactor.getParameter()[1]), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_COLOR_SET)) {
        vType->setAttribute(SUMO_ATTR_COLOR, toString(newVTypeParameter->color), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_EMISSIONCLASS_SET)) {
        vType->setAttribute(SUMO_ATTR_EMISSIONCLASS, PollutantsInterface::getName(newVTypeParameter->emissionClass), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_SHAPE_SET)) {
        vType->setAttribute(SUMO_ATTR_GUISHAPE, getVehicleShapeName(newVTypeParameter->shape), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_WIDTH_SET)) {
        vType->setAttribute(SUMO_ATTR_WIDTH, toString(newVTypeParameter->width), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_HEIGHT_SET)) {
        vType->setAttribute(SUMO_ATTR_HEIGHT, toString(newVTypeParameter->height), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_IMGFILE_SET)) {
        vType->setAttribute(SUMO_ATTR_IMGFILE, toString(newVTypeParameter->imgFile), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_LANE_CHANGE_MODEL_SET)) {
        vType->setAttribute(SUMO_ATTR_LANE_CHANGE_MODEL, SUMOXMLDefinitions::LaneChangeModels.getString(newVTypeParameter->lcModel), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_CAR_FOLLOW_MODEL)) {
        vType->setAttribute(SUMO_ATTR_CAR_FOLLOW_MODEL, SUMOXMLDefinitions::CarFollowModels.getString(newVTypeParameter->cfModel), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_PERSON_CAPACITY)) {
        vType->setAttribute(SUMO_ATTR_PERSON_CAPACITY, toString(newVTypeParameter->personCapacity), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_CONTAINER_CAPACITY)) {
        vType->setAttribute(SUMO_ATTR_CONTAINER_CAPACITY, toString(newVTypeParameter->containerCapacity), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_BOARDING_DURATION)) {
        vType->setAttribute(SUMO_ATTR_BOARDING_DURATION, toString(newVTypeParameter->boardingDuration), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_LOADING_DURATION)) {
        vType->setAttribute(SUMO_ATTR_LOADING_DURATION, toString(newVTypeParameter->loadingDuration), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_LATALIGNMENT_SET)) {
        vType->setAttribute(SUMO_ATTR_LATALIGNMENT, toString(newVTypeParameter->latAlignment), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_MINGAP_LAT_SET)) {
        vType->setAttribute(SUMO_ATTR_MINGAP_LAT, toString(newVTypeParameter->minGapLat), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_MAXSPEED_LAT_SET)) {
        vType->setAttribute(SUMO_ATTR_MAXSPEED_LAT, toString(newVTypeParameter->maxSpeedLat), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_ACTIONSTEPLENGTH_SET)) {
        vType->setAttribute(SUMO_ATTR_ACTIONSTEPLENGTH, toString(newVTypeParameter->actionStepLength), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_HASDRIVERSTATE_SET)) {
        vType->setAttribute(SUMO_ATTR_HASDRIVERSTATE, toString(newVTypeParameter->hasDriverState), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_PROBABILITY_SET)) {
        vType->setAttribute(SUMO_ATTR_PROB, toString(newVTypeParameter->defaultProbability), undoList);
    }
    if (newVTypeParameter->wasSet(VTYPEPARS_OSGFILE_SET)) {
        vType->setAttribute(SUMO_ATTR_OSGFILE, toString(newVTypeParameter->osgFile), undoList);
    }
    if (newVTypeParameter->knowsParameter(toString(SUMO_ATTR_CARRIAGE_LENGTH))) {
        vType->setAttribute(SUMO_ATTR_CARRIAGE_LENGTH, newVTypeParameter->getParameter(toString(SUMO_ATTR_CARRIAGE_LENGTH), ""), undoList);
    }
    if (newVTypeParameter->knowsParameter(toString(SUMO_ATTR_LOCOMOTIVE_LENGTH))) {
        vType->setAttribute(SUMO_ATTR_LOCOMOTIVE_LENGTH, newVTypeParameter->getParameter(toString(SUMO_ATTR_LOCOMOTIVE_LENGTH), ""), undoList);
    }
    if (newVTypeParameter->knowsParameter(toString(SUMO_ATTR_CARRIAGE_GAP))) {
        vType->setAttribute(SUMO_ATTR_CARRIAGE_GAP, newVTypeParameter->getParameter(toString(SUMO_ATTR_CARRIAGE_GAP), ""), undoList);
    }
    // parse generic parameters
    std::string genericParametersStr;
    // Generate an string using the following structure: "key1=value1|key2=value2|...
    for (auto i : newVTypeParameter->getParametersMap()) {
        genericParametersStr += i.first + "=" + i.second + "|";
    }
    // remove the last "|"
    if (!genericParametersStr.empty()) {
        genericParametersStr.pop_back();
    }
    if (genericParametersStr != vType->getGenericParametersStr()) {
        vType->setAttribute(GNE_ATTR_GENERIC, genericParametersStr, undoList);
    }
    // close undo list
    undoList->p_end();
}


void
GNEVehicleType::initRailVisualizationParameters() {
    if (SUMOVTypeParameter::knowsParameter("carriageLength")) {
        carriageLength = StringUtils::toDouble(SUMOVTypeParameter::getParameter("carriageLength"));
        parametersSet |= VTYPEPARS_CARRIAGE_LENGTH_SET;
    } else if (wasSet(VTYPEPARS_SHAPE_SET)) {
        switch (shape) {
            case SVS_BUS_FLEXIBLE:
                carriageLength = 8.25; // 16.5 overall, 2 modules http://de.wikipedia.org/wiki/Ikarus_180
                carriageGap = 0;
                break;
            case SVS_RAIL:
                carriageLength = 24.5; // http://de.wikipedia.org/wiki/UIC-Y-Wagen_%28DR%29
                break;
            case SVS_RAIL_CAR:
                carriageLength = 16.85;  // 67.4m overall, 4 carriages http://de.wikipedia.org/wiki/DB-Baureihe_423
                break;
            case SVS_RAIL_CARGO:
                carriageLength = 13.86; // UIC 571-1 http://de.wikipedia.org/wiki/Flachwagen
                break;
            case SVS_TRUCK_SEMITRAILER:
                carriageLength = 13.5;
                locomotiveLength = 2.5;
                carriageGap = 0.5;
                break;
            case SVS_TRUCK_1TRAILER:
                carriageLength = 6.75;
                locomotiveLength = 2.5 + 6.75;
                carriageGap = 0.5;
                break;
            default:
                break;
        }
    }
    if (SUMOVTypeParameter::knowsParameter("locomotiveLength")) {
        locomotiveLength = StringUtils::toDouble(SUMOVTypeParameter::getParameter("locomotiveLength"));
        parametersSet |= VTYPEPARS_LOCOMOTIVE_LENGTH_SET;
    } else if (locomotiveLength <= 0) {
        locomotiveLength = carriageLength;
    }
    if (SUMOVTypeParameter::knowsParameter("carriageGap")) {
        carriageGap = StringUtils::toDouble(SUMOVTypeParameter::getParameter("carriageGap"));
        parametersSet |= VTYPEPARS_CARRIAGE_GAP_SET;
    }
}


std::string
GNEVehicleType::getGenericParametersStr() const {
    std::string result;
    // Generate an string using the following structure: "key1=value1|key2=value2|...
    for (auto i : getParametersMap()) {
        result += i.first + "=" + i.second + "|";
    }
    // remove the last "|"
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}


std::vector<std::pair<std::string, std::string> >
GNEVehicleType::getGenericParameters() const {
    std::vector<std::pair<std::string, std::string> >  result;
    // iterate over parameters map and fill result
    for (auto i : getParametersMap()) {
        result.push_back(std::make_pair(i.first, i.second));
    }
    return result;
}


void
GNEVehicleType::setGenericParametersStr(const std::string& value) {
    // clear parameters
    clearParameter();
    // separate value in a vector of string using | as separator
    std::vector<std::string> parsedValues;
    StringTokenizer stValues(value, "|", true);
    while (stValues.hasNext()) {
        parsedValues.push_back(stValues.next());
    }
    // check that parsed values (A=B)can be parsed in generic parameters
    for (auto i : parsedValues) {
        std::vector<std::string> parsedParameters;
        StringTokenizer stParam(i, "=", true);
        while (stParam.hasNext()) {
            parsedParameters.push_back(stParam.next());
        }
        // Check that parsed parameters are exactly two and contains valid chracters
        if (parsedParameters.size() == 2 && SUMOXMLDefinitions::isValidGenericParameterKey(parsedParameters.front()) && SUMOXMLDefinitions::isValidGenericParameterValue(parsedParameters.back())) {
            setParameter(parsedParameters.front(), parsedParameters.back());
        }
    }
}

// ===========================================================================
// private
// ===========================================================================

void
GNEVehicleType::setAttribute(SumoXMLAttr key, const std::string& value) {
    // obtain default values depending of vehicle class
    VClassDefaultValues defaultValues(vehicleClass);
    switch (key) {
        case SUMO_ATTR_ID:
            changeDemandElementID(value);
            id = value;
            break;
        // CFM Attributes
        case SUMO_ATTR_ACCEL:
        case SUMO_ATTR_DECEL:
        case SUMO_ATTR_APPARENTDECEL:
        case SUMO_ATTR_EMERGENCYDECEL:
        case SUMO_ATTR_SIGMA:
        case SUMO_ATTR_TAU:
        case SUMO_ATTR_COLLISION_MINGAP_FACTOR:
        case SUMO_ATTR_TMP1:
        case SUMO_ATTR_TMP2:
        case SUMO_ATTR_TMP3:
        case SUMO_ATTR_TMP4:
        case SUMO_ATTR_TMP5:
        case SUMO_ATTR_CF_PWAGNER2009_TAULAST:
        case SUMO_ATTR_CF_PWAGNER2009_APPROB:
        case SUMO_ATTR_CF_IDMM_ADAPT_FACTOR:
        case SUMO_ATTR_CF_IDMM_ADAPT_TIME:
        case SUMO_ATTR_CF_WIEDEMANN_SECURITY:
        case SUMO_ATTR_CF_WIEDEMANN_ESTIMATION:
        case SUMO_ATTR_TRAIN_TYPE:
        case SUMO_ATTR_K:
        case SUMO_ATTR_CF_KERNER_PHI:
        case SUMO_ATTR_CF_IDM_DELTA:
        case SUMO_ATTR_CF_IDM_STEPPING:
            // empty values means that value isn't set
            if (value.empty()) {
                const auto it = cfParameter.find(key);
                if (it != cfParameter.end()) {
                    cfParameter.erase(it);
                }
            } else {
                cfParameter[key] = value;
            }
            break;
        // JM Attributes
        case SUMO_ATTR_JM_CROSSING_GAP:
        case SUMO_ATTR_JM_IGNORE_KEEPCLEAR_TIME:
        case SUMO_ATTR_JM_DRIVE_AFTER_YELLOW_TIME:
        case SUMO_ATTR_JM_DRIVE_AFTER_RED_TIME:
        case SUMO_ATTR_JM_DRIVE_RED_SPEED:
        case SUMO_ATTR_JM_IGNORE_FOE_PROB:
        case SUMO_ATTR_JM_IGNORE_FOE_SPEED:
        case SUMO_ATTR_JM_SIGMA_MINOR:
        case SUMO_ATTR_JM_TIMEGAP_MINOR:
            // empty values means that value isn't set
            if (value.empty()) {
                const auto it = jmParameter.find(key);
                if (it != jmParameter.end()) {
                    jmParameter.erase(it);
                }
            } else {
                jmParameter[key] = value;
            }
            break;
        case SUMO_ATTR_IMPATIENCE:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                impatience = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_IMPATIENCE_SET;
            } else {
                // set default value
                impatience = parse<double>(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_IMPATIENCE_SET;
            }
            break;
        // LCM Attributes
        case SUMO_ATTR_LCA_STRATEGIC_PARAM:
        case SUMO_ATTR_LCA_COOPERATIVE_PARAM:
        case SUMO_ATTR_LCA_SPEEDGAIN_PARAM:
        case SUMO_ATTR_LCA_KEEPRIGHT_PARAM:
        case SUMO_ATTR_LCA_SUBLANE_PARAM:
        case SUMO_ATTR_LCA_OPPOSITE_PARAM:
        case SUMO_ATTR_LCA_PUSHY:
        case SUMO_ATTR_LCA_PUSHYGAP:
        case SUMO_ATTR_LCA_ASSERTIVE:
        case SUMO_ATTR_LCA_IMPATIENCE:
        case SUMO_ATTR_LCA_ACCEL_LAT:
        case SUMO_ATTR_LCA_LOOKAHEADLEFT:
        case SUMO_ATTR_LCA_SPEEDGAINRIGHT:
        case SUMO_ATTR_LCA_MAXSPEEDLATSTANDING:
        case SUMO_ATTR_LCA_MAXSPEEDLATFACTOR:

            /* case SUMO_ATTR_LCA_EXPERIMENTAL1: */
            // empty values means that value isn't set
            if (value.empty()) {
                const auto it = lcParameter.find(key);
                if (it != lcParameter.end()) {
                    lcParameter.erase(it);
                }
            } else {
                lcParameter[key] = value;
            }
            break;
        case SUMO_ATTR_LCA_TURN_ALIGNMENT_DISTANCE:
        case SUMO_ATTR_LCA_TIME_TO_IMPATIENCE:
        case SUMO_ATTR_LCA_OVERTAKE_RIGHT:
            // empty or null values means that value isn't set
            if (value.empty() || (canParse<double>(value) && (parse<double>(value) == 0))) {
                const auto it = lcParameter.find(key);
                if (it != lcParameter.end()) {
                    lcParameter.erase(it);
                }
            } else {
                lcParameter[key] = value;
            }
            break;
        //
        case SUMO_ATTR_LENGTH:
            if (!value.empty() && (value != toString(defaultValues.length))) {
                length = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_LENGTH_SET;
            } else {
                // set default value
                length = defaultValues.length;
                // unset parameter
                parametersSet &= ~VTYPEPARS_LENGTH_SET;
            }
            break;
        case SUMO_ATTR_MINGAP:
            if (!value.empty() && (value != toString(defaultValues.minGap))) {
                minGap = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_MINGAP_SET;
            } else {
                // set default value
                minGap = defaultValues.minGap;
                // unset parameter
                parametersSet &= ~VTYPEPARS_MINGAP_SET;
            }
            break;
        case SUMO_ATTR_MAXSPEED:
            if (!value.empty() && (value != toString(defaultValues.maxSpeed))) {
                maxSpeed = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_MAXSPEED_SET;
            } else {
                // set default value
                maxSpeed = defaultValues.maxSpeed;
                // unset parameter
                parametersSet &= ~VTYPEPARS_MAXSPEED_SET;
            }
            break;
        case SUMO_ATTR_SPEEDFACTOR:
            if (!value.empty() && (value != toString(defaultValues.speedFactor.getParameter()[0]))) {
                speedFactor.getParameter()[0] = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_SPEEDFACTOR_SET;
            } else {
                // set default value
                speedFactor.getParameter()[0] = defaultValues.speedFactor.getParameter()[0];
                // unset parameter
                parametersSet &= ~VTYPEPARS_SPEEDFACTOR_SET;
            }
            break;
        case SUMO_ATTR_SPEEDDEV:
            if (!value.empty() && (value != toString(defaultValues.speedFactor.getParameter()[1]))) {
                speedFactor.getParameter()[1] = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_SPEEDFACTOR_SET;
            } else {
                // set default value
                speedFactor.getParameter()[1] = defaultValues.speedFactor.getParameter()[1];
                // unset parameter
                parametersSet &= ~VTYPEPARS_SPEEDFACTOR_SET;
            }
            break;
        case SUMO_ATTR_COLOR:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                color = parse<RGBColor>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_COLOR_SET;
            } else {
                // unset parameter
                parametersSet &= ~VTYPEPARS_COLOR_SET;
            }
            break;
        case SUMO_ATTR_VCLASS:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                vehicleClass = getVehicleClassID(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_VEHICLECLASS_SET;
            } else {
                // set default value
                vehicleClass = getVehicleClassID(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_VEHICLECLASS_SET;
            }
            // update default values
            updateDefaultVClassAttributes(defaultValues);
            break;
        case SUMO_ATTR_EMISSIONCLASS:
            if (!value.empty() && (value != toString(defaultValues.emissionClass))) {
                emissionClass = PollutantsInterface::getClassByName(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_EMISSIONCLASS_SET;
            } else {
                // set default value
                emissionClass = defaultValues.emissionClass;
                // unset parameter
                parametersSet &= ~VTYPEPARS_EMISSIONCLASS_SET;
            }
            break;
        case SUMO_ATTR_GUISHAPE:
            if (!value.empty() && (value != toString(defaultValues.shape))) {
                shape = getVehicleShapeID(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_SHAPE_SET;
            } else {
                // set default value
                shape = defaultValues.shape;
                // unset parameter
                parametersSet &= ~VTYPEPARS_SHAPE_SET;
            }
            break;
        case SUMO_ATTR_WIDTH:
            if (!value.empty() && (value != toString(defaultValues.width))) {
                width = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_WIDTH_SET;
            } else {
                // set default value
                width = defaultValues.width;
                // unset parameter
                parametersSet &= ~VTYPEPARS_WIDTH_SET;
            }
            break;
        case SUMO_ATTR_HEIGHT:
            if (!value.empty() && (value != toString(defaultValues.height))) {
                height = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_HEIGHT_SET;
            } else {
                // set default value
                height = defaultValues.height;
                // unset parameter
                parametersSet &= ~VTYPEPARS_HEIGHT_SET;
            }
            break;
        case SUMO_ATTR_IMGFILE:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                imgFile = value;
                // mark parameter as set
                parametersSet |= VTYPEPARS_IMGFILE_SET;
            } else {
                // set default value
                imgFile = myTagProperty.getDefaultValue(key);
                // unset parameter
                parametersSet &= ~VTYPEPARS_IMGFILE_SET;
            }
            break;
        case SUMO_ATTR_LANE_CHANGE_MODEL:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                lcModel = SUMOXMLDefinitions::LaneChangeModels.get(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_LANE_CHANGE_MODEL_SET;
            } else {
                // set default value
                lcModel = SUMOXMLDefinitions::LaneChangeModels.get(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_LANE_CHANGE_MODEL_SET;
            }
            break;
        case SUMO_ATTR_CAR_FOLLOW_MODEL:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                cfModel = SUMOXMLDefinitions::CarFollowModels.get(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_CAR_FOLLOW_MODEL;
            } else {
                // set default value
                cfModel = SUMOXMLDefinitions::CarFollowModels.get(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_CAR_FOLLOW_MODEL;
            }
            break;
        case SUMO_ATTR_PERSON_CAPACITY:
            if (!value.empty() && (value != toString(defaultValues.personCapacity))) {
                personCapacity = parse<int>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_PERSON_CAPACITY;
            } else {
                // set default value
                personCapacity = defaultValues.personCapacity;
                // unset parameter
                parametersSet &= ~VTYPEPARS_PERSON_CAPACITY;
            }
            break;
        case SUMO_ATTR_CONTAINER_CAPACITY:
            if (!value.empty() && (value != toString(defaultValues.containerCapacity))) {
                containerCapacity = parse<int>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_CONTAINER_CAPACITY;
            } else {
                // set default value
                containerCapacity = defaultValues.containerCapacity;
                // unset parameter
                parametersSet &= ~VTYPEPARS_CONTAINER_CAPACITY;
            }
            break;
        case SUMO_ATTR_BOARDING_DURATION:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                boardingDuration = string2time(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_BOARDING_DURATION;
            } else {
                // set default value
                boardingDuration = string2time(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_BOARDING_DURATION;
            }
            break;
        case SUMO_ATTR_LOADING_DURATION:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                loadingDuration = string2time(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_LOADING_DURATION;
            } else {
                // set default value
                loadingDuration = string2time(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_LOADING_DURATION;
            }
            break;
        case SUMO_ATTR_LATALIGNMENT:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                latAlignment = SUMOXMLDefinitions::LateralAlignments.get(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_LATALIGNMENT_SET;
            } else {
                // set default value
                latAlignment = SUMOXMLDefinitions::LateralAlignments.get(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_LATALIGNMENT_SET;
            }
            break;
        case SUMO_ATTR_MINGAP_LAT:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                minGapLat = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_MINGAP_LAT_SET;
            } else {
                // set default value
                minGapLat = parse<double>(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_MINGAP_LAT_SET;
            }
            break;
        case SUMO_ATTR_MAXSPEED_LAT:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                maxSpeedLat = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_MAXSPEED_LAT_SET;
            } else {
                // set default value
                maxSpeedLat = parse<double>(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_MAXSPEED_LAT_SET;
            }
            break;
        case SUMO_ATTR_ACTIONSTEPLENGTH:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                actionStepLength = string2time(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_ACTIONSTEPLENGTH_SET;
            } else {
                // set default value
                actionStepLength = string2time(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_ACTIONSTEPLENGTH_SET;
            }
            break;
        case SUMO_ATTR_PROB:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                defaultProbability = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_PROBABILITY_SET;
            } else {
                // set default value
                defaultProbability = parse<double>(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_PROBABILITY_SET;
            }
            break;
        case SUMO_ATTR_HASDRIVERSTATE:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                hasDriverState = parse<bool>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_HASDRIVERSTATE_SET;
            } else {
                // set default value
                hasDriverState = parse<bool>(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_HASDRIVERSTATE_SET;
            }
            break;
        case SUMO_ATTR_OSGFILE:
            if (!value.empty() && (value != defaultValues.osgFile)) {
                osgFile = value;
                // mark parameter as set
                parametersSet |= VTYPEPARS_OSGFILE_SET;
            } else {
                // set default value
                osgFile = defaultValues.osgFile;
                // unset parameter
                parametersSet &= ~VTYPEPARS_OSGFILE_SET;
            }
            break;
        case SUMO_ATTR_CARRIAGE_LENGTH:
            if (!value.empty() && (value != toString(defaultValues.carriageLength))) {
                carriageLength = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_CARRIAGE_LENGTH_SET;
                // set parameter in SUMOVTypeParameter (needed for writting in XML)
                SUMOVTypeParameter::setParameter(toString(key), value);
            } else {
                // set default value
                carriageLength = defaultValues.carriageLength;
                // unset parameter
                parametersSet &= ~VTYPEPARS_CARRIAGE_LENGTH_SET;
                // remove from params (needed for writting in XML)
                SUMOVTypeParameter::unsetParameter(toString(key));
            }
            break;
        case SUMO_ATTR_LOCOMOTIVE_LENGTH:
            if (!value.empty() && (value != toString(defaultValues.containerCapacity))) {
                locomotiveLength = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_LOCOMOTIVE_LENGTH_SET;
                // set parameter in SUMOVTypeParameter (needed for writting in XML)
                SUMOVTypeParameter::setParameter(toString(key), value);
            } else {
                // set default value
                locomotiveLength = defaultValues.locomotiveLength;
                // unset parameter
                parametersSet &= ~VTYPEPARS_LOCOMOTIVE_LENGTH_SET;
                // remove from params (needed for writting in XML)
                SUMOVTypeParameter::unsetParameter(toString(key));
            }
            break;
        case SUMO_ATTR_CARRIAGE_GAP:
            if (!value.empty() && (value != myTagProperty.getDefaultValue(key))) {
                carriageGap = parse<double>(value);
                // mark parameter as set
                parametersSet |= VTYPEPARS_CARRIAGE_GAP_SET;
                // set parameter in SUMOVTypeParameter (needed for writting in XML)
                SUMOVTypeParameter::setParameter(toString(key), value);
            } else {
                // set default value
                carriageGap = parse<double>(myTagProperty.getDefaultValue(key));
                // unset parameter
                parametersSet &= ~VTYPEPARS_CARRIAGE_GAP_SET;
                // remove from params (needed for writting in XML)
                SUMOVTypeParameter::unsetParameter(toString(key));
            }
            break;
        case GNE_ATTR_GENERIC:
            setGenericParametersStr(value);
            break;
        case GNE_ATTR_DEFAULT_VTYPE_MODIFIED:
            myDefaultVehicleTypeModified = parse<bool>(value);
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNEVehicleType::setEnabledAttribute(const int /*enabledAttributes*/) {
    //
}


void
GNEVehicleType::updateDefaultVClassAttributes(const VClassDefaultValues& defaultValues) {
    if (!wasSet(VTYPEPARS_LENGTH_SET)) {
        length = defaultValues.length;
    }
    if (!wasSet(VTYPEPARS_MINGAP_SET)) {
        minGap = defaultValues.minGap;
    }
    if (!wasSet(VTYPEPARS_MAXSPEED_SET)) {
        maxSpeed = defaultValues.maxSpeed;
    }
    if (!wasSet(VTYPEPARS_WIDTH_SET)) {
        width = defaultValues.width;
    }
    if (!wasSet(VTYPEPARS_HEIGHT_SET)) {
        height = defaultValues.height;
    }
    if (!wasSet(VTYPEPARS_SHAPE_SET)) {
        shape = defaultValues.shape;
    }
    if (!wasSet(VTYPEPARS_EMISSIONCLASS_SET)) {
        emissionClass = defaultValues.emissionClass;
    }
    if (!wasSet(VTYPEPARS_SPEEDFACTOR_SET)) {
        speedFactor = defaultValues.speedFactor;
    }
    if (!wasSet(VTYPEPARS_PERSON_CAPACITY)) {
        personCapacity = defaultValues.personCapacity;
    }
    if (!wasSet(VTYPEPARS_CONTAINER_CAPACITY)) {
        containerCapacity = defaultValues.containerCapacity;
    }
    if (!wasSet(VTYPEPARS_OSGFILE_SET)) {
        osgFile = defaultValues.osgFile;
    }
    if (!wasSet(VTYPEPARS_CARRIAGE_LENGTH_SET)) {
        carriageLength = defaultValues.carriageLength;
    }
    if (!wasSet(VTYPEPARS_LOCOMOTIVE_LENGTH_SET)) {
        locomotiveLength = defaultValues.locomotiveLength;
    }
}

/****************************************************************************/
