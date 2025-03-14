/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEDetectorE1.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Nov 2015
/// @version $Id$
///
//
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================

#include <netedit/GNENet.h>
#include <netedit/GNEUndoList.h>
#include <netedit/GNEViewNet.h>
#include <netedit/changes/GNEChange_Attribute.h>
#include <netedit/netelements/GNEEdge.h>
#include <netedit/netelements/GNELane.h>
#include <utils/gui/div/GLHelper.h>
#include <utils/gui/globjects/GLIncludes.h>

#include "GNEDetectorE1.h"
#include "GNEAdditionalHandler.h"


// ===========================================================================
// member method definitions
// ===========================================================================

GNEDetectorE1::GNEDetectorE1(const std::string& id, GNELane* lane, GNEViewNet* viewNet, double pos, SUMOTime freq, const std::string& filename, const std::string& vehicleTypes, const std::string& name, bool friendlyPos, bool blockMovement) :
    GNEDetector(id, viewNet, GLO_E1DETECTOR, SUMO_TAG_E1DETECTOR, pos, freq, filename, vehicleTypes, name, friendlyPos, blockMovement, {
    lane
}) {
}


GNEDetectorE1::~GNEDetectorE1() {
}


bool
GNEDetectorE1::isAdditionalValid() const {
    // with friendly position enabled position are "always fixed"
    if (myFriendlyPosition) {
        return true;
    } else {
        return fabs(myPositionOverLane) <= getLaneParents().front()->getParentEdge().getNBEdge()->getFinalLength();
    }
}


std::string
GNEDetectorE1::getAdditionalProblem() const {
    // declare variable for error position
    std::string errorPosition;
    const double len = getLaneParents().front()->getParentEdge().getNBEdge()->getFinalLength();
    // check positions over lane
    if (myPositionOverLane < -len) {
        errorPosition = (toString(SUMO_ATTR_POSITION) + " < 0");
    }
    if (myPositionOverLane > len) {
        errorPosition = (toString(SUMO_ATTR_POSITION) + " > lanes's length");
    }
    return errorPosition;
}


void
GNEDetectorE1::fixAdditionalProblem() {
    // declare new position
    double newPositionOverLane = myPositionOverLane;
    // fix pos and lenght  checkAndFixDetectorPosition
    GNEAdditionalHandler::checkAndFixDetectorPosition(newPositionOverLane, getLaneParents().front()->getParentEdge().getNBEdge()->getFinalLength(), true);
    // set new position
    setAttribute(SUMO_ATTR_POSITION, toString(newPositionOverLane), myViewNet->getUndoList());
}


void
GNEDetectorE1::moveGeometry(const Position& offset) {
    // Calculate new position using old position
    Position newPosition = myMove.originalViewPosition;
    newPosition.add(offset);
    // filtern position using snap to active grid
    newPosition = myViewNet->snapToActiveGrid(newPosition);
    const bool storeNegative = myPositionOverLane < 0;
    myPositionOverLane = getLaneParents().front()->getGeometry().shape.nearest_offset_to_point2D(newPosition, false);
    if (storeNegative) {
        myPositionOverLane -= getLaneParents().front()->getParentEdge().getNBEdge()->getFinalLength();
    }
    // Update geometry
    updateGeometry();
}


void
GNEDetectorE1::commitGeometryMoving(GNEUndoList* undoList) {
    // commit new position allowing undo/redo
    undoList->p_begin("position of " + getTagStr());
    undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), SUMO_ATTR_POSITION, toString(myPositionOverLane), true, myMove.firstOriginalLanePosition));
    undoList->p_end();
}


void
GNEDetectorE1::updateGeometry() {
    // Clear all containers
    myGeometry.clearGeometry();

    // obtain position over lane
    myGeometry.shape.push_back(getPositionInView());

    // Obtain first position
    Position f = myGeometry.shape[0] - Position(1, 0);

    // Obtain next position
    Position s = myGeometry.shape[0] + Position(1, 0);

    // Save rotation (angle) of the vector constructed by points f and s
    myGeometry.shapeRotations.push_back(getLaneParents().front()->getGeometry().shape.rotationDegreeAtOffset(getGeometryPositionOverLane()) * -1);

    // Set block icon position
    myBlockIcon.position = myGeometry.shape.getLineCenter();

    // Set offset of the block icon
    myBlockIcon.offset = Position(-1, 0);

    // Set block icon rotation, and using their rotation for logo
    myBlockIcon.setRotation(getLaneParents().front());
}


void
GNEDetectorE1::drawGL(const GUIVisualizationSettings& s) const {
    // get values
    const double width = (double) 2.0 * s.scale;
    const double exaggeration = s.addSize.getExaggeration(s, this);
    // start drawing
    glPushName(getGlID());
    glLineWidth(1.0);
    // set color
    if (drawUsingSelectColor()) {
        GLHelper::setColor(s.colorSettings.selectedAdditionalColor);
    } else {
        GLHelper::setColor(s.colorSettings.E1);
    }
    // draw shape
    glPushMatrix();
    glTranslated(0, 0, getType());
    glTranslated(myGeometry.shape[0].x(), myGeometry.shape[0].y(), 0);
    glRotated(myGeometry.shapeRotations[0], 0, 0, 1);
    glScaled(exaggeration, exaggeration, 1);
    glBegin(GL_QUADS);
    glVertex2d(-1.0,  2);
    glVertex2d(-1.0, -2);
    glVertex2d(1.0, -2);
    glVertex2d(1.0,  2);
    glEnd();
    glTranslated(0, 0, .01);
    glBegin(GL_LINES);
    glVertex2d(0, 2 - .1);
    glVertex2d(0, -2 + .1);
    glEnd();
    // outline if isn't being drawn for selecting
    if ((width * exaggeration > 1) && !s.drawForSelecting) {
        // set color
        if (drawUsingSelectColor()) {
            GLHelper::setColor(s.colorSettings.selectionColor);
        } else {
            GLHelper::setColor(RGBColor::WHITE);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
        glVertex2f(-1.0,  2);
        glVertex2f(-1.0, -2);
        glVertex2f(1.0, -2);
        glVertex2f(1.0,  2);
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // position indicator if isn't being drawn for selecting
    if ((width * exaggeration > 1) && !s.drawForSelecting) {
        // set color
        if (drawUsingSelectColor()) {
            GLHelper::setColor(s.colorSettings.selectionColor);
        } else {
            GLHelper::setColor(RGBColor::WHITE);
        }
        glRotated(90, 0, 0, -1);
        glBegin(GL_LINES);
        glVertex2d(0, 1.7);
        glVertex2d(0, -1.7);
        glEnd();
    }
    // Pop shape matrix
    glPopMatrix();
    // Check if the distance is enought to draw details and isn't being drawn for selecting
    if ((s.drawDetail(s.detailSettings.detectorDetails, exaggeration)) && !s.drawForSelecting) {
        // Push matrix
        glPushMatrix();
        // Traslate to center of detector
        glTranslated(myGeometry.shape.getLineCenter().x(), myGeometry.shape.getLineCenter().y(), getType() + 0.1);
        // Rotate depending of myBlockIcon.rotation
        glRotated(myBlockIcon.rotation, 0, 0, -1);
        //move to logo position
        glTranslated(-1, 0, 0);
        // draw E1 logo
        if (drawUsingSelectColor()) {
            GLHelper::drawText("E1", Position(), .1, 1.5, s.colorSettings.selectionColor);
        } else {
            GLHelper::drawText("E1", Position(), .1, 1.5, RGBColor::BLACK);
        }
        // pop matrix
        glPopMatrix();
        // Show Lock icon depending of the Edit mode
        myBlockIcon.drawIcon(s, exaggeration);
    }
    // Finish draw if isn't being drawn for selecting
    if (!s.drawForSelecting) {
        drawName(getPositionInView(), s.scale, s.addName);
    }
    // check if dotted contour has to be drawn
    if (myViewNet->getDottedAC() == this) {
        GLHelper::drawShapeDottedContourRectangle(s, getType(), myGeometry.shape[0], 2, 4, myGeometry.shapeRotations[0]);
    }
    // pop name
    glPopName();
}


std::string
GNEDetectorE1::getAttribute(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ID:
            return getAdditionalID();
        case SUMO_ATTR_LANE:
            return getLaneParents().front()->getID();
        case SUMO_ATTR_POSITION:
            return toString(myPositionOverLane);
        case SUMO_ATTR_FREQUENCY:
            return time2string(myFreq);
        case SUMO_ATTR_NAME:
            return myAdditionalName;
        case SUMO_ATTR_FILE:
            return myFilename;
        case SUMO_ATTR_VTYPES:
            return myVehicleTypes;
        case SUMO_ATTR_FRIENDLY_POS:
            return toString(myFriendlyPosition);
        case GNE_ATTR_BLOCK_MOVEMENT:
            return toString(myBlockMovement);
        case GNE_ATTR_SELECTED:
            return toString(isAttributeCarrierSelected());
        case GNE_ATTR_GENERIC:
            return getGenericParametersStr();
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNEDetectorE1::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
        case SUMO_ATTR_ID:
        case SUMO_ATTR_LANE:
        case SUMO_ATTR_POSITION:
        case SUMO_ATTR_FREQUENCY:
        case SUMO_ATTR_NAME:
        case SUMO_ATTR_FILE:
        case SUMO_ATTR_VTYPES:
        case SUMO_ATTR_FRIENDLY_POS:
        case GNE_ATTR_BLOCK_MOVEMENT:
        case GNE_ATTR_SELECTED:
        case GNE_ATTR_GENERIC:
            undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), key, value));
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }

}


bool
GNEDetectorE1::isValid(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            return isValidDetectorID(value);
        case SUMO_ATTR_LANE:
            if (myViewNet->getNet()->retrieveLane(value, false) != nullptr) {
                return true;
            } else {
                return false;
            }
        case SUMO_ATTR_POSITION:
            return canParse<double>(value) && fabs(parse<double>(value)) < getLaneParents().front()->getParentEdge().getNBEdge()->getFinalLength();
        case SUMO_ATTR_FREQUENCY:
            return (canParse<double>(value) && (parse<double>(value) >= 0));
        case SUMO_ATTR_NAME:
            return SUMOXMLDefinitions::isValidAttribute(value);
        case SUMO_ATTR_FILE:
            return SUMOXMLDefinitions::isValidFilename(value);
        case SUMO_ATTR_VTYPES:
            if (value.empty()) {
                return true;
            } else {
                return SUMOXMLDefinitions::isValidListOfTypeID(value);
            }
        case SUMO_ATTR_FRIENDLY_POS:
            return canParse<bool>(value);
        case GNE_ATTR_BLOCK_MOVEMENT:
            return canParse<bool>(value);
        case GNE_ATTR_SELECTED:
            return canParse<bool>(value);
        case GNE_ATTR_GENERIC:
            return isGenericParametersValid(value);
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}

// ===========================================================================
// private
// ===========================================================================

void
GNEDetectorE1::setAttribute(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            changeAdditionalID(value);
            break;
        case SUMO_ATTR_LANE:
            changeLaneParents(this, value);
            break;
        case SUMO_ATTR_POSITION:
            myPositionOverLane = parse<double>(value);
            break;
        case SUMO_ATTR_FREQUENCY:
            myFreq = parse<SUMOTime>(value);
            break;
        case SUMO_ATTR_FILE:
            myFilename = value;
            break;
        case SUMO_ATTR_NAME:
            myAdditionalName = value;
            break;
        case SUMO_ATTR_VTYPES:
            myVehicleTypes = value;
            break;
        case SUMO_ATTR_FRIENDLY_POS:
            myFriendlyPosition = parse<bool>(value);
            break;
        case GNE_ATTR_BLOCK_MOVEMENT:
            myBlockMovement = parse<bool>(value);
            break;
        case GNE_ATTR_SELECTED:
            if (parse<bool>(value)) {
                selectAttributeCarrier();
            } else {
                unselectAttributeCarrier();
            }
            break;
        case GNE_ATTR_GENERIC:
            setGenericParametersStr(value);
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}

/****************************************************************************/
