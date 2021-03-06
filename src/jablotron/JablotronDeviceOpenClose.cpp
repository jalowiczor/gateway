#include <Poco/NumberParser.h>
#include <Poco/StringTokenizer.h>

#include "jablotron/JablotronDeviceOpenClose.h"

using namespace BeeeOn;
using namespace Poco;
using namespace std;

static const ModuleID MODULE_ID_OPEN_CLOSE = 0;
static const ModuleID MODULE_ID_SECURITY_ALERT = 1;
static const ModuleID MODULE_ID_BATTERY_LEVEL = 2;

static const list<ModuleType> MODULE_TYPES = {
	ModuleType(ModuleType::Type::TYPE_OPEN_CLOSE),
	ModuleType(ModuleType::Type::TYPE_SECURITY_ALERT),
	ModuleType(ModuleType::Type::TYPE_BATTERY),
};

JablotronDeviceOpenClose::JablotronDeviceOpenClose(
		const DeviceID &deviceID, const string &name):
	JablotronDevice(deviceID, name)
{
}

SensorData JablotronDeviceOpenClose::extractSensorData(const string &message)
{
	StringTokenizer tokens(message, " ");

	SensorData sensorData;
	sensorData.setDeviceID(deviceID());

	if (tokens[2] == "SENSOR") {
		sensorData.insertValue(
			SensorValue(
				ModuleID(MODULE_ID_OPEN_CLOSE),
				parseValue(tokens[4])
			)
		);
	}
	else if (tokens[2] == "TAMPER") {
		sensorData.insertValue(
			SensorValue(
				ModuleID(MODULE_ID_SECURITY_ALERT),
				parseValue(tokens[4])
			)
		);
	}
	else if (tokens[2] != "BEACON") {
		throw InvalidArgumentException("unexpected message: " + message);
	}

	sensorData.insertValue(
		SensorValue(
			ModuleID(MODULE_ID_BATTERY_LEVEL),
			extractBatteryLevel(tokens[3])
		)
	);

	return sensorData;
}

list<ModuleType> JablotronDeviceOpenClose::moduleTypes()
{
	return MODULE_TYPES;
}

Timespan JablotronDeviceOpenClose::refreshTime()
{
	return REFRESH_TIME_SUPPORTED_BEACON;
}
