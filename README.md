# BrowserCacheAverager
Project for Decoding Media 

#addons
- [ofxISF](https://github.com/satoruhiga/ofxISF)
- ofxXmlSettings

##Usage

Drag and drop a chrome, safari, or firefox browser cache folder into the application's window.  Please wait while the data is loaded. _note it might take a while depending on your cache size_

The application will then continuously average 10 of the cached images together and save the frame to a Time-Date folder in ```/data```


###Chrome

    ~/Library/Caches/Google/Chrome/Default/Cache

###Safari

    ~/Library/Caches/com.apple.Safari/fsCachedData


###Firefox
To find your cache location for Firefox, open Firefox and type the following into your address bar:" 

    about:cache

Then navigate to the directory in "Storage disk location" via the Finder keyboard: ```Command-Shift-G command.```  _Note The folder is a hidden folder._