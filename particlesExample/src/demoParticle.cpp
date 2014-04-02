#include "demoParticle.h"

//------------------------------------------------------------------
demoParticle::demoParticle(){
	attractPoints = NULL;
}

//------------------------------------------------------------------
void demoParticle::setMode(particleMode newMode){
	mode = newMode;
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints( vector <ofPoint> * attract ){
	attractPoints = attract;
}

//------------------------------------------------------------------
void demoParticle::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
	
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	
	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);
	
	frc   = ofPoint(0,0,0);
	
	scale = ofRandom(0.5, 1.0);
	
	if( mode == PARTICLE_MODE_NOISE ){
		drag  = ofRandom(0.97, 0.99);
		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	}else{
		drag  = ofRandom(0.95, 0.998);	
	}
    ofBackground(0);
    ofSetWindowTitle("The Moon is made of plops");
    
    // read the directory for the images
    // we know that they are named in seq
    ofDirectory dir;
    
    int nFiles = dir.listDir("bats-front");
    if(nFiles) {
        
        for(int i=0; i<dir.numFiles(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().loadImage(filePath);
            
        }
        
    }
    else printf("Could not find folder\n");
    
    // this toggle will tell the sequence
    // be be indepent of the app fps
    bFrameIndependent = true;
    
    // this will set the speed to play
    // the animation back we set the
    // default to 24fps
    sequenceFPS = 60;
    
    // set the app fps
    appFPS = 60;
    ofSetFrameRate(appFPS);

}

//------------------------------------------------------------------
void demoParticle::update(){

	/*//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN
	
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point 
		
		vel *= drag; //apply drag
		vel += frc * 0.6; //apply force
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt-pos; 
		
		//let get the distance and only repel points close to the mouse
		float dist = frc.length();
		frc.normalize(); 
		
		vel *= drag; 
		if( dist < 150 ){
			vel += -frc * 0.6; //notice the frc is negative 
		}else{
			//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
			frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
			frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
			vel += frc * 0.04;
		}
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		//lets simulate falling snow 
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		
		frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;

		vel *= drag; 
		vel += frc * 0.4;
		
		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
		if( pos.y + vel.y > ofGetHeight() ){
			pos.y -= ofGetHeight();
		}
	}*/
	/*else*/ if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		
		if( attractPoints ){

			//1 - find closest attractPoint 
			ofPoint closestPt;
			int closest = -1; 
			float closestDist = 9999999;
			
			for(unsigned int i = 0; i < attractPoints->size(); i++){
				float lenSq = ( attractPoints->at(i)-pos ).lengthSquared();
				if( lenSq < closestDist ){
					closestDist = lenSq;
					closest = i;
				}
			}
			
			//2 - if we have a closest point - lets calcuate the force towards it
			if( closest != -1 ){
				closestPt = attractPoints->at(closest);				
				float dist = sqrt(closestDist);
				
				//in this case we don't normalize as we want to have the force proportional to distance 
				frc = closestPt - pos;
		
				vel *= drag;
				 
				//lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
				if( dist < 1200 && dist > 50 && !ofGetKeyPressed('f') ){
					vel += frc * 0.003;
				}else{
					//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
					frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
					frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
					vel += frc * 0.4;
				}
				
			}
		
		}
		
	}
	
	
	//2 - UPDATE OUR POSITION
	
	pos += vel; 
	
	
	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the testApp level
	if( pos.x > ofGetWidth() ){
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}else if( pos.x < 0 ){
		pos.x = 0;
		vel.x *= -1.0;
	}
	if( pos.y > ofGetHeight() ){
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if( pos.y < 0 ){
		pos.y = 0;
		vel.y *= -1.0;
	}	
		
}

//------------------------------------------------------------------
void demoParticle::draw(){

	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofSetColor(208, 255, 63);
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		ofSetColor(99, 63, 255);
	}
	else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		ofSetColor(103, 160, 237);
	}
			
	//ofCircle(pos.x, pos.y, scale * 4.0);
    
    // we need some images if not return
    if((int)images.size() <= 0) {
        ofSetColor(255);
        ofDrawBitmapString("No Images...", 150, ofGetHeight()/2);
        return;
    }
    
    // this is the total time of the animation based on fps
    //float totalTime = images.size() / sequenceFPS;
    
    
    int frameIndex = 0;
    
    if(bFrameIndependent) {
        // calculate the frame index based on the app time
        // and the desired sequence fps. then mod to wrap
        frameIndex = (int)(ofGetElapsedTimef() * sequenceFPS) % images.size();
    }
    else {
        // set the frame index based on the app frame
        // count. then mod to wrap.
        frameIndex = ofGetFrameNum() % images.size();
    }
    
    // draw the image sequence at the new frame count
    ofSetColor(255);
    images[frameIndex].draw((pos.x - 50), (pos.y - 50), 100,100);
    
    
    /*// draw where we are in the sequence
    float x = 0;
    for(int offset = 0; offset < 5; offset++) {
        int i = (frameIndex + offset) % images.size();
        ofSetColor(255);
        images[i].draw(200+x, ofGetHeight()-40, 40, 40);
        x += 40;
    }
    
    
    // how fast is the app running and some other info
    ofSetColor(50);
    ofRect(0, 0, 200, 200);
    ofSetColor(200);
    string info;
    info += ofToString(frameIndex)+" sequence index\n";
    info += ofToString(appFPS)+"/"+ofToString(ofGetFrameRate(), 0)+" fps\n";
    info += ofToString(sequenceFPS)+" Sequence fps\n\n";
    info += "Keys:\nup & down arrows to\nchange app fps\n\n";
    info += "left & right arrows to\nchange sequence fps";
    info += "\n\n't' to toggle\nframe independent("+ofToString(bFrameIndependent)+")";
    
    ofDrawBitmapString(info, 15, 20);*/

}

