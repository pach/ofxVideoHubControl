//
//  ofxVideoHubControl.cpp
//  videohub_control
//
//  Created by Thomas Pachoud on 09/01/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ofxVideoHubControl.h"


void ofxVideoHubControl::setup(){
    rcvStatus = "";
    addr = VIDEOHUB_ADDR;
    
    counter  = 0;
    connectTime = 0;
    deltaTime = 0;
    
    weConnected = videohubClient.setup(addr, VIDEOHUB_PORT);
    videohubClient.setMessageDelimiter("\n");
    videohubClient.setVerbose("true");
    
    videohubClient.send("\n\n");
}

void ofxVideoHubControl::setAddress(string address){
    addr = address;
    videohubClient.close();
    weConnected = videohubClient.setup(addr, VIDEOHUB_PORT);
    videohubClient.setMessageDelimiter("\n");
    videohubClient.setVerbose("true");
}

void ofxVideoHubControl::update(){
    if(!videohubClient.isConnected()){
        weConnected = false;
    }
    
    if (weConnected){
        string str;
        string rcvMsg = "";
        bool rcv = false;
        while((str = videohubClient.receive()).size() > 0){
            rcvMsg += str;
            rcvMsg += "\n";
            rcv = true;
        }
        
        if (rcv){
            cout<<"receiving : "<<rcvMsg<<endl;
            rcvStatus += rcvMsg;
            rcvStatus += "\n--\n";
            
            int delimiter = rcvMsg.find_first_of("\n");
            string cmd = rcvMsg.substr(0, delimiter);
            rcvMsg = rcvMsg.substr(delimiter+1, rcvMsg.size());
            
            if (cmd.compare("VIDEO OUTPUT ROUTING:")==0){
                delimiter = rcvMsg.find_first_of("\n");
                while (delimiter != string::npos){
                    cmd = rcvMsg.substr(0, delimiter);
                    rcvMsg = rcvMsg.substr(delimiter+1, rcvMsg.size());
                    delimiter = rcvMsg.find_first_of("\n");
                    
                    int input, output;
                    output = ofToInt(cmd.substr(0, 1));
                    input = ofToInt(cmd.substr(2, 3));                 
                    
                    map<int, int>::iterator it = routingMatrix.find(output);
                    if (it==routingMatrix.end()){
                        routingMatrix.insert(pair<int, int>(output, input));
                    }
                    else {
                        it->second = input;
                    }
                    
                }
            }
        }
    }
    else{
		//if we are not connected lets try and reconnect every 5 seconds
		deltaTime = ofGetElapsedTimeMillis() - connectTime;
        
		if( deltaTime > 5000 ){
			weConnected = videohubClient.setup("10.0.0.15", 9990);
			connectTime = ofGetElapsedTimeMillis();
		}
        
	}
}

map<int, int> ofxVideoHubControl::getRoutingMatrix (){
    return routingMatrix;
}

void ofxVideoHubControl::checkRoute(){
    string cmd = "VIDEO OUTPUT ROUTING:\n\n";
    videohubClient.send(cmd);
}

void ofxVideoHubControl::setRoute(int output, int input){
    string cmd = "VIDEO OUTPUT ROUTING:\n";
    cmd += ofToString(output);
    cmd += " ";
    cmd += ofToString(input);
    cmd += "\n\n";
    cout<<"sending : "<<cmd<<endl;
    videohubClient.send(cmd);
}

string ofxVideoHubControl::getReceiveStatus(){
    string rcvReturn = rcvStatus;
    rcvStatus = "";
    return rcvReturn;
}

