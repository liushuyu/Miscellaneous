#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include <string.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    return 1;
  }

  char* binaryPath = NULL;

  CFStringRef bundlePath = CFStringCreateWithCString(NULL, argv[1], kCFStringEncodingUTF8);
  CFURLRef bundleURL = CFURLCreateWithFileSystemPath(
      kCFAllocatorDefault, bundlePath, kCFURLPOSIXPathStyle, true);
  CFRelease(bundlePath);
  std::cout << "-- CFURLCreateWithFileSystemPath" << std::endl;
  CFBundleRef bundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
  if (bundle) {
    CFURLRef executableURL = CFBundleCopyExecutableURL(bundle);
    std::cout << "-- CFBundleCopyExecutableURL" << std::endl;
    if (executableURL) {
      CFURLRef absoluteExecutableURL = CFURLCopyAbsoluteURL(executableURL);
      if (absoluteExecutableURL) {
        std::cout << "-- CFURLCopyAbsoluteURL" << std::endl;
        CFStringRef executablePath = CFURLCopyFileSystemPath(
            absoluteExecutableURL, kCFURLPOSIXPathStyle);
        if (executablePath) {
          std::cout << "-- CFURLCopyFileSystemPath" << std::endl;
          CFIndex len = CFStringGetLength(executablePath);
          CFIndex maxSize = CFStringGetMaximumSizeForEncoding(len, kCFStringEncodingUTF8) + 1;
          binaryPath = (char*)malloc(maxSize);
          CFStringGetCString(executablePath, binaryPath, maxSize, kCFStringEncodingUTF8);
          CFRelease(executablePath);
        }
        CFRelease(absoluteExecutableURL);
      }
      CFRelease(executableURL);
    }
    CFRelease(bundle);
  }
  CFRelease(bundleURL);

  if (!binaryPath) {
    return 1;
  }

  std::cout << binaryPath << std::endl;
  return 0;
}
