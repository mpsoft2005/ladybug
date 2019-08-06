# Hi Ladybug

## How to build Ladybug on Windows?

```Shell
mkdir build && cd build
cmake -G "Visual Studio 15 2017 Win64" ..
msbuild ladybug.sln
```

## Running Ladybug

```Shell
cd build
Debug\ladybug.exe ..\..\Data\TestModels
```
