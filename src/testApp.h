#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxSSAO.h"

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
	
	void drawScene(bool flip);
	
	ofEasyCam cam;
	
	ofShader depthPass;
	ofShader dofPass;
	
	// ofFbo
	ofFbo depthFbo;
	ofFbo colorFbo;
	ofFbo forAO;
	
	// shaders vars
	
	float depthPass_near;
	float depthPass_far;
	
	float dof_FocalLength;
	float dof_FocusDistance;
	float dof_fstop;
	bool dof_showFocus;
	float dof_maxblur;
	
	ofLight directionalLight;
	
	bool drawDepth;
	ofxSSAO ambientO;
	float ao_nearClip;
	float ao_farClip;
	float ao_radius;
	float ao_weight;
	float ao_minThreshold;
	float ao_maxThreshold;
	
		
};
