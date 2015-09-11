#include <iostream>
#include "itkMatrixConvert.h"
#include "itkMatrixConvert.cpp"

#include <vtkErrorCode.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkMatrix4x4.h>
#include <vtkNIFTIImageReader.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>


using namespace std;

void Visualization(vtkSmartPointer<vtkPolyData> input) {
    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();

    mapper->SetInputData(input);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->AddActor(actor);
    renderer->SetBackground(.3, .6, .3); // Background color green

    renderWindow->Render();
    renderWindowInteractor->Start();

}



int main()
{
    ItkMatrixConvert<itk::MatrixOffsetTransformBase<double, 3, 3> > converter;
    converter.SetBasePath("/home/lwong/Documents/qtprojects/build-itkMatrixConvert-Desktop_Qt_5_5_0_GCC_64bit-Default");
    converter.SetOutputFileName("regSkull.vtk");
    converter.SetSkullFileName("skull.vtk");
    converter.SetITKTransformFileName("regctAffine.txt");
    converter.Update();
    return 0;
}

