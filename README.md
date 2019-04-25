PPFileSync
==========
This is a very simple, Linux-only synchronizer between two folders, created as a project for "Operating Systems" subject.

Compilation
-----------
OpenSSL is required, since we use the MD5 hashing for file comparisions.
We provide both CMake based build system and the raw MakeFile.
To compile using make, just type make on root of the repository. To use CMake, create the Build folder, cd Build/ and type cmake ./.
This project contains two programs: PPFileSync is the synchronizer, the other is internal-use test program used to check some functions.

Usage
-----
PPFileSync [OPTIONS] -s [SOURCEDIR] -d [DESTDIR]

PPFileSynce -h print full help.
Full documentation (as PDF) exists, but it is in Polish.

License
-------
MIT
