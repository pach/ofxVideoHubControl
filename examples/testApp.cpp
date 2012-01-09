#include "testApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void testApp::setup(){

	// we don't want to be running to fast
	ofSetVerticalSync(true);
    videohub.setup();
    videohub.setAddress("10.0.0.15");
    
    select = 0;
    input = output = 0;

}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(230, 230, 230);
    videohub.update();
    
  //  cout<<videohub.getReceiveStatus()<<endl;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0, 0, 0);
    string str = "input : ";
    str += ofToString(input);
    str += ", output : ";
    str += ofToString(output);
    str += "\n";
    if (select == 0){
        str += "type output id (0 to 15 for smartvideohub)";
    }
    else if (select == 1){
        str += "type input id (0 to 15 for smartvideohub)";
    }
    else{
        str += "hit 's' to send cmd";
    }
    ofDrawBitmapString(str, 20, 20);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 's'){
        select = 0;
        videohub.setRoute(output, input);
        printf("set route %i %i\n", output, input);
    }
    else{
        if (select == 0)
            output = ofToInt(ofToString((char)key));
        else if (select == 1)
            input = ofToInt(ofToString((char)key));
        select ++;
    }
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
