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
		//��ȡmesh1������polygonID
		MIntArray targetPolygonIDs;
		MStringArray uvSetnames;
		mfnMesh1.getUVSetNames(uvSetnames);
		for (unsigned i = 0; i < pointsArray1.length(); i++)
		{
			int tempID;
			float2 tempUV;
			mfnMesh1.getUVAtPoint(pointsArray1[i], tempUV, MSpace::kObject, &uvSetnames[0], &tempID);
			targetPolygonIDs.append(tempID);
		}
		MPointArray targetPointsByUV;
		for (unsigned i = 0; i < pointsArray2.length(); i++)
		{
			//�Ȼ�ȡ�����UVֵ
			float2 tempuv;
			mfnMesh2.getUVAtPoint(pointsArray2[i], tempuv, MSpace::kObject, &uvSetnames[0]);

			//Ȼ�����UV����Ŀ����Ķ�Ӧ��,
			
			MPointArray tempPoints;
			mfnMesh1.getPointsAtUV(targetPolygonIDs, tempPoints, tempuv, MSpace::kObject, &uvSetnames[0], 0.1);

			if (tempPoints.length() > 0)
				targetPointsByUV.append(tempPoints[0]);
			else
				targetPointsByUV.append(MPoint(0, 0, 0));
		}
		//��󽫸õ����ֵ���豾��
		MStatus statu = mfnMesh2.setPoints(targetPointsByUV);
		if (statu == MS::kSuccess)resultString = "PointsToPointsByUV execute successfully!!";
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

PointsToPointsByUV::PointsToPointsByUV()
{}

PointsToPointsByUV::~PointsToPointsByUV()
{
}

bool PointsToPointsByUV::isUndoable() const
{
	return true;
}
