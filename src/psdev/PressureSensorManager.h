#pragma once

#include <list>
#include <string>

#include <Poco/AtomicCounter.h>
#include <Poco/Event.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "commands/DeviceAcceptCommand.h"
#include "commands/DeviceUnpairCommand.h"
#include "commands/GatewayListenCommand.h"
#include "core/DeviceManager.h"
#include "model/DeviceID.h"
#include "model/ModuleType.h"

namespace BeeeOn {

/**
 * Manager for BeeeOn gateway's internal air pressure sensor.
 * Actuall value is obtained by reading from specified file typically
 * located in sysfs.
 *
 * This class instance is restricted for managing only one physical device
 * with one module representing pressure.
 *
 * Measured values are reported periodically with specified refresh time.
 *
 * This file is typically named "pressure0_input"
 */
class PressureSensorManager : public DeviceManager {
public:

	PressureSensorManager();
	~PressureSensorManager();

	void run() override;
	void stop() override;

	bool accept(const Command::Ptr cmd) override;
	void handle(const Command::Ptr cmd, Answer::Ptr answer) override;

	void setRefresh(const Poco::Timespan &refresh);

	/**
	 * @brief sets the Path to Air Pressure Sensor device
	 * entry output file. Reading from this file invoke measuring
	 * of sensoric value and returns current measured Air pressure.
	 */
	void setPath(const std::string &path);
	void setVendor(const std::string &vendor);

	/**
	 * @brief sets the expected unit, conversion of this unit
	 * needs to be made before shipping measured value.
	 */
	void setUnit(const std::string &unit);

private:
	/**
	 * @brief obtain list of paired devices from a server and
	 * eventually sets the state of this instance to paired.
	 */
	void initialize();

	/**
	 * @brief read value of air pressure and ship it to the Distributor.
	 */
	void shipValue();
	DeviceID pairedID();

	/**
	 * @brief create DeviceID with PREFIX_PRESSURE_SENSOR. Suffix
	 * is calculated by hash of specified path to device output file.
	 */
	DeviceID buildID(const std::string &path);

	/**
	 * @brief convert the measured value to [hPa] for export of the data, based
	 * on the set unit.
	 */
	double convertToHPA(const double value);

	void handleListenCommand(const GatewayListenCommand &cmd, Answer::Ptr answer);
	void handleAcceptCommand(const DeviceAcceptCommand &cmd, Answer::Ptr answer);
	void handleUnpairCommand(const DeviceUnpairCommand &cmd, Answer::Ptr answer);

private:
	Poco::AtomicCounter m_paired;
	Poco::Timespan m_refresh;
	std::string m_vendor;
	std::string m_path;
	Poco::Event m_event;
	std::string m_unit;
};

}
