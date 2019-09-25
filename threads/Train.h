/* 
 * File:   Train.h
 * Author: amin
 *
 * Created on September 6, 2019, 4:32 PM
 */

#ifndef TRAIN_H
#define	TRAIN_H

#include "list.h"
#include "Station.h"
#include "Request.h"
#include "thread.h"

class Train {
public:
    static int lastId;
    Train();
    virtual ~Train();
    List<Station*> getStations();
    void addRequest(Request*);
    bool hasRoom(int, bool);
    List<int> getDepTimes();
    List<Request*> getRequests();
    bool processRequestArrive(Request*);
    void processRequestDepart(Request*);
    void addStation(int, int, int, int);
    int getId();
    friend bool operator==(const Train&, const Train&);
    bool findTrain(Request*);
private:
    int id;
    List<Station*> stations;
    List<int> departureTime;
    List<int> faresCoach, faresBusiness;
    const int seatCountCoach = 40;
    const int seatCountBusiness = 20;
    List<Request*> requests;

};

#endif	/* TRAIN_H */

