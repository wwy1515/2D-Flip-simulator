2D-Flip-simulator
Adopted the code from: [A Fast Variational Framework for Accurate Solid-Fluid Coupling](http://www.cs.ubc.ca/labs/imager/tr/2007/Batty_VariationalFluids/)

Guide:
1. Build a VS Falcor project named GridFluidSim under Falcor\Source\Samples with [Falcor](https://github.com/NVIDIAGameWorks/Falcor)
2. clone all the files into the folder expect git files.
3. create a 'external' folder under the path: Falcor\Source\Samples\GridFluidSim\ and clone the latest eigen library there.
4. build(There will be some error because of warnings given by VS, just ignore them).

To do list: 1.extend the simulator to 3D.
            2.add vorticity confinement.
            3.coupling dynamic rigid bodies.