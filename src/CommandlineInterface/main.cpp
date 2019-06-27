// ================================================================================================
// 
// If not explicitly stated: Copyright (C) 2016, all rights reserved,
//      Rüdiger Göbl 
//		Email r.goebl@tum.de
//      Chair for Computer Aided Medical Procedures
//      Technische Universität München
//      Boltzmannstr. 3, 85748 Garching b. München, Germany
// 
// ================================================================================================

#include <utilities/Logging.h>

#include <SupraManager.h>
#include <chrono>
#include <thread>
#include <string>
#include "CommandlineInterface.h"

using namespace supra;
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

int main(int argc, char** argv) {
	logging::Base::setLogLevel(logging::info);

	if (argc == 1)
	{
		logging::log_always("Usage: SUPRA_CMD <config.xml> [--test]");
	}
	else if (argc >= 2)
	{
		SupraManager::Get()->readFromXml(argv[1]);

		if (argc == 3)
		{
			logging::Base::setLogLevel(
				logging::log |
				logging::info);

			logging::log_always("Nodes:");
			for (string nodeID : SupraManager::Get()->getNodeIDs())
			{
				logging::log_always(nodeID);
			}
			SupraManager::Get()->startOutputs();
			SupraManager::Get()->startInputs();
			sleep_until(system_clock::now() + seconds(5));
		}
		else
		{
			SupraManager::Get()->startOutputs();
			SupraManager::Get()->startInputs();

			//Interface
			CommandlineInterface ui;
			ui.mainMenu();
		}

		//stop inputs
		SupraManager::Get()->stopAndWaitInputs();

		//wait for remaining messages to be processed
		SupraManager::Get()->waitForGraph();
	}
}
