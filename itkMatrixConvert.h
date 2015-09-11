#ifndef ITKREADMATRIX_H
#define ITKREADMATRIX_H

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
#include <itkMatrixOffsetTransformBase.h>
#define _LOAD_ITK_H
#endif

template< class TransformType >
class ItkMatrixConvert
{
public:
    ItkMatrixConvert(std::string basePath="./", std::string skullFileName="skull.vtk", std::string outputFileName="regSkull.vtk", std::string itkTxtFileName="regctAffine.txt")
    {
        // please properly escape the strings during input
        this->m_skullFileName = skullFileName;
        this->m_basePath = basePath;
        this->m_outputFileName = outputFileName;
        this->m_itkTxtFileName = itkTxtFileName;
    }
    ~ItkMatrixConvert() {}

    // Parameters handling
    void        SetBasePath(std::string basePath) {this->m_basePath = basePath;}
    void        SetSkullFileName(std::string skullFileName) {this->m_skullFileName = skullFileName;}
    void        SetOutputFileName(std::string outputFileName) {this->m_outputFileName = outputFileName;}
    void        SetITKTransformFileName(std::string itkTxtFileName) {this->m_itkTxtFileName = itkTxtFileName;}
    std::string GetBasePath() {return this->m_basePath;}
    std::string GetSkullFileName() {return this->m_skullFileName;}
    std::string GetOutputFileName() {return this->m_outputFileName;}
    std::string GetITKTransformFileName() {return this->itkTxt;}

    // Main functions
    void                            Update();                           // Do everything and output a registered skull vtk polydata file
    vtkSmartPointer<vtkMatrix4x4>   ITK2VTK();                          // Return vtkMatrix4x4 type smart pointer for the itk txt read
    vtkSmartPointer<vtkMatrix4x4>   ITK2VTK(std::string filePath);      //

private:
    std::string m_skullFileName;
    std::string m_basePath;
    std::string m_outputFileName;
    std::string m_itkTxtFileName;

    vtkSmartPointer<vtkPolyData>        ReadVTKPolyData(std::string fileName);
    void                                WriteVTKPolyData(std::string outFileName, vtkSmartPointer<vtkPolyData> input);

    itk::SmartPointer<TransformType>    GetTransformFromFile();
    itk::SmartPointer<TransformType>    GetTransformFromFile(std::string fileName);

    bool RegTransform();



};

#endif // ITKREADMATRIX_H
