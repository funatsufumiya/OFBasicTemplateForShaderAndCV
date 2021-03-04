#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	int w = 1200;
	int h = 600;

#if MY_GL_MAJOR == 2 && MY_GL_MINOR == 1

    ofSetupOpenGL(w, h, OF_WINDOW);

#else

    ofGLWindowSettings settings;
    settings.setGLVersion(MY_GL_MAJOR, MY_GL_MINOR);

#if OF_VERSION_MINOR >= 10
    settings.setSize(w, h);
#else // OF_VERSION_MINOR
    settings.width = w;
    settings.height = h;
#endif // OF_VERSION_MINOR

    ofCreateWindow(settings);
#endif

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
