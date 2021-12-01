UnityRemoteDesktopDuplication
=============================

This is an example of sending a desktop image to a remote location. The image is captured by Desktop Duplication API and encoded using NVENC (NvPipe is not used to reduce the overhead). The buffer is then sent over UDP and decoded with NvPipe (NVDEC) in a remove location.

Platforms
---------

- Windows 10
- Unity 2020.3.23f1
- NVIDIA GPU that supports NVENC/NVDEC APIs


Dependencies
------------

- [uDesktopDuplication](https://github.com/hecomi/uDesktopDuplication)
- [uNvEncoder](https://github.com/hecomi/uNvEncoder)
- [uNvPipe](https://github.com/hecomi/uNvPipe)
- [uOSC](https://github.com/hecomi/uOSC)
- [uPacketDivision](https://github.com/hecomi/uPacketDivision)


Screenshot
----------

<img src="https://raw.githubusercontent.com/wiki/hecomi/UnityRemoteDesktopDuplication/UnityRemoteDesktopDuplication.gif" />
