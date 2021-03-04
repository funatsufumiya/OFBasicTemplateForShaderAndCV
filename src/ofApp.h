#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxSimpleOsc.h"
#include "ofxAsync.h"

// OF default gl setting
#define MY_GL_MAJOR 2
#define MY_GL_MINOR 1

// recommended gl setting
//#define MY_GL_MAJOR 4
//#define MY_GL_MINOR 1

// if 4.1 not supported
//#define MY_GL_MAJOR 3
//#define MY_GL_MINOR 3

// the best gl setting
//#define MY_GL_MAJOR 4
//#define MY_GL_MINOR 6

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void oscReceived(const ofxOscMessage& msg);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void loadSettings();
		void saveSettings();

		void exit();
		
		ofShader shader;

		ofTrueTypeFont font;
		ofxImGui::Gui gui;

		ofxSimpleOsc::Receiver oscReceiver;
		ofxSimpleOsc::Sender oscSender;
};
