#include "PointsToPointsByUVCmd.h"

#include <maya/MGlobal.h>


//先选目标体再选本体
MStatus PointsToPointsByUV::doIt( const MArgList& arglist)
{
	MStatus stat = MS::kSuccess;

	//解析参数
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
	MFnMesh mfnMesh1(dagpath1);//1是目标体
	MFnMesh mfnMesh2(dagpath2);//2是本体

	mfnMesh1.getPoints(pointsArray1);
	mfnMesh2.getPoints(pointsArray2);

	if (!isUVactive || !isHasArguments)
	{
		MStatus statu = mfnMesh2.setPoints(pointsArray1);
		if (statu == MS::kSuccess)resultString = "PointsToPoints execute successfully!!";
	}
	else
	{
		//获取mesh1的所有polygonID
		MIntArray targetPolygonIDs = GetPlolygonIDs(dagpath1);

		MString uvSetname1 = mfnMesh1.currentUVSetName();
		MString uvSetname2 = mfnMesh2.currentUVSetName();
		
		MItGeometry Geo2It(dagpath2);
		for (Geo2It.reset(); !Geo2It.isDone(); Geo2It.next())
		{
			MPoint tempPoint = Geo2It.position(MSpace::kObject);
			//先获取本体的UV值
			float2 tempUV;
			mfnMesh2.getUVAtPoint(tempPoint, tempUV, MSpace::kObject, &uvSetname2);

			//根据UV值查找目标体对应的点
			MPointArray tempPointsArray;
			MStatus statu = mfnMesh1.getPointsAtUV(targetPolygonIDs, tempPointsArray, tempUV, MSpace::kObject, &uvSetname1, 0.1);

			if (statu==MS::kSuccess)
			{
				//设置对应点的位置
				if (tempPointsArray.length() > 0) Geo2It.setPosition(tempPointsArray[0], MSpace::kObject);
			}
		}
		resultString = "PointsToPointsByUV execute successfully!!";
	}
	setResult(resultString);
	return MS::kSuccess;
}

MStatus PointsToPointsByUV::undoIt()
{
	//MFnMesh mfnMesh1(dagpath1);//1是目标体
	MFnMesh mfnMesh2(dagpath2);//2是本体

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
