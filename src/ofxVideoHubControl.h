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
    
    void checkRouting ();
    void checkLabels ();
    
    string getReceiveStatus();
    
    bool isInputLabelsUpdate();
    bool isOutputLabelsUpdate();    
    bool isRoutingUpdate();
    
    void setInputLabel(int idLabel, string label);
    void setOutputLabel(int idLabel, string label);
    map <int, string> getInputLabels();
    map <int, string> getOutputLabels();
    
    
    
private:
    /* route map, output use as key*/
    map <int, int> routingMatrix;

    map <int, string> inputLabelTable;
    map <int, string> outputLabelTable;
    
    ofxTCPClient videohubClient;
    
    string addr;
    
    float counter;
    int connectTime;
    int deltaTime;
    
    bool weConnected;
    
    string rcvStatus;
    
    bool routingUpdate, inputLabelsUpdate, outputLabelsUpdate ;
};
