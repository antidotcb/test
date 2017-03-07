rmdir /s/q build 2>NUL
mkdir build & cd build
cmake ../testapp
msbuild testapp.sln /t:Build