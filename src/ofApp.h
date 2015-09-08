#pragma once

#include "ofMain.h"
#include "ofxISF.h"
#include "ofxXmlSettings.h"



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void dragEvent(ofDragInfo dragInfo); 
    void generateShaders(int foomax);
    void generateShaders();
    void keyPressed(int key);
    
    ofxISF::Shader isf;
    ofxISF::Shader isf2;
    ofxISF::Shader isf3;
    ofxISF::Shader isf4;
    deque<ofImage> 	players;
    ofImage tester;
    ofParameter<int> texWidth;
    ofParameter<int> texHeight;
    vector<ofFbo*> 	normalized;
    deque<string> filePaths;
    deque<string> secondRun;
    ofVideoGrabber video;

    ofParameter<int> MAX_TEXTURES;
    ofParameter<int> index;
    ofParameter<bool> recording;
    ofParameter<string> recordingDir;
    deque<string> searchQueue;
    deque<string> loopset;


    ofFbo fbo;
    ofBufferObject pixelBufferBack, pixelBufferFront;
    ofBoxPrimitive box;
    ofPixels pixels;

    ofParameter<int> recordingIndex;
    ofParameter<bool> bFullscreen;
};
