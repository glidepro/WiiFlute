#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 9000


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	void audioOut(float * input, int bufferSize, int nChannels);
	
	
	ofSoundStream soundStream;
	
	float 	pan;
	int		sampleRate;
	bool 	bNoise;
	float 	volume;
	
	vector <float> lAudio;
	vector <float> rAudio;
	
	//------------------- for the simple sine wave synthesis
	float 	targetFrequency;
	float 	phase;
	float 	phaseAdder;
	float 	phaseAdderTarget;
	
	int w, h;
	ofxOscReceiver	receiver;
	float		wiiX, wiiY;
	
	bool down_2;
	bool down_1;
	bool down_home;
	bool down_a;
	bool down_dpad_down;
	
	float counter;
		
};
