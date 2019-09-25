/* 
 * File:   Request.h
 * Author: amin
 *
 * Created on September 23, 2019, 2:59 PM
 */

#ifndef REQUEST_H
#define	REQUEST_H

#include "Station.h"
#include "thread.h"

class Request {
public:
    static int lastId;
    Request();
    virtual ~Request();
    void setDepStation(Station*);
    void setDesStation(Station*);
    void setDepTime(int);
    void setSeatType(bool);
    void setPassengerCount(int);
    void setThread(Thread*);
    Station* getDepStation();
    Station* getDesStation();
    int getPassengerCount();
    Thread* getThread();
    bool isBusiness();
    void setOnTrain();
    void setFinished();
    bool isFinished();
    int getId();
private:
    int id;
    Station* depStation;
    Station* desStation;
    int depTime;
    bool seatType; //true for business
    int passengersCount;
    Thread* reqThread;
    bool onTrain,finished;
    
    
};

#endif	/* REQUEST_H */

