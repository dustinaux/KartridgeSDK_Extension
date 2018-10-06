# KartridgeSDKExtension
Haxe Extension for the Kartridge SDK

Currently only windows is working - maybe someone more knowledgeable than me can figure out how to build for mac?

## Instructions
1. Put the kartridge folder in your game project somewhere, like in a "libs" folder

2. Include it by adding to your project.xml: `<include path="libs/kartridge"/>`

### Kartridge SDK
Included is Kartridge SDK 1.0.2

Check for the latest version at https://github.com/kongregate/kartridge-sdk-releases/releases. 

If a newer version is required you should be able to just replace the "kartridge/project/lib/kartridge-sdk" folder with the newer version and rebuild with `lime rebuild . windows` from the "project" folder.

**Kartridge Docs**: https://docs.kartridge.com/
