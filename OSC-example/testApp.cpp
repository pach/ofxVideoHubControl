#include "testApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void testApp::setup(){

	// we don't want to be running to fast
	ofSetVerticalSync(true);
    videohub.setup();
    videohub.setAddress("10.0.0.15");
    
    outAddr = OUT_ADDR;
    outPort = OUT_PORT;
    inPort = IN_PORT;
    sender.setup(outAddr, outPort);
    receiver.setup(inPort);
    
    select = 0;
    input = output = 0;
    msgRx = "";
    
    printRoutingMatrix();
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(230, 230, 230);
    videohub.update();
    
    oscIn();
    getReceiveStatus();
    isVideohubUpdate();
    
  //  cout<<videohub.getReceiveStatus()<<endl;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0, 0, 0);
    
    ofDrawBitmapString("status dump :", 10, 20);
    ofDrawBitmapString(msgRx, 10, 35);
    
    ofDrawBitmapString(matrixPrint, 350, 20);
}

void testApp::printRoutingMatrix(){
    // print routing matrix
//    videohub.checkRouting();
//    videohub.update();
    map<int, int> matrix = videohub.getRoutingMatrix();
    matrixPrint = "output : input\n";
    map<int, int>::iterator it = matrix.begin();
    map<int, int>::iterator itEnd = matrix.end();
    for (;it != itEnd ; it++){
        matrixPrint += ofToString(it->first);
        matrixPrint += " : ";
        matrixPrint += ofToString(it->second);
        matrixPrint += "\n";
    }
}

void testApp::oscIn(){
    ofxOscMessage m ;
    if(receiver.getNextMessage(&m)){
        
        cout<<"receive osc : "<<m.getAddress()<<endl;
        
        if (m.getAddress() == "/route/set"){
            output = m.getArgAsInt32(0);
            input = m.getArgAsInt32(1);
            videohub.setRoute(output, input);
        }
        if (m.getAddress() == "/route/get"){
            videohub.checkRouting();
        }
        if (m.getAddress() == "/label/get"){
            videohub.checkLabels();
        }
        if (m.getAddress() == "/label/in/set"){
            videohub.setInputLabel(m.getArgAsInt32(0), m.getArgAsString(1));
        }
        if (m.getAddress() == "/label/out/set"){
            videohub.setOutputLabel(m.getArgAsInt32(0), m.getArgAsString(1));
        }
    }
    
}

void testApp::getReceiveStatus(){
    string status = videohub.getReceiveStatus();
    if (status != ""){
        msgRx = status;
    }
}

void testApp::isVideohubUpdate(){
    if (videohub.isRoutingUpdate()){
        ofxOscMessage m;
        map<int, int> route = videohub.getRoutingMatrix();
        map<int, int>::iterator it = route.begin();
        map<int, int>::iterator itEnd = route.end();
        
        for (; it != itEnd ; it++){
            m.clear();
            m.setAddress("/route/dump");
            m.addIntArg(it->first);
            m.addIntArg(it->second);
            sender.sendMessage(m);
        }
        
        printRoutingMatrix();
    }
    
    if (videohub.isInputLabelsUpdate()){
        ofxOscMessage m;
        map<int, string> route = videohub.getInputLabels();
        map<int, string>::iterator it = route.begin();
        map<int, string>::iterator itEnd = route.end();
        
        for (; it != itEnd ; it++){
            m.clear();
            m.setAddress("/labels/in/dump");
            m.addIntArg(it->first);
            m.addStringArg(it->second);
            sender.sendMessage(m);
        }
    }

    if (videohub.isOutputLabelsUpdate()){
        ofxOscMessage m;
        map<int, string> route = videohub.getOutputLabels();
        map<int, string>::iterator it = route.begin();
        map<int, string>::iterator itEnd = route.end();
        
        for (; it != itEnd ; it++){
            m.clear();
            m.setAddress("/labels/out/dump");
            m.addIntArg(it->first);
            m.addStringArg(it->second);
            sender.sendMessage(m);
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
