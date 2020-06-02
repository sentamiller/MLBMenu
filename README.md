# MLBMenu
This application parses a JSON feed of MLB games for a day and displays them in a list.  

release.zip contains the MLBMenu executables for three different resolutions.  
To run them on Windows, unzip all of files to a directory and run the application.  
If the application fails to run due to missing dll files, Visual C++ redistributable 
packages for Visual Studio may need to be installed. 


The C++ project was built with Microsoft Visual Studio 2019.  
Additional libraries: 
 - SDL2 (https://www.libsdl.org/) - For rendering graphics 
 - libcurl (https://curl.haxx.se/) - For handling http requests
 - JSON for Modern C++ (https://nlohmann.github.io/json/) - For parsing JSON

