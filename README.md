# gdal-hss-plugin

"my first plugin" WIP as a copy-paste-modify of legacy drivers
This is an effort to write to a custom vector format 

[![CMake](https://github.com/nls-jajuko/gdal-hss-plugin/actions/workflows/cmake.yml/badge.svg)](https://github.com/nls-jajuko/gdal-hss-plugin/actions/workflows/cmake.yml)

```
git pull
cmake ..
make install

/usr/bin/ogr2ogr -f HSS outs.hss json/sample.json

````
plugins dir
/usr/lib/gdalplugins/

