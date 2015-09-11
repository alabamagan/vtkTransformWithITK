/* Due to the mechanism of itk transformation register being not very coder friendly, you must
 * remember to register all non-defualt possible tranformations and include both the .cpp and .h
 * file into your main program.
 *
 * Matthew W. L.
 * */

#include "itkMatrixConvert.h"
#include <string>


#ifndef _LOAD_VTK_H
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkErrorCode.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPointData.h>
#include <vtkTransformPolyDataFilter.h>
#define _LOAD_VTK_H
#endif

#ifndef _LOAD_ITK_H
#include <itkTransformFileReader.h>
#include <itkMatrix.h>
#include <itkAffineTransform.h>
#include <itkTransformFactoryBase.h>
#include <itkTransformFactory.h>
#include <itkMetaDataDictionary.h>
#define _LOAD_ITK_H
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static const std::string g_slash="\\";
#else
static const std::string g_slash="/";
#endif

template<class TransformType>
vtkSmartPointer<vtkMatrix4x4> ItkMatrixConvert<TransformType>::ITK2VTK(std::string filePath)
{
    // Register default transform
    itk::TransformFactoryBase::RegisterDefaultTransforms();

    // Register all the non-default transform here
    typedef TransformType MatrixOffsetTransformType;

    // Obtain required data from ANTs generated txt file
    itk::SmartPointer<MatrixOffsetTransformType> transformPointer = GetTransformFromFile(filePath);
    itk::Matrix<double, 3, 3> itkMatrix = transformPointer->GetMatrix();
    itk::Vector<double, 3> itkOffset = transformPointer->GetOffset();


    // Generate vtk transform matrix
    vtkSmartPointer<vtkMatrix4x4> vtkTransformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    vtkTransformMatrix->Zero();
    for (int i=0; i < 3; i++) for(int j=0;j<3;j++) {
       if((i==2 && j<2) || (j==2 && i < 2)) {
            vtkTransformMatrix->Element[i][j]=-itkMatrix[i][j];
       }
       else {
           vtkTransformMatrix->Element[i][j]=itkMatrix[i][j];
       }
    }
    for (int k = 0; k< 3; k++) {
        if (k < 2) {
            vtkTransformMatrix->Element[k][3] = -itkOffset[k];
        }
        else {
            vtkTransformMatrix->Element[k][3] = itkOffset[k];
        }
    }
    vtkTransformMatrix->Element[3][3] = 1;

    return vtkTransformMatrix;
}

template<class TransformType>
vtkSmartPointer<vtkMatrix4x4> ItkMatrixConvert<TransformType>::ITK2VTK() {
    // Register default transform
    itk::TransformFactoryBase::RegisterDefaultTransforms();

    // Register all the non-default transform here
    typedef TransformType MatrixOffsetTransformType;

    // Obtain required data from ANTs generated txt file
    itk::SmartPointer<MatrixOffsetTransformType> transformPointer = GetTransformFromFile();
    itk::Matrix<double, 3, 3> itkMatrix = transformPointer->GetMatrix();
    itk::Vector<double, 3> itkOffset = transformPointer->GetOffset();


    // Generate vtk transform matrix
    vtkSmartPointer<vtkMatrix4x4> vtkTransformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    vtkTransformMatrix->Zero();
    for (int i=0; i < 3; i++) for(int j=0;j<3;j++) {
       if((i==2 && j<2) || (j==2 && i < 2)) {
            vtkTransformMatrix->Element[i][j]=-itkMatrix[i][j];
       }
       else {
           vtkTransformMatrix->Element[i][j]=itkMatrix[i][j];
       }
    }
    for (int k = 0; k< 3; k++) {
        if (k < 2) {
            vtkTransformMatrix->Element[k][3] = -itkOffset[k];
        }
        else {
            vtkTransformMatrix->Element[k][3] = itkOffset[k];
        }
    }
    vtkTransformMatrix->Element[3][3] = 1;

    return vtkTransformMatrix;
}

template<class TransformType>
bool ItkMatrixConvert<TransformType>::RegTransform() {
    try {
        itk::TransformFactory<TransformType>::RegisterTransform();
        return EXIT_SUCCESS;
    }
    catch(...) {
        return EXIT_FAILURE;
    }
}

template<class TransformType>
itk::SmartPointer<TransformType> ItkMatrixConvert<TransformType>::GetTransformFromFile(){
    const std::string filePath = this->m_basePath + g_slash + this->m_itkTxtFileName;

    // Register trasnform to format factory
    RegTransform();

    // Create Reader smart pointer
    #if (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR >= 5) || ITK_VERSION_MAJOR > 4
        typedef itk::TransformFileReaderTemplate<double> TransformReaderType;
        TransformReaderType::Pointer reader = itk::TransformFileReaderTemplate<double>::New();
    #else
        itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();
    #endif
    reader->SetFileName(filePath);
    reader->Update();

    // Read transform from file
    typedef TransformReaderType::TransformListType * TransformListType;
    TransformListType transforms = reader->GetTransformList();


    // Error check
    if (transforms->size() > 1)
    {
        // (To be implemented)
        std::cout << "Not yet implemented for more than one transform";
    }

    // Cast the iterator to be a transform pointer
    typedef TransformType MatrixOffsetTransformType;
    typename MatrixOffsetTransformType::Pointer transform_p =
            static_cast<MatrixOffsetTransformType* >((*(transforms->begin())).GetPointer());
    std::cout << *(transforms->begin());
    return transform_p;
}

template<class TransformType>
itk::SmartPointer<TransformType> ItkMatrixConvert<TransformType>::GetTransformFromFile(std::string fileName) {
    const std::string filePath = fileName;

    // Register trasnform to format factory
    RegTransform();

    // Create Reader smart pointer
    #if (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR >= 5) || ITK_VERSION_MAJOR > 4
        typedef itk::TransformFileReaderTemplate<double> TransformReaderType;
        TransformReaderType::Pointer reader = itk::TransformFileReaderTemplate<double>::New();
    #else
        itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();
    #endif
    reader->SetFileName(filePath);
    reader->Update();

    // Read transform from file
    typedef TransformReaderType::TransformListType * TransformListType;
    TransformListType transforms = reader->GetTransformList();


    // Error check
    if (transforms->size() > 1)
    {
        // (To be implemented)
        std::cout << "Not yet implemented for more than one transform";
    }

    // Cast the iterator to be a transform pointer
    typedef TransformType MatrixOffsetTransformType;
    typename MatrixOffsetTransformType::Pointer transform_p =
            static_cast<MatrixOffsetTransformType* >((*(transforms->begin())).GetPointer());
    std::cout << *(transforms->begin());
    return transform_p;
}

template<class TransformType>
vtkSmartPointer<vtkPolyData> ItkMatrixConvert<TransformType>::ReadVTKPolyData(std::string fileName) {
    // Read all the data from the file
    vtkSmartPointer<vtkPolyDataReader> reader =
    vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();

    vtkSmartPointer<vtkPolyData> output =
        vtkSmartPointer<vtkPolyData>::New();
    output = reader->GetOutput();

    return output;
}

template<class TransformType>
void ItkMatrixConvert<TransformType>::WriteVTKPolyData(std::string outFileName, vtkSmartPointer<vtkPolyData> input) {
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName(outFileName.c_str());
    writer->SetInputData(input);
    writer->Write();
    return;
}

template<class TransformType>
void ItkMatrixConvert<TransformType>::Update() {
    // Generate output from existing files

    /* Error check
    /* 1. if file names are empty throw err
     * 2. if file names extensions are not correct throw err */
    if (this->m_skullFileName.empty() || this->m_basePath.empty() || this->m_itkTxtFileName.empty() || this->m_outputFileName.empty()) {
        // throw input error
    }


    // main program
    vtkSmartPointer<vtkMatrix4x4> transformMatrix;
    vtkSmartPointer<vtkPolyData> skull = ReadVTKPolyData(this->m_basePath + g_slash +  this->m_skullFileName);

    transformMatrix = ITK2VTK();
    transformMatrix->Print(std::cout);
    transformMatrix->Invert();
    transformMatrix->Print(std::cout);

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(transformMatrix);

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
      vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputData(skull);
    transformFilter->SetTransform(transform);
    transformFilter->Update();

    skull = transformFilter->GetOutput();

//    Visualization(skull);
    WriteVTKPolyData(this->m_basePath + g_slash + this->m_outputFileName, skull);
}
