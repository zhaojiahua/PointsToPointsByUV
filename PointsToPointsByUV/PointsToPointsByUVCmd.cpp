#include "PointsToPointsByUVCmd.h"

#include <maya/MGlobal.h>


//��ѡĿ������ѡ����
MStatus PointsToPointsByUV::doIt( const MArgList& arglist)
{
	MStatus stat = MS::kSuccess;

	//��������
	uvIndex = arglist.flagIndex(uvshortFlag, uvlongFlag);
	isUVactive = arglist.asBool(uvIndex + 1);
	if (arglist.length() == 0)isHasArguments = false;

	return redoIt();
}

MStatus PointsToPointsByUV::redoIt()
{
	MString resultString;
	
	MSelectionList selectObjs;
	MGlobal::getActiveSelectionList(selectObjs);
	selectObjs.getDagPath(0, dagpath1);
	selectObjs.getDagPath(1, dagpath2);
	MFnMesh mfnMesh1(dagpath1);//1��Ŀ����
	MFnMesh mfnMesh2(dagpath2);//2�Ǳ���

	mfnMesh1.getPoints(pointsArray1);
	mfnMesh2.getPoints(pointsArray2);

	if (!isUVactive || !isHasArguments)
	{
		MStatus statu = mfnMesh2.setPoints(pointsArray1);
		if (statu == MS::kSuccess)resultString = "PointsToPoints execute successfully!!";
	}
	else
	{
		

		MString uvSetname1 = mfnMesh1.currentUVSetName();
		MString uvSetname2 = mfnMesh2.currentUVSetName();
		
		MPointArray shapePointsArray;
		shapePointsArray.clear();
		for (unsigned i = 0; i < pointsArray2.length();i++)
		{
			//�Ȼ�ȡ�����UVֵ
			float2 tempUV;
			mfnMesh2.getUVAtPoint(pointsArray2[i], tempUV, MSpace::kObject, &uvSetname2);

			int targetPolygonID;//�����洢polygonID
			//�ȸ���UVֵ������Ӧ��polygonID
			mfnMesh1.intersectFaceAtUV(tempUV, targetPolygonID, &uvSetname1);
			//Ȼ�����UVֵ����Ŀ�����Ӧ�ĵ�
			MPoint tempPoint1;
			MStatus statu = mfnMesh1.getPointAtUV(targetPolygonID, tempPoint1, tempUV, MSpace::kObject, &uvSetname1, 0.1);
			
			shapePointsArray.append(tempPoint1);
		}
		if (shapePointsArray.length() == pointsArray2.length())
		{
			//���ö�Ӧ���λ��
			mfnMesh2.setPoints(shapePointsArray, MSpace::kObject);
			resultString = "PointsToPointsByUV execute successfully!!";
		}
	}
	setResult(resultString);
	return MS::kSuccess;
}

MStatus PointsToPointsByUV::undoIt()
{
	//MFnMesh mfnMesh1(dagpath1);//1��Ŀ����
	MFnMesh mfnMesh2(dagpath2);//2�Ǳ���

	mfnMesh2.setPoints(pointsArray2);
	
    MGlobal::displayInfo( "PointsToPointsByUV command undone!\n" );

	return MS::kSuccess;
}

void* PointsToPointsByUV::creator()
{
	return new PointsToPointsByUV();
}

MIntArray PointsToPointsByUV::GetPlolygonIDs(MDagPath inpath)
{
	MItMeshPolygon meshPolygonIt(inpath);
	MIntArray tempIntArray;
	for (meshPolygonIt.reset(); !meshPolygonIt.isDone(); meshPolygonIt.next())
	{
		tempIntArray.append(meshPolygonIt.index());
	}
	return tempIntArray;
}


PointsToPointsByUV::PointsToPointsByUV()
{}

PointsToPointsByUV::~PointsToPointsByUV()
{}

bool PointsToPointsByUV::isUndoable() const
{
	return true;
}
