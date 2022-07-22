#include "PointsToPointsByUVCmd.h"

#include <maya/MGlobal.h>


//先选目标体再选本体
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
	MFnMesh mfnMesh1(dagpath1);//1是目标体
	MFnMesh mfnMesh2(dagpath2);//2是本体

	mfnMesh1.getPoints(pointsArray1);
	mfnMesh2.getPoints(pointsArray2);
	MStringArray uvSetnames;
	mfnMesh1.getUVSetNames(uvSetnames);
	
	MPointArray targetPoints;
	for (int i = 0; i < pointsArray2.length();i++)
	{
		//先获取本体的UV值
		float2 tempuv;
		mfnMesh2.getUVAtPoint(pointsArray2[i], tempuv);

		//然后根据UV查找目标体的对应点,
		MIntArray tempIDs;
		MPointArray tempPoints;
		mfnMesh1.getPointsAtUV(tempIDs, tempPoints, tempuv, MSpace::kObject, &uvSetnames[0], 0.0001);

		if (tempPoints.length() > 0)
			targetPoints.append(tempPoints[0]);
		else
			targetPoints.append(MPoint(0, 0, 0));
	}
	//最后将该点的数值赋予本体
	mfnMesh2.setPoints(targetPoints);

	MGlobal::displayInfo("执行成功!");
	return MS::kSuccess;
}

MStatus PointsToPointsByUV::undoIt()
{
	MFnMesh mfnMesh1(dagpath1);//1是目标体
	MFnMesh mfnMesh2(dagpath2);//2是本体

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
