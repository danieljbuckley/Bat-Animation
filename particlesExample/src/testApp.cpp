#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	int num = 15;
    
	p.assign(num, demoParticle());
	currentMode = PARTICLE_MODE_NEAREST_POINTS;

	currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force"; 

	resetParticles();
}

//--------------------------------------------------------------
void testApp::resetParticles(){

	//these are the attraction points used in the forth demo 
	attractPoints.clear();
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
    for(int i = 0; i < 2; i++){
		attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
        }
	}
    
    
	attractPointsWithMovement = attractPoints;
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}
}

//--------------------------------------------------------------
void testApp::update(){
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].update();
	}
	
	//lets add a bit of movement to the attract points
    if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
	for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
		//attractPointsWithMovement[i].x = ofGetMouseX() + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		//attractPointsWithMovement[i].y = ofGetMouseY() + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].x = ofGetMouseX() + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 20;
		attractPointsWithMovement[i].y = ofGetMouseY() + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 20;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(255,255,255);

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	
	ofSetColor(190);
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(unsigned int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(0);
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.", 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if( key == '2'){
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
	}
	if( key == '3'){
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
	}
	if( key == ' ' ){
		resetParticles();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if( key == '2') {
        currentMode = PARTICLE_MODE_NEAREST_POINTS;
    }
    
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
