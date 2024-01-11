import os
import subprocess
import sys
import importlib.util as importlib_util
import zipfile

from pathlib import Path


import urllib.request

premakeVersion = "5.0.0-beta2"
premakeZipUrls = f"https://github.com/premake/premake-core/releases/download/v{premakeVersion}/premake-{premakeVersion}-windows.zip"
premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
premakeDirectory = "../vendor/premake/bin"
premakeZipPath = os.path.join(premakeDirectory, f'premake.zip')
premakeLicensePath = os.path.join(premakeDirectory, 'License.txt')

def PremakeConfiguration():
    premakeExe = Path(f"{premakeDirectory}/premake5.exe");
    if(not os.path.exists(premakeExe)):
        if(not os.path.exists(premakeDirectory)):
            os.makedirs(premakeDirectory)
        return InstallPremake()
    else:
        print("Premake Found!")
        return True

def InstallPremake():
    permissionGranted = False
    while not permissionGranted:
        reply = str(input("Premake not found. Would you like to download Premake {0:s}? [Y/N]: ".format(premakeVersion))).lower().strip()[:1]
        if reply == 'n':
            return False
        permissionGranted = (reply == 'y')
    premakePath = f"{premakeDirectory}/premake-{premakeVersion}-windows.zip"
    print("Downloading {0:s} to {1:s}".format(premakeZipUrls, premakePath))
    urllib.request.urlretrieve(premakeZipUrls, premakeZipPath)
    print("Extracting", premakePath)
    with zipfile.ZipFile(premakeZipPath, 'r') as zip_ref:
        zip_ref.extractall(premakeDirectory)
    print(f"Premake {premakeVersion} has been downloaded to '{premakeDirectory}'")
    premakeLicensePath = f"{premakeDirectory}/LICENSE.txt"
    print("Downloading {0:s} to {1:s}".format(premakeLicenseUrl, premakeLicensePath))
    urllib.request.urlretrieve(premakeLicenseUrl, premakeLicensePath)
    print(f"Premake License file has been downloaded to '{premakeDirectory}'")

    # Delete the file
    try:
        os.remove(premakeZipPath)
        print(f"Successfully deleted {premakeZipPath}")
    except OSError as e:
        print(f"Error: {e.filename} - {e.strerror}")
    
    return True

if(PremakeConfiguration()):
    print("Setup Completed Successfully!")
else:
    print("Setup Failed...")
