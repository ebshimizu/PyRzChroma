# PyRzChroma
An extremely simple python wrapper for Razer Chroma Keyboards using the Razer Chroma SDK.
Please note that at the moment this is not a full wrapper for the SDK and only works with _keyboards_.

## How To Build
There are a rather large number of dependencies for a project that is basically just a few functions.
* CMake >= 2.6
* SWIG >= 3.0.0
* The Razer Chroma SDK

There will also be pre-built binaries available soon, but I haven't gotten around to uploading them yet.

## How To Use
`import PyRzChroma`

You'll probably want to do something more like `import PyRzChroma as Rz`. From there, the wrapper exposes an object to handle initialization and communication with the SDK.

The Hello World of PyRzChroma:
```
import PyRzChroma as Rz
driver = Rz.RzChroma()
driver.Initialize()

# Set all keys to Red
driver.setAll(1, 0, 0)

# Set D key to Blue
driver.setKey(Rz.RZKEY_D, 0, 0, 1)
```

The set functions take a RGB color as a floating point value [0,1].