# Particle-System
A Particle System(Independent/Dependent) implemented with opengl.

![Independent Particle System](https://github.com/Code-Guy/Particle-System/blob/master/DependentPS/snapshots/dragonflies.png?raw=true)
###Introduction
This is my undergraduate graduation thesis. Particle system can be splitted into two categories: Independent and dependent. Independent particle system means all particles are irrelevant, they don't know the information of other particles. Fireworks, snow, rain, smoke, fire are classic independent particle system, which are all lifeless objects; On the contrary, dependent particle system owns lots of particles which are related to a local neighborhood, they influence each other in some degrees. Birds, fishes, butterflies are independent particle systems without doubt, because they are living beings.

This paper integrated the Independent Particle System and the Dependent Particle System into a whole system, and implemented sorts of classic flocking algorithms.
###Feature
 - The Boid flocking model and PSO algorithm have been adopted, with some other behavior rules added, such as scatter, chase, scare
 - Implemented some spatial subdivision algorithm:uniform grid subdivision, octree, to speed up the procedure of neighbor searching

###Framework
Qt + Ork(a lightweight opengl wrapper)
###More Examples
![Fireworks](https://github.com/Code-Guy/Particle-System/blob/master/DependentPS/snapshots/fireworks.png?raw=true)
 
![Smoke](https://github.com/Code-Guy/Particle-System/blob/master/DependentPS/snapshots/smoke.png?raw=true)
 
![Bubble](https://github.com/Code-Guy/Particle-System/blob/master/DependentPS/snapshots/bubbles.png?raw=true)
