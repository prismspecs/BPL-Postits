#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofFileUtils.h"
#include "postit.h"
#include "ofxThreadedImageLoader.h"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    void checkFolder(bool init);
    
    void keyPressed(int key);
    
    float counter;
    
    ofxSyphonServer overlay, multiply;
    string filePath;
    
    Postit postits [13];
    ofImage postImg [1];
    ofImage postImgMult [1];
    int imgID;
    int index = 0;
    
    ofFbo fbo_overlay, fbo_multiply;  // composite of all post-its
    
    float folderLastChecked;
    float folderCheckInterval;
    ofDirectory previousDir;
    
    string lastNewImagePath;
    int lastNewImageID;
    
    bool debug = false;
    bool GRID = true;
    
    //ofxThreadedImageLoader loader;

};
