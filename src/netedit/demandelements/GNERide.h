/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2016-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNERide.h
/// @author  Pablo Alvarez Lopez
/// @date    Jun 2019
/// @version $Id$
///
// A class for visualizing rides in Netedit
/****************************************************************************/
#ifndef GNERide_h
#define GNERide_h


// ===========================================================================
// included modules
// ===========================================================================


#include <utils/gui/globjects/GUIGLObjectPopupMenu.h>

#include "GNEDemandElement.h"

// ===========================================================================
// class declarations
// ===========================================================================
class GNEEdge;
class GNEConnection;
class GNEVehicle;

// ===========================================================================
// class definitions
// ===========================================================================

class GNERide : public GNEDemandElement, public Parameterised {

public:
    /**@brief parameter constructor
     * @param[in] viewNet view in which this Ride is placed
     * @param[in] personParent person parent
     * @param[in] edges list of consecutive edges of this ride
     * @param[in] arrivalPosition arrival position on the destination edge
     * @param[in] lines valid line or vehicle ids or ANY
     */
    GNERide(GNEViewNet* viewNet, GNEDemandElement* personParent, const std::vector<GNEEdge*>& edges, double arrivalPosition, const std::vector<std::string>& lines);

    /**@brief parameter constructor
     * @param[in] viewNet view in which this Ride is placed
     * @param[in] personParent person parent
     * @param[in] edges list of consecutive edges of this ride
     * @param[in] busStop destination busStop
     * @param[in] lines valid line or vehicle ids or ANY
     */
    GNERide(GNEViewNet* viewNet, GNEDemandElement* personParent, const std::vector<GNEEdge*>& edges, GNEAdditional* busStop, const std::vector<std::string>& lines);

    /// @brief destructor
    ~GNERide();

    /**@brief writte demand element element into a xml file
     * @param[in] device device in which write parameters of demand element element
     */
    void writeDemandElement(OutputDevice& device) const;

    /// @brief check if current demand element is valid to be writed into XML (by default true, can be reimplemented in children)
    bool isDemandElementValid() const;

    /// @brief return a string with the current demand element problem (by default empty, can be reimplemented in children)
    std::string getDemandElementProblem() const;

    /// @brief fix demand element problem (by default throw an exception, has to be reimplemented in children)
    void fixDemandElementProblem();

    /// @name members and functions relative to elements common to all demand elements
    /// @{
    /// @brief obtain from edge of this demand element
    GNEEdge* getFromEdge() const;

    /// @brief obtain to edge of this demand element
    GNEEdge* getToEdge() const;

    /// @brief obtain VClass related with this demand element
    SUMOVehicleClass getVClass() const;

    /// @brief get color
    const RGBColor& getColor() const;

    /// @brief compute demand element
    void compute();

    /// @}

    /// @name Functions related with geometry of element
    /// @{
    /// @brief begin geometry movement
    void startGeometryMoving();

    /// @brief end geometry movement
    void endGeometryMoving();

    /**@brief change the position of the element geometry without saving in undoList
     * @param[in] offset Position used for calculate new position of geometry without updating RTree
     */
    void moveGeometry(const Position& offset);

    /**@brief commit geometry changes in the attributes of an element after use of moveGeometry(...)
     * @param[in] undoList The undoList on which to register changes
     */
    void commitGeometryMoving(GNEUndoList* undoList);

    /// @brief update pre-computed geometry information
    void updateGeometry();

    /// @brief Returns position of additional in view
    Position getPositionInView() const;
    /// @}

    /// @name inherited from GUIGlObject
    /// @{

    /**@brief Returns an own popup-menu
     *
     * @param[in] app The application needed to build the popup-menu
     * @param[in] parent The parent window needed to build the popup-menu
     * @return The built popup-menu
     * @see GUIGlObject::getPopUpMenu
     */
    GUIGLObjectPopupMenu* getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent);

    /**@brief Returns the name of the parent object
     * @return This object's parent id
     */
    std::string getParentName() const;

    /**@brief Returns the boundary to which the view shall be centered in order to show the object
     * @return The boundary the object is within
     */
    Boundary getCenteringBoundary() const;

    /**@brief Draws the object
     * @param[in] s The settings for the current view (may influence drawing)
     * @see GUIGlObject::drawGL
     */
    void drawGL(const GUIVisualizationSettings& s) const;
    /// @}

    /// @brief inherited from GNEAttributeCarrier
    /// @{
    /// @brief select attribute carrier using GUIGlobalSelection
    void selectAttributeCarrier(bool changeFlag = true);

    /// @brief unselect attribute carrier using GUIGlobalSelection
    void unselectAttributeCarrier(bool changeFlag = true);

    /* @brief method for getting the Attribute of an XML key
    * @param[in] key The attribute key
    * @return string with the value associated to key
    */
    std::string getAttribute(SumoXMLAttr key) const;

    /* @brief method for getting the Attribute of an XML key in double format (to avoid unnecessary parse<double>(...) for certain attributes)
     * @param[in] key The attribute key
     * @return double with the value associated to key
     */
    double getAttributeDouble(SumoXMLAttr key) const;

    /* @brief method for setting the attribute and letting the object perform additional changes
    * @param[in] key The attribute key
    * @param[in] value The new value
    * @param[in] undoList The undoList on which to register changes
    * @param[in] net optionally the GNENet to inform about gui updates
    */
    void setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList);

    /* @brief method for setting the attribute and letting the object perform additional changes
    * @param[in] key The attribute key
    * @param[in] value The new value
    * @param[in] undoList The undoList on which to register changes
    */
    bool isValid(SumoXMLAttr key, const std::string& value);

    /* @brief method for enabling attribute
     * @param[in] key The attribute key
     * @param[in] undoList The undoList on which to register changes
     * @note only certain attributes can be enabled, and can produce the disabling of other attributes
     */
    void enableAttribute(SumoXMLAttr key, GNEUndoList* undoList);

    /* @brief method for check if the value for certain attribute is set
     * @param[in] key The attribute key
     */
    bool isAttributeEnabled(SumoXMLAttr key) const;

    /// @brief get PopPup ID (Used in AC Hierarchy)
    std::string getPopUpID() const;

    /// @brief get Hierarchy Name (Used in AC Hierarchy)
    std::string getHierarchyName() const;
    /// @}

    /// @name Functions related with Generic Parameters
    /// @{
    /// @brief return generic parameters in string format
    std::string getGenericParametersStr() const;

    /// @brief return generic parameters as vector of pairs format
    std::vector<std::pair<std::string, std::string> > getGenericParameters() const;

    /// @brief set generic parameters in string format
    void setGenericParametersStr(const std::string& value);

    /// @}

protected:
    /// @brief variable for move rides
    DemandElementMove myRideMove;

    /// @brief valid line or vehicle ids or ANY
    std::vector<std::string> myLines;

    /// @brief arrival position
    double myArrivalPosition;

private:
    /// @brief method for setting the attribute and nothing else
    void setAttribute(SumoXMLAttr key, const std::string& value);

    /// @brief method for enabling the attribute and nothing else (used in GNEChange_EnableAttribute)
    void setEnabledAttribute(const int enabledAttributes);

    /// @brief Invalidated copy constructor.
    GNERide(GNERide*) = delete;

    /// @brief Invalidated assignment operator.
    GNERide& operator=(GNERide*) = delete;
};


#endif

/****************************************************************************/

