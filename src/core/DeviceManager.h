#ifndef BEEEON_DEVICE_MANAGER_H
#define BEEEON_DEVICE_MANAGER_H

#include <set>

#include <Poco/AtomicCounter.h>

#include "core/AnswerQueue.h"
#include "core/CommandHandler.h"
#include "core/CommandSender.h"
#include "core/Distributor.h"
#include "loop/StoppableRunnable.h"
#include "model/DeviceID.h"
#include "model/DevicePrefix.h"
#include "model/ModuleID.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * All classes that manage devices should inherit from this
 * abstract class. It provides a common functionality for this
 * purpose.
 *
 * There is usually a main thread that performs communication
 * with the physical devices and translates the specific device
 * communication protocol into the Command & Answer interface
 * or into SensorData interface.
 *
 * Communication in the direction to physical devices is served
 * via the CommandHandler interface. By accepting Commands asking
 * for specific tasks, the physical devices can be queried as
 * expected by the server.
 */
class DeviceManager:
	public CommandHandler,
	public CommandSender,
	protected Loggable,
	public StoppableRunnable {
public:
	DeviceManager(const DevicePrefix &prefix);
	virtual ~DeviceManager();

	/**
	* A generic stop implementation to be used by most DeviceManager
	* implementations. It just atomically sets the m_stop variable.
	*/
	void stop() override;

	void setDistributor(Poco::SharedPtr<Distributor> distributor);

protected:
	/**
	* Ship data received from a physical device into a collection point.
	*/
	void ship(const SensorData &sensorData);

	/**
	 * Obtain device list from server, method is blocking/non-blocking.
	 * Type of blocking is divided on the basis of timeout.
	 * Blocking waiting returns device list from server and non-blocking
	 * waiting returns device list from server or TimeoutException.
	 */
	std::set<DeviceID> deviceList(
		const Poco::Timespan &timeout = DEFAULT_REQUEST_TIMEOUT);

	/**
	 * Obtain Answer with Results which contains last measured value
	 * from server, method is blocking/non-blocking. Type of blocking
	 * is divided on the basis of timeout.
	 * Blocking waiting returns Answer with last measured value result
	 * from server and non-blocking waiting returns Answer with last
	 * measured value result from server or TimeoutException.
	 *
	 * If Answer contains several Results, the first Result SUCCESS will
	 * be selected.
	 */
	double lastValue(const DeviceID &deviceID, const ModuleID &moduleID,
		const Poco::Timespan &waitTime = DEFAULT_REQUEST_TIMEOUT);

private:
	void requestDeviceList(Answer::Ptr answer);
	std::set<DeviceID> responseDeviceList(
		const Poco::Timespan &waitTime, Answer::Ptr answer);

protected:
	static const Poco::Timespan DEFAULT_REQUEST_TIMEOUT;

	Poco::AtomicCounter m_stop;
	DevicePrefix m_prefix;

private:
	Poco::SharedPtr<Distributor> m_distributor;
};

}

#endif
