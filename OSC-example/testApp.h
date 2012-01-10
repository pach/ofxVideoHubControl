#pragma once

#include "ofMain.h"
#include "ofxVideoHubControl.h"
#include "ofxOsc.h"

#define IN_PORT 11111
#define OUT_PORT 11112
#define OUT_ADDR "localhost"

class testApp : public ofBaseApp{

public:

    void setup();
    void update();
    void draw();
    
    void printRoutingMatrix();
    void getReceiveStatus();
    
    void isVideohubUpdate();

    void oscIn();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);		
    
private:
    
    ofxVideoHubControl videohub;
    string msgRx;
    
    string matrixPrint;

    int select;
    int input, output;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    int inPort, outPort;
    string outAddr;
    
};

