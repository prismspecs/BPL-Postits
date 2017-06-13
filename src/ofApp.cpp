#include "ofApp.h"
#include "stdio.h"

#define GRID_UNIT 743
#define NUM_POSTITS 13

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle("BPL");
    
    overlay.setName("overlay");
    multiply.setName("multiply");
    
    ofEnableAlphaBlending();
    //ofSetVerticalSync(true);
    
    if(GRID) {
        fbo_overlay.allocate(GRID_UNIT * 4, GRID_UNIT * 4, GL_RGBA);
        fbo_multiply.allocate(GRID_UNIT * 4, GRID_UNIT * 4, GL_RGB);
    } else {
        fbo_overlay.allocate(4400, 2200, GL_RGBA);
        fbo_multiply.allocate(4400, 2200, GL_RGB);
    }
    
    // clear the fbos
    fbo_overlay.begin();
    ofClear(255, 255, 255, 0);
    fbo_overlay.end();
    
    fbo_multiply.begin();
    ofClear(255, 255, 255, 0);
    fbo_multiply.end();
    
    ofFileDialogResult result = ofSystemLoadDialog("Load file", true);
    if(result.bSuccess) {
        filePath = result.getPath();
    }
    
    // how often should we look for new images?
    folderCheckInterval = 1.0;
    
    folderLastChecked = ofGetElapsedTimef();
    
    // do initial folder check so any existing images
    // dont start piping in
    checkFolder(true);
    
    // load in postit pixel coordinates manually
    postits[0].setPos(ofVec2f(1422, 1616));
    postits[1].setPos(ofVec2f(929, 1423));
    postits[2].setPos(ofVec2f(1405, 1083));
    postits[3].setPos(ofVec2f(924, 894));
    postits[4].setPos(ofVec2f(1111, 364));
    postits[5].setPos(ofVec2f(1651, 438));
    postits[6].setPos(ofVec2f(2203, 374));
    postits[7].setPos(ofVec2f(2733, 438));
    postits[8].setPos(ofVec2f(3278, 384));
    postits[9].setPos(ofVec2f(2967, 1029));
    postits[10].setPos(ofVec2f(3458, 938));
    postits[11].setPos(ofVec2f(2965, 1583));
    postits[12].setPos(ofVec2f(3460, 1507));
    
    // or set up as a grid
    // 4 x 4, 2160x2160 at 540x540
    if(GRID) {
        for(int i = 0; i < 13; i++) {
            float x = (i % 4) * GRID_UNIT + GRID_UNIT/2;
            float y = (i / 4) * GRID_UNIT + GRID_UNIT/2;
            postits[i].setPos(ofVec2f(x, y));
        }
    }
    
    
    // load the post it background images into memory
    for(int i = 0; i < 3; i++) {
//        string fn = ofToString(i);
//        if(i>=10)
//            f
        postImg[i].load("0" + ofToString(i+1) + "_img.png");
        postImgMult[i].load("0" + ofToString(i+1) + "_shadow.png");
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(ofGetElapsedTimef() > folderLastChecked + folderCheckInterval) {
        checkFolder(false);
    }
    
    for(int i = 0; i < NUM_POSTITS; i++) {
        postits[i].update();
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    fbo_overlay.begin();
    // Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fbo_overlay.end();
    
    fbo_multiply.begin();
    ofBackground(0, 0, 0);
    fbo_multiply.end();
    
    //fbo_overlay.clear();
    for(int i = 0; i < NUM_POSTITS; i++) {
        postits[i].draw(fbo_overlay, fbo_multiply);
    }
    
    if(GRID)
        fbo_overlay.draw(0,0,ofGetWidth()/2, ofGetHeight());
    else
        fbo_overlay.draw(0,0,ofGetWidth(), ofGetHeight());

    overlay.publishTexture(&fbo_overlay.getTexture());
    multiply.publishTexture(&fbo_multiply.getTexture());
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key) {
    switch(key) {
        case 'd':
        debug = !debug;
    }
}

//--------------------------------------------------------------
void ofApp::checkFolder(bool init) {
    
    if(init) {
    }
    
    // check folder for new files
    ofDirectory dir(filePath);
    // only show png files
    dir.allowExt("png");
    // populate the directory object
    dir.listDir();
    dir.sort();
    
    vector<string> newFiles;
    
    // run thru the directory and look for new files
    if(dir.size() != previousDir.size()) {
        
        for(int i = 0; i < dir.size(); i++){
            
            bool matchFound = false;
            
            for(int n = 0; n < previousDir.size(); n++){
                if(dir.getPath(i) != previousDir.getPath(n)) {
                    // no match here
                } else {
                    // there was a match
                    matchFound = true;
                    break;
                }
            }
            
            // no matches found, add it to the list of new files
            if(!matchFound) {
                //cout << "no matches found" << endl;
                newFiles.push_back(dir.getPath(i));
            }
        }
        
        cout << "new files:" << endl;
        for(int i = 0; i < newFiles.size(); i++){
            cout << newFiles[i] << endl;
            
            
            // only setup a new post it if we arent doing this
            // for the first time
            if(!init) {
                postits[index].setup(newFiles[i], postImg[imgID], postImgMult[imgID]);
                
                index++;
                if(index >= NUM_POSTITS) {
                    index = 0;
                }
                imgID++;
                if(imgID >= 3) {
                    imgID = 0;
                }
            }
        }
        
    }
    
//    
//
//    // check the folder for new images
//    //if(lastNewImagePath != dir.getPath(dir.size()-1)) {
//    
//    // are there more images than before?
//    if(lastNewImageID != dir.size() - 1) {
//        
//        // go through and print out all the paths
//        if(debug)
//            for(int i = 0; i < dir.size(); i++){
//                ofLogNotice(dir.getPath(i));
//            }
//        
//        int howManyNew = dir.size() - 1 - lastNewImageID;
//        
//        if(debug)
//            cout << "# of new postits: " << howManyNew << endl;
//        
//        for(int i = 0; i < howManyNew; i++) {
//
//            // only setup a new post it if we arent doing this
//            // for the first time
//            if(!init) {
//                //postits[index].setup(imgID, dir.getPath(lastNewImageID + i + 1));
//                
//                postits[index].setup(imgID, dir.getPath(lastNewImageID + i + 1), postImg[0]);
//                
//                index++;
//                if(index >= NUM_POSTITS) {
//                    index = 0;
//                }
//                imgID++;
//                if(imgID >= NUM_POSTITS) {
//                    imgID = 0;
//                }
//            }
//            
//        }
//        
//        lastNewImagePath = dir.getPath(dir.size()-1);
//        lastNewImageID = dir.size() - 1;
//        
//    }
    
    previousDir = dir;
    
    folderLastChecked = ofGetElapsedTimef();
}
