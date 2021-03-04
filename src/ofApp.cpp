#include "ofApp.h"

#define STRINGIFY(a) #a

#if MY_GL_MAJOR >= 4 && MY_GL_MINOR >= 1
#define GLSL410(shader) "#version 410\n" #shader
#define GLSL GLSL410
#elif MY_GL_MAJOR >= 4 && MY_GL_MINOR >= 0
#define GLSL400(shader) "#version 400\n" #shader
#define GLSL GLSL400
#elif MY_GL_MAJOR >= 3 && MY_GL_MINOR >= 3
#define GLSL330(shader) "#version 330\n" #shader
#define GLSL GLSL330
#else
#define GLSL150(shader) "#version 150\n " #shader
#define GLSL GLSL150
#endif

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("ofApp");

	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofLogToConsole();

	ofDisableArbTex();

	loadSettings();

	oscReceiver.setup(oscSettings.receiver_port, [&](const ofxOscMessage& msg) {
		oscReceived(msg);
	});
	oscSender.setup(oscSettings.sender_host, oscSettings.sender_port);


	ofLog() << "Vendor :" << glGetString(GL_VENDOR);
	ofLog() << "GPU : " << glGetString(GL_RENDERER);
	ofLog() << "OpenGL ver. " << glGetString(GL_VERSION);
	ofLog() << "GLSL ver. " << glGetString(GL_SHADING_LANGUAGE_VERSION);
	ofLog() << "";
	ofLog() << "Current Using OpenGL version: " << MY_GL_MAJOR << "." << MY_GL_MINOR;

	string passThroughVert = GLSL(
		uniform mat4 modelViewProjectionMatrix; // oF Default
		in vec4 position; // oF Default
		in vec2 texcoord; // oF Default

		out vec2 vTexCoord;

		void main() {
			vTexCoord = texcoord;
			gl_Position = modelViewProjectionMatrix * position;
		}
	);

	string basicShaderFrag = GLSL(
		out vec4 outputColor;

		uniform float time;

		void main() {
			//vec2 coord_xy = gl_FragCoord.xy;
			//vec4 color = texture(tex, coord_xy);
			float _c = abs(sin(time * 0.2)) * 0.5;
			outputColor = vec4(_c, _c, _c, 1.0);
		}
	);

	if (ofIsGLProgrammableRenderer()) {
		shader.setupShaderFromSource(GL_VERTEX_SHADER, passThroughVert);
	}
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, basicShaderFrag);

	if (ofIsGLProgrammableRenderer()) {
		shader.bindDefaults();
	}
	shader.linkProgram();

	gui.setup();
	font.load("inconsolata.ttf", 20);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	// Main ===================

	shader.begin();
	shader.setUniform1f("time", ofGetElapsedTimef());
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	shader.end();


	// GUI ===================

	gui.begin();

	ImGui::Begin("config");
	{
		static int savedf = 0;
		if (ImGui::Button("save")) {
			saveSettings();
			savedf = 120;
		}
		ImGui::SameLine();
		if (ImGui::Button("load")) {
			loadSettings();
		}
		if (savedf > 0) {
			ImGui::SameLine();
			ImGui::Text(" saved!");
			--savedf;
		}

	}
	ImGui::End();

	ImGui::Begin("osc");
	{
		ImGui::Text("* restart required");
		ImGui::InputInt("receiver_port", &oscSettings.receiver_port);
		ImGui::InputInt("sender_port", &oscSettings.sender_port);
		{
			const int buf_len = 256;
			char buf[buf_len];
			strcpy(buf, oscSettings.sender_host.c_str());
			if (ImGui::InputText("sender_host", buf, buf_len)) {
				oscSettings.sender_host = buf;
			}
		}
		//{
		//	const int buf_len = 256;
		//	char buf[buf_len];
		//	strcpy(buf, oscSettings.receiver_addr_prefix.c_str());
		//	if (ImGui::InputText("receiver_addr_prefix", buf, buf_len)) {
		//		oscSettings.receiver_addr_prefix = buf;
		//	}
		//}
		//{
		//	const int buf_len = 256;
		//	char buf[buf_len];
		//	strcpy(buf, oscSettings.sender_addr_prefix.c_str());
		//	if (ImGui::InputText("sender_addr_prefix", buf, buf_len)) {
		//		oscSettings.sender_addr_prefix = buf;
		//	}
		//}

		static int savedf = 0;
		if (ImGui::Button("save")) {
			saveSettings();
			savedf = 120;
		}
		ImGui::SameLine();
		if (ImGui::Button("load")) {
			loadSettings();
		}
		if (savedf > 0) {
			ImGui::SameLine();
			ImGui::Text(" saved!");
			--savedf;
		}
	}
	ImGui::End();


	gui.end();
}

void ofApp::oscReceived(const ofxOscMessage & msg)
{
	ofxSimpleOsc::filter(msg)
		.when("/test", [&]() {
			ofLog() << "/test received";
			ofxSimpleOsc::logger.log(msg, true);
		})
		.else_show_warning();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::loadSettings()
{
	ofxCereal::loadJson("osc_settings.json", oscSettings);
}

//--------------------------------------------------------------
void ofApp::saveSettings()
{
	ofxCereal::saveJson("osc_settings.json", oscSettings);
}


//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	//for (int i = 0; i< dragInfo.files.size(); i++) {
	//	try {
	//		ofImage img;
	//		img.load(dragInfo.files[i]);

	//		int image_num = 0;
	//		if (dragInfo.position.y > ofGetHeight() / 2) {
	//			image_num = 1;
	//		}
	//		if (dragInfo.position.x > ofGetWidth() / 5 * 4){
	//			image_num = 4;
	//		}

	//		if (img.isAllocated()) {
	//			auto& wh_image = wh_images[image_num];
	//			wh_image.begin();
	//			ofClear(0, 0);
	//			ofSetColor(255);
	//			img.draw(0, 0, wh_image.getWidth(), wh_image.getHeight());
	//			wh_image.end();

	//			//resetAdjustParams(image_num);

	//			updateAdjustedImage(image_num);
	//		}
	//	}
	//	catch (boost::filesystem::filesystem_error e) {
	//		ofLog() << "Could not load dropped file";
	//	}
	//}
}

//--------------------------------------------------------------
void ofApp::exit() {

}
