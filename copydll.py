import shutil

sourceFile = r'C:/Git/RocketNet/Intermediate/Debug/bin/RocketNet.dll'
destinationFolderHost = r'C:/Git/RocketNet/Source/Tests/RocketNetCS/RocketNetHost/bin/Debug/net6.0/RocketNet.dll'
destinationFolderClient = r'C:/Git/RocketNet/Source/Tests/RocketNetCS/RocketNetClient/bin/Debug/net6.0/RocketNet.dll'
shutil.copyfile(sourceFile, destinationFolderHost)
shutil.copyfile(sourceFile, destinationFolderClient)