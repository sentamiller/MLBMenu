# MLBMenu
This application parses a JSON feed of MLB games for a day and displays them in a list.  

release.zip contains the MLBMenu executables for three different resolutions.  
To run them on Windows, unzip all of files to a directory and run the application.  
If the application fails to run due to missing dll files, Visual C++ redistributable 
packages for Visual Studio may need to be installed. 


**The C++ project was built with Microsoft Visual Studio 2019.**

Additional libraries (uploaded in SDL2-2.0.12.zip and curl.zip): 
 - SDL2 (https://www.libsdl.org/) - For rendering graphics 
 - libcurl (https://curl.haxx.se/) - For handling http requests
 - JSON for Modern C++ (https://nlohmann.github.io/json/) - For parsing JSON

**Visual Studio Project Properties:**

**Configuration Properties->VC++ Directories->Include Directories:**
* C:\SDL2-2.0.12\include
* C:\curl\x86Debug\include

**Configuration Properties->VC++ Directories->Library Directories:**
* C:\SDL2-2.0.12\lib
* C:\curl\x86Debug\lib

**Configuration Properties->C/C++->Preprocessor Definitions:**
* CURL_STATICLIB

**Configuration Properties->Linker->Linker->Additional Dependencies:**
* SDL2.lib
* SDL2main.lib
* SDL2_image.lib
* SDL2_ttf.lib
* C:\curl\x86Debug\lib\libcurl_a_debug.lib
* Ws2_32.lib
* Wldap32.lib
* Crypt32.lib
* Normaliz.lib
