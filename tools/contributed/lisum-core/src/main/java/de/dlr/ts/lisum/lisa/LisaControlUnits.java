/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2016-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    LisaControlUnits.java
/// @author  Maximiliano Bottazzi
/// @date    2016
/// @version $Id$
///
//
/****************************************************************************/
package de.dlr.ts.lisum.lisa;

import de.dlr.ts.commons.logger.DLRLogger;
import de.dlr.ts.lisum.exceptions.LisaRESTfulServerNotFoundException;

import de.dlr.ts.lisum.interfaces.ControlUnitInterface;
import java.util.ArrayList;
import java.util.List;


/**
 *
 * @author @author <a href="mailto:maximiliano.bottazzi@dlr.de">Maximiliano Bottazzi</a>
 */
public class LisaControlUnits {
    private final List<LisaControlUnit> controlUnits = new ArrayList<>();

    /**
     *
     * @param aThis
     * @param configurationFiles
     * @param lisaCommands
     */
    void load(LisaConfigurationFiles configurationFiles, LisaCommands lisaCommands) {
        for (LisaConfigurationFiles.ControlUnit cu : configurationFiles) {
            DLRLogger.config(this, "Creating Control Unit " + cu.fullName);

            LisaControlUnit lisaControlUnit = new LisaControlUnit();
            lisaControlUnit.setCommands(lisaCommands);
            lisaControlUnit.setLisaDirectory(configurationFiles.getLisaDirectory());
            lisaControlUnit.load(cu);
            controlUnits.add(lisaControlUnit);
        }
    }

    /**
     *
     * @return
     */
    public int getControlUnitsCount() {
        return controlUnits.size();
    }

    /**
     *
     * @param controlUnit
     */
    void addControlUnit(LisaControlUnit controlUnit) {
        controlUnits.add(controlUnit);
    }

    /**
     *
     * @return
     */
    public ControlUnitInterface[] getControlUnits() {
        ControlUnitInterface[] array = new ControlUnitInterface[controlUnits.size()];
        return controlUnits.toArray(array);
    }

    /**
     *
     * @param name
     * @return
     */
    public ControlUnitInterface getControlUnit(String name) {
        for (LisaControlUnit controlUnit : controlUnits)
            if (controlUnit.getFullName().equals(name)) {
                return controlUnit;
            }

        return null;
    }

    /**
     *
     * @param simulationTime
     * @throws de.dlr.ts.lisum.exceptions.LisaRESTfulServerNotFoundException
     */
    public void executeSimulationStep(long simulationTime) throws LisaRESTfulServerNotFoundException {
        for (LisaControlUnit sgt : controlUnits) {
            DLRLogger.finer(this, "Executing on " + sgt);
            sgt.executeSimulationStep(simulationTime);
        }

    }

    /**
     *
     * @throws LisaRESTfulServerNotFoundException
     */
    public void initBeforePlay() throws LisaRESTfulServerNotFoundException {
        for (LisaControlUnit lisaControlUnit : controlUnits) {
            lisaControlUnit.initBeforePlay();
        }
    }

}
