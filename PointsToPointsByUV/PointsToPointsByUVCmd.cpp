#include "PointsToPointsByUVCmd.h"

#include <maya/MGlobal.h>


//��ѡĿ������ѡ����
MStatus PointsToPointsByUV::doIt( const MArgList& )
{
	MStatus stat = MS::kSuccess;


	return redoIt();
}

MStatus PointsToPointsByUV::redoIt()
{
	setResult( "PointsToPointsByUV command executed!\n" );

	MSelectionList selectObjs;
	MGlobal::getActiveSelectionList(selectObjs);
	selectObjs.getDagPath(0, dagpath1);
	selectObjs.getDagPath(1, dagpath2);
	MFnMesh mfnMesh1(dagpath1);//1��Ŀ����
	MFnMesh mfnMesh2(dagpath2);//2�Ǳ���

	mfnMesh1.getPoints(pointsArray1);
	mfnMesh2.getPoints(pointsArray2);
	MStringArray uvSetnames;
	mfnMesh1.getUVSetNames(uvSetnames);
	
	MPointArray targetPoints;
	for (int i = 0; i < pointsArray2.length();i++)
	{
		//�Ȼ�ȡ�����UVֵ
		float2 tempuv;
		mfnMesh2.getUVAtPoint(pointsArray2[i], tempuv);

		//Ȼ�����UV����Ŀ����Ķ�Ӧ��,
		MIntArray tempIDs;
		MPointArray tempPoints;
		mfnMesh1.getPointsAtUV(tempIDs, tempPoints, tempuv, MSpace::kObject, &uvSetnames[0], 0.0001);

		if (tempPoints.length() > 0)
			targetPoints.append(tempPoints[0]);
		else
			targetPoints.append(MPoint(0, 0, 0));
	}
	//��󽫸õ����ֵ���豾��
	mfnMesh2.setPoints(targetPoints);

	MGlobal::displayInfo("ִ�гɹ�!");
	return MS::kSuccess;
}

MStatus PointsToPointsByUV::undoIt()
{
	MFnMesh mfnMesh1(dagpath1);//1��Ŀ����
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
