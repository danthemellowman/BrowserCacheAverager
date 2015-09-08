#include "ofApp.h"

void ofApp::exit(){
    
}

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_SILENT);
    

    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetLogLevel(OF_LOG_ERROR);
    ofEnableArbTex();
    
    texWidth = 512;
    texHeight = 512;
    MAX_TEXTURES = 10;
    
//    ofSystem(ofToDataPath("copy.sh"));
    
    
//    ofDirectory dir;
//    dir.listDir("cache");
//    dir.sort();
//    for(int i = 0; i < dir.size(); i++){
//        if(dir.getFile(i).isDirectory()){
//            ofDirectory dir2;
//            dir2.listDir(dir.getPath(i));
//            for(int j = 0; j < dir2.size(); j++){
//                if(tester.load(dir2.getPath(j))){
//                    filePaths.push_back(dir2.getPath(j));
//                    if(players.size() < MAX_TEXTURES){
//                        players.push_back(ofImage());
//                        if(players.back().load(filePaths.back())){
//                            cout<<"loaded"<<endl;
//                        }
//                    }
//                }else{
//                    ofFile::removeFile(dir2.getPath(j));
//                }
//            }
//        }else{
//            if(tester.load(dir.getPath(i))){
//                filePaths.push_back(dir.getPath(i));
//                if(players.size() < MAX_TEXTURES){
//                    players.push_back(ofImage());
//                    if(players.back().load(filePaths.back())){
//                        cout<<"loaded"<<endl;
//                    }
//                }
//            }else{
//                ofFile::removeFile(dir.getPath(i));
//            }
//        }
//    }
    generateShaders(MAX_TEXTURES);

    index = players.size()-1;

    
    recording = false;

    
    fbo.allocate(texWidth, texHeight, GL_RGB);
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
    
    pixelBufferBack.allocate(texWidth*texHeight*4,GL_DYNAMIC_READ);
    pixelBufferFront.allocate(texWidth*texHeight*4,GL_DYNAMIC_READ);
    
    recordingDir = ofToDataPath(ofGetTimestampString());
    ofDirectory::createDirectory(recordingDir);
    
    recordingIndex = 0;
}

void ofApp::update()
{
    
    for(int i = 0; i < players.size(); i++){
        if(filePaths.size() > 0 && i == 0){
            players.pop_front();
            players.push_back(ofImage());
            players.back().load(filePaths[index]);
            
            index++;
            if(index >= filePaths.size()){
                index = 0;
                if(recording){
                    recording = false;
                }
            }
        }
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        normalized[i]->begin();
        ofClear(0, 0, 0);
        ofSetColor(255, 255, 255);
        ofPushMatrix();
        float width = texWidth;
        float height = players[i].getHeight()/players[i].getWidth()*width;
        

        players[i].draw((texWidth - width)/2.0, (texHeight - height)/2.0, width, height);
        ofPopMatrix();
        normalized[i]->end();
        ofDisableAlphaBlending();
    }
    isf.update();

    if(recording){
        // copy the fbo texture to a buffer
        fbo.getTexture().texData = isf.getTextureReference().getTextureData();
        fbo.getTexture().copyTo(pixelBufferBack);
        
        // map the buffer so we can access it from the cpu
        // and wrap the memory in an ofPixels to save it
        // easily. Finally unmap it.
        pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
        unsigned char * p = pixelBufferFront.map<unsigned char>(GL_READ_ONLY);
        pixels.setFromExternalPixels(p,texWidth,texHeight,OF_PIXELS_RGB);
        ofSaveImage(pixels,recordingDir.get()+"/"+ofToString(recordingIndex)+".jpg");
        pixelBufferFront.unmap();
        
        // swap the front and back buffer so we are always
        // copying the texture to one buffer and reading
        // back from another to avoid stalls
        swap(pixelBufferBack,pixelBufferFront);
        recordingIndex++;
    }
}

void ofApp::draw()
{
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255, 255, 255);
    float width = ofGetWindowHeight();
    float height = texWidth/texHeight*width;
    isf.draw((ofGetWindowWidth()-width)/2, (ofGetWindowHeight()-height)/2, width, height);
    ofDisableAlphaBlending();
}

void ofApp::keyPressed(int key)
{
    if(key == 'f' || key == ' '){
        bFullscreen = !bFullscreen;
        ofSetFullscreen(bFullscreen);
        if(!bFullscreen){
            ofSetWindowShape(640, 640);
        }
    }
}
void ofApp::dragEvent(ofDragInfo info){
    if(info.files.size() > 0){
        ofDirectory dir;
        dir.listDir(info.files[0]);
        if(dir.isDirectory()){
            string newDirPath = ofToDataPath("cache/"+ofGetTimestampString(), true);
            ofFile::copyFromTo(dir.getAbsolutePath(), newDirPath);
            dir.listDir(newDirPath);
            for(int i = 0; i < dir.size(); i++){
                if(dir.getFile(i).isDirectory()){
                    ofDirectory dir2;
                    dir2.listDir(dir.getPath(i));
                    for(int j = 0; j < dir2.size(); j++){
                        if(tester.load(dir2.getPath(j))){
                            filePaths.push_back(dir2.getPath(j));
                            if(players.size() < MAX_TEXTURES){
                                players.push_back(ofImage());
                                if(players.back().load(filePaths.back())){
                                    cout<<"loaded"<<endl;
                                }
                            }
                        }else{
                            ofFile::removeFile(dir2.getPath(j));
                        }
                    }
                }else{
                    if(tester.load(dir.getPath(i))){
                        filePaths.push_back(dir.getPath(i));
                        if(players.size() < MAX_TEXTURES){
                            players.push_back(ofImage());
                            if(players.back().load(filePaths.back())){
                                cout<<"loaded"<<endl;
                            }
                        }
                    }else{
                        ofFile::removeFile(dir.getPath(i));
                    }
                }
            }
        }
    }
    recordingDir = ofToDataPath(ofGetTimestampString());
    ofDirectory::createDirectory(recordingDir);
    recording = true;
    recordingIndex = 0;
    index = players.size()-1;
}

void ofApp::generateShaders(int numVids){

    stringstream src;
    src<<"/*{\n \"DESCRIPTION\": \"RGB Average\"\n,\"CREDIT\": \"by dantheman\"\n,\"CATEGORIES\": [\"stylized Effect\"]\n,\"INPUTS\": [";
    for(int i = 0; i < numVids; i++){
        src<<"{\"NAME\": \"image"<<ofToString(i)<<"\",\n\"TYPE\": \"image\"}";
        if(i < numVids-1){
            src<<",\n";
        }
    }
    src<<"]}*/\n\n";
    src << "void main(void) {\n";
    src << "vec2 uv = vv_FragNormCoord.xy;\n";
    src <<"vec4	final = vec4(0.0);\n";
    for(int i = 0; i < numVids; i++) {
        src <<"vec4	srcPixel"<<ofToString(i)<<" = IMG_NORM_PIXEL(image"<<ofToString(i)<<",uv);\n";
        src<<"final+=srcPixel"<<ofToString(i)<<";\n";
    }
    
    src<<"gl_FragColor = vec4(vec3(final)/"<<numVids<<".0, 1.0);\n"<<endl;
    src << "}\n";
    
    string shaderFile = ("shaders/avg-0.fs");
    string path = ofToDataPath(shaderFile, true);
    ofstream isfShader;
    isfShader.open(path.c_str());
    isfShader << src.str();
    isfShader.close();
    
    isf.setup(texWidth, texHeight, GL_RGB);
    isf.load(shaderFile);

    normalized.resize(numVids);
    
    for(int i = 0; i < numVids; i++){
        normalized[i] = new ofFbo();
        normalized[i]->allocate(texWidth, texHeight, GL_RGB);
        normalized[i]->begin();
        ofClear(0, 0, 0, 0);
        normalized[i]->end();
        isf.setImage("image"+ofToString(i), normalized[i]->getTexture());
    }
}
