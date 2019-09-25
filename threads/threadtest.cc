#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "list.h"
#include "Simulator.h"
#include <stdlib.h>
#include <time.h>
#include "Request.h"
#include "Station.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "Train.h"

using namespace std;

std::vector<std::string> split(std::string str, std::string sep)
{
	char* cstr = const_cast<char*> (str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}


Simulator sim;
int hour, minute;
Thread* trainT[1000];
Thread* reqT[1000];
int numOfStations = 0;
void trainThread(Train* train);
void reservationThread(Request* req);

void adminThread(int x)
{
	kernel->interrupt->SetLevel(IntOff);

	srand(time(NULL));
	Train::lastId = 0;
	Request::lastId = 0;
	//read from files
	ifstream in;
	string line;
	in.open("../threads/in.txt");
	while (getline(in, line)) {
		Train* train = new Train();
		trainT[train->getId()] = new Thread("train thread");
		//also set train info here
		vector<string> stations;
		stations = split(line, "-");
		int i;
		for (i = 0; i < stations.size(); i++) {
			vector<string> stationInfo;
			stationInfo = split(stations[i], "*");
			int depTime = stoi(split(stationInfo[1], ":")[0])*60 + stoi(split(stationInfo[1], ":")[1]);
			train->addStation(stoi(stationInfo[0]), depTime, stoi(stationInfo[1]), stoi(stationInfo[2]));
			if (sim.addStation(stoi(stationInfo[0]), depTime, stoi(stationInfo[1]), stoi(stationInfo[2])))
				numOfStations++;
		}
		sim.addTrain(train);

		trainT[train->getId()]->Fork((VoidFunctionPtr) trainThread, (void*) train);
	}

	for (hour = 6; hour < 23; hour++) {

		for (minute = 0; minute < 60; minute += 10) {
			printf("TIME %d:%d\n", hour, minute);
			int reqCount;
			for (reqCount = 0; reqCount < 5; reqCount++) {
				Request* req = new Request();
				reqT[req->getId()] = new Thread("forked thread");
				reqT[req->getId()]->Fork((VoidFunctionPtr) reservationThread, (void *) req);
			}
			kernel->currentThread->Yield();
		}

	}
}

void trainThread(Train* train)
{
	printf("*I'm in train thread\n");
	kernel->interrupt->SetLevel(IntOff);

	while (true) { //or hour<23?
		//check train arrival-departure hours and see if it's arrival time
		List<Station*> stations = train->getStations();
		ListIterator<Station*> stationsIter(&stations);
		for (; !stationsIter.IsDone(); stationsIter.Next()) {
			int t = stationsIter.Item()->getDepTime();

			if ((int) (t - 10) / 60 == hour && (t - 10) % 60 == minute) { //arrival
				List<Request*> requests = train->getRequests();
				ListIterator<Request*> reqIter(&requests);

				for (; !reqIter.IsDone(); reqIter.Next()) {
					if (reqIter.Item()->getDepStation()->getId() == stationsIter.Item()->getId()) {
						train->processRequestArrive(reqIter.Item());
						printf("00000 Train %d arrived for request %d\n", train->getId(), reqIter.Item()->getId());
						kernel->scheduler->ReadyToRun(reqT[reqIter.Item()->getId()]);
					}
				}

			}
			if ((int) t / 60 == hour && t % 60 == minute) { //departure
				List<Request*> requests = train->getRequests();

				ListIterator<Request*> reqIter(&requests);

				for (; !reqIter.IsDone(); reqIter.Next()) {
					if (reqIter.Item()->getDesStation()->getId() == stationsIter.Item()->getId()) {
						train->processRequestDepart(reqIter.Item());
						printf("00000 Train %d reached destination for request %d\n", train->getId(), reqIter.Item()->getId());
						kernel->scheduler->ReadyToRun(reqT[reqIter.Item()->getId()]);
					}
				}
			}

		}
		kernel->currentThread->Yield();
	}

}

void reservationThread(Request* req)
{
	printf("*I'm in reservation thread\n");
	kernel->interrupt->SetLevel(IntOff);
	printf("num of stations %d\n", numOfStations);
	int depStationId = rand() % numOfStations + 1;
	int desStationId = rand() % numOfStations + 1;
	while (depStationId == desStationId)
		desStationId = rand() % numOfStations + 1;
	if (desStationId < depStationId) {
		int temp = desStationId;
		desStationId = depStationId;
		depStationId = temp;
	}
	printf("Creating new request %d with departure station %d and destination station %d\n", req->getId(), depStationId, desStationId);
	Station* depStation = new Station();
	depStation = sim.getStationById(depStationId);
	Station* desStation = new Station();
	desStation = sim.getStationById(desStationId);
	req->setSeatType((rand() % 2) == 1 ? true : false);
	req->setPassengerCount(rand() % 8 + 1);
	req->setDepStation(depStation);
	req->setDesStation(desStation);
	//done in addrequest: check if there is any train with appropriate path and seats to handle this req
	if (sim.addRequest(req))
		kernel->currentThread->Sleep(false);
	else
		kernel->currentThread->Finish();
	//at this point, a train has been assigned to req and arrived, wake up and load passengers
	kernel->currentThread->Sleep(false);
	//now get the passengers off the train
	kernel->currentThread->Finish();
}

void
ThreadTest()
{

	kernel->interrupt->SetLevel(IntOff);
	Thread *adminT = new Thread("admin thread");
	adminT->Fork((VoidFunctionPtr) adminThread, (void*) 1);
}
