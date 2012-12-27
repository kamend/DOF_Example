#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

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
	
	void drawScene();
	
	ofEasyCam cam;
	
	ofShader depthPass;
	ofShader dofPass;
	
	// ofFbo
	ofFbo depthFbo;
	ofFbo colorFbo;
	
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
	
		
};
