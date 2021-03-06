#include "commands/DeviceUnpairCommand.h"

using namespace BeeeOn;
using namespace std;

DeviceUnpairCommand::DeviceUnpairCommand(const DeviceID &deviceID):
	m_deviceID(deviceID)
{
}

DeviceUnpairCommand::~DeviceUnpairCommand()
{
}

DeviceID DeviceUnpairCommand::deviceID() const
{
	return m_deviceID;
}

string DeviceUnpairCommand::toString() const
{
	return name() + " " + m_deviceID.toString();
}
