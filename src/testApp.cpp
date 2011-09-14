#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetCircleResolution(50);
	ofSetFrameRate(60);
	ofSetWindowShape(1024, 768);
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofSetFullscreen(false);
	
	counter = 0;
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	bNoise 				= false;
	targetFrequency = 587.33;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
	
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	
	//soundStream.listDevices();
	
	//if you want to set the device id to be different than the default
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only 
	
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
	
	w = 0;
	h = 0;	// set in update()
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
}

//--------------------------------------------------------------
void testApp::update(){
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		if(w == 0 || h == 0){
			w = ofGetWidth();
			h = ofGetHeight();
		}
		// get the next message
		ofxOscMessage m;
		
		float x,y;
		receiver.getNextMessage( &m );
		
		if ( m.getAddress() == "/wii/1/button/2" )
		{
			if (m.getArgAsInt32(0)==0) {
				down_2 = false;
			} else {
				down_2 = true;
			}
		}
		if ( m.getAddress() == "/wii/1/button/1" )
		{
			if (m.getArgAsInt32(0)==0) {
				down_1 = false;
			} else {
				down_1 = true;
			}
		}
		if ( m.getAddress() == "/wii/1/button/Home" )
		{
			if (m.getArgAsInt32(0)==0) {
				down_home = false;
			} else {
				down_home = true;
			}
		}
		if ( m.getAddress() == "/wii/1/button/A" )
		{
			if (m.getArgAsInt32(0)==0) {
				down_a = false;
			} else {
				down_a = true;
			}
		}
		if ( m.getAddress() == "/wii/1/button/Down" )
		{
			if (m.getArgAsInt32(0)==0) {
				down_dpad_down = false;
			} else {
				down_dpad_down = true;
			}
		}
				
		if (down_2 && down_1 && down_home && down_a && down_dpad_down) {
			targetFrequency = 293.66;
		}
		if (down_2 && down_1 && down_home && down_a && !down_dpad_down) {
			targetFrequency = 329.63;
		}
		if (down_2 && down_1 && down_home && !down_a && !down_dpad_down) {
			targetFrequency = 369.99;
		}
		if (down_2 && down_1 && !down_home && !down_a && !down_dpad_down) {
			targetFrequency = 440.00;
		}
		if (down_2 && !down_1 && !down_home && !down_a && !down_dpad_down) {
			targetFrequency = 493.88;
		}
		if (!down_2 && !down_1 && !down_home && !down_a && !down_dpad_down) {
			targetFrequency = 587.33;
		}
		
		cout << down_2 << down_1 << down_home << down_a << down_dpad_down << "\n";
		
		phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
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

//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;
	
	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phase > TWO_PI){
		phase -= TWO_PI;
	}
	
	if ( bNoise == true){
		// ---------------------- noise --------------
		for (int i = 0; i < bufferSize; i++){
			lAudio[i] = output[i*nChannels    ] = ofRandom(0, 1) * volume * leftScale;
			rAudio[i] = output[i*nChannels + 1] = ofRandom(0, 1) * volume * rightScale;
		}
	} else {
		phaseAdder = 0.1f * phaseAdder + 0.9f * phaseAdderTarget;
		for (int i = 0; i < bufferSize; i++){
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i] = output[i*nChannels    ] = sample * volume * leftScale;
			rAudio[i] = output[i*nChannels + 1] = sample * volume * rightScale;
		}
	}
	
}
