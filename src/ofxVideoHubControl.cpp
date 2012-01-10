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
    
    routingUpdate = outputLabelsUpdate = inputLabelsUpdate = false;
    
    weConnected = videohubClient.setup(addr, VIDEOHUB_PORT);
    videohubClient.setMessageDelimiter("\n");
    videohubClient.setVerbose("true");
    
    videohubClient.send("\n\n");
    update();
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
            cout<<"receiving : "<<rcvMsg<<"\n----- go into"<<endl;
            rcvStatus += rcvMsg;
            rcvStatus += "\n--\n";
            
            int delimiter = rcvMsg.find_first_of("\n");
            string cmd = rcvMsg.substr(0, delimiter);
            rcvMsg = rcvMsg.substr(delimiter+1, rcvMsg.size());
            
            if (cmd.compare("VIDEO OUTPUT ROUTING:")==0){
                
                printf("video routing stuff :\n");
                
                delimiter = rcvMsg.find_first_of("\n");
                while (delimiter != string::npos){
                    cmd = rcvMsg.substr(0, delimiter);
                    rcvMsg = rcvMsg.substr(delimiter+1, rcvMsg.size());
                    
                    int input, output;
                    delimiter = cmd.find_first_of(" ");
                    output = ofToInt(cmd.substr(0, delimiter));
                    input = ofToInt(cmd.substr(delimiter+1, 3));    
                    
                    printf("o:i %i %i\n", output, input);
                    
                    map<int, int>::iterator it = routingMatrix.find(output);
                    if (it==routingMatrix.end()){
                        routingMatrix.insert(pair<int, int>(output, input));
                    }
                    else {
                        it->second = input;
                    }
                    
                    delimiter = rcvMsg.find_first_of("\n");
                }
                routingUpdate = true;
            }
            if (cmd.compare("INPUT LABELS:")==0){
                
                printf("input labels stuff:\n");
                
                delimiter = rcvMsg.find_first_of("\n");
                while (delimiter != string::npos){
                    cmd = rcvMsg.substr(0, delimiter);
                    rcvMsg = rcvMsg.substr(delimiter+1, rcvMsg.size());

                    int input;
                    string labels;
                    delimiter = cmd.find_first_of(" ");
                    input = ofToInt(cmd.substr(0, delimiter));                 
                    labels = ofToString(cmd.substr(delimiter+1, cmd.size()));
                    
                    cout<<"i:l "<<input<<":"<<labels<<endl;
                    
                    map<int, string>::iterator it = inputLabelTable.find(input);
                    if (it==inputLabelTable.end()){
                        inputLabelTable.insert(pair<int, string>(input, labels));
                    }
                    else {
                        it->second = labels;
                    }
                    
                    delimiter = rcvMsg.find_first_of("\n");
                }
                inputLabelsUpdate=true;
            }
            if (cmd.compare("OUTPUT LABELS:")==0){
                
                printf("output labels stuff:\n");
                
                delimiter = rcvMsg.find_first_of("\n");
                while (delimiter != string::npos){
                    cmd = rcvMsg.substr(0, delimiter);
                    rcvMsg = rcvMsg.substr(delimiter+1, rcvMsg.size());
                    
                    int output;
                    string labels;
                    delimiter = cmd.find_first_of(" ");
                    output = ofToInt(cmd.substr(0, delimiter));                 
                    labels = ofToString(cmd.substr(delimiter+1, cmd.size()));
                    
                    cout<<"o:l "<<output<<":"<<labels<<endl;
                    
                    map<int, string>::iterator it = outputLabelTable.find(output);
                    if (it==outputLabelTable.end()){
                        outputLabelTable.insert(pair<int, string>(output, labels));
                    }
                    else {
                        it->second = labels;
                    }
                    
                    delimiter = rcvMsg.find_first_of("\n");
                }
                outputLabelsUpdate = true;
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

bool ofxVideoHubControl::isInputLabelsUpdate(){
    if (inputLabelsUpdate){
        inputLabelsUpdate = false;
        return true;
    }
    else {
        return false;
    }
}

bool ofxVideoHubControl::isOutputLabelsUpdate(){
    if (outputLabelsUpdate){
        outputLabelsUpdate = false;
        return true;
    }
    else {
        return false;
    }
}

bool ofxVideoHubControl::isRoutingUpdate(){
    if (routingUpdate){
        routingUpdate = false;
        return true;
    }
    else {
        return false;
    }
}


map<int, int> ofxVideoHubControl::getRoutingMatrix (){
    return routingMatrix;
}

map<int, string> ofxVideoHubControl::getInputLabels (){
    return inputLabelTable;
}

map<int, string> ofxVideoHubControl::getOutputLabels (){
    return outputLabelTable;
}




void ofxVideoHubControl::checkRouting(){
    string cmd = "VIDEO OUTPUT ROUTING:\n\n";
    videohubClient.send(cmd);
}

void ofxVideoHubControl::checkLabels(){
    string cmd = "VIDEO INPUT LABELS:\n\n";
    videohubClient.send(cmd);
    cmd = "VIDEO OUTPUT LABELS:\n\n";
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

void ofxVideoHubControl::setInputLabel(int idLabel, string label){
    string cmd = "INPUT LABELS:\n";
    cmd += ofToString(idLabel);
    cmd += " new input label ";
    cmd += ofToString(label);
    cmd += "\n\n";
    cout<<"sending : "<<cmd<<endl;
    videohubClient.send(cmd);
}

void ofxVideoHubControl::setOutputLabel(int idLabel, string label){
string cmd = "OUTPUT LABELS:\n";
cmd += ofToString(idLabel);
cmd += " new output label ";
cmd += ofToString(label);
cmd += "\n\n";
cout<<"sending : "<<cmd<<endl;
videohubClient.send(cmd);
}

string ofxVideoHubControl::getReceiveStatus(){
    string rcvReturn = rcvStatus;
    rcvStatus = "";
    return rcvReturn;
}

