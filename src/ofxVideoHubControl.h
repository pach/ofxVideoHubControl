//
//  ofxVideoHubControl.h
//  videohub_control
//
//  Created by Thomas Pachoud on 09/01/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofxTCPClient.h"
#include "ofMain.h"

#define VIDEOHUB_PORT 9990
#define VIDEOHUB_ADDR "localhost"

class ofxVideoHubControl {
public:
    void setup();
    void setAddress(string address);
    void update();
    
    map <int, int> getRoutingMatrix ();
    void setRoute (int output, int input);
    void checkRoute ();
    
    string getReceiveStatus();
    
private:
    /* route map, output use as key*/
    map <int, int> routingMatrix;
    
    ofxTCPClient videohubClient;
    
    string addr;
    
    float counter;
    int connectTime;
    int deltaTime;
    
    bool weConnected;
    
    string rcvStatus;
};
