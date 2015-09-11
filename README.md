# vtkTransformWithITK

This is a learning project I do to familarize with class template, itk and vtk. The executable compiled from this project will read vtk polydata file and an itk transform .txt file and carry out a inverse transform (of the itk .txt file) on the vtk polydata. 

[*Note*] You must include both .cpp and .h file in order to compile this library.

_Sample Code as Follow_
``` c++
	#include <itkMatrixConvert.h>
	#include <itkMatrixConvert.cpp>
	
	// Declare Object
	itkMatrixConvert converter;
	
	// Set object properties
	converter.SetBasePath("./");
	converter.SetOutputFileName("regSkull.vtk");
	converter.SetSkullFileName("skull.vtk");
	converter.SetITKTransformFileName("affine.txt")

	// Run all 
	converter.Update();

```
