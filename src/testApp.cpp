/*
 
Depth of Field Example
 

 
*/

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// shaders
	dofPass.load("shaders/dofPass");
	depthPass.load("shaders/depthPass");
	
	//fbo
	
	depthFbo.allocate(ofGetWidth(), ofGetHeight());
	colorFbo.allocate(ofGetWidth(), ofGetHeight());
	forAO.allocate(ofGetWidth(), ofGetHeight());
	
	// gui
	
	gui.addPage("Depth Of Field");
	
	gui.addSlider("Dof Focus Distance", dof_FocusDistance, 0.0, 10.0);
	gui.addSlider("Dof Focal Length", dof_FocalLength, 0, 10.0);
	gui.addSlider("Dof fstop", dof_fstop, 0, 24.0);
	gui.addSlider("Dof MaxBlur", dof_maxblur, 0, 10.0);
	gui.addToggle("Dof Show Focus", dof_showFocus);
	
	gui.addSlider("Depth Far", depthPass_far, 100, 10000);
	gui.addSlider("Depth Near", depthPass_near, 0, 1000);
	
	gui.addSlider("AO Min Threshold", ao_minThreshold, 0, 1.0);
	gui.addSlider("AO Max Threshold", ao_maxThreshold, 0, 10);
	gui.addSlider("AO Radius", ao_radius, 0, 200.0);
	gui.addSlider("AO Weight", ao_weight, 0, 10.0);
	gui.addSlider("AO nearClip", ao_nearClip, 0, 5.0);
	gui.addSlider("AO farClip", ao_farClip, 0, 5000.0);
	
	
	
	gui.loadFromXML();

	directionalLight.setDirectional();
	directionalLight.setAmbientColor(ofColor(255,255,255));
	directionalLight.setDiffuseColor(ofColor(0,255,255));
	
	ofSetSmoothLighting(true);
	
	directionalLight.setOrientation(ofVec3f(10,50,50));
	
	drawDepth = false;
	
	// ambient oclussion
	ambientO.setup(ofGetWidth(), ofGetHeight(), GL_RGBA16);
}

//--------------------------------------------------------------
void testApp::update(){
	
	ambientO.setClipPlanes(ao_nearClip, ao_farClip);
	ambientO.setMaxThreshold(ao_maxThreshold);
	ambientO.setMinThreshold(ao_minThreshold);
	ambientO.setRadius(ao_radius);
	ambientO.setWeight(ao_weight);
	
	
	if(gui.isOn()) {
		cam.disableMouseInput();
	} else {
		cam.enableMouseInput();
	}
	
	

}
//--------------------------------------------------------------
void testApp::drawScene(bool flip) {
	
	if(flip) {
		cam.setScale(1,-1,1);
	} else {
		cam.setScale(1,1,1);
	}
	
	ofPushMatrix();
	cam.begin();
	ofRotateX(-30);
	for(int i=0;i<15;i++) {
		for(int j=0;j<15;j++) {
			
			float x = ofMap(i,0, 15,-500,500);
			float z = ofMap(j,0, 15, 0, -2000);
			
			float hue = ofMap(i, 0,30,100,200);
			ofColor bc;
			bc.setHsb(hue, 255, 255);
			ofSetColor(bc);
			//ofBox(x,0,z,50);
			ofSphere(x,0,z,30);
		}
	}
	cam.end();
	ofPopMatrix();
}
//--------------------------------------------------------------
void testApp::draw(){
	glEnable(GL_DEPTH_TEST);
	//ofBackgroundGradient(ofColor(0,0,0), ofColor(50,50,50));
	
	ofEnableLighting();
	colorFbo.begin();
	ofClear(0,0,0,0);
	
	directionalLight.enable();
	drawScene(false);
	directionalLight.disable();
	
	colorFbo.end();
	ofDisableLighting();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	ambientO.begin();
	ofBackgroundGradient(ofColor(255,100,100), ofColor(0, 0, 0));
	directionalLight.enable();
	drawScene(true);
	directionalLight.disable();
	ambientO.end(true);
	glPopAttrib();

	depthFbo.begin();
	ofClear(0,0,0,0);
	depthPass.begin();
	depthPass.setUniform1f("near", depthPass_near);
	depthPass.setUniform1f("far", depthPass_far);
	drawScene(false);
	depthPass.end();
	depthFbo.end();
	
	ofEnableAlphaBlending();
	
	ofBackgroundGradient(ofColor(255,100,100), ofColor(0, 0, 0));
	if(drawDepth) {
		ofSetColor(255,255,255);
		depthFbo.draw(0,0);
	} else {
		ofSetColor(255,255,255);
		dofPass.begin();
		dofPass.setUniformTexture("bgl_RenderedTexture", ambientO.getDrawFbo()->getTextureReference(),0);
		dofPass.setUniformTexture("bgl_DepthTexture", depthFbo.getTextureReference(),1);
		dofPass.setUniform1f("bgl_RenderedTextureWidth", ofGetWidth());
		dofPass.setUniform1f("bgl_RenderedTextureHeight", ofGetHeight());
		dofPass.setUniform1f("focalDepth", dof_FocusDistance);
		dofPass.setUniform1f("focalLength", dof_FocalLength);
		dofPass.setUniform1f("fstop", dof_fstop);
		dofPass.setUniform1f("showFocus", dof_showFocus);
		dofPass.setUniform1f("maxblur", dof_maxblur);
		
		ambientO.getDrawFbo()->draw(0,0);
		dofPass.end();
	
	}
	
	ofDisableAlphaBlending();
	ofSetColor(255,255,255);

	if(gui.isOn()) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		gui.draw();
		glPopAttrib();
	}



}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'd') {
		drawDepth = !drawDepth;
	}
	
	if(key == ' ') {
		gui.toggleDraw();
	}

	if(key == 's') {
		dofPass.load("shaders/dofPass");
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