2D-Flip-simulator

Adopted the code from: [A Fast Variational Framework for Accurate Solid-Fluid Coupling](http://www.cs.ubc.ca/labs/imager/tr/2007/Batty_VariationalFluids/)

Guide:
1. Build a VS Falcor project named GridFluidSim under Falcor\Source\Samples with [Falcor](https://github.com/NVIDIAGameWorks/Falcor)
2. clone all the files into the folder expect git files.
3. create a 'external' folder under the path: Falcor\Source\Samples\GridFluidSim\ and clone the latest eigen library there.
4. build(There will be some error because of warnings given by VS, just ignore them).

To do list: 
 1.Extend the simulator to 3D. 
 2.Add levelset methods so that simulating smoke will be supported. Add a MacCormack method solver for advecting smoke.
 3.Add vorticity confinement or other post-processing techniques.
 4.Coupling dynamic rigid bodies.
 5.Add openVDB support.