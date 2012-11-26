Maya Cache DLL Project
==============

_Maya Cache DLL Project_ is a research initiative by 100cells (Costanzo D'Angelis, Stefano Cieri).

Introduction
------------

_Maya Cache DLL Project_ is an open source C library aimed to write Maya nCache for nParticles.
This implementation is merely resulting out of observations (and deductions) of .mc binary files and Python scripts provided by Autodesk.

####Maya Caches, nCaches and nParticles
The Maya Cache file format (.mc) is used in Maya for a variety of caching purposes; nCaches are a sub-class of Maya Caches, which store simulation data for nucleus objects or fluid effects; nParticles 
are a specific kind of nucleus objects.
Presently, the library only deals with the file structure of nCaches for nParticles.

Most likely, this will be the first step of a wider research, aimed to generalize the format description (so that it might include every kind of Maya Cache).


Documentation
--------------
The official project's page can be found at: [http://blog.100cells.com/maya-cache-dll-project](http://blog.100cells.com/maya-cache-dll-project).
More detailed instructions can be found at: [http://blog.100cells.com/maya-cache-dll-project/maya-cache-dll-instructions/] (http://blog.100cells.com/maya-cache-dll-project/maya-cache-dll-instructions/).