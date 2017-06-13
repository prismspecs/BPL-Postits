#include "postit.h"

#define PSIZE 743   // should be same as grid unit from ofApp.cpp
#define WSIZE 350   // size of writing image

Postit::Postit() {
    
}

void Postit::setPos(ofVec2f _pos) {
    pos = _pos;
}

void Postit::setup(string path, ofImage & imgOverlay, ofImage & imgMultiply) {
    fadeIn = false;
    fadeOut = true;
    
    // load in next postit graphic, anchor to center
    //img_overlay.load("postit" + ofToString(imgID) + ".png");
    //img_overlay.load("postit0.png");
    img_overlay.setAnchorPoint(PSIZE * .5, PSIZE * .5);
    img_multiply.setAnchorPoint(PSIZE * .5, PSIZE * .5);

    // threaded...
    //loader.loadFromDisk(img_overlay, "postit0.png");
    
    img_overlay = imgOverlay;
    img_multiply = imgMultiply;
    
    myPath = path;
    
}

void Postit::update() {
    if(fadeIn) {
        opacity += ofGetLastFrameTime() * 255.0;
        if(opacity > 255) {
            opacity = 255;
            fadeIn = false;
        }
    }
    
    if(fadeOut) {
        opacity -= ofGetLastFrameTime() * 255.0;
        if(opacity < 0) {
            opacity = 0;
            fadeOut = false;
            fadeIn = true;
            
            // done fading out, init postit
            img_writing.load(myPath);
            img_writing.resize(WSIZE, WSIZE);
            img_writing.setAnchorPoint(WSIZE/2, WSIZE/2);
            
            rot = ofRandom(-10, 10);
        }
    }
}

void Postit::draw(ofFbo &fbo_overlay, ofFbo &fbo_multiply) {
    if(img_overlay.isAllocated()) {
        
        fbo_multiply.begin();
        ofSetColor(255,255,255,opacity);
        ofPushMatrix();
        ofTranslate(pos);
        ofRotate(rot);
        img_multiply.draw(0,0);
        ofPopMatrix();
        fbo_multiply.end();
        
        
        fbo_overlay.begin();
        ofSetColor(255,255,255,opacity);
        ofPushMatrix();
        ofTranslate(pos);
        ofRotate(rot);
        img_overlay.draw(0,0);
        img_writing.draw(0,0);
        ofPopMatrix();
        fbo_overlay.end();
        
        
    }
}
