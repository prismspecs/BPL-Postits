#pragma once

#include "ofMain.h"
#include "ofxThreadedImageLoader.h"

class Postit {
    
public:
    void setPos(ofVec2f _pos);
    void setup(string path, ofImage & imgOverlay, ofImage & imgMultiply);
    void update();
    void draw(ofFbo &fbo_overlay, ofFbo &fbo_multiply);
    
    
    Postit();
    
    ofImage img_overlay;
    ofImage img_writing;
    ofImage img_multiply;
    
    float opacity = 0.0;
    float rot;
    ofVec2f pos;
    float width;
    float height;
    bool fadeOut;
    bool fadeIn;
    
    string myPath;
    ofxThreadedImageLoader loader;
    
private:
};
