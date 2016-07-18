# Space-Simulation
C++ Qt code parses a configuration file and uses Newtonian mechanics to simulate interaction of celestial bodies.

**Requirements**

Qt Creator 3.6.0
Based on Qt 5.5.1 (MSVC 2013, 32 bit)
https://www.qt.io/download/

**Guide**

Code uses object oriented design in C++. The design patterns used include:

1. Singleton
2. Parameterised factory method
3. Composite
4. Visitor

The configuration file can be updated, more celestial bodies can be added. You must only create planets, stars, black holes, solar systems, galaxies, and clusters. The following heirarchy applies:

- solar sytems contain: planets, stars
- galaxies contain: solar systems and black holes
- clusters contain: galaxies and other clusters

**Author**

Ryan Michael Thomas
